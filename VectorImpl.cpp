#include "VectorImpl.h"
#include <cmath>
#include <memory.h>

ILogger *VectorImpl::LOGGER = nullptr;

RC VectorImpl::setLogger(ILogger *const logger) {
    if (logger == nullptr)
        return RC::NULLPTR_ERROR;
    LOGGER = logger;
    return RC::SUCCESS;
}

RC VectorImpl::elemCheck(double elem) {
    if (std::isinf(elem)) {
        if (LOGGER != nullptr) LOGGER->severe(RC::INFINITY_OVERFLOW, __FILE__, __func__, __LINE__);
        return RC::INFINITY_OVERFLOW;
    }
    if (std::isnan(elem)) {
        if (LOGGER != nullptr) LOGGER->severe(RC::NOT_NUMBER, __FILE__, __func__, __LINE__);
        return RC::NOT_NUMBER;
    }
    return RC::SUCCESS;
}

VectorImpl::VectorImpl(size_t dim) {
    this->dim = dim;
    if (LOGGER != nullptr) LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
}

double const *VectorImpl::getData() const {
    if (LOGGER != nullptr) LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return (double const *) ((uint8_t *) this + sizeof(VectorImpl));
}

RC VectorImpl::getCord(size_t index, double &val) const {
    if (index >= dim) {
        if (LOGGER != nullptr) LOGGER->warning(RC::INDEX_OUT_OF_BOUND, __FILE__, __func__, __LINE__);
        return RC::INDEX_OUT_OF_BOUND;
    }
    const double *data = getData();
    val = data[index];
    if (LOGGER != nullptr) LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

RC VectorImpl::setCord(size_t index, double val) {
    if (index >= dim) {
        if (LOGGER != nullptr) LOGGER->warning(RC::INDEX_OUT_OF_BOUND, __FILE__, __func__, __LINE__);
        return RC::INDEX_OUT_OF_BOUND;
    }
    RC temp = elemCheck(val);
    if (temp != RC::SUCCESS) {
        if (LOGGER != nullptr) LOGGER->warning(temp, __FILE__, __func__, __LINE__);
        return temp;
    }
    double *data = (double *) ((uint8_t *) this + sizeof(VectorImpl));
    data[index] = val;
    if (LOGGER != nullptr) LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

RC VectorImpl::scale(double multiplier) {
    RC temp = elemCheck(multiplier);
    if (temp != RC::SUCCESS) {
        if (LOGGER != nullptr) LOGGER->warning(temp, __FILE__, __func__, __LINE__);
        return temp;
    }
    double max = doChebyshev();
    temp = elemCheck(max * multiplier);
    if (temp != RC::SUCCESS) {
        if (LOGGER != nullptr) LOGGER->warning(temp, __FILE__, __func__, __LINE__);
        return temp;
    }
    double *data = (double *) ((uint8_t *) this + sizeof(VectorImpl));
    for (size_t i = 0; i < dim; i++)
        data[i] *= multiplier;
    if (LOGGER != nullptr) LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

size_t VectorImpl::getDim() const {
    if (LOGGER != nullptr) LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return dim;
}

RC VectorImpl::doSum(double *dest, double const *src, size_t const dim, bool doMinus) {
    int s = doMinus ? -1 : 1;
    double *data = new double[dim];
    if (data == nullptr) {
        if (LOGGER != nullptr) LOGGER->warning(RC::ALLOCATION_ERROR, __FILE__, __func__, __LINE__);
        return RC::ALLOCATION_ERROR;
    }
    for (size_t i = 0; i < dim; i++) {
        data[i] = dest[i] + s * src[i];
        RC code = elemCheck(data[i]);
        if (code != RC::SUCCESS) {
            if (LOGGER != nullptr) LOGGER->warning(code, __FILE__, __func__, __LINE__);
            delete data;
            return code;
        }
    }
    memcpy(dest, data, dim * sizeof(double));
    delete[] data;
    if (LOGGER != nullptr) LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

RC VectorImpl::inc(const IVector *const &op) {
    if (op == nullptr) {
        if (LOGGER != nullptr) LOGGER->warning(RC::NULLPTR_ERROR, __FILE__, __func__, __LINE__);
        return RC::NULLPTR_ERROR;
    }
    if (op->getDim() != dim) {
        if (LOGGER != nullptr) LOGGER->warning(RC::MISMATCHING_DIMENSIONS, __FILE__, __func__, __LINE__);
        return RC::MISMATCHING_DIMENSIONS;
    }

    RC code = doSum((double *) ((uint8_t *) this + sizeof(VectorImpl)), op->getData(), dim);

    if (code == RC::SUCCESS && LOGGER != nullptr) LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return code;
}

RC VectorImpl::dec(const IVector *const &op) {
    if (op == nullptr) {
        if (LOGGER != nullptr) LOGGER->warning(RC::NULLPTR_ERROR, __FILE__, __func__, __LINE__);
        return RC::NULLPTR_ERROR;
    }
    if (op->getDim() != dim) {
        if (LOGGER != nullptr) LOGGER->warning(RC::MISMATCHING_DIMENSIONS, __FILE__, __func__, __LINE__);
        return RC::MISMATCHING_DIMENSIONS;
    }

    RC code = doSum((double *) ((uint8_t *) this + sizeof(VectorImpl)), op->getData(), dim, true);

    if (code == RC::SUCCESS && LOGGER != nullptr) LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return code;
}

double VectorImpl::doChebyshev() const {
    const double *data = getData();
    double res = fabs(data[0]);
    for (size_t i = 0; i < dim; i++)
        if (res < fabs(data[i]))
            res = fabs(data[i]);
    return res;
}

double VectorImpl::doFirst() const {
    const double *data = getData();
    double res = 0;
    for (size_t i = 0; i < dim; i++)
        res += fabs(data[i]);
    return res;
}

double VectorImpl::doSecond() const {
    const double *data = getData();
    double res = 0;
    for (size_t i = 0; i < dim; i++)
        res += pow(data[i], 2);
    return sqrt(res);
}

double VectorImpl::norm(NORM n) const {
    double res = 0;
    switch (n) {
        case IVector::NORM::CHEBYSHEV:
            res = doChebyshev();
            break;
        case IVector::NORM::FIRST:
            res = doFirst();
            break;
        case IVector::NORM::SECOND:
            res = doSecond();
            break;
        case IVector::NORM::AMOUNT:
            res = NAN;
            break;
    }
    if (LOGGER != nullptr) LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return res;
}

RC VectorImpl::applyFunction(const std::function<double(double)> &fun) {
    double *data = (double *) ((uint8_t *) this + sizeof(VectorImpl));
    for (size_t i = 0; i < dim; i++)
        data[i] = fun(data[i]);
    if (LOGGER != nullptr) LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

RC VectorImpl::foreach(const std::function<void(double)> &fun) const {
    double *data = (double *) ((uint8_t *) this + sizeof(VectorImpl));
    for (size_t i = 0; i < dim; i++)
        fun(data[i]);
    if (LOGGER != nullptr) LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

size_t VectorImpl::sizeAllocated() const {
    if (LOGGER != nullptr) LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return sizeof(VectorImpl) + dim * sizeof(double);
}

RC VectorImpl::setData(size_t dim, const double *const &ptr_data) {
    if (dim == 0 || this->dim != dim) {
        if (LOGGER != nullptr) LOGGER->warning(RC::MISMATCHING_DIMENSIONS, __FILE__, __func__, __LINE__);
        return RC::MISMATCHING_DIMENSIONS;
    }
    if (ptr_data == nullptr) {
        if (LOGGER != nullptr) LOGGER->warning(RC::NULLPTR_ERROR, __FILE__, __func__, __LINE__);
        return RC::NULLPTR_ERROR;
    }

    RC temp = RC::SUCCESS;
    for (size_t i = 0; i < dim; i++) {
        temp = elemCheck(ptr_data[i]);
        if (temp != RC::SUCCESS) {
            if (LOGGER != nullptr) LOGGER->warning(temp, __FILE__, __func__, __LINE__);
            return temp;
        }
    }

    double *data = (double *) ((uint8_t *) this + sizeof(VectorImpl));
    memcpy(data, ptr_data, dim * sizeof(double));
    if (LOGGER != nullptr) LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

inline IVector::~IVector() {};
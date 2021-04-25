//
// Created by mikita on 25.04.2021.
//

#include "VectorImpl.h"
#include <memory.h>
#include <cmath>

RC IVector::setLogger(ILogger *const logger) {
    return VectorImpl::setLogger(logger);
}

IVector *IVector::createVector(size_t dim, const double *const &ptr_data) {
    ILogger *const LOGGER = VectorImpl::getLogger();
    if (dim == 0 || ptr_data == nullptr) {
        if (LOGGER != nullptr) LOGGER->severe(RC::NULLPTR_ERROR, __FILE__, __func__, __LINE__);
        return nullptr;
    }

    for (size_t i = 0; i < dim; i++)
        if (VectorImpl::elemCheck(ptr_data[i]) != RC::SUCCESS)
            return nullptr;

    size_t size = sizeof(VectorImpl) + dim * sizeof(double);
    uint8_t *pInstance = new(std::nothrow) uint8_t[size];
    if (pInstance == nullptr) {
        if (LOGGER != nullptr) LOGGER->severe(RC::ALLOCATION_ERROR, __FILE__, __func__, __LINE__);
        return nullptr;
    }

    uint8_t *pData = pInstance + sizeof(VectorImpl);
    memcpy(pData, (uint8_t *) ptr_data, dim * sizeof(double));
    if (LOGGER != nullptr) LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return new(pInstance) VectorImpl(dim);
}

RC IVector::copyInstance(IVector *const dest, const IVector *const &src) {
    ILogger *const LOGGER = VectorImpl::getLogger();
    if (dest == nullptr || src == nullptr) {
        if (LOGGER != nullptr) LOGGER->warning(RC::NULLPTR_ERROR, __FILE__, __func__, __LINE__);
        return RC::NULLPTR_ERROR;
    }
    if (dest->sizeAllocated() < src->sizeAllocated()) {
        if (LOGGER != nullptr) LOGGER->warning(RC::AMOUNT, __FILE__, __func__, __LINE__);
        return RC::AMOUNT;
    }
    if (abs((uint8_t *) src - (uint8_t *) dest) < dest->sizeAllocated()) {
        if (LOGGER != nullptr) LOGGER->warning(RC::MEMORY_INTERSECTION, __FILE__, __func__, __LINE__);
        return RC::MEMORY_INTERSECTION;
    }

    dest->setData(src->getDim(), src->getData());

    if (LOGGER != nullptr) LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

RC IVector::moveInstance(IVector *const dest, IVector *&src) {
    ILogger *const LOGGER = VectorImpl::getLogger();
    RC res = copyInstance(dest, src);
    if (res != RC::SUCCESS)
        return res;
    delete src;
    src = nullptr;
    if (LOGGER != nullptr) LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

IVector *VectorImpl::clone() const {
    if (LOGGER != nullptr) LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return createVector(dim, getData());
}

IVector *IVector::add(const IVector *const &op1, const IVector *const &op2) {
    ILogger *const LOGGER = VectorImpl::getLogger();
    if (op1 == nullptr || op2 == nullptr) {
        if (LOGGER != nullptr) LOGGER->warning(RC::NULLPTR_ERROR, __FILE__, __func__, __LINE__);
        return nullptr;
    }
    size_t dim = op1->getDim();
    if (op2->getDim() != dim) {
        if (LOGGER != nullptr) LOGGER->warning(RC::MISMATCHING_DIMENSIONS, __FILE__, __func__, __LINE__);
        return nullptr;
    }

    IVector *newVector = createVector(dim, op1->getData());
    if (newVector == nullptr)
        return nullptr;

    RC temp = newVector->inc(op2);
    if (temp != RC::SUCCESS) {
        if (LOGGER != nullptr) LOGGER->warning(temp, __FILE__, __func__, __LINE__);
        delete newVector;
        return nullptr;
    }

    if (LOGGER != nullptr) LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return newVector;
}

IVector *IVector::sub(const IVector *const &op1, const IVector *const &op2) {
    ILogger *const LOGGER = VectorImpl::getLogger();
    if (op1 == nullptr || op2 == nullptr) {
        if (LOGGER != nullptr) LOGGER->warning(RC::NULLPTR_ERROR, __FILE__, __func__, __LINE__);
        return nullptr;
    }
    size_t dim = op1->getDim();
    if (op2->getDim() != dim) {
        if (LOGGER != nullptr) LOGGER->warning(RC::MISMATCHING_DIMENSIONS, __FILE__, __func__, __LINE__);
        return nullptr;
    }

    IVector *newVector = createVector(dim, op1->getData());
    if (newVector == nullptr)
        return nullptr;

    RC temp = newVector->dec(op2);
    if (temp != RC::SUCCESS) {
        if (LOGGER != nullptr) LOGGER->warning(temp, __FILE__, __func__, __LINE__);
        delete newVector;
        return nullptr;
    }

    if (LOGGER != nullptr) LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return newVector;
}

double IVector::dot(const IVector *const &op1, const IVector *const &op2) {
    ILogger *const LOGGER = VectorImpl::getLogger();
    if (op1 == nullptr || op2 == nullptr) {
        if (LOGGER != nullptr) LOGGER->warning(RC::NULLPTR_ERROR, __FILE__, __func__, __LINE__);
        return NAN;
    }
    size_t dim = op1->getDim();
    if (op2->getDim() != dim) {
        if (LOGGER != nullptr) LOGGER->warning(RC::MISMATCHING_DIMENSIONS, __FILE__, __func__, __LINE__);
        return NAN;
    }

    double res = 0;
    const double *one = op1->getData(), *two = op2->getData();
    for (size_t i = 0; i < dim; i++)
        res += one[i] * two[i];

    if (LOGGER != nullptr) LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return res;
}

bool IVector::equals(const IVector *const &op1, const IVector *const &op2, NORM n, double tol) {
    ILogger *const LOGGER = VectorImpl::getLogger();
    IVector *temp = sub(op1, op2);
    if (temp == nullptr)
        return false;
    double res = temp->norm(n);
    delete temp;
    if (LOGGER != nullptr) LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return res <= tol;
}
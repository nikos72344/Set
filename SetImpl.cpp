//
// Created by mikita on 15.04.2021.
//

#include <cmath>
#include "SetImpl.h"

ILogger *SetImpl::LOGGER = nullptr;

SetImpl::SetImpl() : data(nullptr), indices(nullptr), lastIndex(0), dim(0), size(0), allocSize(0) {
    controlBlock = std::make_shared<SetControlBlockImpl>(data, indices, dim, size);
    if (LOGGER != nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
}

RC SetImpl::setLogger(ILogger *const logger) {
    if (logger == nullptr)
        return RC::NULLPTR_ERROR;
    LOGGER = logger;
    return RC::SUCCESS;
}

ILogger *const SetImpl::getLogger() {
    return LOGGER;
}

ISet *SetImpl::clone() const {
    SetImpl *newSet = new SetImpl;
    if (newSet == nullptr) {
        if (LOGGER != nullptr)LOGGER->severe(RC::ALLOCATION_ERROR, __FILE__, __func__, __LINE__);
        return nullptr;
    }
    newSet->lastIndex = lastIndex;
    newSet->dim = dim;
    newSet->size = size;
    newSet->allocSize = allocSize;
    newSet->data = new double[allocSize * dim];
    if (newSet->data == nullptr) {
        if (LOGGER != nullptr)LOGGER->severe(RC::ALLOCATION_ERROR, __FILE__, __func__, __LINE__);
        delete newSet;
        return nullptr;
    }
    newSet->indices = new size_t[allocSize];
    if (newSet->indices == nullptr) {
        if (LOGGER != nullptr)LOGGER->severe(RC::ALLOCATION_ERROR, __FILE__, __func__, __LINE__);
        delete newSet;
        return nullptr;
    }

    memcpy(newSet->data, data, dim * size * sizeof(double));
    memcpy(newSet->indices, indices, size * sizeof(size_t));

    if (LOGGER != nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return (ISet *) newSet;
}

size_t SetImpl::getDim() const {
    if (LOGGER != nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return dim;
}

size_t SetImpl::getSize() const {
    if (LOGGER != nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return size;
}

RC SetImpl::getCopy(size_t index, IVector *&val) const {
    if (index >= size) {
        if (LOGGER != nullptr)LOGGER->warning(RC::INDEX_OUT_OF_BOUND, __FILE__, __func__, __LINE__);
        return RC::INDEX_OUT_OF_BOUND;
    }
    IVector *vector = IVector::createVector(dim, &data[index * dim]);
    if (vector == nullptr) {
        if (LOGGER != nullptr)LOGGER->warning(RC::ALLOCATION_ERROR, __FILE__, __func__, __LINE__);
        return RC::ALLOCATION_ERROR;
    }
    val = vector;
    if (LOGGER != nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

RC SetImpl::findFirstAndCopy(const IVector *const &pat, IVector::NORM n, double tol, IVector *&val) const {
    if (pat == nullptr) {
        if (LOGGER != nullptr)LOGGER->warning(RC::NULLPTR_ERROR, __FILE__, __func__, __LINE__);
        return RC::NULLPTR_ERROR;
    }
    if (pat->getDim() != dim) {
        if (LOGGER != nullptr)LOGGER->warning(RC::MISMATCHING_DIMENSIONS, __FILE__, __func__, __LINE__);
        return RC::MISMATCHING_DIMENSIONS;
    }
    if (n == IVector::NORM::AMOUNT || tol < 0.0) {
        if (LOGGER != nullptr)LOGGER->warning(RC::INVALID_ARGUMENT, __FILE__, __func__, __LINE__);
        return RC::INVALID_ARGUMENT;
    }
    if (std::isnan(tol)) {
        if (LOGGER != nullptr)LOGGER->warning(RC::NOT_NUMBER, __FILE__, __func__, __LINE__);
        return RC::NOT_NUMBER;
    }
    if (std::isinf(tol)) {
        if (LOGGER != nullptr)LOGGER->warning(RC::INFINITY_OVERFLOW, __FILE__, __func__, __LINE__);
        return RC::INFINITY_OVERFLOW;
    }

    IVector *buffer = IVector::createVector(dim, data);
    if (buffer == nullptr) {
        if (LOGGER != nullptr)LOGGER->warning(RC::ALLOCATION_ERROR, __FILE__, __func__, __LINE__);
        return RC::ALLOCATION_ERROR;
    }
    if (IVector::equals(buffer, pat, n, tol)) {
        val = buffer;
        if (LOGGER != nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
        return RC::SUCCESS;
    }

    RC code = RC::SUCCESS;
    for (size_t i = 1; i < size; i++) {
        code = buffer->setData(dim, &data[i * dim]);
        if (code != RC::SUCCESS) {
            delete buffer;
            return code;
        }
        if (IVector::equals(buffer, pat, n, tol)) {
            val = buffer;
            if (LOGGER != nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
            return RC::SUCCESS;
        }
    }

    delete buffer;

    if (LOGGER != nullptr)LOGGER->info(RC::VECTOR_NOT_FOUND, __FILE__, __func__, __LINE__);
    return RC::VECTOR_NOT_FOUND;
}

RC SetImpl::getCoords(size_t index, IVector *const &val) const {
    if (val == nullptr) {
        if (LOGGER != nullptr)LOGGER->warning(RC::NULLPTR_ERROR, __FILE__, __func__, __LINE__);
        return RC::NULLPTR_ERROR;
    }
    if (index >= size) {
        if (LOGGER != nullptr)LOGGER->warning(RC::INDEX_OUT_OF_BOUND, __FILE__, __func__, __LINE__);
        return RC::INDEX_OUT_OF_BOUND;
    }

    RC code = val->setData(dim, &data[index * dim]);
    if (code != RC::SUCCESS)
        return code;

    if (LOGGER != nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

RC SetImpl::findFirstAndCopyCoords(const IVector *const &pat, IVector::NORM n, double tol, IVector *const &val) const {
    if (val == nullptr || pat == nullptr) {
        if (LOGGER != nullptr)LOGGER->warning(RC::NULLPTR_ERROR, __FILE__, __func__, __LINE__);
        return RC::NULLPTR_ERROR;
    }
    if (n == IVector::NORM::AMOUNT || tol < 0.0) {
        if (LOGGER != nullptr)LOGGER->warning(RC::INVALID_ARGUMENT, __FILE__, __func__, __LINE__);
        return RC::INVALID_ARGUMENT;
    }
    if (std::isnan(tol)) {
        if (LOGGER != nullptr)LOGGER->warning(RC::NOT_NUMBER, __FILE__, __func__, __LINE__);
        return RC::NOT_NUMBER;
    }
    if (std::isinf(tol)) {
        if (LOGGER != nullptr)LOGGER->warning(RC::INFINITY_OVERFLOW, __FILE__, __func__, __LINE__);
        return RC::INFINITY_OVERFLOW;
    }
    IVector *temp = nullptr;
    RC code = findFirstAndCopy(pat, n, tol, temp);
    if (code != RC::SUCCESS)
        return code;
    val->setData(temp->getDim(), temp->getData());
    delete temp;
    if (LOGGER != nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

RC SetImpl::findFirst(const IVector *const &pat, IVector::NORM n, double tol) const {
    IVector *temp = nullptr;
    RC code = findFirstAndCopy(pat, n, tol, temp);
    if (code != RC::SUCCESS)
        return code;
    delete temp;
    if (LOGGER != nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

RC SetImpl::insertFirstVec(const IVector *const &val) {
    data = new double[minAlloc * val->getDim()];
    if (data == nullptr) {
        if (LOGGER != nullptr)LOGGER->warning(RC::ALLOCATION_ERROR, __FILE__, __func__, __LINE__);
        return RC::ALLOCATION_ERROR;
    }
    indices = new size_t[minAlloc];
    if (indices == nullptr) {
        delete data;
        data = nullptr;
        if (LOGGER != nullptr)LOGGER->warning(RC::ALLOCATION_ERROR, __FILE__, __func__, __LINE__);
        return RC::ALLOCATION_ERROR;
    }
    indices[0] = 0;
    lastIndex = 0;
    dim = val->getDim();
    size = 1;
    allocSize = minAlloc;
    memcpy(data, val->getData(), dim * sizeof(double));
    if (LOGGER != nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

RC SetImpl::addNewVec(const IVector *const &val, IVector::NORM n, double tol) {
    if (val->getDim() != dim) {
        if (LOGGER != nullptr)LOGGER->warning(RC::MISMATCHING_DIMENSIONS, __FILE__, __func__, __LINE__);
        return RC::MISMATCHING_DIMENSIONS;
    }
    RC code = findFirst(val, n, tol);
    if (code != RC::VECTOR_NOT_FOUND) {
        code = code == RC::SUCCESS ? RC::VECTOR_ALREADY_EXIST : code;
        if (LOGGER != nullptr)LOGGER->warning(code, __FILE__, __func__, __LINE__);
        return code;
    }

    if (size == allocSize) {
        double *newData = new double[2 * allocSize * dim];
        if (newData == nullptr) {
            if (LOGGER != nullptr)LOGGER->warning(RC::ALLOCATION_ERROR, __FILE__, __func__, __LINE__);
            return RC::ALLOCATION_ERROR;
        }
        size_t *newIndices = new size_t[2 * allocSize];
        if (newIndices == nullptr) {
            delete newData;
            if (LOGGER != nullptr)LOGGER->warning(RC::ALLOCATION_ERROR, __FILE__, __func__, __LINE__);
            return RC::ALLOCATION_ERROR;
        }

        memcpy(newData, data, size * dim * sizeof(double));
        memcpy(newIndices, indices, size * sizeof(size_t));

        delete[] data;
        delete[] indices;

        data = newData;
        indices = newIndices;

        allocSize *= 2;
    }

    memcpy(&data[dim * size], val->getData(), dim * sizeof(double));
    indices[size++] = ++lastIndex;
    if (LOGGER != nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

RC SetImpl::insert(const IVector *const &val, IVector::NORM n, double tol) {
    if (val == nullptr) {
        if (LOGGER != nullptr)LOGGER->warning(RC::NULLPTR_ERROR, __FILE__, __func__, __LINE__);
        return RC::NULLPTR_ERROR;
    }
    if (n == IVector::NORM::AMOUNT || tol < 0.0) {
        if (LOGGER != nullptr)LOGGER->warning(RC::INVALID_ARGUMENT, __FILE__, __func__, __LINE__);
        return RC::INVALID_ARGUMENT;
    }
    if (std::isnan(tol)) {
        if (LOGGER != nullptr)LOGGER->warning(RC::NOT_NUMBER, __FILE__, __func__, __LINE__);
        return RC::NOT_NUMBER;
    }
    if (std::isinf(tol)) {
        if (LOGGER != nullptr)LOGGER->warning(RC::INFINITY_OVERFLOW, __FILE__, __func__, __LINE__);
        return RC::INFINITY_OVERFLOW;
    }
    RC code = RC::SUCCESS;
    if (size == 0)
        code = insertFirstVec(val);
    else
        code = addNewVec(val, n, tol);
    controlBlock->updateData(data, indices, dim, size);
    return code;
}

RC SetImpl::remove(size_t index) {
    if (index >= size) {
        if (LOGGER != nullptr)LOGGER->warning(RC::INDEX_OUT_OF_BOUND, __FILE__, __func__, __LINE__);
        return RC::INDEX_OUT_OF_BOUND;
    }
    if (index != size - 1) {
        memcpy(&data[dim * index], &data[dim * (index + 1)], dim * (size - index - 1) * sizeof(double));
        memcpy(&indices[index], &data[index + 1], (size - index - 1) * sizeof(size_t));
    }
    size--;
    controlBlock->updateData(data, indices, dim, size);
    if (LOGGER != nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

RC SetImpl::remove(const IVector *const &pat, IVector::NORM n, double tol) {
    if (pat == nullptr) {
        if (LOGGER != nullptr)LOGGER->warning(RC::NULLPTR_ERROR, __FILE__, __func__, __LINE__);
        return RC::NULLPTR_ERROR;
    }
    if (pat->getDim() != dim) {
        if (LOGGER != nullptr)LOGGER->warning(RC::MISMATCHING_DIMENSIONS, __FILE__, __func__, __LINE__);
        return RC::MISMATCHING_DIMENSIONS;
    }
    if (n == IVector::NORM::AMOUNT || tol < 0.0) {
        if (LOGGER != nullptr)LOGGER->warning(RC::INVALID_ARGUMENT, __FILE__, __func__, __LINE__);
        return RC::INVALID_ARGUMENT;
    }
    if (std::isnan(tol)) {
        if (LOGGER != nullptr)LOGGER->warning(RC::NOT_NUMBER, __FILE__, __func__, __LINE__);
        return RC::NOT_NUMBER;
    }
    if (std::isinf(tol)) {
        if (LOGGER != nullptr)LOGGER->warning(RC::INFINITY_OVERFLOW, __FILE__, __func__, __LINE__);
        return RC::INFINITY_OVERFLOW;
    }

    IVector *buffer = IVector::createVector(dim, data);
    if (buffer == nullptr) {
        if (LOGGER != nullptr)LOGGER->warning(RC::ALLOCATION_ERROR, __FILE__, __func__, __LINE__);
        return RC::ALLOCATION_ERROR;
    }
    if (IVector::equals(buffer, pat, n, tol)) {
        delete buffer;
        return remove(0);
    }

    RC code = RC::SUCCESS;
    for (size_t i = 1; i < size; i++) {
        code = buffer->setData(dim, &data[i * dim]);
        if (code != RC::SUCCESS) {
            delete buffer;
            return code;
        }
        if (IVector::equals(buffer, pat, n, tol)) {
            delete buffer;
            return remove(i);
        }
    }

    delete buffer;

    if (LOGGER != nullptr)LOGGER->info(RC::VECTOR_NOT_FOUND, __FILE__, __func__, __LINE__);
    return RC::VECTOR_NOT_FOUND;
}

ISet::IIterator *SetImpl::getIterator(size_t index) const {
    return IteratorImpl::createIterator(controlBlock, index, dim);
}

ISet::IIterator *SetImpl::getBegin() const {
    return IteratorImpl::createIterator(controlBlock, 0, dim);
}

ISet::IIterator *SetImpl::getEnd() const {
    return IteratorImpl::createIterator(controlBlock, size - 1, dim);
}

SetImpl::~SetImpl() noexcept {
    delete[] data;
    delete[] indices;
    controlBlock->updateData(nullptr, nullptr, dim, 0);
}

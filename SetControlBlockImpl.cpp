//
// Created by mikita on 25.04.2021.
//

#include "SetControlBlockImpl.h"
#include "iostream"

ILogger *SetControlBlockImpl::LOGGER = nullptr;

SetControlBlockImpl::SetControlBlockImpl() : data(nullptr), dim(0), indices(nullptr), size(0) {}

SetControlBlockImpl::SetControlBlockImpl(double *data, size_t *indices, size_t dim, size_t size) {
    this->data = data;
    this->dim = dim;
    this->indices = indices;
    this->size = size;
    if (LOGGER != nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
}

RC SetControlBlockImpl::setLogger(ILogger *const logger) {
    if (logger == nullptr)
        return RC::NULLPTR_ERROR;
    LOGGER = logger;
    if (LOGGER != nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

RC SetControlBlockImpl::updateData(double *data, size_t *indices, size_t dim, size_t size) {
    this->data = data;
    this->dim = dim;
    this->indices = indices;
    this->size = size;
    if (LOGGER != nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

size_t SetControlBlockImpl::getIndex(size_t hash) const {
    size_t s = indices[hash] > hash ? -1 : 1;
    //std::cout << "{ " << indices[hash] << "}\n";
    size_t index = hash;
    while (true) {
        if (indices[index] == hash)
            break;
        if (index == size || index == 0)
            return SIZE_MAX;
        index += s;
    }
    return index;
}

RC SetControlBlockImpl::getNext(IVector *const &vec, size_t &index, size_t indexInc) const {
    if (data == nullptr || indices == nullptr) {
        if (LOGGER != nullptr)LOGGER->info(RC::SOURCE_SET_DESTROYED, __FILE__, __func__, __LINE__);
        return RC::SOURCE_SET_DESTROYED;
    }
    if (index + indexInc >= size) {
        if (LOGGER != nullptr)LOGGER->warning(RC::INDEX_OUT_OF_BOUND, __FILE__, __func__, __LINE__);
        index = SIZE_MAX;
        return RC::INDEX_OUT_OF_BOUND;
    }
    size_t newIndex = getIndex(index);  //находим индекс хеша в массиве
    if (newIndex == SIZE_MAX) {
        index = SIZE_MAX;
        if (LOGGER != nullptr)LOGGER->warning(RC::INDEX_OUT_OF_BOUND, __FILE__, __func__, __LINE__);
        return RC::INDEX_OUT_OF_BOUND;
    }
    newIndex += indexInc;
    index = indices[newIndex];  //устанваливаем хеш
    if (vec != nullptr) {
        RC code = vec->setData(dim, &data[newIndex * dim]);
        if (code != RC::SUCCESS)
            return code;
    }
    if (LOGGER != nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

RC SetControlBlockImpl::getPrevious(IVector *const &vec, size_t &index, size_t indexInc) const {
    if (data == nullptr || indices == nullptr) {
        if (LOGGER != nullptr)LOGGER->info(RC::SOURCE_SET_DESTROYED, __FILE__, __func__, __LINE__);
        return RC::SOURCE_SET_DESTROYED;
    }
    if (index - indexInc >= size) {
        if (LOGGER != nullptr)LOGGER->warning(RC::INDEX_OUT_OF_BOUND, __FILE__, __func__, __LINE__);
        index = SIZE_MAX;
        return RC::INDEX_OUT_OF_BOUND;
    }
    size_t newIndex = getIndex(index);  //находим индекс хеша в массиве
    if (newIndex == SIZE_MAX) {
        index = SIZE_MAX;
        if (LOGGER != nullptr)LOGGER->warning(RC::INDEX_OUT_OF_BOUND, __FILE__, __func__, __LINE__);
        return RC::INDEX_OUT_OF_BOUND;
    }
    newIndex -= indexInc;
    index = indices[newIndex];  //устанваливаем хеш
    if (vec != nullptr) {
        RC code = vec->setData(dim, &data[newIndex * dim]);
        if (code != RC::SUCCESS)
            return code;
    }
    if (LOGGER != nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

RC SetControlBlockImpl::getBegin(IVector *const &vec, size_t &index) const {
    if (size == 0) {
        if (LOGGER != nullptr)LOGGER->info(RC::SOURCE_SET_EMPTY, __FILE__, __func__, __LINE__);
        return RC::SOURCE_SET_EMPTY;
    }
    index = indices[0];
    if (vec != nullptr) {
        RC code = vec->setData(dim, data);
        if (code != RC::SUCCESS)
            return code;
    }
    if (LOGGER != nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

RC SetControlBlockImpl::getEnd(IVector *const &vec, size_t &index) const {
    if (size == 0) {
        if (LOGGER != nullptr)LOGGER->info(RC::SOURCE_SET_EMPTY, __FILE__, __func__, __LINE__);
        return RC::SOURCE_SET_EMPTY;
    }
    index = indices[size - 1];
    if (vec != nullptr) {
        RC code = vec->setData(dim, &data[(size - 1) * dim]);
        if (code != RC::SUCCESS)
            return code;
    }
    if (LOGGER != nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

ISetControlBlock::~ISetControlBlock() noexcept {}
//
// Created by mikita on 25.04.2021.
//

#include "SetControlBlockImpl.h"

ILogger* SetControlBlockImpl::LOGGER = nullptr;

SetControlBlockImpl::SetControlBlockImpl(): data(nullptr), dim(0), indices(nullptr), size(0) {}

SetControlBlockImpl::SetControlBlockImpl(double *data, size_t *indices, size_t dim, size_t size) {
    this->data = data;
    this->dim = dim;
    this->indices = indices;
    this->size = size;
    LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
}

RC SetControlBlockImpl::setLogger(ILogger *const logger) {
    if(logger == nullptr)
        return RC::NULLPTR_ERROR;
    LOGGER = logger;
    LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

RC SetControlBlockImpl::updateData(double *data, size_t *indices, size_t size) {
    this->data = data;
    this->dim = dim;
    this->indices = indices;
    this->size = size;
    LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

size_t SetControlBlockImpl::getIndex(size_t hash) const{
    if(hash >= size) {
        LOGGER->warning(RC::INDEX_OUT_OF_BOUND, __FILE__, __func__, __LINE__);
        return size;
    }
    int s = indices[hash] > hash ? -1 : 1;
    int index = hash;
    while (true){
        if(indices[index] == hash || index == size || index == 0)
            break;
        index+=s;
    }
    return index;
}

RC SetControlBlockImpl::getNext(IVector *const &vec, size_t &index, size_t indexInc) const {
    if(data == nullptr || indices == nullptr){
        LOGGER->info(RC::SOURCE_SET_DESTROYED, __FILE__, __func__, __LINE__);
        return RC::SOURCE_SET_DESTROYED;
    }
    size_t newIndex = getIndex(index + indexInc);  //находим индекс хеша в массиве
    if(newIndex == size){
        index = SIZE_MAX;
        LOGGER->warning(RC::INDEX_OUT_OF_BOUND, __FILE__, __func__, __LINE__);
        return RC::INDEX_OUT_OF_BOUND;
    }
    index = indices[newIndex];  //устанваливаем хеш
    if(vec!= nullptr) {
        RC code = vec->setData(dim, &data[newIndex * dim]);
        if (code != RC::SUCCESS)
            return code;
    }
    LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

RC SetControlBlockImpl::getPrevious(IVector *const &vec, size_t &index, size_t indexInc) const {
    if(data == nullptr || indices == nullptr){
        LOGGER->info(RC::SOURCE_SET_DESTROYED, __FILE__, __func__, __LINE__);
        return RC::SOURCE_SET_DESTROYED;
    }
    if(index < indexInc)
        indexInc = index;
    size_t newIndex = getIndex(index - indexInc);  //находим индекс хеша в массиве
    if(newIndex == size){
        index = SIZE_MAX;
        LOGGER->warning(RC::INDEX_OUT_OF_BOUND, __FILE__, __func__, __LINE__);
        return RC::INDEX_OUT_OF_BOUND;
    }
    index = indices[newIndex];  //устанваливаем хеш
    if(vec!= nullptr) {
        RC code = vec->setData(dim, &data[newIndex * dim]);
        if (code != RC::SUCCESS)
            return code;
    }
    LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

RC SetControlBlockImpl::getBegin(IVector *const &vec, size_t &index) const {
    if(size == 0){
        LOGGER->info(RC::SOURCE_SET_EMPTY, __FILE__, __func__, __LINE__);
        return RC::SOURCE_SET_EMPTY;
    }
    index = indices[0];
    if(vec!= nullptr) {
        RC code = vec->setData(dim, data);
        if (code != RC::SUCCESS)
            return code;
    }
    LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

RC SetControlBlockImpl::getEnd(IVector *const &vec, size_t &index) const {
    if(size == 0){
        LOGGER->info(RC::SOURCE_SET_EMPTY, __FILE__, __func__, __LINE__);
        return RC::SOURCE_SET_EMPTY;
    }
    index = indices[size-1];
    if(vec!= nullptr) {
        RC code = vec->setData(dim, &data[(size - 1) * dim]);
        if (code != RC::SUCCESS)
            return code;
    }
    LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

ISetControlBlock::~ISetControlBlock() noexcept {}
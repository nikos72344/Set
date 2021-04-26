//
// Created by mikita on 24.04.2021.
//

#include "IteratorImpl.h"

ILogger* IteratorImpl::LOGGER = nullptr;

IteratorImpl::IteratorImpl() : controlBlock(nullptr), buffer(nullptr), hash(0), dim(0) {}

ISet::IIterator* IteratorImpl::createIterator(std::shared_ptr<ISetControlBlock> controlBlock, size_t index, size_t dim) {
    if(controlBlock == nullptr){
        if(LOGGER !=nullptr)LOGGER->severe(RC::NULLPTR_ERROR, __FILE__, __func__, __LINE__);
        return nullptr;
    }

    double* temp = new double[dim];
    if(temp == nullptr){
        if(LOGGER !=nullptr)LOGGER->severe(RC::ALLOCATION_ERROR, __FILE__, __func__, __LINE__);
        return nullptr;
    }

    for(size_t i = 0; i < dim; i++)
        temp[i] = 0;

    IVector* buffer = IVector::createVector(dim, temp);
    delete temp;
    size_t hash = 0;
    RC code = controlBlock->getNext(buffer, hash, index);
    if(code != RC::SUCCESS){
        if(LOGGER !=nullptr)LOGGER->severe(code, __FILE__, __func__, __LINE__);
        return nullptr;
    }
    IteratorImpl* newIterator = new IteratorImpl;
    if(newIterator == nullptr){
        delete buffer;
        if(LOGGER !=nullptr)LOGGER->severe(RC::ALLOCATION_ERROR, __FILE__, __func__, __LINE__);
        return nullptr;
    }

    newIterator->controlBlock = controlBlock;
    newIterator->buffer = buffer;
    newIterator->hash = hash;
    newIterator->dim = dim;

    if(LOGGER !=nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return newIterator;
}

ISet::IIterator* IteratorImpl::getNext(size_t indexInc) const {
    return createIterator(controlBlock, hash + indexInc, dim);
}

ISet::IIterator* IteratorImpl::getPrevious(size_t indexInc) const {
    return createIterator(controlBlock, hash - indexInc, dim);
}

ISet::IIterator* IteratorImpl::clone() const {
    IteratorImpl* newIterator = new IteratorImpl;
    if(newIterator == nullptr){
        if(LOGGER !=nullptr)LOGGER->severe(RC::ALLOCATION_ERROR, __FILE__, __func__, __LINE__);
        return nullptr;
    }

    newIterator->controlBlock = controlBlock;
    newIterator->buffer = buffer->clone();
    if(newIterator->buffer == nullptr){
        delete newIterator;
        return nullptr;
    }
    newIterator->hash = hash;
    newIterator->dim = dim;

    if(LOGGER !=nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return newIterator;
}

RC IteratorImpl::setLogger(ILogger *const pLogger) {
    if(pLogger == nullptr)
        return RC::NULLPTR_ERROR;
    LOGGER = pLogger;
    if(LOGGER !=nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

RC IteratorImpl::next(size_t indexInc) {
    return controlBlock->getNext(buffer, hash, indexInc);
}

RC IteratorImpl::previous(size_t indexInc) {
    return controlBlock->getPrevious(buffer, hash, indexInc);
}

bool IteratorImpl::isValid() const {
    return hash != SIZE_MAX;
}

RC IteratorImpl::makeBegin() {
    return controlBlock->getBegin(buffer, hash);
}

RC IteratorImpl::makeEnd() {
    return controlBlock->getEnd(buffer, hash);
}

RC IteratorImpl::getVectorCopy(IVector * &val) const {
    val = buffer->clone();
    if(LOGGER !=nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

RC IteratorImpl::getVectorCoords(IVector *const &val) const {
    if(val== nullptr){
        if(LOGGER !=nullptr)LOGGER->warning(RC::NULLPTR_ERROR, __FILE__, __func__, __LINE__);
        return RC::NULLPTR_ERROR;
    }
    if(val->getDim() != buffer->getDim()){
        if(LOGGER !=nullptr)LOGGER->warning(RC::MISMATCHING_DIMENSIONS, __FILE__, __func__, __LINE__);
        return RC::MISMATCHING_DIMENSIONS;
    }

    RC code = val->setData(buffer->getDim(), buffer->getData());
    if(code!=RC::SUCCESS){
        if(LOGGER !=nullptr)LOGGER->warning(code, __FILE__, __func__, __LINE__);
        return code;
    }

    if(LOGGER !=nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return RC::SUCCESS;
}

ISet::IIterator::~IIterator() noexcept {}

IteratorImpl::~IteratorImpl() noexcept {
    delete buffer;
}
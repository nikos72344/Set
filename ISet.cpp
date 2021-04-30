//
// Created by mikita on 25.04.2021.
//

#include "SetImpl.h"

RC ISet::setLogger(ILogger *const logger) {
    return SetImpl::setLogger(logger);
}

ISet *ISet::createSet() {
    return (ISet*)(new SetImpl);
}

ISet* ISet::makeIntersection(const ISet *const &op1, const ISet *const &op2, IVector::NORM n, double tol) {
    ILogger* const LOGGER = SetImpl::getLogger();
    if(op1 == nullptr || op2 == nullptr){
        if(LOGGER !=nullptr)LOGGER->severe(RC::NULLPTR_ERROR, __FILE__, __func__, __LINE__);
        return nullptr;
    }
    if(op1->getDim() != op2->getDim()){
        if(LOGGER !=nullptr)LOGGER->severe(RC::MISMATCHING_DIMENSIONS, __FILE__, __func__, __LINE__);
        return nullptr;
    }

    ISet* newSet = nullptr;
    const ISet* biggerSet = nullptr, *smallerSet = nullptr;
    if(op1->getSize() > op2->getSize()) {
        biggerSet = op1;
        smallerSet = op2;
    }
    else{
        smallerSet = op2;
        biggerSet = op1;
    }
    newSet = smallerSet->clone();
    if(newSet == nullptr)
        return nullptr;

    IVector* buffer = nullptr;
    size_t i = smallerSet->getSize()- 1;
    if(smallerSet->getCopy(i, buffer) != RC::SUCCESS) {
        delete newSet;
        return nullptr;
    }
    RC code = RC::SUCCESS;
    while (true){
        code = biggerSet->findFirst(buffer, n, tol);
        if(code == RC::VECTOR_NOT_FOUND) {
            if (newSet->remove(i) != RC::SUCCESS) {
                delete newSet;
                delete buffer;
                return nullptr;
            }
        }
            else if(code!=RC::SUCCESS){
                delete newSet;
                delete buffer;
                return nullptr;
            }
        if(i == 0)
            break;
        if(smallerSet->getCoords(--i, buffer) != RC::SUCCESS){
            delete newSet;
            delete buffer;
            return nullptr;
        }
    }

    delete buffer;

    if(LOGGER !=nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return newSet;
}

ISet* ISet::makeUnion(const ISet *const &op1, const ISet *const &op2, IVector::NORM n, double tol) {
    ILogger* const LOGGER = SetImpl::getLogger();
    if(op1 == nullptr || op2 == nullptr){
        if(LOGGER !=nullptr)LOGGER->severe(RC::NULLPTR_ERROR, __FILE__, __func__, __LINE__);
        return nullptr;
    }
    if(op1->getDim() != op2->getDim()){
        if(LOGGER !=nullptr)LOGGER->severe(RC::MISMATCHING_DIMENSIONS, __FILE__, __func__, __LINE__);
        return nullptr;
    }

    ISet* newSet = nullptr;
    const ISet* biggerSet = nullptr, *smallerSet = nullptr;
    if(op1->getSize() > op2->getSize()) {
        biggerSet = op1;
        smallerSet = op2;
    }
    else{
        smallerSet = op2;
        biggerSet = op1;
    }
    newSet = biggerSet->clone();
    if(newSet == nullptr)
        return nullptr;

    IVector* buffer = nullptr;
    if(smallerSet->getCopy(0, buffer) != RC::SUCCESS){
        delete newSet;
        return nullptr;
    }
    RC code = newSet->insert(buffer, n, tol);
    if(code != RC::SUCCESS && code != RC::VECTOR_ALREADY_EXIST){
        delete newSet;
        return nullptr;
    }
    for(size_t i = 1; i < smallerSet->getSize(); i++){
        if(smallerSet->getCoords(i, buffer) != RC::SUCCESS){
            delete newSet;
            delete buffer;
            return nullptr;
        }
        newSet->insert(buffer, n, tol);
    }

    delete buffer;

    if(LOGGER !=nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return newSet;
}

ISet* ISet::sub(const ISet *const &op1, const ISet *const &op2, IVector::NORM n, double tol) {
    ILogger* const LOGGER = SetImpl::getLogger();
    if(op1 == nullptr || op2 == nullptr){
        if(LOGGER !=nullptr)LOGGER->severe(RC::NULLPTR_ERROR, __FILE__, __func__, __LINE__);
        return nullptr;
    }
    if(op1->getDim() != op2->getDim()){
        if(LOGGER !=nullptr)LOGGER->severe(RC::MISMATCHING_DIMENSIONS, __FILE__, __func__, __LINE__);
        return nullptr;
    }

    ISet* newSet = op1->clone();
    if(newSet== nullptr)
        return nullptr;

    IVector* buffer = nullptr;
    size_t i = op1->getSize()-1;
    if(op1->getCopy(i, buffer) != RC::SUCCESS){
        delete newSet;
        return nullptr;
    }
    RC code = RC::SUCCESS;
    while (true){
        code = op2->findFirst(buffer, n, tol);
        if(code == RC::SUCCESS) {
            if (newSet->remove(i) != RC::SUCCESS) {
                delete newSet;
                delete buffer;
                return nullptr;
            }
        } else if(code!=RC::VECTOR_NOT_FOUND){
            delete newSet;
            delete buffer;
            return nullptr;
        }
        if(i == 0)
            break;
        if(op1->getCoords(--i, buffer) != RC::SUCCESS){
            delete newSet;
            delete buffer;
            return nullptr;
        }
    }

    delete buffer;

    if(LOGGER !=nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return newSet;
}

ISet* ISet::symSub(const ISet *const &op1, const ISet *const &op2, IVector::NORM n, double tol) {
    ILogger* const LOGGER = SetImpl::getLogger();
    if(op1 == nullptr || op2 == nullptr){
        if(LOGGER !=nullptr)LOGGER->severe(RC::NULLPTR_ERROR, __FILE__, __func__, __LINE__);
        return nullptr;
    }
    if(op1->getDim() != op2->getDim()){
        if(LOGGER !=nullptr)LOGGER->severe(RC::MISMATCHING_DIMENSIONS, __FILE__, __func__, __LINE__);
        return nullptr;
    }

    ISet* newSet = makeUnion(op1, op2, n, tol);
    if(newSet == nullptr)
        return nullptr;

    IVector* buffer = nullptr;
    size_t i = newSet->getSize() - 1;
    if(newSet->getCopy(i, buffer) != RC::SUCCESS){
        delete newSet;
        return nullptr;
    }
    RC codeOne = RC::SUCCESS, codeTwo = RC::SUCCESS;
    while (true){
        codeOne = op1->findFirst(buffer, n, tol);
        codeTwo = op2->findFirst(buffer, n, tol);
        if(codeOne == RC::SUCCESS && codeTwo == RC::SUCCESS) {
            if (newSet->remove(i) != RC::SUCCESS) {
                delete newSet;
                delete buffer;
                return nullptr;
            }
        } else if(codeOne != RC::VECTOR_NOT_FOUND && codeTwo != RC::VECTOR_NOT_FOUND){
            delete newSet;
            delete buffer;
            return nullptr;
        }
        if(i == 0)
            break;
        if(newSet->getCoords(--i, buffer) != RC::SUCCESS){
            delete newSet;
            delete buffer;
            return nullptr;
        }
    }

    delete buffer;

    if(LOGGER !=nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return newSet;
}

bool ISet::equals(const ISet *const &op1, const ISet *const &op2, IVector::NORM n, double tol) {
    ILogger* const LOGGER = SetImpl::getLogger();
    if(op1 == nullptr || op2 == nullptr){
        if(LOGGER !=nullptr)LOGGER->severe(RC::NULLPTR_ERROR, __FILE__, __func__, __LINE__);
        return false;
    }
    if(op1->getDim() != op2->getDim()){
        if(LOGGER !=nullptr)LOGGER->severe(RC::MISMATCHING_DIMENSIONS, __FILE__, __func__, __LINE__);
        return false;
    }
    if(op1->getSize() != op2->getSize()){
        if(LOGGER !=nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
        return false;
    }

    size_t equalVecs = 0;
    IVector* buffer = nullptr;
    if(op1->getCopy(0, buffer) != RC::SUCCESS)
        return false;
    size_t i = 1;
    while (true){
        if(op2->findFirst(buffer, n, tol) == RC::SUCCESS)
            equalVecs++;
        if(i == op1->getSize())
            break;
        if(op1->getCoords(i, buffer) != RC::SUCCESS){
            delete buffer;
            return false;
        }
        i++;
    }

    delete buffer;

    if(LOGGER !=nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return equalVecs == op1->getSize();
}

bool ISet::subSet(const ISet *const &op1, const ISet *const &op2, IVector::NORM n, double tol) {
    ILogger* const LOGGER = SetImpl::getLogger();
    if(op1 == nullptr || op2 == nullptr){
        if(LOGGER !=nullptr)LOGGER->severe(RC::NULLPTR_ERROR, __FILE__, __func__, __LINE__);
        return false;
    }
    if(op1->getDim() != op2->getDim()){
        if(LOGGER !=nullptr)LOGGER->severe(RC::MISMATCHING_DIMENSIONS, __FILE__, __func__, __LINE__);
        return false;
    }
    if(op1->getSize() > op2->getSize()){
        if(LOGGER !=nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
        return false;
    }

    IVector* buffer = nullptr;
    if(op1->getCopy(0, buffer) != RC::SUCCESS)
        return false;
    size_t i = 1;
    while (true){
        if(op2->findFirst(buffer, n, tol) != RC::SUCCESS){
            if(LOGGER !=nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
            delete buffer;
            return false;
        }
        if(i == op1->getSize())
            break;
        RC code =op1->getCoords(i, buffer);
        if(code != RC::SUCCESS && code != RC::VECTOR_NOT_FOUND){
            delete buffer;
            return false;
        }
        i++;
    }

    delete buffer;

    if(LOGGER !=nullptr)LOGGER->info(RC::SUCCESS, __FILE__, __func__, __LINE__);
    return true;
}

ISet::~ISet() noexcept {}

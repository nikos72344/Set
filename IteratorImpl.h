//
// Created by mikita on 24.04.2021.
//

#ifndef SET_ITERATORIMPL_H
#define SET_ITERATORIMPL_H
#include "ISet.h"
#include "ISetControlBlock.h"
#include "memory"


class IteratorImpl : public ISet::IIterator{
private:
    static ILogger* LOGGER;
    std::shared_ptr<ISetControlBlock> controlBlock; //контрольных блок
    IVector* buffer;    //буфферный вектор
    size_t hash;   //уникальный индекс вектора
    size_t dim; //размерность вектора

    IteratorImpl(const IteratorImpl&);
    IteratorImpl& operator=(const IteratorImpl&);
    IteratorImpl();

public:
    static ISet::IIterator* createIterator(std::shared_ptr<ISetControlBlock> controlBlock, size_t index, size_t dim);

    IIterator * getNext(size_t indexInc = 1) const;
    IIterator * getPrevious(size_t indexInc = 1) const;
    IIterator * clone() const;

    RC setLogger(ILogger * const pLogger);

    RC next(size_t indexInc = 1);
    RC previous(size_t indexInc = 1);

    bool isValid() const;

    RC makeBegin();
    RC makeEnd();

    RC getVectorCopy(IVector *& val) const;
    RC getVectorCoords(IVector * const& val) const;

    ~IteratorImpl();
};


#endif //SET_ITERATORIMPL_H

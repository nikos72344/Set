//
// Created by mikita on 25.04.2021.
//

#ifndef SET_SETCONTROLBLOCKIMPL_H
#define SET_SETCONTROLBLOCKIMPL_H

#include "ISetControlBlock.h"

//установка размера буффера, самого буффера и хэшей

class SetControlBlockImpl : public ISetControlBlock {
private:
    static ILogger *LOGGER;
    double *data;
    size_t dim;
    size_t *indices;
    size_t size;

    SetControlBlockImpl();

    size_t getIndex(size_t hash) const; //находит индекс хеша в массиве

public:
    SetControlBlockImpl(double *data, size_t *indices, size_t dim, size_t size);

    RC updateData(double *data, size_t *indices, size_t dim, size_t size);

    RC setLogger(ILogger *const logger);

    RC getNext(IVector *const &vec, size_t &index, size_t indexInc = 1) const;

    RC getPrevious(IVector *const &vec, size_t &index, size_t indexInc = 1) const;

    RC getBegin(IVector *const &vec, size_t &index) const;

    RC getEnd(IVector *const &vec, size_t &index) const;
};


#endif //SET_SETCONTROLBLOCKIMPL_H

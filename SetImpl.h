//
// Created by mikita on 15.04.2021.
//

#ifndef SET_SETIMPL_H
#define SET_SETIMPL_H

#include <memory>
#include <memory.h>

#include "ISet.h"
#include "IVector.h"
#include "ILogger.h"
#include "IteratorImpl.h"
#include "SetControlBlockImpl.h"

class SetImpl: public ISet{
private:
    static const size_t minAlloc = 100;   //кол-во векторов для изначального выделения
    static ILogger* LOGGER;

    double* data;   //массив данных векторов
    size_t* indices;    //массив уникальных индексов векторов
    size_t lastIndex;   //индекс последнего добавленного вектора
    size_t dim;    //размерность векторов
    size_t size;   //кол-во активных векторов
    size_t allocSize;  //кол-во выделенных векторов
    std::shared_ptr<SetControlBlockImpl> controlBlock;  //контрольный блок

    RC insertFirstVec(const IVector* const &val);
    RC addNewVec(const IVector *const &val, IVector::NORM n, double tol);

    SetImpl(const SetImpl& other);
    SetImpl& operator=(const SetImpl& other);

public:
    SetImpl();

    static RC setLogger(ILogger *const logger);
    static ILogger* const getLogger(void);

    ISet* clone() const;

    size_t getDim() const;
    size_t getSize() const;

    /*
     * Method creating new IVector and assigning new address to val
     */
    RC getCopy(size_t index, IVector *& val) const;
    RC findFirstAndCopy(IVector const * const& pat, IVector::NORM n, double tol, IVector *& val) const;

    /*
     * Method copy data from vector in ISet to vector val
     */
    RC getCoords(size_t index, IVector * const& val) const;
    RC findFirstAndCopyCoords(IVector const * const& pat, IVector::NORM n, double tol, IVector * const& val) const;
    RC findFirst(IVector const * const& pat, IVector::NORM n, double tol) const;

    RC insert(IVector const * const& val, IVector::NORM n, double tol);

    RC remove(size_t index);
    RC remove(IVector const * const& pat, IVector::NORM n, double tol);

    IIterator *getIterator(size_t index) const;
    IIterator *getBegin() const;
    IIterator *getEnd() const;

    ~SetImpl();
};


#endif //SET_SETIMPL_H

//
// Created by mikita on 09.04.2021.
//

#ifndef VECTOR_VECTORIMPL_H
#define VECTOR_VECTORIMPL_H

#include "IVector.h"

class VectorImpl : public IVector {
private:
    static ILogger *LOGGER;
    size_t dim;

    RC doSum(double *dest, double const *src, size_t const dim, bool doMinus = false);

    double doChebyshev() const;

    double doFirst() const;

    double doSecond() const;

    VectorImpl();

    VectorImpl(const VectorImpl &vector);

    VectorImpl &operator=(const VectorImpl &vector);

public:

    VectorImpl(size_t dim);

    static RC elemCheck(double elem);

    IVector *clone() const;

    double const *getData() const;

    RC setData(size_t dim, double const *const &ptr_data);

    static RC setLogger(ILogger *const logger);

    static ILogger *const getLogger(void) { return LOGGER; };

    RC getCord(size_t index, double &val) const;

    RC setCord(size_t index, double val);

    RC scale(double multiplier);

    size_t getDim() const;

    RC inc(IVector const *const &op);

    RC dec(IVector const *const &op);

    double norm(NORM n) const;

    RC applyFunction(const std::function<double(double)> &fun);

    RC foreach(const std::function<void(double)> &fun) const;

    size_t sizeAllocated() const;

    ~VectorImpl() {};
};

#endif //VECTOR_VECTORIMPL_H

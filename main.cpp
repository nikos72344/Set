#include "LoggerImpl.h"
#include "VectorImpl.h"
#include <iostream>

using namespace std;

double foo(double d) {
    return d * d;
}

void loo(double d) {
    cout << "[" << d << "]\n";
}

int main() {
    ILogger *logger = ILogger::createLogger("log.txt");
    VectorImpl::setLogger(logger);

    double data[] = {1, 2, 3};

    IVector *one = IVector::createVector(3, data);
    IVector *two = one->clone();
    data[0] = -2;
    RC c = two->setData(sizeof(data) / sizeof(double), data);

    for (size_t i = 0; i < one->getDim(); i++) {
        double t;
        one->getCord(i, t);
        cout << t << " ";
    }
    cout << endl;

    double const *d = two->getData();

    for (size_t i = 0; i < two->getDim(); i++) {
        cout << d[i] << " ";
    }
    cout << endl;

    one->setCord(1, 15.5);

    d = one->getData();

    for (size_t i = 0; i < one->getDim(); i++) {
        cout << d[i] << " ";
    }
    cout << endl;

    two->inc(one);

    for (size_t i = 0; i < two->getDim(); i++) {
        double t;
        two->getCord(i, t);
        cout << t << " ";
    }
    cout << endl;

    IVector *n = VectorImpl::sub(two, two);

    for (size_t i = 0; i < n->getDim(); i++) {
        double t;
        n->getCord(i, t);
        cout << t << " ";
    }
    cout << endl;

    double t;
    n->getCord(0, t);
    two->scale(0);

    for (size_t i = 0; i < two->getDim(); i++) {
        double t;
        two->getCord(i, t);
        cout << t << " ";
    }
    cout << endl;

    delete n;

    n = VectorImpl::add(one, two);

    for (size_t i = 0; i < n->getDim(); i++) {
        double t;
        n->getCord(i, t);
        cout << t << " ";
    }
    cout << endl;

    cout << VectorImpl::dot(n, one) << endl;
    cout << VectorImpl::equals(n, one, IVector::NORM::CHEBYSHEV, 0.001) << " "
         << VectorImpl::equals(n, two, IVector::NORM::CHEBYSHEV, 0.001) << endl;
    n->applyFunction(foo);
    for (size_t i = 0; i < n->getDim(); i++) {
        double t;
        n->getCord(i, t);
        cout << t << " ";
    }

    n->foreach(loo);

    return 0;
}

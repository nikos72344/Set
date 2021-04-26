#include "LoggerImpl.h"
#include "VectorImpl.h"
#include "SetImpl.h"
#include <iostream>

using namespace std;

const double tol = 0.001;

int main() {
    ILogger *logger = ILogger::createLogger("log.txt");
    if (logger == nullptr)
        cerr << "Logger wasn't created! " << __LINE__ << endl;

    RC code = ISet::setLogger(nullptr);
    if (code == RC::SUCCESS)
        cerr << "setLogger doesn't work properly " << __LINE__ << endl;
    code = ISet::setLogger(logger);
    ISet *set1 = ISet::createSet();
    double data = 1;
    IVector *vec1 = IVector::createVector(1, &data);
    code = set1->insert(vec1, IVector::NORM::AMOUNT, tol);
    if (code == RC::SUCCESS)
        cerr << "insert doesn't work properly " << __LINE__ << endl;
    code = set1->insert(vec1, IVector::NORM::CHEBYSHEV, tol);
    if (code != RC::SUCCESS)
        cerr << "insert doesn't work properly " << __LINE__ << endl;
    data = 5;
    vec1->setData(1, &data);
    code = set1->insert(vec1, IVector::NORM::SECOND, tol);
    if (code != RC::SUCCESS)
        cerr << "insert doesn't work properly " << __LINE__ << endl;

    //Проверка итераторов
    ISet::IIterator *iter1 = set1->getBegin();
    if (iter1 == nullptr)
        cerr << "getBegin doesn't work properly " << __LINE__ << endl;
    IVector *temp;
    code = iter1->getVectorCopy(temp);
    if (code != RC::SUCCESS)
        cerr << "getVectorCopy doesn't work properly " << __LINE__ << endl;
    for (size_t i = 0; i < temp->getDim(); i++) {
        temp->getCord(i, data);
        cout << "[" << data << "] ";
    }
    cout << endl;
    code = iter1->next();
    if (code != RC::SUCCESS)
        cerr << "next doesn't work properly " << __LINE__ << endl;
    code = iter1->getVectorCopy(temp);
    if (code != RC::SUCCESS)
        cerr << "getVectorCopy doesn't work properly " << __LINE__ << endl;
    for (size_t i = 0; i < temp->getDim(); i++) {
        temp->getCord(i, data);
        cout << "[" << data << "] ";
    }
    cout << endl;

    ISet::IIterator *iter2 = iter1->getNext();
    if (iter2 != nullptr)
        cerr << "getNext doesn't work properly " << __LINE__ << endl;
    iter2 = iter1->clone();
    if (iter2 == nullptr)
        cerr << "clone doesn't work properly " << __LINE__ << endl;
    code = iter2->getVectorCopy(temp);
    if (code != RC::SUCCESS)
        cerr << "getVectorCopy doesn't work properly " << __LINE__ << endl;
    for (size_t i = 0; i < temp->getDim(); i++) {
        temp->getCord(i, data);
        cout << "[" << data << "] ";
    }
    cout << endl;
    code = iter2->previous();
    if (code != RC::SUCCESS)
        cerr << "previous doesn't work properly " << __LINE__ << endl;
    code = iter2->getVectorCopy(temp);
    if (code != RC::SUCCESS)
        cerr << "getVectorCopy doesn't work properly " << __LINE__ << endl;
    for (size_t i = 0; i < temp->getDim(); i++) {
        temp->getCord(i, data);
        cout << "[" << data << "] ";
    }
    cout << endl;
    code = iter2->previous();
    if (code == RC::SUCCESS)
        cerr << "previous doesn't work properly " << __LINE__ << endl;
    code = iter2->getVectorCopy(temp);
    for (size_t i = 0; i < temp->getDim(); i++) {
        temp->getCord(i, data);
        cout << "[" << data << "] ";
    }
    cout << endl;

    code = set1->remove(2);
    if (code == RC::SUCCESS)
        cerr << "previous doesn't work properly " << __LINE__ << endl;
    code = set1->remove(1);
    if (code != RC::SUCCESS)
        cerr << "previous doesn't work properly " << __LINE__ << endl;
    code = iter1->getVectorCopy(temp);
    if (code != RC::SUCCESS)
        cerr << "getVectorCopy doesn't work properly " << __LINE__ << endl;
    for (size_t i = 0; i < temp->getDim(); i++) {
        temp->getCord(i, data);
        cout << "[" << data << "] ";
    }
    cout << endl;
    cout << iter1->isValid() << endl;

    data = 1;
    vec1->setData(1, &data);
    code = set1->insert(vec1, IVector::NORM::FIRST, tol);
    if (code == RC::SUCCESS)
        cerr << "insert doesn't work properly " << __LINE__ << endl;
    data = 2;
    vec1->setData(1, &data);
    code = set1->insert(vec1, IVector::NORM::FIRST, tol);
    if (code != RC::SUCCESS)
        cerr << "insert doesn't work properly " << __LINE__ << endl;

    cout << iter2->isValid() << endl;
    ISet::IIterator *iter3 = set1->getEnd();
    if (iter3 == nullptr)
        cerr << "getNext doesn't work properly " << __LINE__ << endl;
    code = iter3->getVectorCopy(temp);
    for (size_t i = 0; i < temp->getDim(); i++) {
        temp->getCord(i, data);
        cout << "[" << data << "] ";
    }
    cout << endl;

    code = iter2->makeEnd();
    if (code != RC::SUCCESS)
        cerr << "makeEnd doesn't work properly " << __LINE__ << endl;
    iter2->getVectorCopy(temp);
    for (size_t i = 0; i < temp->getDim(); i++) {
        temp->getCord(i, data);
        cout << "[" << data << "] ";
    }
    cout << endl;
    code = iter2->makeBegin();
    if (code != RC::SUCCESS)
        cerr << "makeBegin doesn't work properly " << __LINE__ << endl;
    code = iter2->getVectorCoords(temp);
    if (code != RC::SUCCESS)
        cerr << "getVectorCoords doesn't work properly " << __LINE__ << endl;
    for (size_t i = 0; i < temp->getDim(); i++) {
        temp->getCord(i, data);
        cout << "[" << data << "] ";
    }
    cout << endl;
    cout << iter2->isValid() << endl;

    cout << iter3->isValid() << endl;

    //Проверка сета
    ISet *set2 = set1->clone();
    if (!set2)
        cerr << "clone doesn't work properly " << __LINE__ << endl;
    double d[] = {1, 2};
    IVector *temp1 = IVector::createVector(2, d);
    code = set2->insert(temp1, IVector::NORM::FIRST, tol);
    if (code == RC::SUCCESS)
        cerr << "insert doesn't work properly " << __LINE__ << endl;

    data = 10;
    temp->setData(1, &data);
    set2->insert(temp, IVector::NORM::CHEBYSHEV, tol);

    data = 20;
    temp->setData(1, &data);
    set1->insert(temp, IVector::NORM::SECOND, tol);

    ISet *set3 = ISet::makeIntersection(set1, set2, IVector::NORM::CHEBYSHEV, tol);
    delete temp1;
    for (size_t i = 0; i < set3->getSize(); i++) {
        set3->getCopy(i, temp1);
        for (size_t j = 0; j < set3->getDim(); j++) {
            temp1->getCord(j, data);
            cout << "{" << data << "} ";
        }
        cout << endl;
    }
    delete set3;

    cout << "-----------------------" << endl;

    set3 = ISet::makeUnion(set1, set2, IVector::NORM::CHEBYSHEV, tol);
    delete temp1;
    for (size_t i = 0; i < set3->getSize(); i++) {
        set3->getCopy(i, temp1);
        for (size_t j = 0; j < set3->getDim(); j++) {
            temp1->getCord(j, data);
            cout << "{" << data << "} ";
        }
        cout << endl;
    }
    delete set3;

    cout << "-----------------------" << endl;

    set3 = ISet::sub(set1, set2, IVector::NORM::CHEBYSHEV, tol);
    delete temp1;
    for (size_t i = 0; i < set3->getSize(); i++) {
        set3->getCopy(i, temp1);
        for (size_t j = 0; j < set3->getDim(); j++) {
            temp1->getCord(j, data);
            cout << "{" << data << "} ";
        }
        cout << endl;
    }
    delete set3;

    cout << "-----------------------" << endl;

    set3 = ISet::sub(set2, set1, IVector::NORM::CHEBYSHEV, tol);
    delete temp1;
    for (size_t i = 0; i < set3->getSize(); i++) {
        set3->getCopy(i, temp1);
        for (size_t j = 0; j < set3->getDim(); j++) {
            temp1->getCord(j, data);
            cout << "{" << data << "} ";
        }
        cout << endl;
    }
    delete set3;

    cout << "-----------------------" << endl;

    set3 = ISet::symSub(set1, set2, IVector::NORM::CHEBYSHEV, tol);
    delete temp1;
    for (size_t i = 0; i < set3->getSize(); i++) {
        set3->getCopy(i, temp1);
        for (size_t j = 0; j < set3->getDim(); j++) {
            temp1->getCord(j, data);
            cout << "{" << data << "} ";
        }
        cout << endl;
    }

    cout << "-----------------------" << endl;
    cout << ISet::equals(set3, set1, IVector::NORM::CHEBYSHEV, tol) << endl;
    cout << ISet::equals(set1, set1, IVector::NORM::CHEBYSHEV, tol) << endl;
    data = 10;
    temp->setData(1, &data);
    set3->remove(temp, IVector::NORM::SECOND, tol);
    cout << ISet::subSet(set3, set1, IVector::NORM::CHEBYSHEV, tol) << endl;
    set3 = ISet::makeUnion(set1, set2, IVector::NORM::CHEBYSHEV, tol);
    cout << ISet::subSet(set1, set3, IVector::NORM::CHEBYSHEV, tol) << endl;

    ISet::IIterator *iter4 = set3->getBegin();
    delete set3;
    code = iter4->next();
    if (code != RC::SOURCE_SET_DESTROYED)
        cerr << "next doesn't work properly " << __LINE__ << endl;

    return 0;
}

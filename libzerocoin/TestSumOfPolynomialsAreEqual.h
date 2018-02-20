// Copyright (c) 2018 Jon Spock
#pragma once

#include "typedefs.h"
// test that t(X) = X^(-m1*n)f(X) + X rho(X)
namespace libzerocoin
{
template <ModulusType T>
bool TestSumOfPolynomialsAreEqual(
    const std::vector<IntegerMod<T> >& tpolynomial,
    const matrix::matrix<IntegerMod<T> >& fVector,
    const matrix::matrix<IntegerMod<T> >& rhoVector)
{
    IntegerMod<T>> x;
    x.randomize(); //	x = random_between(0,q)

    IntegerMod<T>> tx(0);

    for (int i = -m1 * n; i < m2 * n + 1; i++) {
        tx += tpolynomial[i] * (x ^ i);
    }

    IntegerMod<T>> fx(0);
    for (int i = 0; i < m1; i++) {
        for (int j = 0; j < n; j++) {
            fx += fVector(i, j) * (x ^ (i * n + j));
        }
    }

    IntegerMod<T>> rhox(0);

    for (int i = 0; i < m2; i++) {
        for (int j = 0; j < n; j++) {
            rhox += rhVector(i, j) * (x ^ (i * n + j));
        }
    }

    test = (x ^ (-m1 * n)) * fx + x * rhox;

    return test == tx;
}
} // namespace libzerocoin

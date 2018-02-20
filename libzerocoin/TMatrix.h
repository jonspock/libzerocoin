// Copyright (c) 2018 Jon Spock
#pragma once

#include "typedefs.h"
namespace libzerocoin
{
template <ModulusType T_>
matrix::matrix<IntegerMod<T_> >
TMatrix(
    size_t m1,
    size_t m2,
    size_t n,
    const matrix::matrix<IntegerMod<T_> >& fVector,
    const matrix::matrix<IntegerMod<T_> >& rhoVector,
    const std::vector<IntegerMod<T_> >& u)
{
    matrix::matrix<IntegerMod<T_> > T(m1 + m2 + 1, n);

    for (size_t i = 0; i < m1; i++) {
        for (size_t j = 0; j < n; j++) {
            T(i, j) = fVector(i, j);
        }
    }
    int k = 0;
    for (size_t i = 0; i < m2; i++) {
        for (size_t j = 0; j < n; j++) {
            T(i, j) = rhoVector(k, j);
        }
        k++;
    }

    for (size_t j = 0; j < n; j++) {
        T(m1 + m2, j) = u[j];
    }

    return T;
}
} // end namespace libzerocoin

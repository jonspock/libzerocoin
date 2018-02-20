// Copyright (c) 2017 Jon Spock
#pragma once
#include "MultiCommit.h"
#include "typedefs.h"

namespace libzerocoin
{
template <ModulusType T>
void FindVectorsInTMatrix(
    size_t m1,
    size_t m2,
    size_t n,
    const std::vector<IntegerMod<T> >& t,
    matrix::matrix<IntegerMod<T> >& fVector,
    matrix::matrix<IntegerMod<T> >& rhoVector,
    std::vector<IntegerMod<T> >& u)
{
    u.resize(n);
    // We must add this offset to index for t because we want it to replicate Sage
    // code otherwise but that has negative indices while we don't
    int offset = m1 * n;

    /// choose n-1 random components in a vector u.  Set the last componet to 0
    for (size_t i = 0; i < n - 1; i++)
        u[i].randomize();
    u[n - 1] = 0;


    /// the values f_{i,j} (called t_{i,j}' in the original paper
    for (size_t i = 0; i < m1; i++) {
        for (size_t j = 0; j < n; j++) {
            fVector(i, j) = t[i * n + j];
        }
    }

    /// the values rho_{i,j} (called t_{i,j}'' in the original paper
    for (size_t i = 0; i < m2; i++) {
        for (size_t j = 0; j < n; j++) {
            rhoVector(i, j) = t[i * n + j + 1 + offset];
        }
    }

    for (size_t j = 1; j < n; j++) {
        rhoVector(0, j) = rhoVector(0, j) - u[j - 1];
    }
}

} // end namespace libzerocoin

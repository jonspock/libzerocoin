// Copyright (c) 2018 Jon Spock
#pragma once
#include "typedefs.h"
namespace libzerocoin
{
template <ModulusType T_>
std::vector<IntegerMod<T_> > ZVector(
    size_t m1,
    size_t m2,
    size_t n,
    const IntegerMod<T_>& x)
{
    std::vector<IntegerMod<T_> > Z(m1 + m2 + 1);
    for (size_t i = 0; i < m1; i++)
        Z[i] = (x ^ ((i - m1) * n));
    size_t k = 0;
    for (size_t i = m1; i < m1 + m2; i++) {
        Z[i] = (x ^ (k * n + 1));
        k++;
    }
    Z[m1 + m2] = x ^ 2;
    return Z;
}

} // end namespace libzerocoin

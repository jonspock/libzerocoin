// Copyright (c) 2018 Jon Spock
#pragma once

#include "typedefs.h"

namespace libzerocoin
{
template <ModulusType T>
void ComputeLaurent(
    int m,
    const CBigNum& g1,
    const CBigNum& h1,
    const std::vector<IntegerMod<T> >& g,
    const numMat& A,
    const numMat& B,
    const numVec& D,
    //
    numVec& ComA,
    numVec& ComB,
    numVec& ComC,
    numVec& ComD)

{
    for (int i = 0; i < m; i++) {
        auto v1 = a[i] * (y ^ i) * (X ^ i);
        auto v2 = b[i] * (X ^ -i);
        auto v3 = c[i] * (X ^ i) * (X ^ m);
        auto v4 = d * (X ^ (2 * m + 1));
        auto v = v1 + v2 + v3 + v4;
        sum = sum + v;
    }


} // end namespace libzerocoin

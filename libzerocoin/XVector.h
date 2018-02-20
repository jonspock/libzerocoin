// Copyright (c) 2018 Jon Spock
#pragma once

#include "typedefs.h"

namespace libzerocoin
{
template <ModulusType T_>
std::vector<IntegerMod<T_> > XVector(
    size_t n,
    const IntegerMod<T_>& x)
{
    std::vector<IntegerMod<T_> > X(n);
    for (size_t i = 0; i < n; i++)
        X[i] = (x ^ i);
    return X;
}

} // end namespace libzerocoin

// Copyright (c) 2018 Jon Spock
#pragma once
#include "typedefs.h"

namespace libzerocoin
{
std::vector<CBigNum> VectorTimesConstant(
    const std::vector<CBigNum>& v,
    const CBigNum& t)
{
    std::vector<CBigNum> L(v);
    for (size_t i = 0; i < v.size(); i++)
        L[i] = v[i] * t;
    return L;
}

template <ModulusType T_>
std::vector<IntegerMod<T_> > VectorTimesConstant(
    const std::vector<IntegerMod<T_> >& v,
    const IntegerMod<T_>& t)
{
    std::vector<IntegerMod<T_> > L(v);
    for (size_t i = 0; i < v.size(); i++)
        L[i] = v[i] * t;
    return L;
}

} // end namespace libzerocoin

// Copyright (c) 2018 Jon Spock
#pragma once
#include "typedefs.h"

namespace libzerocoin
{
template <ModulusType T_>
std::vector<IntegerMod<T_> > AddVectors(const std::vector<IntegerMod<T_> >& v1,
    const std::vector<IntegerMod<T_> >& v2)
{
    std::vector<IntegerMod<T_> > L(v1);
    for (size_t i = 0; i < v1.size(); i++)
        L[i] = v1[i] + v2[i];
    return L;
}

} // end namespace libzerocoin

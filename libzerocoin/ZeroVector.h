// Copyright (c) 2018 Jon Spock
#pragma once
#include "typedefs.h"
namespace libzerocoin
{
template <ModulusType T_>
std::vector<IntegerMod<T_> > ZeroVector(size_t n)
{
    std::vector<IntegerMod<T_> > Z(n);
    for (size_t i = 0; i < n; i++)
        Z[i] = 0;
    return Z;
}
} // end namespace libzerocoin

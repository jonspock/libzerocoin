// Copyright (c) 2017 Jon Spock
#pragma once

#include "ZeroVector.h"
#include "typedefs.h"

namespace libzerocoin
{
template <ModulusType T_>
std::vector<IntegerMod<T_> > UnitVector(int i, size_t n)
{
    std::vector<IntegerMod<T_> > Z = ZeroVector<T_>(n - 1);
    Z[i] = 1;
    return Z;
}
} // end namespace libzerocoin

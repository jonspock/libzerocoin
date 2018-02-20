// Copyright (c) 2018 Jon Spock
#pragma once
#include "typedefs.h"

namespace libzerocoin
{
template <ModulusType T_>
IntegerMod<T_> DotProduct(
    const std::vector<IntegerMod<T_> >& s,
    const std::vector<IntegerMod<T_> >& t)
{
    int n = s.size();
    IntegerMod<T_> c = s[0] * t[0];

    for (int i = 1; i < n; i++) {
        c += s[i] * t[i];
    }

    return c;
}

} // end namespace libzerocoin

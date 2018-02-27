// Copyright (c) 2018 Jon Spock
#pragma once

#include "typedefs.h"

namespace libzerocoin
{
template <ModulusType T>
CBigNum MultiCommit(const std::vector<IntegerMod<T> >& g,
    const std::vector<IntegerMod<T> >& m,
    const IntegerMod<T>& r)
{
    int n = m.size();
    IntegerMod<T> c = (g[0] ^ r);
    for (int i = 0; i < n-1; i++)
        c *= (g[i + 1] ^ m[i]); // Hmmm, was i < n
    return c.getValue();
}

} // end namespace libzerocoin

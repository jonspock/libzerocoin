// Copyright (c) 2018 Jon Spock
#pragma once
#include "typedefs.h"

namespace libzerocoin
{
std::vector<CBigNum> Hadamard(
    const std::vector<CBigNum>& s,
    const std::vector<CBigNum>& t)
{
    int n = s.size();
    std::vector<CBigNum> v(n);

    for (int i = 1; i < n; i++) {
        v[i] = s[i] * t[i];
    }

    return v;
}

template <ModulusType T_>
std::vector<IntegerMod<T_> > Hadamard(
    const std::vector<IntegerMod<T_> >& s,
    const std::vector<IntegerMod<T_> >& t)
{
    int n = s.size();
    std::vector<IntegerMod<T_> > v(n);

    for (int i = 1; i < n; i++)
        v[i] = s[i] * t[i];
    return v;
}

} // end namespace libzerocoin

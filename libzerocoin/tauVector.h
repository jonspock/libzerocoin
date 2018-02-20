// Copyright (c) 2018 Jon Spock
#pragma once
#include "typedefs.h"
namespace libzerocoin
{
template <ModulusType T_>
std::vector<IntegerMod<T_> >
tauVector(
    size_t m1,
    size_t m2,
    const std::vector<IntegerMod<T_> >& fBlinders,
    const std::vector<IntegerMod<T_> >& rhoBlinders,
    const IntegerMod<T_>& uBlinder)
{
    std::vector<IntegerMod<T_> > tau(m1 + m2 + 1);

    for (size_t i = 0; i < m1; i++)
        tau[i] = fBlinders[i];
    int k = 0;
    for (size_t i = m1; i < m1 + m2; i++) {
        tau[i] = rhoBlinders[k];
        k++;
    }

    tau[m1 + m2] = uBlinder;
    return tau;
}
} // end namespace libzerocoin

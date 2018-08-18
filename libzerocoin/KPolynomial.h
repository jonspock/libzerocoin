// Copyright (c) 2018 Jon Spock
#pragma once
#include "typedefs.h"
template <ModulusType T_>
IntegerMod<T_> KPolynomial(
    int n,
    int m,
    int N,
    const std::vector<IntegerMod<T_> >& K,
    const IntegerMod<T_>& Y)
{
    IntegerMod<T_> KPoly;
    for (size_t k = 0; k < K.size(); k++)
      KPoly += (Y ^ (int64_t)(N + m + k + 1));
    return KPoly;
}

// Copyright (c) 2018 Jon Spock
#pragma once
#include "typedefs.h"
template <ModulusType T_>
std::vector<IntegerMod<T_> > YDashPolynomial(
    int n,
    int m,
    const IntegerMod<T_>& Y)
{
    std::vector<IntegerMod<T_> > YDash(n);
    for (int k = 0; k < n; k++)
        YDash[k] = (Y ^ ((k + 1) * m));
    return YDash;
}

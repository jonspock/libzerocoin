// Copyright (c) 2018 Jon Spock
#pragma once
#include "typedefs.h"
template <ModulusType T_>
matrix::matrix<IntegerMod<T_> > wAjPolynomial(
    int n,
    int m,
    int N,
    const tensor::tensor<IntegerMod<T_> >& wA,
    const IntegerMod<T_>& Y)
{
    int wALen = 4 * N - 2;
    matrix::matrix<IntegerMod<T_> > wAPoly(m, n);

    std::vector<IntegerMod<T_> > y_exp(wALen);

    for (int i = 0; i < wALen; i++)
        y_exp[i] = (Y ^ (N + m + i + 1));

    for (int j = 0; j < m; j++) {
        for (int k = 0; k < n; k++) {
            IntegerMod<T_> sum(0);
            for (int i = 0; i < wALen; i++) {
                IntegerMod<T_> val = wA(i, j, k) * y_exp[i]; ///(Y ^ ( N + m + i + 1 ));
                sum += val;                                  // Not sure if should remain in SubGroup or not
            }
            wAPoly(j, k) = sum;
        }
    }

    return wAPoly;
}

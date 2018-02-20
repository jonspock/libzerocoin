// Copyright (c) 2017 Jon Spock
#pragma once

#include "typedefs.h"
#include <cmath> // for floor

namespace libzerocoin
{
template <ModulusType T_>
void SetWireValues(size_t m,
    size_t n,
    size_t N,
    const IntegerMod<T_>& S,                      // Serial Number
    const std::vector<IntegerMod<T_> >& binary_v, // Randomness
    const IntegerMod<T_>& a,                      // Group Element 'a'
    const IntegerMod<T_>& b,                      // Group Element 'b'

    matrix::matrix<IntegerMod<T_> >& A,
    matrix::matrix<IntegerMod<T_> >& B,
    matrix::matrix<IntegerMod<T_> >& C)
{
    A.resize(m, n);
    B.resize(m, n);
    C.resize(m, n);

    // Clear Matrices first
    A.reset();
    B.reset();
    C.reset();

    for (size_t i = 0; i < N; i++) {
        int i_div_n = floor((double)i / n);
        int i_mod_n = i % n;
        A(i_div_n, i_mod_n) = binary_v[i];
        B(i_div_n, i_mod_n) = binary_v[i] - IntegerMod<T_>(1);
    }

    IntegerMod<T_> product(1);
    int k = 0;

    for (size_t i = N; i < 2 * N; i++) {
        int i_div_n = floor((double)i / n);
        int i_mod_n = i % n;
        int exp = (1 << k) - 1;
        IntegerMod<T_> val = binary_v[k] * ((b ^ CBigNum(exp)) + IntegerMod<T_>(1));
        exp = (1 << (k + 1)) - 1;
        IntegerMod<T_> b_val = (binary_v[k + 1] * (b ^ CBigNum(exp))) + IntegerMod<T_>(1);
        product *= val;
        A(i_div_n, i_mod_n) = product;
        B(i_div_n, i_mod_n) = b_val;
        C(i_div_n, i_mod_n) = product * B(i_div_n, i_mod_n);

        if (i == 2 * N - 2) {
            A(i_div_n, i_mod_n) *= (a ^ S);
            C(i_div_n, i_mod_n) *= (a ^ S);
        }
        k++;
    }
}
} // end namespace libzerocoin

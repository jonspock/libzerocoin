// Copyright (c) 2018 Jon Spock
#pragma once
#include "Binary.h"
#include "MultiCommit.h"
#include "SetWireValues.h"
#include "typedefs.h"

namespace libzerocoin
{
template <ModulusType T_>
void ProveSOK_Step1(
    size_t m,
    size_t n,
    size_t N,
    std::vector<IntegerMod<ACCUMULATOR_MODULUS> >& ck,
    IntegerMod<T_> S,
    IntegerMod<T_> a,
    IntegerMod<T_> b,
    IntegerMod<T_> y1,
    IntegerMod<T_> v,
    IntegerMod<T_> r,
    // Outputs
    std::vector<IntegerMod<T_> >& alpha,
    std::vector<IntegerMod<T_> >& beta,
    std::vector<IntegerMod<T_> >& gamma,
    IntegerMod<T_>& delta,
    matrix::matrix<IntegerMod<T_> >& A,
    matrix::matrix<IntegerMod<T_> >& B,
    matrix::matrix<IntegerMod<T_> >& C,
    std::vector<IntegerMod<T_> >& ComA,
    std::vector<IntegerMod<T_> >& ComB,
    std::vector<IntegerMod<T_> >& ComC,
    IntegerMod<T_>& ComD)
{
    // Step 1 - Generate Commitments
    delta.randomize(); // = random_between(0,q)

    for (size_t i = 0; i < m; i++) {
        alpha[i].randomize();
        beta[i].randomize();
        gamma[i].randomize();
    }

    /// wireValues
    std::vector<IntegerMod<T_> > binary_v = Binary<T_>(v.getValue());

    SetWireValues(m, n, N, S, binary_v, a, b,
        // outputs
        A, B, C);

    std::vector<IntegerMod<T_> > D(n);
    for (size_t i = 0; i < n; i++)
        D[i].randomize();

    /// commitments
    for (size_t i = 0; i < m; i++) {
        ComA[i] = MultiCommit<T_>(ck, A.get_row(i), alpha[i]);
        ComB[i] = MultiCommit<T_>(ck, B.get_row(i), beta[i]);
        ComC[i] = MultiCommit<T_>(ck, C.get_row(i), gamma[i]);
    }
    ComC[m - 1] = y1; //??
    gamma[m - 1] = r;

    ComD = MultiCommit<T_>(ck, D, delta);
}
} // namespace libzerocoin

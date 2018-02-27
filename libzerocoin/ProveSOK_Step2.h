// Copyright (c) 2018 Jon Spock
#pragma once
#include "KPolynomial.h"
#include "hash.h"
#include "setConstraints.h"
#include "typedefs.h"
#include "wAjPolynomial.h"

template <ModulusType T_>
void ProveSOK_Step2(
    size_t m,
    size_t n,
    size_t N,
    const CBigNum& message,
    const IntegerMod<T_>& S,

    const std::vector<IntegerMod<T_> >& K,
    const tensor::tensor<IntegerMod<T_> >& wA,
    const tensor::tensor<IntegerMod<T_> >& wB,
    const tensor::tensor<IntegerMod<T_> >& wC,

    const std::vector<IntegerMod<T_> >& ComA,
    const std::vector<IntegerMod<T_> >& ComB,
    const std::vector<IntegerMod<T_> >& ComC,
    const IntegerMod<T_>& ComD,
    // Outputs
    matrix::matrix<IntegerMod<T_> >& wAj,
    matrix::matrix<IntegerMod<T_> >& wBj,
    matrix::matrix<IntegerMod<T_> >& wCj,
    IntegerMod<T_>& Kconst,
    IntegerMod<T_>& y

)
{
    //////////////////////////////////////////////////////////////////////
    ///Step 2 - Challenge Component and evaluate w polynomials ////////////
    //////////////////////////////////////////////////////////////////////
    CHashWriter h(0, 0);
    h << message << ComD;
    for (size_t i = 0; i < m; i++)
        h << ComA[i] << ComB[i] << ComC[i];

    CBigNum hash_num;
    hash_num.setuint256(h.GetHash());

    IntegerMod<T_> yy(hash_num);
    y = yy;

    /// Most lengthy step here.  Might be worth using a multiexponentiation algorithm instead.
    wAj = wAjPolynomial(n, m, N, wA, y);
    wBj = wAjPolynomial(n, m, N, wB, y);
    wCj = wAjPolynomial(n, m, N, wC, y);

    Kconst = KPolynomial(n, m, N, K, y);
}

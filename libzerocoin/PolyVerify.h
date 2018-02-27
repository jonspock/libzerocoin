// Copyright (c) 2018 Jon Spock
#pragma once

#include "DotProduct.h"
#include "MultiCommit.h"
#include "XVector.h"
#include "typedefs.h"

namespace libzerocoin
{
template <ModulusType T_>
IntegerMod<T_> PolyVerify(
    size_t m1,
    size_t m2,
    size_t n,
    const std::vector<IntegerMod<T_> >& ck,
    const std::vector<IntegerMod<T_> >& tbar,
    const IntegerMod<T_>& taubar,

    const std::vector<IntegerMod<T_> >& Tf,
    const std::vector<IntegerMod<T_> >& Trho,
    const IntegerMod<T_>& U,

    const IntegerMod<T_>& x)
{
    IntegerMod<T_> v;
    auto C = MultiCommit<T_>(ck, tbar, taubar);


    IntegerMod<T_> test(1);
    for (size_t i = 0; i < m1; i++) {
        auto T = Tf[i] ^ (x ^ ((i - m1) * n));
        test *= T;
    }
    for (size_t i = 0; i < m2; i++) {
        auto T = Trho[i] ^ (x ^ ((i * n) + 1));
        test *= T;
    }

    auto T = U ^ (x ^ 2);
    test *= T;

    auto X = XVector(n, x);
    v = DotProduct<T_>(tbar, X);
    return v;
}

} // end namespace libzerocoin

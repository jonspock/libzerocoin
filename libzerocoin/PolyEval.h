// Copyright (c) 2018 Jon Spock
#pragma once

#include "TMatrix.h"
#include "ZVector.h"
#include "tauVector.h"
#include "typedefs.h"

namespace libzerocoin
{
template <ModulusType T_>
void PolyEval(
    size_t m1,
    size_t m2,
    size_t n,
    const matrix::matrix<IntegerMod<T_> >& fVector,
    const matrix::matrix<IntegerMod<T_> >& rhoVector,
    const std::vector<IntegerMod<T_> >& u,
    const std::vector<IntegerMod<T_> >& fBlinders,
    const std::vector<IntegerMod<T_> >& rhoBlinders,
    const IntegerMod<T_>& uBlinder,
    const IntegerMod<T_>& x,

    std::vector<IntegerMod<T_> >& tbar,
    IntegerMod<T_>& taubar)
{
    std::vector<IntegerMod<T_> > Z = ZVector<T_>(m1, m2, n, x);
    matrix::matrix<IntegerMod<T_> > T = TMatrix<T_>(m1, m2, n, fVector, rhoVector, u);

    tbar = Z * T; 

    auto tau = tauVector<T_>(m1, m2, fBlinders, rhoBlinders, uBlinder);
    taubar = DotProduct<T_>(Z, tau);
}

} // end namespace libzerocoin

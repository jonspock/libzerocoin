// Copyright (c) 2018 Jon Spock
#pragma once

#include "MultiCommit.h"
#include "typedefs.h"

namespace libzerocoin
{
template <ModulusType T_>
std::vector<IntegerMod<T_> > FindAtPolynomial(
    size_t m1,
    size_t m2,
    size_t n)
{
    std::vector<IntegerMod<T_> > tpoly(m1 * n + m2 * n + 1);

    /// These are negative indices
    for (size_t i = 0; i < m1 * n; i++) {
        tpoly[i].randomize(); // should check for 0
    }
    tpoly[m1 * n] = 0;

    /// These are negative indices
    for (size_t i = 0; i < m2 * n; i++) {
        tpoly[m1 * n + i].randomize(); // should check for 0
    }
    return tpoly;
}

} // end namespace libzerocoin

// Copyright (c) 2018 Jon Spock
#pragma once

#include "FindVectorsInTMatrix.h"
#include "MultiCommit.h"
#include "typedefs.h"

namespace libzerocoin
{
template <ModulusType T>
void PolyCommit(
    size_t m1,
    size_t m2,
    size_t n,
    const std::vector<IntegerMod<T> >& t,
    const std::vector<IntegerMod<T> >& ck,
    std::vector<IntegerMod<T> >& fBlinders,
    std::vector<IntegerMod<T> >& rhoBlinders,
    IntegerMod<T>& uBlinder,
    std::vector<IntegerMod<T> >& Tf,
    std::vector<IntegerMod<T> >& Trho,
    IntegerMod<T>& U,
    matrix::matrix<IntegerMod<T> >& fVector,
    matrix::matrix<IntegerMod<T> >& rhoVector,
    std::vector<IntegerMod<T> >& u)
{
    FindVectorsInTMatrix<T>(m1, m2, n, t, fVector, rhoVector, u);

    // Randomize
    fBlinders.resize(m1);
    for (size_t i = 0; i < fBlinders.size(); i++)
        fBlinders[i].randomize();

    rhoBlinders.resize(m2);
    for (size_t i = 0; i < rhoBlinders.size(); i++)
        rhoBlinders[i].randomize();

    uBlinder.randomize();

    Tf.resize(m1);
    for (size_t i = 0; i < m1; i++) {
        // ck needs to be vector here
        Tf[i] = MultiCommit<T>(ck, fVector.get_row(i), fBlinders[i]); // Ti'
    }

    Trho.resize(m2);
    for (size_t i = 0; i < m2; i++) {
        Trho[i] = MultiCommit<T>(ck, rhoVector.get_row(i), rhoBlinders[i]); // Ti''
    }

    U = MultiCommit<T>(ck, u, uBlinder); // U
}
} // end namespace libzerocoin

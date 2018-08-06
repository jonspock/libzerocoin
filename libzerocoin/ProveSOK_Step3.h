// Copyright (c) 2018 Jon Spock
#pragma once
#include "DotProduct.h"
#include "Hadamard.h"
#include "VectorTimesConstant.h"
#include "YDashPolynomial.h"
#include "typedefs.h"

namespace libzerocoin
{
template <ModulusType T_>
void ProveSOK_Step3(
    size_t m,
    size_t n,
    size_t N,
    size_t m1dash,
    size_t m2dash,
    size_t ndash,
    const std::vector<IntegerMod<ACCUMULATOR_MODULUS> >& ck,
    const CBigNum& message,
    const IntegerMod<ACCUMULATOR_MODULUS>& S, // Serial Number
    const IntegerMod<T_>& a,
    const IntegerMod<T_>& b,
    const IntegerMod<ACCUMULATOR_MODULUS>& y1,
    const IntegerMod<ACCUMULATOR_MODULUS>& v,
    const IntegerMod<ACCUMULATOR_MODULUS>& r,
    const IntegerMod<T_>& Y,


    const IntegerMod<T_>& Kconst,

    const matrix::matrix<IntegerMod<ACCUMULATOR_MODULUS> >& A,
    const matrix::matrix<IntegerMod<ACCUMULATOR_MODULUS> >& B,
    const matrix::matrix<IntegerMod<ACCUMULATOR_MODULUS> >& C,
    const std::vector<IntegerMod<ACCUMULATOR_MODULUS> >& D,

    const matrix::matrix<IntegerMod<T_> >& wAj,
    const matrix::matrix<IntegerMod<T_> >& wBj,
    const matrix::matrix<IntegerMod<T_> >& wCj,

    // Outputs
    std::vector<IntegerMod<ACCUMULATOR_MODULUS> >& fBlinders,
    std::vector<IntegerMod<ACCUMULATOR_MODULUS> >& rhoBlinders,

    IntegerMod<ACCUMULATOR_MODULUS>& uBlinder,
    std::vector<IntegerMod<ACCUMULATOR_MODULUS> >& Tf,
    std::vector<IntegerMod<ACCUMULATOR_MODULUS> >& Trho,
    IntegerMod<ACCUMULATOR_MODULUS>& U,

    matrix::matrix<IntegerMod<ACCUMULATOR_MODULUS> >& fVector,
    matrix::matrix<IntegerMod<ACCUMULATOR_MODULUS> >& rhoVector,

    std::vector<IntegerMod<ACCUMULATOR_MODULUS> >& u,

    matrix::matrix_zero<IntegerMod<T_> >& rPoly,
    matrix::matrix_zero<IntegerMod<T_> >& sPoly,
    matrix::matrix_zero<IntegerMod<T_> >& rDashPoly


)

{
    ///Step 3 - Laurent Polynomial /////////////////////////////////////////
    std::vector<IntegerMod<T_> > yDash = YDashPolynomial(n, m, Y);

    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            rPoly(i, j) = A(i, j) * (Y ^ (i + 1));
            rPoly(-i, j) = B(i, j);
            rPoly(i + m, j) = C(i, j);
        }
    }

    //rPoly = {2*m + 1 : D , 0 : zero_vector(n)};
    for (size_t j = 0; j < n; j++) {
        rPoly(2 * m + 1, j) = D[j];
        rPoly(0, j) = 0;
    }


    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            sPoly(i, j) = wAj(i, j) * (Y ^ (-i + 1));
            sPoly(-i, j) = wBj(i, j);
            sPoly(-i - m, j) = wCj(i, j);
        }
    }

    //sPoly = {0 : zero_vector(n)};
    for (size_t j = 0; j < n; j++) {
        sPoly(0, j) = 0;
    }


    for (size_t i = 0; i < m; i++) {
        std::vector<IntegerMod<T_> > v1 = Hadamard(rPoly.get_row(-i), yDash);
        std::vector<IntegerMod<T_> > v2 = Hadamard(rPoly.get_row(i), yDash);
        std::vector<IntegerMod<T_> > v3 = Hadamard(rPoly.get_row(i + m), yDash);
        for (size_t j = 0; j < n; j++) {
            rDashPoly(-i - m, j) = sPoly(-i - m, j) * 2;
            rDashPoly(-i, j) = v1[j] + 2 * sPoly(-i, j);
            rDashPoly(i, j) = v2[j] + 2 * sPoly(i, j);
            rDashPoly(i + m, j) = v3[j];
        }
    }

    //rDashPoly = {2*m + 1 : hadamard(D,yDash), 0 : zero_vector(n)}
    std::vector<IntegerMod<T_> > v4 = Hadamard(D, yDash);
    for (size_t j = 0; j < n; j++) {
        rDashPoly(2 * m + 1, j) = v4[j];
        rDashPoly(0, j) = 0;
    }


    /// This is the polynomial multiplication problem and FFTs could speed it up if it's too slow.
    matrix::vector_zero<IntegerMod<T_> > tPoly(-3 * m, 4 * m + 3);

    for (size_t k = -3 * m; k < 4 * m + 3; k++) {
        IntegerMod<T_> tcoef(0);
        for (size_t i = std::max(k - 2 * m - 1, -m); i < std::min(k + 2 * m, 2 * m + 1) + 1; i++) {
            int j = k - i;
            tcoef += DotProduct<T_>(rPoly.get_row(i), rDashPoly.get_row(j));
        }
        tPoly[k] = tcoef;
    }

    ///sanity check
    assert(tPoly[0] != 2 * Kconst); // print "error at tPoly[0]"

    tPoly[0] = 0;

    std::vector<IntegerMod<T_> > t = tPoly.get_data();

    // (pc, st) = PolyCommit(ck,m1dash,m2dash,ndash,tPoly)
    PolyCommit<T_>(m1dash, m2dash, ndash, t, // tPoly,
        ck,
        // Outputs
        fBlinders, rhoBlinders,
        uBlinder, Tf, Trho, U, fVector, rhoVector, u);
}
} // namespace libzerocoin

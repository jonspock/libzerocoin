// Copyright (c) 2018 Jon Spock
#pragma once
#include "AddVectors.h"
#include "DotProduct.h"
#include "Hadamard.h"
#include "VectorTimesConstant.h"
#include "YDashPolynomial.h"
#include "typedefs.h"

namespace libzerocoin
{
template <ModulusType T_>
void ProveSOK_Step4(
    size_t m,
    size_t n,
    size_t N,
    size_t m1dash,
    size_t m2dash,
    size_t ndash,
    const matrix::matrix_zero<IntegerMod<T_> >& rPoly,

    const std::vector<IntegerMod<ACCUMULATOR_MODULUS> >& fBlinders,
    const std::vector<IntegerMod<ACCUMULATOR_MODULUS> >& rhoBlinders,
    const std::vector<IntegerMod<ACCUMULATOR_MODULUS> >& u,

    const IntegerMod<ACCUMULATOR_MODULUS>& uBlinder,
    const std::vector<IntegerMod<ACCUMULATOR_MODULUS> >& Tf,
    const std::vector<IntegerMod<ACCUMULATOR_MODULUS> >& Trho,
    const IntegerMod<ACCUMULATOR_MODULUS>& U,

    const matrix::matrix<IntegerMod<ACCUMULATOR_MODULUS> >& fVector,
    const matrix::matrix<IntegerMod<ACCUMULATOR_MODULUS> >& rhoVector,

    const IntegerMod<T_>& delta,
    const IntegerMod<T_>& y,

    const std::vector<IntegerMod<T_> >& alpha,
    const std::vector<IntegerMod<T_> >& beta,
    const std::vector<IntegerMod<T_> >& gamma,


    std::vector<IntegerMod<T_> >& r_vec,
    IntegerMod<ACCUMULATOR_MODULUS>& rho


)

{
    // sizes???
    std::vector<IntegerMod<ACCUMULATOR_MODULUS> > tbar;
    IntegerMod<ACCUMULATOR_MODULUS> taubar;


    std::cout << "Start Step4\n";
    ///////////////////////////////////////////////////////////////////////
    ///Step 4 - Challenge component ///////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////

    CHashWriter hh;
    hh << U;
    for (size_t i = 0; i < Tf.size(); i++)
        hh << Tf[i];
    for (size_t i = 0; i < Trho.size(); i++)
        hh << Trho[i];

    // Convert to uint256 -> CBigNum -> SubGroup
    uint256 hh_hash = hh.GetHash();
    CBigNum c_hash(hh_hash);
    IntegerMod<T_> x(c_hash);

    ///////////////////////////////////////////////////////////////////////
    ///Step 5 - PolyEval inner product not used (yet!)
    //////////////////////////////////////////////////////////////////////

    PolyEval<T_>(m1dash, m2dash, ndash, fVector, rhoVector, u,
        fBlinders, rhoBlinders, uBlinder, x,
        // Outputs
        tbar, taubar);

    std::cout << "PolyEval done\n";
    /*
    std::cout << "rPoly # rows = " << rPoly.num_rows() << "\n";
    std::cout << "rPoly # cols = " << rPoly.num_cols() << "\n";
    std::cout << "rvec size = " << r_vec.size() << "\n";
    */

    auto r_vec_coef = r_vec;
    for (int i = 0; i < rPoly.num_rows(); i++) {
        r_vec_coef = VectorTimesConstant(rPoly.get_iterated_row(i), x ^ i); // Use iterated row to go avoid needing offset
        //        std::cout << "Adding vectors r_vec + r_vec_coef for i = " << i << " r_vec size " << r_vec.size() << " , r_vec_coef size " << r_vec_coef.size() << " ";
        r_vec = AddVectors(r_vec, r_vec_coef);
        //        std::cout << "...\n";
    }

    std::cout << "r_vec done\n";

    std::cout << "alpha.size = " << alpha.size() << "\n";
    std::cout << "beta.size = " << beta.size() << "\n";
    std::cout << "gamma.size = " << gamma.size() << "\n";


    exit(0);

    rho = delta * (x ^ (int64_t)(2 * m + 1));
    auto xm = x ^ (int64_t)m;

    for (size_t i = 1; i < m + 1; i++) {
        auto xi = (x ^ (int64_t)i);
        rho += alpha[i - 1] * xi * (y ^ (int64_t)i);
        rho += beta[i - 1] * (x ^ ((int64_t)-i));
        rho += xm * gamma[i - 1] * xi;
    }

    std::cout << "rho done\n";
    ///????    ComC.remove(y1)
    //return (ComA, ComB, ComC, ComD, pc, pe, r_vec, rho)
}
} // namespace libzerocoin

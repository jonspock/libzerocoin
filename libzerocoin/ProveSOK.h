// Copyright (c) 2018 Jon Spock
#include "ProveSOK_Step1.h"
#include "ProveSOK_Step2.h"
#include "ProveSOK_Step3.h"
#include "ProveSOK_Step4.h"

namespace libzerocoin
{
template <ModulusType T_>
void ProveSOK(
    size_t m,
    size_t n,
    size_t N,
    size_t m1dash,
    size_t m2dash,
    size_t ndash,
    const CBigNum& message,
    std::vector<IntegerMod<T_> >& ck,
    std::vector<IntegerMod<T_> > D,
    IntegerMod<T_> S,
    IntegerMod<T_> a,
    IntegerMod<T_> b,
    IntegerMod<T_> y1,
    IntegerMod<T_> rv,
    IntegerMod<T_> r,


    // Outputs
    // pc
    std::vector<IntegerMod<T_> >& Tf,
    std::vector<IntegerMod<T_> >& Trho,
    IntegerMod<T_>& U,

    // pe
    std::vector<IntegerMod<T_> >& tbar,
    IntegerMod<T_>& taubar,

    std::vector<IntegerMod<T_> >& r_vec,
    IntegerMod<T_>& rho,

    std::vector<IntegerMod<T_> >& ComA,
    std::vector<IntegerMod<T_> >& ComB,
    std::vector<IntegerMod<T_> >& ComC,
    IntegerMod<T_>& ComD)
{
    // Intermediate/Temporary Variables
    std::vector<IntegerMod<T_> > fBlinders;
    std::vector<IntegerMod<T_> > rhoBlinders;
    IntegerMod<T_> uBlinder;

    matrix::matrix<IntegerMod<T_> > fVector(m1dash, n);
    matrix::matrix<IntegerMod<T_> > rhoVector(m2dash, n);

    std::vector<IntegerMod<T_> > u;

    matrix::matrix<IntegerMod<T_> > A(m, n);
    matrix::matrix<IntegerMod<T_> > B(m, n);
    matrix::matrix<IntegerMod<T_> > C(m, n);

    // Not sure about this sub-group here but need way to randomize vector
    CBigNum v = rv.getValue();

    tensor::tensor<IntegerMod<T_> > wA(4 * N - 2, m, n);
    tensor::tensor<IntegerMod<T_> > wB(4 * N - 2, m, n);
    tensor::tensor<IntegerMod<T_> > wC(4 * N - 2, m, n);
    std::vector<IntegerMod<T_> > K(4 * N - 2);

    setConstraints(m, n, N, S, a, b, K, wA, wB, wC);

    // Outputs
    std::vector<IntegerMod<T_> > alpha(m);
    std::vector<IntegerMod<T_> > beta(m);
    std::vector<IntegerMod<T_> > gamma(m);
    IntegerMod<T_> delta;

    ProveSOK_Step1<T_>(
        m, n, N, ck,
        S, a, b, y1, v, r,
        // Outputs
        alpha, beta, gamma, delta,
        A, B, C,
        ComA, ComB, ComC, ComD);


    matrix::matrix<IntegerMod<T_> > wAj;
    matrix::matrix<IntegerMod<T_> > wBj;
    matrix::matrix<IntegerMod<T_> > wCj;
    IntegerMod<T_> Kconst;
    IntegerMod<T_> y;


    ProveSOK_Step2<T_>(
        m,
        n,
        N,
        message,
        S,
        K,
        wA,
        wB,
        wC,
        ComA,
        ComB,
        ComC,
        ComD,
        // outputs
        wAj,
        wBj,
        wCj,
        Kconst, y);

    matrix::matrix_zero<IntegerMod<T_> > rPoly(3 * m + 1, n, m);
    matrix::matrix_zero<IntegerMod<T_> > sPoly(3 * m + 1, n, 2 * m);
    matrix::matrix_zero<IntegerMod<T_> > rDashPoly(4 * m + 1, n, 2 * m);


    ProveSOK_Step3<T_>(
        m,
        n,
        N,
        m1dash,
        m2dash,
        ndash,
        ck,
        message,
        S,
        a,
        b,
        y1,
        v,
        r,
        y,
        Kconst,
        A, B, C, D,
        wAj,
        wBj,
        wCj,
        //outputs
        fBlinders, rhoBlinders,
        uBlinder,
        Tf,
        Trho,
        U,
        fVector, rhoVector,
        u,
        rPoly, sPoly, rDashPoly);


    ProveSOK_Step4<T_>(
        m,
        n,
        N,
        m1dash,
        m2dash,
        ndash,
        rPoly,
        fBlinders, rhoBlinders, u,
        uBlinder,
        Tf, Trho, U,
        fVector, rhoVector,
        delta, y,
        alpha, beta, gamma,
        // output
        r_vec,
        rho);
}
} // namespace libzerocoin

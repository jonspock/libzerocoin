/**
 * @file       AccumulatorProofOfKnowledge.cpp
 *
 * @brief      AccumulatorProofOfKnowledge class for the Zerocoin library.
 *
 * @author     Ian Miers, Christina Garman and Matthew Green
 * @date       June 2013
 *
 * @copyright  Copyright 2013 Ian Miers, Christina Garman and Matthew Green
 *  license    This project is released under the MIT license.
 **/
// Copyright (c) 2018 The PIVX developer
// Copyright (c) 2018 The ClubChain developers
// Copyright (c) 2018 The ClubChain developers

#include "AccumulatorProofOfKnowledge.h"
#include "IntegerMod.h"
#include "ModulusType.h"
#include "hash.h"
#include "rand_bignum.h"

namespace libzerocoin
{
AccumulatorProofOfKnowledge::AccumulatorProofOfKnowledge(const AccumulatorAndProofParams* p,
    const Commitment& commitmentToCoin,
    const AccumulatorWitness& witness,
    Accumulator& a)
    : params(p)
{
    /** Generates a proof that a commitment to a coin c was accumulated
   **/
    const CBigNum& PoKmod = IntegerModModulus<ACCUMULATOR_POK_COMMITMENT_MODULUS>::getModulus();

    const IntegerMod<ACCUMULATOR_MODULUS> g_n(params->accumulatorQRNCommitmentGroup.g);
    const IntegerMod<ACCUMULATOR_MODULUS> h_n(params->accumulatorQRNCommitmentGroup.h);

    const CBigNum& e = commitmentToCoin.getSerial();
    const CBigNum& r = commitmentToCoin.getRandomness();

    const CBigNum aM_4 = params->accumulatorModulus / CBigNum((uint8_t)4);

    const IntegerMod<ACCUMULATOR_MODULUS> r_1(randBignum(aM_4));
    const IntegerMod<ACCUMULATOR_MODULUS> r_2(randBignum(aM_4));
    const IntegerMod<ACCUMULATOR_MODULUS> r_3(randBignum(aM_4));

    /// Auxiliary commitments
    ///
    /// \f$ C_e = h^{r_1} g^e \f$
    C_e = (g_n ^ e) * (h_n ^ r_1);
    /// \f$ C_u = witness * h^{r_2} \f$
    C_u = witness.getValue() * (h_n ^ r_2);
    /// \f$ C_r = h^{r_3} g^{r_2} \f$
    C_r = (g_n ^ r_2) * (h_n ^ r_3);

    const CBigNum power_value = CBigNum(2).pow(params->k_prime + params->k_dprime);

    CBigNum r_alpha = randBignum(params->maxCoinValue * power_value);
    if (!(randBignum(CBigNum(3)) % 2)) {
        r_alpha = 0 - r_alpha;
    }

    /// The proves chooses \f$ r_&gamma;, r_&phi;, r_&sigma;, r_&epsilon;, r_&xi;, r_&eta;, r_&zeta;, r_&beta;, r_&delta;
    /// \f$
    const CBigNum r_gamma = randBignum(PoKmod);
    const CBigNum r_phi = randBignum(PoKmod);
    const CBigNum r_psi = randBignum(PoKmod);
    const CBigNum r_sigma = randBignum(PoKmod);
    const CBigNum r_xi = randBignum(PoKmod);

    CBigNum r_epsilon = randBignum((aM_4)*power_value);
    if (!(randBignum(CBigNum(3)) % 2)) {
        r_epsilon = 0 - r_epsilon;
    }
    CBigNum r_eta = randBignum((aM_4)*power_value);
    if (!(randBignum(CBigNum(3)) % 2)) {
        r_eta = 0 - r_eta;
    }
    CBigNum r_zeta = randBignum((aM_4)*power_value);
    if (!(randBignum(CBigNum(3)) % 2)) {
        r_zeta = 0 - r_zeta;
    }

    CBigNum r_beta = randBignum((aM_4)*PoKmod * power_value);
    if (!(randBignum(CBigNum(3)) % 2)) {
        r_beta = 0 - r_beta;
    }
    CBigNum r_delta = randBignum((aM_4)*PoKmod * power_value);
    if (!(randBignum(CBigNum(3)) % 2)) {
        r_delta = 0 - r_delta;
    }

    const IntegerMod<ACCUMULATOR_POK_COMMITMENT_MODULUS> sg(params->accumulatorPoKCommitmentGroup.g);
    const IntegerMod<ACCUMULATOR_POK_COMMITMENT_MODULUS> sh(params->accumulatorPoKCommitmentGroup.h);

    /// \f$ st_1 = g^{r_&alpha;} * h^{r_&phi;} \f$
    this->st_1 = (sg ^ r_alpha) * (sh ^ r_phi);

    const IntegerMod<ACCUMULATOR_POK_COMMITMENT_MODULUS> gmp5 = commitmentToCoin.getCommitmentValue() * sg.inverse();
    const IntegerMod<ACCUMULATOR_POK_COMMITMENT_MODULUS> gmp6 = commitmentToCoin.getCommitmentValue() * sg;

    /// \f$ st_2 = (C/g)^{r_&alpha;} * h^{r_&psi;} \f$
    this->st_2 = (gmp5 ^ r_gamma) * (sh ^ r_psi);

    /// \f$ st_2 = (g*C)^{r_&sigma;} * h^{r_&xi;} \f$
    this->st_3 = (gmp6 ^ r_sigma) * (sh ^ r_xi);

    /// The prover computes...
    ///
    /// \f$ t_1 = h^{r_&zeta;} * g^{r_&epsilon;} \f$
    this->t_1 = ((h_n ^ r_zeta) * (g_n ^ r_epsilon));
    /// \f$ t_2 = h^{r_&eta;} * g^{r_&alpha;} \f$
    this->t_2 = ((h_n ^ r_eta) * (g_n ^ r_alpha));
    /// \f$ t_3 = C_u^{r_&alpha;} * (1/h)^{r_&beta;} \f$
    this->t_3 = ((C_u ^ r_alpha) * (h_n.inverse() ^ r_beta));
    /// \f$ t_4 = C_r^{r_&alpha;} * (1/h)^{r_&delta;} * (1/g)^{r_&beta;} \f$
    this->t_4 = ((C_r ^ r_alpha) * (h_n.inverse() ^ r_delta) * (g_n.inverse() ^ r_beta));

    CHashWriter hasher;
    hasher << *params << sg.getValue() << sh.getValue() << g_n.getValue() << h_n.getValue()
           << commitmentToCoin.getCommitmentValue() << C_e << C_u << C_r << st_1 << st_2 << st_3 << t_1 << t_2 << t_3
           << t_4;

    CBigNum c = CBigNum(hasher.GetHash());

    /// The prover also computes

    /// \f$ s_&alpha; \f$ := \f$ r_&alpha; \f$ - c*e
    this->s_alpha = r_alpha - c * e;

    /// \f$ s_&beta; \f$ := \f$ r_&beta; \f$ - c*r_2*e
    this->s_beta = r_beta - c * r_2.getValue() * e;

    /// \f$ s_&zeta; \f$ := \f$ r_&zeta; \f$ - c*r_3
    this->s_zeta = r_zeta - c * r_3.getValue();

    /// \f$ s_&sigma; \f$ := \f$ r_&sigma; \f$ - c/(e+1) mod q
    this->s_sigma = r_sigma - c * ((e + 1).inverse(params->accumulatorPoKCommitmentGroup.groupOrder));

    /// \f$ s_&eta; \f$ := \f$ r_&eta; \f$ - c*r_1
    this->s_eta = r_eta - c * r_1.getValue();

    /// \f$ s_&epsilon; \f$ := \f$ r_&epsilon; \f$ - c*r_2
    this->s_epsilon = r_epsilon - c * r_2.getValue();

    /// \f$ s_&delta; \f$ := \f$ r_&delta; \f$ - c*r_3*e
    this->s_delta = r_delta - c * r_3.getValue() * e;

    /// \f$ s_&xi; \f$ := \f$ r_&xi; \f$ - c*r/(e+1) mod q
    this->s_xi = r_xi + c * r * ((e + 1).inverse(params->accumulatorPoKCommitmentGroup.groupOrder));

    /// \f$ s_&phi; \f$ := \f$ (r_&phi; \f$ - c*r)
    this->s_phi = (r_phi - c * r) % params->accumulatorPoKCommitmentGroup.groupOrder;

    /// \f$ s_&gamma; \f$ := \f$ r_&gamma; \f$ - c/(e-1) mod q
    this->s_gamma = r_gamma - c * ((e - 1).inverse(params->accumulatorPoKCommitmentGroup.groupOrder));

    /// \f$ s_&psi; \f$ := \f$ r_&psi; \f$ - c*r/(e-1) mod q
    this->s_psi = r_psi + c * r * ((e - 1).inverse(params->accumulatorPoKCommitmentGroup.groupOrder));

    /// and sends them to the verifier
}

/** Verifies that a commitment c is accumulated in accumulator a */
bool AccumulatorProofOfKnowledge::Verify(const Accumulator& a, const CBigNum& valueOfCommitmentToCoin) const
{
    const IntegerMod<ACCUMULATOR_MODULUS> g_n(params->accumulatorQRNCommitmentGroup.g);
    const IntegerMod<ACCUMULATOR_MODULUS> h_n(params->accumulatorQRNCommitmentGroup.h);

    const IntegerMod<ACCUMULATOR_POK_COMMITMENT_MODULUS> sg(params->accumulatorPoKCommitmentGroup.g);
    const IntegerMod<ACCUMULATOR_POK_COMMITMENT_MODULUS> sh(params->accumulatorPoKCommitmentGroup.h);
    const IntegerMod<ACCUMULATOR_POK_COMMITMENT_MODULUS> commitment(valueOfCommitmentToCoin);

    CHashWriter hasher;
    hasher << *params << sg.getValue() << sh.getValue() << g_n.getValue() << h_n.getValue() << valueOfCommitmentToCoin
           << C_e.getValue() << C_u << C_r << st_1 << st_2 << st_3 << t_1 << t_2 << t_3 << t_4;

    const CBigNum c = CBigNum(hasher.GetHash()); // this hash should be of length k_prime bits

    /// \f$ S_1 \f$ = \f$ commitment^c * g^{s_&alpha;} * h^{s_&phi;} \f$
    const CBigNum st_1_prime = ((commitment ^ c) * (sg ^ s_alpha) * (sh ^ s_phi)).getValue();
    /// \f$ S_2 \f$ = \f$ g^c * (commitment/g)^{s_&gamma;} * h^{s_&phi;} \f$
    const CBigNum st_2_prime = ((sg ^ c) * ((commitment / sg) ^ s_gamma) * (sh ^ s_psi)).getValue();
    /// \f$ S_2 \f$ = \f$ g^c * (commitment*g)^{s_&sigma;} * h^{s_&xi;} \f$
    const CBigNum st_3_prime = ((sg ^ c) * ((sg * commitment) ^ s_sigma) * (sh ^ s_xi)).getValue();

    // Note change of Modulus
    const IntegerMod<ACCUMULATOR_MODULUS> A(a.getValue());

    /// \f$ T_1 \f$ = \f$ C_r^c * h^{s_&zeta;} * g^{s_&epsilon;} \f$
    const CBigNum t_1_prime = ((C_r ^ c) * (h_n ^ s_zeta) * (g_n ^ s_epsilon)).getValue();
    /// \f$ T_2 \f$ = \f$ C_e^c * h^{s_&eta;} * g^{s_&alpha;} \f$
    const CBigNum t_2_prime = ((C_e ^ c) * (h_n ^ s_eta) * (g_n ^ s_alpha)).getValue();
    /// \f$ T_3 \f$ = \f$ A^c * C_u^{s_&alpha;} * (1/h)^{s_&beta;} \f$
    const CBigNum t_3_prime = ((A ^ c) * (C_u ^ s_alpha) * (h_n.inverse() ^ s_beta)).getValue();
    /// \f$ T_4 \f$ = \f$ C_r^{s_&alpha;} * (1/h)^{s_&delta;} * (1/g)^{s_&beta;} \f$
    const CBigNum t_4_prime = ((C_r ^ s_alpha) * (h_n.inverse() ^ s_delta) * (g_n.inverse() ^ s_beta)).getValue();

    bool result = false;

    /// Test \f$ s_1 == S_1 \f$
    bool result_st1 = (st_1.getValue() == st_1_prime);
    /// Test \f$ s_3 == S_2 \f$
    bool result_st2 = (st_2.getValue() == st_2_prime);
    /// Test \f$ s_3 == S_3 \f$
    bool result_st3 = (st_3.getValue() == st_3_prime);

    /// Test \f$ t_1 == T_1 \f$
    bool result_t1 = (t_1 == t_1_prime);
    /// Test \f$ t_3 == T_2 \f$
    bool result_t2 = (t_2 == t_2_prime);
    /// Test \f$ t_3 == T_3 \f$
    bool result_t3 = (t_3 == t_3_prime);
    /// Test \f$ t_4 == T_4 \f$
    bool result_t4 = (t_4 == t_4_prime);

    const CBigNum range_value = params->maxCoinValue * CBigNum(2).pow(params->k_prime + params->k_dprime + 1);

    /// Do range check \f$ -rangeValue < s_&alpha; < rangeValue \f$
    bool result_range = ((s_alpha >= -range_value) && (s_alpha <= range_value));

    /// Make sure all of above true
    result = result_st1 && result_st2 && result_st3 && result_t1 && result_t2 && result_t3 && result_t4 && result_range;

    return result;
}

} /* namespace libzerocoin */

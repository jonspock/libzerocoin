/**
 * @file       AccumulatorProofOfKnowledge.h
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
// Copyright (c) 2018 Jon Spock
#pragma once

#include "AccumulatorWitness.h"
#include "Commitment.h"

namespace libzerocoin {

/**A prove that a value insde the commitment commitmentToCoin is in an accumulator a.
 *
 */
class AccumulatorProofOfKnowledge {
 public:
  AccumulatorProofOfKnowledge(const AccumulatorAndProofParams* p) : params(p) {}
  /**
   * @param p  Cryptographic parameters
   * @param commitmentToCoin commitment containing the coin we want to prove is accumulated
   * @param witness The witness to the accumulation of the coin
   * @param a
   */
  AccumulatorProofOfKnowledge(const AccumulatorAndProofParams* p, const Commitment& commitmentToCoin,
                              const AccumulatorWitness& witness, Accumulator& a);
  /** Verifies that  a commitment c is accumulated in accumulated a	 */
  bool Verify(const Accumulator& a, const CBigNum& valueOfCommitmentToCoin) const;

  ADD_SERIALIZE_METHODS
  template <typename Stream, typename Operation> inline void SerializationOp(Stream& s, Operation ser_action) {
    READWRITE(C_e);
    READWRITE(C_u);
    READWRITE(C_r);
    READWRITE(st_1);
    READWRITE(st_2);
    READWRITE(st_3);
    READWRITE(t_1);
    READWRITE(t_2);
    READWRITE(t_3);
    READWRITE(t_4);
    READWRITE(s_alpha);
    READWRITE(s_beta);
    READWRITE(s_zeta);
    READWRITE(s_sigma);
    READWRITE(s_eta);
    READWRITE(s_epsilon);
    READWRITE(s_delta);
    READWRITE(s_xi);
    READWRITE(s_phi);
    READWRITE(s_gamma);
    READWRITE(s_psi);
  }

 private:
  const AccumulatorAndProofParams* params;

  /* Return values for proof */
  IntegerMod<ACCUMULATOR_MODULUS> C_e;
  IntegerMod<ACCUMULATOR_MODULUS> C_u;
  IntegerMod<ACCUMULATOR_MODULUS> C_r;

  IntegerMod<ACCUMULATOR_POK_COMMITMENT_MODULUS> st_1;
  IntegerMod<ACCUMULATOR_POK_COMMITMENT_MODULUS> st_2;
  IntegerMod<ACCUMULATOR_POK_COMMITMENT_MODULUS> st_3;

  IntegerMod<ACCUMULATOR_MODULUS> t_1;
  IntegerMod<ACCUMULATOR_MODULUS> t_2;
  IntegerMod<ACCUMULATOR_MODULUS> t_3;
  IntegerMod<ACCUMULATOR_MODULUS> t_4;

  CBigNum s_alpha;
  CBigNum s_beta;
  CBigNum s_zeta;
  CBigNum s_sigma;
  CBigNum s_eta;
  CBigNum s_epsilon;
  CBigNum s_delta;
  CBigNum s_xi;
  CBigNum s_phi;
  CBigNum s_gamma;
  CBigNum s_psi;
};

} /* namespace libzerocoin */

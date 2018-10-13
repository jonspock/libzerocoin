/**
 * @file       CommitmentProofOfKnowledge.h
 *
 * @brief      Commitment and CommitmentProof classes for the Zerocoin library.
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

#include "Commitment.h"
#include "ZerocoinParams.h"
#include "serialize.h"

// We use a SHA256 hash for our PoK challenges. Update the following
// if we ever change hash functions.
#define COMMITMENT_EQUALITY_CHALLENGE_SIZE 256

// A 512-bit security parameter for the statistical ZK PoK.
#define COMMITMENT_EQUALITY_SECMARGIN 512

namespace libzerocoin {

/**Proof that two commitments open to the same value.
 *
 */
class CommitmentProofOfKnowledge {
 public:
  CommitmentProofOfKnowledge(const SerialNumberGroupParams* ap, const IntegerGroupParams* bp);
  /** Generates a proof that two commitments, a and b, open to the same value.
   *
   * @param ap the SerialNumberGroup for commitment a
   * @param bp the IntegerGroup for commitment b
   * @param a the first commitment
   * @param b the second commitment
   */
  CommitmentProofOfKnowledge(const SerialNumberGroupParams* aParams, const IntegerGroupParams* bParams,
                             const Commitment& a, const Commitment& b);
  template <typename Stream>
  CommitmentProofOfKnowledge(const SerialNumberGroupParams* aParams, const IntegerGroupParams* bParams, Stream& strm)
      : ap(aParams), bp(bParams) {
    strm >> *this;
  }

  const CBigNum calculateChallenge(const CBigNum& a, const CBigNum& b, const CBigNum& commitOne,
                                   const CBigNum& commitTwo) const;

  /**Verifies the proof of equality of the two commitments
   *
   * @param A value of commitment one
   * @param B value of commitment two
   * @return
   */
  bool Verify(const CBigNum& A, const CBigNum& B) const;

  ADD_SERIALIZE_METHODS
  template <typename Stream, typename Operation> inline void SerializationOp(Stream& s, Operation ser_action) {
    READWRITE(S1);
    READWRITE(S2);
    READWRITE(S3);
    READWRITE(challenge);
  }

 private:
  const SerialNumberGroupParams* ap;
  const IntegerGroupParams* bp;

  CBigNum S1, S2, S3, challenge;
};

} /* namespace libzerocoin */

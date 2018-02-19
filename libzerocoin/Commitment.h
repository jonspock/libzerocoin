/**
 * @file       Commitment.h
 *
 * @brief      Commitment and CommitmentProof classes for the Zerocoin library.
 *
 * @author     Ian Miers, Christina Garman and Matthew Green
 * @date       June 2013
 *
 * @copyright  Copyright 2013 Ian Miers, Christina Garman and Matthew Green
 *  license    This project is released under the MIT license.
 **/
// Copyright (c) 2018 The PIVX developers
#pragma once
#include "ZerocoinParams.h"
#include "serialize.h"
#include "ModulusType.h"
#include "IntegerMod.h"
namespace libzerocoin {

/**
 * A commitment, complete with contents and opening randomness.
 * These should remain secret. Publish only the commitment value.
 */
class Commitment {
public:
  Commitment(const CBigNum& r, const CBigNum& v, const CBigNum c) {
    randomness = r;
    contents = v;
    commitmentValue = c;
  }
	const CBigNum& getCommitmentValue() const { return this->commitmentValue; }
  const CBigNum& getRandomness() const { return this->randomness; }
	const CBigNum& getContents() const { return this->contents; }

 private:
	CBigNum commitmentValue;
	CBigNum randomness;
  CBigNum contents;

	ADD_SERIALIZE_METHODS;
  template <typename Stream, typename Operation>  inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion) {
	    READWRITE(commitmentValue);
	    READWRITE(randomness);
	    READWRITE(contents);
	}
public:

};


  /// This is now a free function instead of a member function

  /** Generates a Pedersen commitment to the given value.
   *
   * @param g1 the g value
   * @param h1 the h value
   * @param value the value to commit to
   */
  template <ModulusType T, ModulusType G> Commitment commit(const CBigNum& g1, const CBigNum& h1, const CBigNum& value) {
    const IntegerMod<T> g(g1);
    const IntegerMod<T> h(h1);
    CBigNum m = IntegerModModulus<T>::getModulus();
    CBigNum r = CBigNum::randBignum(m);
    
    /// \f$ commitment = g^{value} * h^{randomness} \f$
    CBigNum commitmentValue = ((g^value)*(h^r)).getValue();
    Commitment commit(r, value, commitmentValue);
    return commit;
  }


  
} /* namespace libzerocoin */

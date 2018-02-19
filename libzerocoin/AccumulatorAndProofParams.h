/**
* @file       Params.h
*
* @brief      Parameter classes for Zerocoin.
*
* @author     Ian Miers, Christina Garman and Matthew Green
* @date       June 2013
*
* @copyright  Copyright 2013 Ian Miers, Christina Garman and Matthew Green
*  license    This project is released under the MIT license.
**/
// Copyright (c) 2018 The PIVX developers
#pragma once

#include "bignum.h"
#include "ZerocoinDefines.h"
#include "IntegerGroupParams.h"

namespace libzerocoin {

class AccumulatorAndProofParams {
public:
	/** @brief Construct a set of Zerocoin parameters from a modulus "N".
	* @param N                A trusted RSA modulus
	* @param securityLevel    A security level expressed in symmetric bits (default 80)
	*
	* Allocates and derives a set of Zerocoin parameters from
	* a trustworthy RSA modulus "N". This routine calculates all
	* of the remaining parameters (group descriptions etc.) from N
	* using a verifiable, deterministic procedure.
	*
	* Note: this constructor makes the fundamental assumption that "N"
	* encodes a valid RSA-style modulus of the form "e1 * e2" where
	* "e1" and "e2" are safe primes. The factors "e1", "e2" MUST NOT
	* be known to any party, or the security of Zerocoin is
	* compromised. The integer "N" must be a MINIMUM of 1024
	* in length. 3072 bits is strongly recommended.
	**/
  AccumulatorAndProofParams() { 	this->initialized = false;}

	bool initialized;

	/**
	 * Modulus used for the accumulator.
	 * Product of two safe primes who's factorization is unknown.
	 */
	CBigNum accumulatorModulus;

	/**
	 * The initial value for the accumulator
	 * A random Quadratic residue mod n thats not 1
	 */
	CBigNum accumulatorBase;

	/**
	 * Lower bound on the value for committed coin.
	 * Required by the accumulator proof.
	 */
	CBigNum minCoinValue;

	/**
	 * Upper bound on the value for a comitted coin.
	 * Required by the accumulator proof.
	 */
	CBigNum maxCoinValue;

	/**
	 * The second of two groups used to form a commitment to
	 * a coin (which it self is a commitment to a serial number).
	 * This one differs from serialNumberSokCommitment due to
	 * restrictions from Camenisch and Lysyanskaya's paper.
	 */
	IntegerGroupParams accumulatorPoKCommitmentGroup;

	/**
	 * Hidden order quadratic residue group mod N.
	 * Used in the accumulator proof.
	 */
	IntegerGroupParams accumulatorQRNCommitmentGroup;

	/**
	 * Security parameter.
	 * Bit length of the challenges used in the accumulator proof.
	 */
	uint32_t k_prime;

	/**
	 * Security parameter.
	 * The statistical zero-knowledgeness of the accumulator proof.
	 */
	uint32_t k_dprime;
	ADD_SERIALIZE_METHODS;
  template <typename Stream, typename Operation>  inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion) {
	    READWRITE(initialized);
	    READWRITE(accumulatorModulus);
	    READWRITE(accumulatorBase);
	    READWRITE(accumulatorPoKCommitmentGroup);
	    READWRITE(accumulatorQRNCommitmentGroup);
	    READWRITE(minCoinValue);
	    READWRITE(maxCoinValue);
	    READWRITE(k_prime);
	    READWRITE(k_dprime);
  }
};

}

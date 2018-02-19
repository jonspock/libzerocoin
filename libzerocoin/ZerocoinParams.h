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
#include "AccumulatorAndProofParams.h"
#include "SerialNumberGroupParams.h"
#include "IntegerGroupParams.h"

namespace libzerocoin {

class ZerocoinParams {
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
	ZerocoinParams(CBigNum accumulatorModulus,
	       uint32_t securityLevel = ZEROCOIN_DEFAULT_SECURITYLEVEL);

	bool initialized;

	AccumulatorAndProofParams accumulatorParams;

	/**
	 * The Quadratic Residue group from which we form
	 * a coin as a commitment  to a serial number.
	 */
	IntegerGroupParams coinCommitmentGroup;

	/**
	 * One of two groups used to form a commitment to
	 * a coin (which it self is a commitment to a serial number).
	 * This is the one used in the serial number poof.
	 * It's order must be equal to the modulus of coinCommitmentGroup.
	 */
	SerialNumberGroupParams serialNumberSoKCommitmentGroup;

	/**
	 * The number of iterations to use in the serial
	 * number proof.
	 */
	uint32_t zkp_iterations;

	/**
	 * The amount of the hash function we use for
	 * proofs.
	 */
	uint32_t zkp_hash_len;
	
	ADD_SERIALIZE_METHODS;
  template <typename Stream, typename Operation>  inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion) {
	    READWRITE(initialized);
	    READWRITE(accumulatorParams);
	    READWRITE(coinCommitmentGroup);
	    READWRITE(serialNumberSoKCommitmentGroup);
	    READWRITE(zkp_iterations);
	    READWRITE(zkp_hash_len);
	}
};

} /* namespace libzerocoin */


/**
 * @file       Commitment.cpp
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

#include "CommitmentProofOfKnowledge.h"
#include "hash.h"
#include "IntegerMod.h"
#include <iostream>
namespace libzerocoin
{
// CommitmentProofOfKnowledge class
CommitmentProofOfKnowledge::CommitmentProofOfKnowledge(const SerialNumberGroupParams* ap, const IntegerGroupParams* bp)
    : ap(ap), bp(bp) {}

// TODO: get parameters from the commitment group
CommitmentProofOfKnowledge::CommitmentProofOfKnowledge(const SerialNumberGroupParams* aParams,
    const IntegerGroupParams* bParams,
    const Commitment& a,
    const Commitment& b)
    : ap(aParams), bp(bParams)
{
    // First: make sure that the two commitments have the
    // same contents.
    if (a.getContents() != b.getContents()) {
        std::cout << "a = " << a.getContents().ToString(16) << "\n";
        std::cout << "b = " << b.getContents().ToString(16) << "\n";
        throw std::runtime_error("Both commitments must contain the same value");
    }

    // Select three random values "r1, r2, r3" in the range 0 to (2^l)-1 where l is:
    // length of challenge value + max(modulus 1, modulus 2, order 1, order 2) + margin.
    // We set "margin" to be a relatively generous  security parameter.
    //
    // We choose these large values to ensure statistical zero knowledge.
    uint32_t randomSize = COMMITMENT_EQUALITY_CHALLENGE_SIZE + COMMITMENT_EQUALITY_SECMARGIN +
        std::max(std::max(this->ap->modulus.bitSize(), this->bp->modulus.bitSize()),
                 std::max(this->ap->groupOrder.bitSize(), this->bp->groupOrder.bitSize()));
    
    CBigNum maxRange = (CBigNum(2).pow(randomSize) - CBigNum(1));

    const CBigNum r1 = CBigNum::randBignum(maxRange);
    const CBigNum r2 = CBigNum::randBignum(maxRange);
    const CBigNum r3 = CBigNum::randBignum(maxRange);

    const IntegerMod<SERIAL_NUMBER_SOK_COMMITMENT_MODULUS> g1(ap->g);
    const IntegerMod<SERIAL_NUMBER_SOK_COMMITMENT_MODULUS> h1(ap->h);
    const IntegerMod<ACCUMULATOR_POK_COMMITMENT_MODULUS> g2(bp->g);
    const IntegerMod<ACCUMULATOR_POK_COMMITMENT_MODULUS> h2(bp->h);
    
    // Generate two random, ephemeral commitments "T1, T2"
    // of the form:
    // T1 = g1^r1 * h1^r2 mod p1
    // T2 = g2^r1 * h2^r3 mod p2
    // Where (g1, h1, p1) are from "aParams" and (g2, h2, p2) are from "bParams".
    const IntegerMod<SERIAL_NUMBER_SOK_COMMITMENT_MODULUS> T1 = (g1^r1)*(h1^r2); // ap->modulus
    const IntegerMod<ACCUMULATOR_POK_COMMITMENT_MODULUS> T2 = (g2^r1)*(h2^r3); // bp->modulus

    // Now hash commitment "A" with commitment "B" as well as the
    // parameters and the two ephemeral commitments "T1, T2" we just generated
    this->challenge = calculateChallenge(a.getCommitmentValue(), b.getCommitmentValue(), T1.getValue(), T2.getValue());

    // Let "m" be the contents of the commitments "A, B". We have:
    // A =  g1^m  * h1^x  mod p1
    // B =  g2^m  * h2^y  mod p2
    // T1 = g1^r1 * h1^r2 mod p1
    // T2 = g2^r1 * h2^r3 mod p2
    //
    // Now compute:
    //  S1 = r1 + (m * challenge)   -- note, not modular arithmetic
    //  S2 = r2 + (x * challenge)   -- note, not modular arithmetic
    //  S3 = r3 + (y * challenge)   -- note, not modular arithmetic
    this->S1 = r1 + (a.getContents() * this->challenge);
    this->S2 = r2 + (a.getRandomness() * this->challenge);
    this->S3 = r3 + (b.getRandomness() * this->challenge);

    // We're done. The proof is S1, S2, S3 and "challenge", all of which
    // are stored in member variables.
}

bool CommitmentProofOfKnowledge::Verify(const CBigNum& A, const CBigNum& B) const
{
    // Compute the maximum range of S1, S2, S3 and verify that the given values are
    // in a correct range. This might be an unnecessary check.
    uint32_t maxSize = 64 * (COMMITMENT_EQUALITY_CHALLENGE_SIZE + COMMITMENT_EQUALITY_SECMARGIN +
                             std::max(std::max(
                                               IntegerModModulus<SERIAL_NUMBER_SOK_COMMITMENT_MODULUS>::getModulus().bitSize(),
                                               IntegerModModulus<ACCUMULATOR_POK_COMMITMENT_MODULUS>::getModulus().bitSize()),                    
                                      std::max(IntegerModModulus<SERIAL_NUMBER_SOK_COMMITMENT_GROUP>::getModulus().bitSize(),
                                               IntegerModModulus<ACCUMULATOR_POK_COMMITMENT_GROUP>::getModulus().bitSize()
                                               )));

    if ((uint32_t)this->S1.bitSize() > maxSize || (uint32_t)this->S2.bitSize() > maxSize ||
        (uint32_t)this->S3.bitSize() > maxSize || this->S1 < CBigNum(0) || this->S2 < CBigNum(0) ||
        this->S3 < CBigNum(0) || this->challenge < CBigNum(0) ||
        this->challenge > (CBigNum(2).pow(COMMITMENT_EQUALITY_CHALLENGE_SIZE) - CBigNum(1))) {
        // Invalid inputs. Reject.
        return false;
    }

    const IntegerMod<SERIAL_NUMBER_SOK_COMMITMENT_MODULUS> A1(A);
    const IntegerMod<SERIAL_NUMBER_SOK_COMMITMENT_MODULUS> chal(challenge);
    const IntegerMod<SERIAL_NUMBER_SOK_COMMITMENT_MODULUS> g1(ap->g);
    const IntegerMod<SERIAL_NUMBER_SOK_COMMITMENT_MODULUS> h1(ap->h);

    const IntegerMod<ACCUMULATOR_POK_COMMITMENT_MODULUS> B2(B);
    const IntegerMod<ACCUMULATOR_POK_COMMITMENT_MODULUS> g2(bp->g);
    const IntegerMod<ACCUMULATOR_POK_COMMITMENT_MODULUS> h2(bp->h);

    // Compute T1 = g1^S1 * h1^S2 / (A^{challenge}) mod p1
    const IntegerMod<SERIAL_NUMBER_SOK_COMMITMENT_MODULUS> T1( (g1^S1) * (h1^S2) / (A1^challenge) );
   
    // Compute T2 = g2^S1 * h2^S3 / (B^{challenge}) mod p2
    const IntegerMod<ACCUMULATOR_POK_COMMITMENT_MODULUS> T2( (g2^S1) * (h2^S3) / (B2^challenge) );
  
    // Hash T1 and T2 along with all of the public parameters
    const CBigNum computedChallenge = calculateChallenge(A, B, T1.getValue(), T2.getValue());

    // Return success if the computed challenge matches the incoming challenge
    if (computedChallenge == this->challenge) {
        return true;
    }

    // Otherwise return failure
    return false;
}

const CBigNum CommitmentProofOfKnowledge::calculateChallenge(const CBigNum& a, const CBigNum& b, const CBigNum& commitOne, const CBigNum& commitTwo) const
{
    CHashWriter hasher(0, 0);

    // Hash together the following elements:
    // * A string identifying the proof
    // * Commitment A
    // * Commitment B
    // * Ephemeral commitment T1
    // * Ephemeral commitment T2
    // * A serialized instance of the commitment A parameters
    // * A serialized instance of the commitment B parameters

    hasher << std::string(ZEROCOIN_COMMITMENT_EQUALITY_PROOF);
    hasher << commitOne;
    hasher << std::string("||");
    hasher << commitTwo;
    hasher << std::string("||");
    hasher << a;
    hasher << std::string("||");
    hasher << b;
    hasher << std::string("||");
    hasher << *(this->ap);
    hasher << std::string("||");
    hasher << *(this->bp);

    // Convert the SHA256 result into a Bignum
    // Note that if we ever change the size of the hash function we will have
    // to update COMMITMENT_EQUALITY_CHALLENGE_SIZE appropriately!
    return CBigNum(hasher.GetHash());
}

} /* namespace libzerocoin */

/**
 * @file       PrivateCoin.cpp
 *
 * @brief      PublicCoin and PrivateCoin classes for the Zerocoin library.
 *
 * @author     Ian Miers, Christina Garman and Matthew Green
 * @date       June 2013
 *
 * @copyright  Copyright 2013 Ian Miers, Christina Garman and Matthew Green
 *  license    This project is released under the MIT license.
 **/

// Copyright (c) 2018 The PIVX developers
#include "PrivateCoin.h"
#include "Denominations.h"
#include "ModulusType.h"
#include "IntegerMod.h"
#include <stdexcept>

namespace libzerocoin
{
PrivateCoin::PrivateCoin(const ZerocoinParams* p, const CoinDenomination denomination) : params(p)
{
    // Verify that the parameters are valid
    if (this->params->initialized == false) {
        throw std::runtime_error("Params are not initialized");
    }

    // Mint a new coin with a random serial number using the fast process.
    // This is more vulnerable to timing attacks so don't mint coins when
    // somebody could be timing you.
    this->mintCoinFast(denomination);
}

/**
 * @brief Mint a new coin using a faster process.
 * @param denomination the denomination of the coin to mint
 * @throws ZerocoinException if the process takes too long
 *
 * Generates a new Zerocoin by (a) selecting a random serial
 * number, (b) committing to this serial number and repeating until
 * the resulting commitment is prime. Stores the
 * resulting commitment (coin) and randomness (trapdoor).
 * This routine is substantially faster than the
 * mintCoin() routine, but could be more vulnerable
 * to timing attacks. Don't use it if you think someone
 * could be timing your coin minting.
 **/
void PrivateCoin::mintCoinFast(const CoinDenomination denomination)
{
    // Generate a random serial number in the range 0...{q-1} where
    // "q" is the order of the commitment group.
    const CBigNum& groupOrder = this->params->coinCommitmentGroup.groupOrder;
    const IntegerMod<COIN_COMMITMENT_MODULUS> g(this->params->coinCommitmentGroup.g);
    const IntegerMod<COIN_COMMITMENT_MODULUS> h(this->params->coinCommitmentGroup.h);

    CBigNum s = CBigNum::randBignum(groupOrder);

    /// Generate a random number "r" in the range 0...{q-1}
    CBigNum r = CBigNum::randBignum(groupOrder);

    /// Manually compute a Pedersen commitment to the serial number "s" under randomness "r"
    /// \f$ C = g^s * h^r (mod p) \f$
    IntegerMod<COIN_COMMITMENT_MODULUS> C = (g^s) * (h^r);
    
    /// Repeat this process up to MAX_COINMINT_ATTEMPTS times until obtain a prime number
    for (uint32_t attempt = 0; attempt < MAX_COINMINT_ATTEMPTS; attempt++) {
        // First verify that the commitment is a prime number
        // in the appropriate range. If not, we'll throw this coin
        // away and generate a new one.
        if (C.isPrime(ZEROCOIN_MINT_PRIME_PARAM) &&
            C >= params->accumulatorParams.minCoinValue &&
            C <= params->accumulatorParams.maxCoinValue) {
            // Found a valid coin. Store it.
            this->serialNumber = s;
            this->randomness = r;
            this->publicCoin = PublicCoin(C.getValue(), denomination);

            // Success! We're done.
            return;
        }

        // Generate a new random "r_delta" in 0...{q-1}
        CBigNum r_delta = CBigNum::randBignum(this->params->coinCommitmentGroup.groupOrder);

        /// The commitment was not prime. Increment "r" and recalculate "C":
        /// \f$ r = r + r_delta mod q \f$
        /// \f$ C = C * h mod p \f$
        r = (r + r_delta) % this->params->coinCommitmentGroup.groupOrder;
        C *= (h^r_delta);
    }

    // We only get here if we did not find a coin within
    // MAX_COINMINT_ATTEMPTS. Throw an exception.
    throw std::runtime_error("Unable to mint a new Zerocoin (too many attempts)");
}

} /* namespace libzerocoin */

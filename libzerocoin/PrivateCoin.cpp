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

// Copyright (c) 2018 The PIVX developer
// Copyright (c) 2018 Jon Spock
#include "PrivateCoin.h"
#include "Commit.h"
#include "Commitment.h"
#include "Denominations.h"
#include "IntegerMod.h"
#include "ModulusType.h"
#include "pubkey.h"
#include "hash.h"
#include "uint512.h"
#include <stdexcept>

using namespace std;

namespace libzerocoin {

bool IsValidCoinValue(const ZerocoinParams* params, const IntegerMod<COIN_COMMITMENT_MODULUS>& C) {
  return (C >= params->accumulatorParams.minCoinValue) && (C <= params->accumulatorParams.maxCoinValue) &&
         C.isPrime(ZEROCOIN_MINT_PRIME_PARAM);
}

bool GenerateKeyPair(const CBigNum& bnGroupOrder, const uint256& nPrivkey, CKey& key, CBigNum& bnSerial) {
  // Generate a new key pair, which also has a 256-bit pubkey hash that qualifies as a serial #
  // This builds off of Tim Ruffing's work on libzerocoin, but has a different implementation
  if (nPrivkey.IsNull()) throw runtime_error("Private Key was Null - Determinism not possible");

  CKey keyPair(nPrivkey.begin());
  CPubKey pubKey = keyPair.GetPubKey();
  std::vector<uint8_t> b = pubKey.ToStdVector();
  uint256 hashPubKey = Hash(b.begin(), b.end());

  CBigNum s(hashPubKey);
  key = keyPair;
  bnSerial = s;
  return true;
}

PrivateCoin::PrivateCoin(const ZerocoinParams* p) : params(p) { assert(p); }

PrivateCoin::PrivateCoin(const ZerocoinParams* p, const CoinDenomination denomination, const CBigNum& Serial,
                         const CBigNum& Randomness)
    : params(p) {
  // Verify that the parameters are valid
  assert(p);

  // Save Parameters & do Commitment
  serialNumber = Serial;
  randomness = Randomness;

  // 2 TEMPLATE PARAMS are the same
  Commitment c = commit<COIN_COMMITMENT_MODULUS, COIN_COMMITMENT_MODULUS>(
      p->coinCommitmentGroup.g, p->coinCommitmentGroup.h, serialNumber, randomness);
  this->publicCoin = PublicCoin(c.getCommitmentValue(), denomination);
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

CBigNum PrivateCoin::CoinFromSeed(const uint512& seedZerocoin) {
  CBigNum bnRandomness;
  CBigNum bnSerial;
  CoinDenomination denomination = CoinDenomination::ZQ_ONE;  // Not used

  // convert state seed into a seed for the private key
  uint256 nSeedPrivKey = seedZerocoin.trim256();
  const IntegerMod<COIN_COMMITMENT_MODULUS> g(this->params->coinCommitmentGroup.g);
  const IntegerMod<COIN_COMMITMENT_MODULUS> h(this->params->coinCommitmentGroup.h);

  bool isValidKey = false;
  CKey key = CKey();
  while (!isValidKey) {
    nSeedPrivKey = Hash(nSeedPrivKey.begin(), nSeedPrivKey.end());
    isValidKey = GenerateKeyPair(params->coinCommitmentGroup.groupOrder, nSeedPrivKey, key, bnSerial);
    setPrivKey(key.GetPrivKey());
  }

  // hash randomness seed with Bottom 256 bits of seedZerocoin & attempts256 which is initially 0
  arith_uint512 seed = UintToArith512(seedZerocoin) >> 256;
  uint256 randomnessSeed = ArithToUint512(seed).trim256();
  uint256 hashRandomness = Hash(randomnessSeed.begin(), randomnessSeed.end());
  bnRandomness.setuint256(hashRandomness);
  bnRandomness = bnRandomness % params->coinCommitmentGroup.groupOrder;

  CBigNum r = bnRandomness;
  CBigNum s = bnSerial;

  /// Manually compute a Pedersen commitment to the serial number "s" under randomness "r"
  /// \f$ C = g^s * h^r (mod p) \f$
  IntegerMod<COIN_COMMITMENT_MODULUS> C = (g ^ s) * (h ^ r);

  CBigNum random;
  arith_uint256 attempts256 = 0;
  // Iterate on Randomness until a valid commitmentValue is found
  while (true) {
    // Now verify that the commitment is a prime number
    // in the appropriate range. If not, we'll throw this coin
    // away and generate a new one.
    if (IsValidCoinValue(params, C.getValue())) {
      serialNumber = s;
      randomness = r;
      publicCoin = PublicCoin(C.getValue(), denomination);
      return C.getValue();
    }

    // Did not create a valid commitment value.
    /// The commitment was not prime. Increment "r" and recalculate "C":
    attempts256++;
    hashRandomness = Hash(randomnessSeed.begin(), randomnessSeed.end(), attempts256.begin(), attempts256.end());
    random.setuint256(hashRandomness);
    /// \f$ r = r + r_delta mod q \f$
    /// \f$ C = C * h mod p \f$
    r = (r + random) % params->coinCommitmentGroup.groupOrder;
    C *= (h ^ random);
  }
}

CPubKey PrivateCoin::getPubKey() const {
  CKey key(privkey);
  return key.GetPubKey();
}

void PrivateCoin::sign(const uint256& hash, vector<uint8_t>& vchSig) const {
  CKey key(privkey);
  key.Sign(hash, vchSig);
}

} /* namespace libzerocoin */

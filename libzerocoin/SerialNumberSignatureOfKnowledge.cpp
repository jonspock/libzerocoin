/**
 * @file       SerialNumberSignatureOfKnowledge.cpp
 *
 * @brief      SerialNumberSignatureOfKnowledge class for the Zerocoin library.
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

#include "SerialNumberSignatureOfKnowledge.h"
#include "IntegerMod.h"
#include <streams.h>

using namespace std;

namespace libzerocoin {
SerialNumberSignatureOfKnowledge::SerialNumberSignatureOfKnowledge(const ZerocoinParams* p) : params(p) {}

// Use one 256 bit seed and concatenate 4 unique 256 bit hashes to make a 1024 bit hash
CBigNum SeedTo1024(uint256 hashSeed) {
  CHashWriter hasher;
  hasher << hashSeed;

  vector<uint8_t> vResult;
  for (int i = 0; i < 4; i++) {
    vector<uint8_t> vHash = CBigNum(hasher.GetHash()).getvch();
    vResult.insert(vResult.end(), vHash.begin(), vHash.end());
    hasher << vResult;
  }

  CBigNum bnResult;
  bnResult.setvch(vResult);
  return bnResult;
}

SerialNumberSignatureOfKnowledge::SerialNumberSignatureOfKnowledge(const ZerocoinParams* p, const PrivateCoin& coin,
                                                                   const Commitment& commitmentToCoin, uint256 msghash)
    : params(p), s_notprime(p->zkp_iterations), sprime(p->zkp_iterations) {
  // Sanity check: verify that the order of the "accumulatedValueCommitmentGroup" is
  // equal to the modulus of "coinCommitmentGroup". Otherwise we will produce invalid proofs.
  if (IntegerModModulus<COIN_COMMITMENT_MODULUS>::getModulus() !=
      IntegerModModulus<SERIAL_NUMBER_SOK_COMMITMENT_GROUP>::getModulus()) {
    throw std::runtime_error("Groups are not structured correctly.");
  }

  const IntegerMod<SERIAL_NUMBER_SOK_COMMITMENT_GROUP> b(params->coinCommitmentGroup.h);

  CHashWriter hasher;
  hasher << *params << commitmentToCoin.getCommitmentValue() << coin.getSerialNumber() << msghash;

  vector<CBigNum> r(params->zkp_iterations);
  vector<CBigNum> v_seed(params->zkp_iterations);
  vector<CBigNum> v_expanded(params->zkp_iterations);
  vector<CBigNum> c(params->zkp_iterations);

  for (uint32_t i = 0; i < params->zkp_iterations; i++) {
    r[i] = CBigNum::randBignum(params->coinCommitmentGroup.groupOrder);

    // use a random 256 bit seed that expands to 1024 bit for v[i]
    while (true) {
        
      uint256 hashRand = CBigNum::randBignum(CBigNum(~arith_uint256(0))).getuint256();
      CBigNum bnExpanded = SeedTo1024(hashRand);

      if (bnExpanded > params->serialNumberSoKCommitmentGroup.groupOrder) continue;

      v_seed[i] = CBigNum(hashRand);
      v_expanded[i] = bnExpanded;
      break;
    }
  }

  for (uint32_t i = 0; i < params->zkp_iterations; i++) {
    // compute g^{ {a^x b^r} h^v} mod q
    c[i] = challengeCalculation(coin.getSerialNumber(), r[i], v_expanded[i]);
  }

  for (uint32_t i = 0; i < params->zkp_iterations; i++) { hasher << c[i]; }

  this->hash = hasher.GetHash();
  uint8_t* hashbytes = (uint8_t*)&hash;

  for (uint32_t i = 0; i < params->zkp_iterations; i++) {
    int bit = i % 8;
    int byte = i / 8;

    bool challenge_bit = ((hashbytes[byte] >> bit) & 0x01);
    if (challenge_bit) {
      s_notprime[i] = r[i];
      sprime[i] = v_seed[i];
    } else {
      s_notprime[i] = r[i] - coin.getRandomness();
      sprime[i] = v_expanded[i] - (commitmentToCoin.getRandomness() * (b ^ (r[i] - coin.getRandomness())).getValue());
    }
  }
}

inline CBigNum SerialNumberSignatureOfKnowledge::challengeCalculation(const CBigNum& a_exp, const CBigNum& b_exp,
                                                                      const CBigNum& h_exp) const {
  const IntegerMod<SERIAL_NUMBER_SOK_COMMITMENT_GROUP> a(params->coinCommitmentGroup.g);
  const IntegerMod<SERIAL_NUMBER_SOK_COMMITMENT_GROUP> b(params->coinCommitmentGroup.h);

  // Extract as CBigNum as Modulus will change in next usage
  CBigNum exponent = ((a ^ a_exp) * (b ^ b_exp)).getValue();

  // Note: Change of Modulus
  const IntegerMod<SERIAL_NUMBER_SOK_COMMITMENT_MODULUS> g(params->serialNumberSoKCommitmentGroup.g);
  const IntegerMod<SERIAL_NUMBER_SOK_COMMITMENT_MODULUS> h(params->serialNumberSoKCommitmentGroup.h);

  return (((g ^ exponent) * (h ^ h_exp)).getValue());
}

bool SerialNumberSignatureOfKnowledge::Verify(const CBigNum& coinSerialNumber, const CBigNum& valueOfCommitmentToCoin,
                                              const uint256 msghash) const {
  const IntegerMod<SERIAL_NUMBER_SOK_COMMITMENT_GROUP> b(params->coinCommitmentGroup.h);
  const IntegerMod<SERIAL_NUMBER_SOK_COMMITMENT_MODULUS> h(params->serialNumberSoKCommitmentGroup.h);
  const IntegerMod<SERIAL_NUMBER_SOK_COMMITMENT_MODULUS> valueOfCoinCommitment(valueOfCommitmentToCoin);

  CHashWriter hasher;
  hasher << *params << valueOfCommitmentToCoin << coinSerialNumber << msghash;

  uint8_t* hashbytes = (uint8_t*)&this->hash;

  for (uint32_t i = 0; i < params->zkp_iterations; i++) {
    CBigNum tprime;
    int bit = i % 8;
    int byte = i / 8;
    bool challenge_bit = ((hashbytes[byte] >> bit) & 0x01);
    if (challenge_bit) {
      tprime = challengeCalculation(coinSerialNumber, s_notprime[i], SeedTo1024(sprime[i].getuint256()));
    } else {
      CBigNum exp = (b ^ s_notprime[i]).getValue();  // Convert to CBigNum because below is different Modulus
      tprime = ((valueOfCoinCommitment ^ exp) * (h ^ sprime[i])).getValue();
    }
    hasher << tprime;
  }

  return hasher.GetHash() == hash;
}

} /* namespace libzerocoin */

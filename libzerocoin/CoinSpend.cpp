/**
 * @file       CoinSpend.cpp
 *
 * @brief      CoinSpend class for the Zerocoin library.
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

#include "CoinSpend.h"
#include "Commit.h"
#include <iostream>

namespace libzerocoin {

bool CoinSpend::HasValidSerial(ZerocoinParams* params) const {
  if (getCoinSerialNumber() <= 0) return false;
  return getCoinSerialNumber() < params->coinCommitmentGroup.groupOrder;
}

// Additional verification layer that requires the spend be signed by the private key associated with the serial
bool CoinSpend::HasValidSignature() const {
  // requires that the signature hash be signed by the public key associated with the serial
  uint256 hashedPubkey = Hash(pubkey.begin(), pubkey.end());
  if (hashedPubkey != coinSerialNumber.getuint256()) {
    // cout << "CoinSpend::HasValidSignature() hashedpubkey is not equal to the serial!\n";
    return false;
  }

  return pubkey.Verify(signatureHash(), vchSig);
}

CoinSpend::CoinSpend(const ZerocoinParams* p, const PrivateCoin& coin, Accumulator& a, const uint32_t checksum,
                     const AccumulatorWitness& witness, const uint256& ptxHash)
    : accChecksum(checksum),
      ptxHash(ptxHash),
      coinSerialNumber((coin.getSerialNumber())),
      accumulatorPoK(&p->accumulatorParams),
      commitmentPoK(&p->serialNumberSoKCommitmentGroup, &p->accumulatorParams.accumulatorPoKCommitmentGroup) {
  denomination = coin.getPublicCoin().getDenomination();
  // Sanity check: let's verify that the Witness is valid with respect to
  // the coin and Accumulator provided.
  if (!(witness.VerifyWitness(a, coin.getPublicCoin()))) {
    std::cout << "CoinSpend: Accumulator witness does not verify\n";
    throw std::runtime_error("Accumulator witness does not verify");
  }

  // 1: Generate two separate commitments to the public coin (C), each under
  // a different set of public parameters. We do this because the RSA accumulator
  // has specific requirements for the commitment parameters that are not
  // compatible with the group we use for the serial number proof.
  // Specifically, our serial number proof requires the order of the commitment group
  // to be the same as the modulus of the upper group. The Accumulator proof requires a
  // group with a significantly larger order.
  Commitment fullCommitmentToCoinUnderSerialParams =
      commit<SERIAL_NUMBER_SOK_COMMITMENT_MODULUS, SERIAL_NUMBER_SOK_COMMITMENT_GROUP>(
          p->serialNumberSoKCommitmentGroup.g, p->serialNumberSoKCommitmentGroup.h, coin.getPublicCoin().getValue());

  this->serialCommitmentToCoinValue = fullCommitmentToCoinUnderSerialParams.getCommitmentValue();

  Commitment fullCommitmentToCoinUnderAccParams =
      commit<ACCUMULATOR_POK_COMMITMENT_MODULUS, ACCUMULATOR_POK_COMMITMENT_GROUP>(
          p->accumulatorParams.accumulatorPoKCommitmentGroup.g, p->accumulatorParams.accumulatorPoKCommitmentGroup.h,
          coin.getPublicCoin().getValue());

  this->accCommitmentToCoinValue = fullCommitmentToCoinUnderAccParams.getCommitmentValue();

  // 2. Generate a ZK proof that the two commitments contain the same public coin.
  this->commitmentPoK = CommitmentProofOfKnowledge(
      &p->serialNumberSoKCommitmentGroup, &p->accumulatorParams.accumulatorPoKCommitmentGroup,
      fullCommitmentToCoinUnderSerialParams, fullCommitmentToCoinUnderAccParams);

  // Now generate the two core ZK proofs:
  // 3. Proves that the committed public coin is in the Accumulator (PoK of "witness")
  this->accumulatorPoK =
      AccumulatorProofOfKnowledge(&p->accumulatorParams, fullCommitmentToCoinUnderAccParams, witness, a);

  // 4. Proves that the coin is correct w.r.t. serial number and hidden coin secret
  // (This proof is bound to the coin 'metadata', i.e., transaction hash)
  uint256 hashSig = signatureHash();  // used twice
  this->serialNumberSoK = SerialNumberSignatureOfKnowledge(p, coin, fullCommitmentToCoinUnderSerialParams, hashSig);

  // 5. Sign the transaction using the private key associated with the serial number
  this->pubkey = coin.getPubKey();
  if (!coin.sign(hashSig, this->vchSig)) throw std::runtime_error("Coinspend failed to sign signature hash");
}

bool CoinSpend::Verify(const Accumulator& a) const {
  // Verify both of the sub-proofs using the given meta-data
  return (a.getDenomination() == this->denomination) &&
         commitmentPoK.Verify(serialCommitmentToCoinValue, accCommitmentToCoinValue) &&
         accumulatorPoK.Verify(a, accCommitmentToCoinValue) &&
         serialNumberSoK.Verify(coinSerialNumber, serialCommitmentToCoinValue, signatureHash());
}

const uint256 CoinSpend::signatureHash() const {
  CHashWriter h;
  h << serialCommitmentToCoinValue << accCommitmentToCoinValue << commitmentPoK << accumulatorPoK << ptxHash
    << coinSerialNumber << accChecksum << denomination << spendType;  // spendType for compatibility
  return h.GetHash();
}

} /* namespace libzerocoin */

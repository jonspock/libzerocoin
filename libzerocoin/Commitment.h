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
// Copyright (c) 2018 The PIVX developer
// Copyright (c) 2018 Jon Spock
#pragma once
#include "IntegerMod.h"
#include "ModulusType.h"
#include "ZerocoinParams.h"
#include "serialize.h"
namespace libzerocoin {

/**
 * A commitment, complete with serial and opening randomness.
 * These should remain secret. Publish only the commitment value.
 */
class Commitment {
 public:
  Commitment(const CBigNum& r, const CBigNum& v, const CBigNum& c) {
    randomness = r;
    serial = v;
    commitmentValue = c;
  }
  const CBigNum& getCommitmentValue() const { return this->commitmentValue; }
  const CBigNum& getRandomness() const { return this->randomness; }
  const CBigNum& getSerial() const { return this->serial; }

 private:
  CBigNum commitmentValue;
  CBigNum randomness;
  CBigNum serial;

  ADD_SERIALIZE_METHODS
  template <typename Stream, typename Operation> inline void SerializationOp(Stream& s, Operation ser_action) {
    READWRITE(commitmentValue);
    READWRITE(randomness);
    READWRITE(serial);
  }

 public:
};

} /* namespace libzerocoin */

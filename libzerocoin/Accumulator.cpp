/**
 * @file       Accumulator.cpp
 *
 * @brief      Accumulator and AccumulatorWitness classes for the Zerocoin library.
 *
 * @author     Ian Miers, Christina Garman and Matthew Green
 * @date       June 2013
 *
 * @copyright  Copyright 2013 Ian Miers, Christina Garman and Matthew Green
 *  license    This project is released under the MIT license.
 **/
// Copyright (c) 2018 The PIVX developer
// Copyright (c) 2018 Jon Spock

#include "Accumulator.h"
#include "ZerocoinDefines.h"
#include <iostream>
#include <sstream>

namespace libzerocoin {
// Accumulator class
Accumulator::Accumulator(const AccumulatorAndProofParams* p, const CoinDenomination d, int iterations) : params(p) {
  assert(p);
  denomination = d;
  this->value = this->params->accumulatorBase;
  this->zkp_iterations = iterations;
}

Accumulator::Accumulator(const ZerocoinParams* p, const CoinDenomination d) : denomination(d) {
  assert(p);
  this->params = &(p->accumulatorParams);
  this->zkp_iterations = p->zkp_iterations;
  this->value = this->params->accumulatorBase;
}
Accumulator::Accumulator(const ZerocoinParams* p, const Bignum& bnValue, const CoinDenomination d) : denomination(d) {
  assert(p);
  this->params = &(p->accumulatorParams);
  this->zkp_iterations = p->zkp_iterations;

  if (bnValue != 0)
    this->value = bnValue;
  else
    this->value = this->params->accumulatorBase;
}

void Accumulator::increment(const CBigNum& bnValue) {
  // Compute new accumulator = "old accumulator"^{element} mod N
  this->value = this->value.pow_mod(bnValue, this->params->accumulatorModulus);
}

void Accumulator::accumulate(const PublicCoin& coin) {
  if (this->denomination != coin.getDenomination()) {
    std::cout << "Wrong denomination for coin. Expected coins of denomination: ";
    std::cout << this->denomination;
    std::cout << ". Instead, got a coin of denomination: ";
    std::cout << coin.getDenomination();
    std::cout << "\n";
    throw std::runtime_error("Wrong denomination for coin");
  }

  if (coin.validate(params, zkp_iterations)) {
    increment(coin.getValue());
  } else {
    std::cout << "Coin not valid\n";
    throw std::runtime_error("Coin is not valid");
  }
}

Accumulator& Accumulator::operator+=(const PublicCoin& c) {
  this->accumulate(c);
  return *this;
}

bool Accumulator::operator==(const Accumulator rhs) const { return this->value == rhs.value; }

} /* namespace libzerocoin */

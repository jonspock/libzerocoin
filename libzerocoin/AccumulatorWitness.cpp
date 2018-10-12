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

#include "AccumulatorWitness.h"
#include "ZerocoinDefines.h"
#include <sstream>

namespace libzerocoin {

void AccumulatorWitness::resetValue(const Accumulator& checkpoint, const PublicCoin& coin) {
  this->witness.setValue(checkpoint.getValue());
  this->element = coin;
}

void AccumulatorWitness::AddElement(const PublicCoin& c) {
  if (element.getValue() != c.getValue()) { witness += c; }
}

bool AccumulatorWitness::VerifyWitness(const Accumulator& a, const PublicCoin& publicCoin) const {
  Accumulator temp(witness);
  temp += element;
  if (!(temp == a)) {
    std::cout << "VerifyWitness: failed verify temp does not equal a\n";
    return false;
  } else if (this->element != publicCoin) {
    std::cout << "VerifyWitness: failed verify pubcoins not equal\n";
    return false;
  }
  return true;
}

AccumulatorWitness& AccumulatorWitness::operator+=(const PublicCoin& rhs) {
  this->AddElement(rhs);
  return *this;
}

} /* namespace libzerocoin */

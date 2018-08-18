/**
 * @file       Accumulator.h
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
// x Copyright (c) 2018 The ClubChain developers
#pragma once

#include "Accumulator.h"
#include "PublicCoin.h"

namespace libzerocoin {

/**A witness that a PublicCoin is in the accumulation of a set of coins
 *
 */
class AccumulatorWitness {
 public:
  template <typename Stream> AccumulatorWitness(const ZerocoinParams* p, Stream& strm) { strm >> *this; }

  /**  Construct's a witness.  You must add all elements after the witness
   * @param p pointer to params
   * @param checkpoint the last known accumulator value before the element was added
   * @param coin the coin we want a witness to
   */
  AccumulatorWitness(const ZerocoinParams* p, const Accumulator& checkpoint, const PublicCoin coin)
      : witness(checkpoint), element(coin) {}

  /** Adds element to the set whose's accumulation we are proving coin is a member of
   *
   * @param c the coin to add
   */
  void AddElement(const PublicCoin& c);

  /** Adds element to the set whose's accumulation we are proving coin is a member of. No checks performed!
   *
   * @param bnValue the coin's value to add
   */
  // warning check pubcoin value & denom outside of this function!
  void addRawValue(const CBigNum& bnValue) { witness.increment(bnValue); }

  /**
   *
   * @return the value of the witness
   */
  const CBigNum& getValue() const { return this->witness.getValue(); }
  void resetValue(const Accumulator& checkpoint, const PublicCoin coin);

  /** Checks that this is a witness to the accumulation of coin
   * @param a             the accumulator we are checking against.
   * @param publicCoin    the coin we're providing a witness for
   * @return True if the witness computation validates
   */
  bool VerifyWitness(const Accumulator& a, const PublicCoin& publicCoin) const;

  /**
   * Adds rhs to the set whose's accumulation ware proving coin is a member of
   * @param rhs the PublicCoin to add
   * @return
   */
  AccumulatorWitness& operator+=(const PublicCoin& rhs);

 private:
  Accumulator witness;
  PublicCoin element;  // was const but changed to use setting in assignment
};

} /* namespace libzerocoin */

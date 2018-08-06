// Copyright (c) 2018 The ClubChain developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include "Commitment.h"

namespace libzerocoin {

/// These are free functions instead of a member function

/** Generates a Pedersen commitment to the given value.
 *
 * @param g1 the g value
 * @param h1 the h value
 * @param value the value to commit to
 */
template <ModulusType T, ModulusType G>
Commitment commit(const CBigNum& g1, const CBigNum& h1, const CBigNum& S, const CBigNum& R) {
  const IntegerMod<T> g(g1);
  const IntegerMod<T> h(h1);
  CBigNum m = IntegerModModulus<T>::getModulus();
  /// \f$ commitment = g^{value} * h^{randomness} \f$
  CBigNum commitmentValue = ((g ^ S) * (h ^ R)).getValue();
  // Pack together into a Commitment object to return with
  Commitment commit(R, S, commitmentValue);
  return commit;
}
// Same as above with internal RandBigNum
template <ModulusType T, ModulusType G> Commitment commit(const CBigNum& g1, const CBigNum& h1, const CBigNum& value) {
  const IntegerMod<T> g(g1);
  const IntegerMod<T> h(h1);
  CBigNum m = IntegerModModulus<T>::getModulus();
  CBigNum r = CBigNum::randBignum(m);
  /// \f$ commitment = g^{value} * h^{randomness} \f$
  CBigNum commitmentValue = ((g ^ value) * (h ^ r)).getValue();
  Commitment commit(r, value, commitmentValue);
  return commit;
}

} /* namespace libzerocoin */

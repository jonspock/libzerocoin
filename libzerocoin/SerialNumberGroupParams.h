// Copyright (c) 2018 The PIVX developers
#pragma once

#include "bignum.h"

namespace libzerocoin {

class SerialNumberGroupParams {
public:
  /// A generator for the group.
  CBigNum g;

  /// Another generator for the group. 
  CBigNum h;

  CBigNum g1, g2, g3, g4, g5, g6, g7, g8, g9, ga, gb;

  /// The modulus for the group.
  CBigNum modulus;

  /// The order of the group
  CBigNum groupOrder;

  // Later make const
  int N;
  int n;
  int m;
  int n_prime;
  int m1;
  int m2;

  SerialNumberGroupParams() : N(264), n(48), m(11), n_prime(18), m1(2), m2(11) { }

  
  ADD_SERIALIZE_METHODS;
  template <typename Stream, typename Operation>
  inline void SerializationOp(Stream &s, Operation ser_action, int nType,
                              int nVersion) {
    // Should we add extra params here for new code??
    //    READWRITE(initialized);
    READWRITE(g);
    READWRITE(h);
    READWRITE(modulus);
    READWRITE(groupOrder);
  }
};

} // namespace libzerocoin

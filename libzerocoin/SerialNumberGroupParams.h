// Copyright (c) 2018 The PIVX developer
// Copyright (c) 2018 Jon Spock
#pragma once

#include "bignum.h"

namespace libzerocoin {

class SerialNumberGroupParams {
 public:
  /// A generator for the group.
  CBigNum g;

  /// Another generator for the group.
  CBigNum h;

  /// The modulus for the group.
  CBigNum modulus;

  /// The order of the group
  CBigNum groupOrder;

  SerialNumberGroupParams() = default;

  ADD_SERIALIZE_METHODS
  template <typename Stream, typename Operation> inline void SerializationOp(Stream &s, Operation ser_action) {
    // Should we add extra params here for new code??
    READWRITE(g);
    READWRITE(h);
    READWRITE(modulus);
    READWRITE(groupOrder);
  }
};

}  // namespace libzerocoin

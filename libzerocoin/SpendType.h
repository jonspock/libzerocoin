// Copyright (c) 2018 The PIVX developer
// Copyright (c) 2018 Jon Spock
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <cstdint>

namespace libzerocoin {
enum SpendType : uint8_t {
  SPEND,        // Used for a typical spend transaction, ZKP should be unusable after
  SIGN_MESSAGE  // Used to sign messages that do not belong above (future)
};
}

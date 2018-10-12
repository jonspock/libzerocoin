// Copyright (c) 2018 Jon Spock
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <sodium.h>
#include "privkey.h"
#include "pubkey.h"
#include <vector>

class CKey {
 private:
  //! byte data for public/secret keys
  unsigned char pk[crypto_box_PUBLICKEYBYTES];
  unsigned char sk[crypto_box_SECRETKEYBYTES];

 public:
  CKey() = default;
  // Handles uint256
  template <typename T> CKey(const T pbegin) { memcpy(sk, (uint8_t*)&pbegin[0], crypto_box_SECRETKEYBYTES); }

  CSecretKey GetPrivKey() const {
    CSecretKey sec(crypto_box_SECRETKEYBYTES);
    for (size_t i = 0; i < crypto_box_SECRETKEYBYTES; i++) sec[i] = sk[i];
    return sec;
  }
  CPubKey GetPubKey() const { return CPubKey(pk); }
  void Sign(const uint256& hash, std::vector<uint8_t>& vchSig) const;
};

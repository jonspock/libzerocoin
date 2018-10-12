// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2016-2017 The PIVX developers
// Copyright (c) 2018 Jon Spock
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include "hash.h"
#include "serialize.h"
#include "uint256.h"
#include <sodium.h>
#include <vector>

class CPubKey {
 private:
  uint8_t vch[crypto_box_PUBLICKEYBYTES];

 public:
  //! Construct an invalid public key.
  CPubKey() = default;

  explicit CPubKey(const unsigned char v[crypto_box_PUBLICKEYBYTES]) {
    for (size_t i = 0; i < crypto_box_PUBLICKEYBYTES; i++) vch[i] = v[i];
  }
  unsigned int size() const { return crypto_box_PUBLICKEYBYTES; }

  //! Implement serialization, as if this was a byte vector.
  unsigned int GetSerializeSize() const { return size() + 1; }
  template <typename Stream> void Serialize(Stream& s) const {
    unsigned int len = size();
    ::WriteCompactSize(s, len);
    s.write((char*)vch, len);
  }
  template <typename Stream> void Unserialize(Stream& s) {
    unsigned int len = ::ReadCompactSize(s);
    if (len <= size()) { s.read((char*)vch, len); }
  }

  //! Get the 256-bit hash of this public key.
  uint256 GetHash() const { return Hash(vch, vch + size()); }
  std::vector<uint8_t> ToStdVector() const { return std::vector<uint8_t>(vch, vch + size()); }
  std::string ToString() {
    std::string my_std_string(reinterpret_cast<const char*>(vch), crypto_box_PUBLICKEYBYTES);
    return my_std_string;
  }

  bool Verify(const uint256& hash, const std::vector<uint8_t>& vchSig) const {
    unsigned char unsigned_message[32]; // message is Hash
    unsigned long long unsigned_message_len;
    bool ok = (crypto_sign_open(unsigned_message, &unsigned_message_len,
                                &vchSig[0], vchSig.size(), vch) != 0);
    std::vector<uint8_t> hashmsg(hash.begin(),hash.end());
    for (int i=0;i<32;i++) {
        if (unsigned_message[i] != hashmsg[i]) return false;
    }
    return ok;
  }
  
};

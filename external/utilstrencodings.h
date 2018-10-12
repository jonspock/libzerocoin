// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2017 The PIVX developers
// Copyright (c) 2018 Jon Spock
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

/**
 * Utilities for converting data from/to strings.
 */
#pragma once

#include <cstdint>
#include <string>
#include <vector>

signed char HexDigit(char c);
template <typename T> std::string HexStr(const T itbegin, const T itend, bool fSpaces = false) {
  std::string rv;
  static const char hexmap[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  rv.reserve((itend - itbegin) * 3);
  for (T it = itbegin; it < itend; ++it) {
    uint8_t val = (uint8_t)(*it);
    if (fSpaces && it != itbegin) rv.push_back(' ');
    rv.push_back(hexmap[val >> 4]);
    rv.push_back(hexmap[val & 15]);
  }

  return rv;
}

template <typename T> inline std::string HexStr(const T& vch, bool fSpaces = false) {
  return HexStr(vch.begin(), vch.end(), fSpaces);
}


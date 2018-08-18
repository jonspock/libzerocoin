// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <cstdint>

class CBigNum;
CBigNum randBignum(const CBigNum& range);
CBigNum RandKBitBigum(const uint32_t k);
CBigNum generatePrime(const unsigned int numBits, bool safe = false);


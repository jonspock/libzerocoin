// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include "uint256.h"
#include <cstdint>
#include <vector>

// Functions to gather random data
void GetRandBytes(uint8_t *buf, int num);
void GetRandBytes(std::vector<uint8_t> &buf);
void GetStrongRandBytes(uint8_t *buf, int num);
uint64_t GetRand(uint64_t nMax);
int GetRandInt(int nMax);
uint256 GetRandHash();


// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

/**
 * Server/client environment: argument handling, config file parsing,
 * logging, thread wrappers
 */
#pragma once

#include "logging.h"
#include "tinyformat.h"

#include <exception>
#include <map>
#include <stdint.h>
#include <string>
#include <vector>

#include <unordered_set>

extern std::string strMiscWarning;

/**
 * Bypass Translation function: Retrofit if needed to use translation later
 */
inline std::string _(const char* psz) { return psz; }

template <typename... Args> bool error(const char* fmt, const Args&... args) {
  LogPrintf("ERROR: " + tfm::format(fmt, args...) + "\n");
  return false;
}



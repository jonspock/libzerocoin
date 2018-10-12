// Copyright (c) 2018 Jon Spock
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include "support/allocators/secure.h"
#include <vector>

using CSecretKey = std::vector<uint8_t, secure_allocator<uint8_t> >;


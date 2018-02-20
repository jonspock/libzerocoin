// Copyright (c) 2018 Jon Spock
#pragma once
#include "ZeroVector.h"
#include "typedefs.h"
#include <algorithm>
#include <iostream>

// Decompose randomness into binary form.
namespace libzerocoin
{
template <ModulusType T_>
std::vector<IntegerMod<T_> > Binary(const CBigNum& v)
{
    std::vector<IntegerMod<T_> > binary_v(2048);
    std::vector<unsigned char> vec = v.getvch();

    size_t max_size = std::min(v.bitSize(), (int)binary_v.size());

    for (size_t i = 0; i < max_size; i++) {
        int bitcount = (i % 8);
        int bytecount = (i / 8);
        IntegerMod<T_> bit = (vec[bytecount] & (1 << bitcount)) ? IntegerMod<T_>(1) : IntegerMod<T_>(0);
        binary_v[v.bitSize() - 1 - i] = bit;
    }

    for (int i = v.bitSize(); i < 256; i++) {
        binary_v[i] = IntegerMod<T_>(0);
    }
    return binary_v;
}
} // end namespace libzerocoin

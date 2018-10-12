// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2018 Jon Spock
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "rand_bignum.h"
#include "bignum.h"
#include <sodium/randombytes.h>

/** Generates a cryptographically secure random number between zero and range exclusive
 * i.e. 0 < returned number < range
 * @param range The upper bound on the number.
 * @return
 */
CBigNum randBignum(const CBigNum &range) {
  size_t size = (mpz_sizeinbase(range.bn, 2) + CHAR_BIT - 1) / CHAR_BIT;
  std::vector<unsigned char> buf(size);

  randombytes_buf(&buf, size);
  CBigNum ret(buf);
  if (ret < 0) mpz_neg(ret.bn, ret.bn);
  return ret;
}

/** Generates a cryptographically secure random k-bit number
 * @param k The bit length of the number.
 * @return
 */
CBigNum RandKBitBigum(const uint32_t k) {
  std::vector<unsigned char> buf((k + 7) / 8);

  randombytes_buf(&buf, (k + 7) / 8);
  CBigNum ret(buf);
  if (ret < 0) mpz_neg(ret.bn, ret.bn);
  return ret;
}

/**
 * Generates a random prime of numBits bits
 * @param numBits the number of bits
 * @param safe true for a safe prime
 * @return the prime
 */
CBigNum generatePrime(const uint32_t numBits) {
  CBigNum rand = RandKBitBigum(numBits);
  CBigNum prime;
  mpz_nextprime(prime.bn, rand.bn);
  return prime;
}

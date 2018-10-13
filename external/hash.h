// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include "sha256.h"
#include "sha512.h"
#include "serialize.h"
#include "uint256.h"
#include "uint512.h"

#include <iomanip>
#include <sstream>
#include <vector>

typedef uint256 ChainCode;

/** A hasher class for Bitcoin's 256-bit hash (double SHA-256). */
class CHash256 {
 private:
  CSHA256 sha;

 public:
  static const size_t OUTPUT_SIZE = CSHA256::OUTPUT_SIZE;

  void Finalize(uint8_t hash[OUTPUT_SIZE]) {
    uint8_t buf[CSHA256::OUTPUT_SIZE];
    sha.Finalize(buf);
    sha.Reset().Write(buf, CSHA256::OUTPUT_SIZE).Finalize(hash);
  }

  CHash256& Write(const uint8_t* data, size_t len) {
    sha.Write(data, len);
    return *this;
  }

  CHash256& Reset() {
    sha.Reset();
    return *this;
  }
};

class CHash512 {
 private:
  CSHA512 sha;

 public:
  static const size_t OUTPUT_SIZE = CSHA512::OUTPUT_SIZE;

  void Finalize(uint8_t hash[OUTPUT_SIZE]) {
    uint8_t buf[CSHA512::OUTPUT_SIZE];
    sha.Finalize(buf);
    sha.Reset().Write(buf, CSHA512::OUTPUT_SIZE).Finalize(hash);
  }

  CHash512& Write(const uint8_t* data, size_t len) {
    sha.Write(data, len);
    return *this;
  }

  CHash512& Reset() {
    sha.Reset();
    return *this;
  }
};

#ifdef GLOBALDEFINED
#define GLOBAL
#else
#define GLOBAL extern
#endif

/** Compute the 512-bit hash of an object. */
template <typename T1> inline uint512 Hash512(const T1 pbegin, const T1 pend) {
  static const uint8_t pblank[1] = {};
  uint512 result;
  CHash512()
      .Write(pbegin == pend ? pblank : (const uint8_t*)&pbegin[0], (pend - pbegin) * sizeof(pbegin[0]))
      .Finalize((uint8_t*)&result);
  return result;
}
template <typename T1, typename T2>
inline uint512 Hash512(const T1 p1begin, const T1 p1end, const T2 p2begin, const T2 p2end) {
  static const uint8_t pblank[1] = {};
  uint512 result;
  CHash512()
      .Write(p1begin == p1end ? pblank : (const uint8_t*)&p1begin[0], (p1end - p1begin) * sizeof(p1begin[0]))
      .Write(p2begin == p2end ? pblank : (const uint8_t*)&p2begin[0], (p2end - p2begin) * sizeof(p2begin[0]))
      .Finalize((uint8_t*)&result);
  return result;
}

/** Compute the 256-bit hash of an object. */
template <typename T1> inline uint256 Hash(const T1 pbegin, const T1 pend) {
  static const uint8_t pblank[1] = {};
  uint256 result;
  CHash256()
      .Write(pbegin == pend ? pblank : (const uint8_t*)&pbegin[0], (pend - pbegin) * sizeof(pbegin[0]))
      .Finalize((uint8_t*)&result);
  return result;
}

/** Compute the 256-bit hash of the concatenation of two objects. */
template <typename T1, typename T2>
inline uint256 Hash(const T1 p1begin, const T1 p1end, const T2 p2begin, const T2 p2end) {
  static const uint8_t pblank[1] = {};
  uint256 result;
  CHash256()
      .Write(p1begin == p1end ? pblank : (const uint8_t*)&p1begin[0], (p1end - p1begin) * sizeof(p1begin[0]))
      .Write(p2begin == p2end ? pblank : (const uint8_t*)&p2begin[0], (p2end - p2begin) * sizeof(p2begin[0]))
      .Finalize((uint8_t*)&result);
  return result;
}

/** Compute the 256-bit hash of the concatenation of three objects. */
template <typename T1, typename T2, typename T3>
inline uint256 Hash(const T1 p1begin, const T1 p1end, const T2 p2begin, const T2 p2end, const T3 p3begin,
                    const T3 p3end) {
  static const uint8_t pblank[1] = {};
  uint256 result;
  CHash256()
      .Write(p1begin == p1end ? pblank : (const uint8_t*)&p1begin[0], (p1end - p1begin) * sizeof(p1begin[0]))
      .Write(p2begin == p2end ? pblank : (const uint8_t*)&p2begin[0], (p2end - p2begin) * sizeof(p2begin[0]))
      .Write(p3begin == p3end ? pblank : (const uint8_t*)&p3begin[0], (p3end - p3begin) * sizeof(p3begin[0]))
      .Finalize((uint8_t*)&result);
  return result;
}

/** Compute the 256-bit hash of the concatenation of three objects. */
template <typename T1, typename T2, typename T3, typename T4>
inline uint256 Hash(const T1 p1begin, const T1 p1end, const T2 p2begin, const T2 p2end, const T3 p3begin,
                    const T3 p3end, const T4 p4begin, const T4 p4end) {
  static const uint8_t pblank[1] = {};
  uint256 result;
  CHash256()
      .Write(p1begin == p1end ? pblank : (const uint8_t*)&p1begin[0], (p1end - p1begin) * sizeof(p1begin[0]))
      .Write(p2begin == p2end ? pblank : (const uint8_t*)&p2begin[0], (p2end - p2begin) * sizeof(p2begin[0]))
      .Write(p3begin == p3end ? pblank : (const uint8_t*)&p3begin[0], (p3end - p3begin) * sizeof(p3begin[0]))
      .Write(p4begin == p4end ? pblank : (const uint8_t*)&p4begin[0], (p4end - p4begin) * sizeof(p4begin[0]))
      .Finalize((uint8_t*)&result);
  return result;
}

/** Compute the 256-bit hash of the concatenation of three objects. */
template <typename T1, typename T2, typename T3, typename T4, typename T5>
inline uint256 Hash(const T1 p1begin, const T1 p1end, const T2 p2begin, const T2 p2end, const T3 p3begin,
                    const T3 p3end, const T4 p4begin, const T4 p4end, const T5 p5begin, const T5 p5end) {
  static const uint8_t pblank[1] = {};
  uint256 result;
  CHash256()
      .Write(p1begin == p1end ? pblank : (const uint8_t*)&p1begin[0], (p1end - p1begin) * sizeof(p1begin[0]))
      .Write(p2begin == p2end ? pblank : (const uint8_t*)&p2begin[0], (p2end - p2begin) * sizeof(p2begin[0]))
      .Write(p3begin == p3end ? pblank : (const uint8_t*)&p3begin[0], (p3end - p3begin) * sizeof(p3begin[0]))
      .Write(p4begin == p4end ? pblank : (const uint8_t*)&p4begin[0], (p4end - p4begin) * sizeof(p4begin[0]))
      .Write(p5begin == p5end ? pblank : (const uint8_t*)&p5begin[0], (p5end - p5begin) * sizeof(p5begin[0]))
      .Finalize((uint8_t*)&result);
  return result;
}

/** Compute the 256-bit hash of the concatenation of three objects. */
template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
inline uint256 Hash(const T1 p1begin, const T1 p1end, const T2 p2begin, const T2 p2end, const T3 p3begin,
                    const T3 p3end, const T4 p4begin, const T4 p4end, const T5 p5begin, const T5 p5end,
                    const T6 p6begin, const T6 p6end) {
  static const uint8_t pblank[1] = {};
  uint256 result;
  CHash256()
      .Write(p1begin == p1end ? pblank : (const uint8_t*)&p1begin[0], (p1end - p1begin) * sizeof(p1begin[0]))
      .Write(p2begin == p2end ? pblank : (const uint8_t*)&p2begin[0], (p2end - p2begin) * sizeof(p2begin[0]))
      .Write(p3begin == p3end ? pblank : (const uint8_t*)&p3begin[0], (p3end - p3begin) * sizeof(p3begin[0]))
      .Write(p4begin == p4end ? pblank : (const uint8_t*)&p4begin[0], (p4end - p4begin) * sizeof(p4begin[0]))
      .Write(p5begin == p5end ? pblank : (const uint8_t*)&p5begin[0], (p5end - p5begin) * sizeof(p5begin[0]))
      .Write(p6begin == p6end ? pblank : (const uint8_t*)&p6begin[0], (p6end - p6begin) * sizeof(p6begin[0]))
      .Finalize((uint8_t*)&result);
  return result;
}

/** A writer stream (for serialization) that computes a 256-bit hash. */
class CHashWriter {
 private:
  CHash256 ctx;

 public:
  CHashWriter() {}

  CHashWriter& write(const char* pch, size_t size) {
    ctx.Write((const uint8_t*)pch, size);
    return (*this);
  }

  // invalidates the object
  uint256 GetHash() {
    uint256 result;
    ctx.Finalize((uint8_t*)&result);
    return result;
  }

  template <typename T> CHashWriter& operator<<(const T& obj) {
    // Serialize to this stream
    ::Serialize(*this, obj);
    return (*this);
  }
};

/** Compute the 256-bit hash of an object's serialization. */
template <typename T> uint256 SerializeHash(const T& obj) {
  CHashWriter ss;
  ss << obj;
  return ss.GetHash();
}

unsigned int MurmurHash3(unsigned int nHashSeed, const std::vector<uint8_t>& vDataToHash);

void BIP32Hash(const ChainCode chainCode, unsigned int nChild, uint8_t header, const uint8_t data[32],
               uint8_t output[64]);

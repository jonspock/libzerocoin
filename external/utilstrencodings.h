// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2017 The PIVX developers
// Copyright (c) 2018 The ClubChain developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

/**
 * Utilities for converting data from/to strings.
 */
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#define BEGIN(a) ((char*)&(a))
#define END(a) ((char*)&((&(a))[1]))
#define UBEGIN(a) ((uint8_t*)&(a))
#define UEND(a) ((uint8_t*)&((&(a))[1]))
#define ARRAYLEN(array) (sizeof(array) / sizeof((array)[0]))

std::string SanitizeString(const std::string& str);
std::vector<uint8_t> ParseHex(const char* psz);
std::vector<uint8_t> ParseHex(const std::string& str);
signed char HexDigit(char c);
bool IsHex(const std::string& str);
std::vector<uint8_t> DecodeBase64(const char* p, bool* pfInvalid = nullptr);
std::string DecodeBase64(const std::string& str);
std::string EncodeBase64(const uint8_t* pch, size_t len);
std::string EncodeBase64(const std::string& str);
std::vector<uint8_t> DecodeBase32(const char* p, bool* pfInvalid = nullptr);
std::string DecodeBase32(const std::string& str);
std::string EncodeBase32(const uint8_t* pch, size_t len);
std::string EncodeBase32(const std::string& str);

/**
 * Convert string to signed 32-bit integer with strict parse error feedback.
 * @returns true if the entire string could be parsed as valid integer,
 *   false if not the entire string could be parsed or when overflow or underflow occurred.
 */
bool ParseInt32(const std::string& str, int32_t* out);

/**
 * Convert string to signed 64-bit integer with strict parse error feedback.
 * @returns true if the entire string could be parsed as valid integer,
 *   false if not the entire string could be parsed or when overflow or underflow occurred.
 */
bool ParseInt64(const std::string& str, int64_t* out);

/**
 * Convert string to double with strict parse error feedback.
 * @returns true if the entire string could be parsed as valid double,
 *   false if not the entire string could be parsed or when overflow or underflow occurred.
 */
bool ParseDouble(const std::string& str, double* out);

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

/** Reverse the endianess of a string */
inline std::string ReverseEndianString(std::string in) {
  std::string out = "";
  unsigned int s = in.size();
  for (unsigned int i = 0; i < s; i += 2) { out += in.substr(s - i - 2, 2); }

  return out;
}

/**
 * Format a paragraph of text to a fixed width, adding spaces for
 * indentation to any added line.
 */
std::string FormatParagraph(const std::string in, size_t width = 79, size_t indent = 0);

/**
 * Timing-attack-resistant comparison.
 * Takes time proportional to length
 * of first argument.
 */
template <typename T> bool TimingResistantEqual(const T& a, const T& b) {
  if (b.size() == 0) return a.size() == 0;
  size_t accumulator = a.size() ^ b.size();
  for (size_t i = 0; i < a.size(); i++) accumulator |= a[i] ^ b[i % b.size()];
  return accumulator == 0;
}
/**
 * Converts the given character to its lowercase equivalent.
 * This function is locale independent. It only converts uppercase
 * characters in the standard 7-bit ASCII range.
 * @param[in] c     the character to convert to lowercase.
 * @return          the lowercase equivalent of c; or the argument
 *                  if no conversion is possible.
 */
constexpr unsigned char ToLower(unsigned char c)
{
    return (c >= 'A' && c <= 'Z' ? (c - 'A') + 'a' : c);
}

/**
 * Converts the given string to its lowercase equivalent.
 * This function is locale independent. It only converts uppercase
 * characters in the standard 7-bit ASCII range.
 * @param[in,out] str   the string to convert to lowercase.
 */
void Downcase(std::string& str);
 /**
 * Converts the given character to its uppercase equivalent.
 * This function is locale independent. It only converts lowercase
 * characters in the standard 7-bit ASCII range.
 * @param[in] c     the character to convert to uppercase.
 * @return          the uppercase equivalent of c; or the argument
 *                  if no conversion is possible.
 */
constexpr unsigned char ToUpper(unsigned char c)
{
    return (c >= 'a' && c <= 'z' ? (c - 'a') + 'A' : c);
}
 /**
 * Capitalizes the first character of the given string.
 * This function is locale independent. It only capitalizes the
 * first character of the argument if it has an uppercase equivalent
 * in the standard 7-bit ASCII range.
 * @param[in] str   the string to capitalize.
 * @return          string with the first letter capitalized.
 */
std::string Capitalize(std::string str);

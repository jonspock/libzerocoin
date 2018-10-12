// Copyright (c) 2018 The PIVX developer
// Copyright (c) 2018 Jon Spock
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#pragma once
#include "bignum.h"
#include "serialize.h"
#include <stdexcept>
#include <vector>

#include "ModulusType.h"

template <ModulusType T> class IntegerMod {
 public:
  CBigNum Value;
  static const CBigNum Mod;

 public:
  IntegerMod() = default;
  IntegerMod(const CBigNum& val) {
    Value = val % IntegerMod<T>::Mod;  // Make sure it's reduced at init
  }

  IntegerMod& operator=(const IntegerMod& b) {
    Value = b.Value % Mod;  // Make sure it's reduced (shouldn't be needed)
    return *this;
  }

  IntegerMod& operator=(const CBigNum& b) {
    Value = b % Mod;  // Make sure it's modulo Modulus
    return *this;
  }

  ~IntegerMod() = default;

  void setValue(const CBigNum& b) {
    Value = b % Mod;  // Make sure it's modulo Modulus
  }

  CBigNum getValue() const { return Value; }
  bool isPrime(const int checks = 15) const { return Value.isPrime(checks); }

  void randomize() { throw std::runtime_error("Not implemented yet"); }

  explicit IntegerMod(const std::vector<uint8_t>& vch) { Value.setvch(vch); }

  int bitSize() const { return Value.bitSize(); }

  void setvch(const std::vector<uint8_t>& vch) { Value.setvch(vch); }
  std::vector<uint8_t> getvch() const { return Value.getvch(); }
  void SetHex(const std::string& str) { Value.SetHex(str); }
  std::string ToString(int nBase = 10) const { return Value.ToString(nBase); }
  std::string GetHex() const { return ToString(16); }
  IntegerMod operator^(const IntegerMod& e) const {
    IntegerMod ret(*this);
    if (e.Value < 0) {
      // g^-x = (g^-1)^x
      CBigNum inv = Value.inverse(Mod);
      CBigNum posE = e.Value * -1;
      ret.Value = inv.pow_mod(posE, Mod);
    } else {
      ret.Value = Value.pow_mod(e.Value, Mod);
    }
    return ret;
  }
  IntegerMod operator^(const CBigNum& e) const {
    IntegerMod ret(*this);
    if (e < 0) {
      // g^-x = (g^-1)^x
      CBigNum inv = Value.inverse(Mod);
      CBigNum posE = e * -1;
      ret.Value = inv.pow_mod(posE, Mod);
    } else {
      ret.Value = Value.pow_mod(e, Mod);
    }
    return ret;
  }

  IntegerMod inverse() const {
    IntegerMod ret(*this);
    mpz_invert(ret.Value.bn, Value.bn, Mod.bn);
    return ret;
  }

  IntegerMod& operator+=(const IntegerMod& b) {
    Value += b.getValue();
    Value = Value % Mod;
    return *this;
  }

  IntegerMod& operator-=(const IntegerMod& b) {
    Value -= b.getValue();
    Value = Value % Mod;
    return *this;
  }

  IntegerMod& operator*=(const IntegerMod& b) {
    Value = Value.mul_mod(b.Value, Mod);
    return *this;
  }

  IntegerMod& operator/=(const IntegerMod& b) {
    Value = getValue() / b.getValue();
    return *this;
  }

  IntegerMod& operator++() {
    // prefix operator
    mpz_add(Value.bn, Value.bn, CBigNum(1).bn);
    Value = Value % Mod;
    return *this;
  }

  const IntegerMod operator++(int) {
    // postfix operator
    const IntegerMod ret = *this;
    ++(*this);
    return ret;
  }

  IntegerMod& operator--() {
    // prefix operator
    IntegerMod r(*this);
    mpz_sub(Value.bn, Value.bn, CBigNum(1).bn);
    Value = r.Value % Mod;
    return *this;
  }

  const IntegerMod operator--(int) {
    // postfix operator
    const IntegerMod ret = *this;
    --(*this);
    return ret;
  }

  uint32_t GetSerializeSize() const { return ::GetSerializeSize(getvch()); }

  template <typename Stream> void Serialize(Stream& s) const { ::Serialize(s, getvch()); }

  template <typename Stream> void Unserialize(Stream& s) {
    std::vector<uint8_t> vch;
    ::Unserialize(s, vch);
    setvch(vch);
  }
};

template <ModulusType T> inline const IntegerMod<T> operator+(const IntegerMod<T>& a, const IntegerMod<T>& b) {
  IntegerMod<T> r(a);
  CBigNum sum;
  mpz_add(sum.bn, a.Value.bn, b.Value.bn);
  mpz_mmod(r.Value.bn, sum.bn, a.Mod.bn);
  return r;
}

template <ModulusType T> inline const IntegerMod<T> operator-(const IntegerMod<T>& a, const IntegerMod<T>& b) {
  IntegerMod<T> r(a);
  CBigNum sum;
  mpz_sub(sum.bn, a.Value.bn, b.Value.bn);
  mpz_mmod(r.Value.bn, sum.bn, a.Mod.bn);
  return r;
}

template <ModulusType T> inline const IntegerMod<T> operator-(const IntegerMod<T>& a) {
  IntegerMod<T> r(a);
  mpz_neg(r.Value.bn, a.Value.bn);
  return r;
}

template <ModulusType T> inline const IntegerMod<T> operator*(const IntegerMod<T>& a, const IntegerMod<T>& b) {
  IntegerMod<T> r(a);
  r.Value = (a.Value * b.Value) % r.Mod;
  return r;
}
template <ModulusType T> inline const IntegerMod<T> operator*(const CBigNum& a, const IntegerMod<T>& b) {
  IntegerMod<T> r(b);
  r.Value = (a * b.Value) % r.Mod;
  return r;
}
template <ModulusType T> inline const IntegerMod<T> operator*(const IntegerMod<T>& a, const CBigNum& b) {
  IntegerMod<T> r(a);
  r.Value = (a.Value * b) % r.Mod;
  return r;
}

template <ModulusType T> inline const IntegerMod<T> operator/(const IntegerMod<T>& a, const IntegerMod<T>& b) {
  IntegerMod<T> r(a);
  CBigNum t = b.Value.inverse(a.Mod);
  IntegerMod<T> ti(t);
  IntegerMod<T> ret = r * ti;
  return ret;
}
/*
template <ModulusType T> inline const IntegerMod<T> operator%(const IntegerMod<T>& a, const IntegerMod<T>& b) {
  IntegerMod<T> r(a);
  if (!BN_nnmod(&r.Value.bn, &a.Value.bn, &b.Value.bn, pctx))
    throw std::runtime_error("IntegerMod<T>::operator% : BN_div failed");
  return r;
}
*/

template <ModulusType T> inline bool operator==(const IntegerMod<T>& a, const IntegerMod<T>& b) {
  return (a.Value == b.Value);
}
template <ModulusType T> inline bool operator!=(const IntegerMod<T>& a, const IntegerMod<T>& b) {
  return (a.Value != b.Value);
}
template <ModulusType T> inline bool operator<=(const IntegerMod<T>& a, const IntegerMod<T>& b) {
  return (a.Value <= b.Value);
}
template <ModulusType T> inline bool operator>=(const IntegerMod<T>& a, const IntegerMod<T>& b) {
  return (a.Value >= b.Value);
}
template <ModulusType T> inline bool operator<(const IntegerMod<T>& a, const IntegerMod<T>& b) {
  return (a.Value < b.Value);
}
template <ModulusType T> inline bool operator>(const IntegerMod<T>& a, const IntegerMod<T>& b) {
  return (a.Value > b.Value);
}

template <ModulusType T> inline bool operator==(const IntegerMod<T>& a, const CBigNum& b) { return (a.Value == b); }
template <ModulusType T> inline bool operator!=(const IntegerMod<T>& a, const CBigNum& b) { return (a.Value != b); }
template <ModulusType T> inline bool operator<=(const IntegerMod<T>& a, const CBigNum& b) { return (a.Value <= b); }
template <ModulusType T> inline bool operator>=(const IntegerMod<T>& a, const CBigNum& b) { return (a.Value >= b); }
template <ModulusType T> inline bool operator<(const IntegerMod<T>& a, const CBigNum& b) { return (a.Value < b); }
template <ModulusType T> inline bool operator>(const IntegerMod<T>& a, const CBigNum& b) { return (a.Value > b); }

template <ModulusType T> inline std::ostream& operator<<(std::ostream& strm, const IntegerMod<T>& b) {
  return strm << b.Value.ToString(10);
}

template <> const CBigNum IntegerMod<ACCUMULATOR_MODULUS>::Mod;
template <> const CBigNum IntegerMod<COIN_COMMITMENT_MODULUS>::Mod;
template <> const CBigNum IntegerMod<SERIAL_NUMBER_SOK_COMMITMENT_GROUP>::Mod;
template <> const CBigNum IntegerMod<SERIAL_NUMBER_SOK_COMMITMENT_MODULUS>::Mod;
template <> const CBigNum IntegerMod<ACCUMULATOR_POK_COMMITMENT_MODULUS>::Mod;
template <> const CBigNum IntegerMod<ACCUMULATOR_POK_COMMITMENT_GROUP>::Mod;

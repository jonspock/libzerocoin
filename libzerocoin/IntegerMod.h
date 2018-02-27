// Copyright (c) 2018 The PIVX developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#pragma once
#include "serialize.h"
#include <libzerocoin/bignum.h>
#include <openssl/bn.h>
#include <stdexcept>
#include <vector>

template <ModulusType T>
class IntegerMod
{
public:
    CBigNum Value;
    static const CBigNum Mod;

public:
    IntegerMod() {}
    IntegerMod(CBigNum val)
    {
        Value = val % IntegerMod<T>::Mod; // Make sure it's reduced at init
    }

    IntegerMod& operator=(const IntegerMod& b) 
    {
        Value = b.Value % Mod; // Make sure it's reduced (shouldn't be needed)
        return *this;
    }

    IntegerMod& operator=(const CBigNum& b)
    {
        Value = b % Mod; // Make sure it's modulo Modulus
        return *this;
    }

    ~IntegerMod()
    {
    }

    void setValue(CBigNum b)
    {
        Value = b % Mod; // Make sure it's modulo Modulus
    }

    CBigNum getValue() const { return Value; }
    bool isPrime(const int checks = BN_prime_checks) const
    {
        return Value.isPrime(checks);
    }

    void randomize()
    {
        if(!BN_rand_range(&Value, &Mod)){
          throw std::runtime_error("IntegerMod:rand : BN_rand_range failed");
        }
    }

    explicit IntegerMod(const std::vector<unsigned char>& vch)
    {
        Value.setvch(vch);
    }

    int bitSize() const
    {
        return BN_num_bits(&Value);
    }

    void setvch(const std::vector<unsigned char>& vch)
    {
        Value.setvch(vch);
    }
    std::vector<unsigned char> getvch() const { return Value.getvch(); }
    void SetHex(const std::string& str)
    {
        Value.SetHex(str);
    }
    std::string ToString(int nBase = 10) const { return Value.ToString(nBase); }
    std::string GetHex() const
    {
        return ToString(16);
    }
    IntegerMod operator^(const IntegerMod& e) const
    {
        CAutoBN_CTX pctx;
        IntegerMod ret(*this);
        if (e.Value < 0) {
            // g^-x = (g^-1)^x
            CBigNum inv = Value.inverse(Mod);
            CBigNum posE = e.Value * -1;
            if (!BN_mod_exp(&ret.Value, &inv, &posE, &Mod, pctx))
                throw std::runtime_error("IntegerMod::pow_mod: BN_mod_exp failed on negative exponent");
        } else if (!BN_mod_exp(&ret.Value, &Value, &e.Value, &Mod, pctx))
            throw std::runtime_error("IntegerMod::operator^GF pow_mod : BN_mod_exp failed");
        return ret;
    }
    IntegerMod operator^(const CBigNum& e) const
    {
        CAutoBN_CTX pctx;
        IntegerMod ret(*this);
        if (e < 0) {
            // g^-x = (g^-1)^x
            CBigNum inv = Value.inverse(Mod);
            CBigNum posE = e * -1;
            if (!BN_mod_exp(&ret.Value, &inv, &posE, &Mod, pctx))
                throw std::runtime_error("IntegerMod::pow_mod: BN_mod_exp failed on negative exponent");
        } else if (!BN_mod_exp(&ret.Value, &Value, &e, &Mod, pctx))
            throw std::runtime_error("IntegerMod operator^CBigNum pow_mod : BN_mod_exp failed");
        return ret;
    }

    IntegerMod inverse() const
    {
        CAutoBN_CTX pctx;
        IntegerMod ret(*this);
        if (!BN_mod_inverse(&ret.Value, &Value, &Mod, pctx))
            throw std::runtime_error("IntegerMod::inverse*= :BN_mod_inverse");
        return ret;
    }

    IntegerMod& operator+=(const IntegerMod& b)
    {
        Value += b.getValue();
        Value = Value % Mod;
        return *this;
    }

    IntegerMod& operator-=(const IntegerMod& b)
    {
        Value -= b.getValue();
        Value = Value % Mod; 
        return *this;
    }

    IntegerMod& operator*=(const IntegerMod& b)
    {
        CAutoBN_CTX pctx;
        if (!BN_mod_mul(&Value, &Value, &b.Value, &Mod, pctx))
            throw std::runtime_error("IntegerMod::operator*= : BN_mul failed");
        return *this;
    }

    IntegerMod& operator/=(const IntegerMod& b)
    {
        Value = getValue() / b.getValue();
        return *this;
    }

    IntegerMod& operator++()
    {
        // prefix operator
        if (!BN_add(&Value, &Value, BN_value_one()))
            throw std::runtime_error("IntegerMod::operator++ : BN_add failed");
        Value = Value % Mod;
        return *this;
    }

    const IntegerMod operator++(int)
    {
        // postfix operator
        const IntegerMod ret = *this;
        ++(*this);
        return ret;
    }

    IntegerMod& operator--()
    {
        // prefix operator
        IntegerMod r(*this);
        if (!BN_sub(&r.Value, &Value, BN_value_one()))
            throw std::runtime_error("IntegerMod::operator-- : BN_sub failed");
        Value = r.Value % Mod;
        return *this;
    }

    const IntegerMod operator--(int)
    {
        // postfix operator
        const IntegerMod ret = *this;
        --(*this);
        return ret;
    }

    unsigned int GetSerializeSize(int nType = 0, int nVersion = PROTOCOL_VERSION) const
    {
        return ::GetSerializeSize(getvch(), nType, nVersion);
    }

    template <typename Stream>
    void Serialize(Stream& s, int nType = 0, int nVersion = PROTOCOL_VERSION) const
    {
        ::Serialize(s, getvch(), nType, nVersion);
    }

    template <typename Stream>
    void Unserialize(Stream& s, int nType = 0, int nVersion = PROTOCOL_VERSION)
    {
        std::vector<unsigned char> vch;
        ::Unserialize(s, vch, nType, nVersion);
        setvch(vch);
    }
};


template <ModulusType T>
inline const IntegerMod<T> operator+(const IntegerMod<T>& a, const IntegerMod<T>& b)
{
    IntegerMod<T> r(a);
    if (!BN_add(&r.Value, &a.Value, &b.Value))
        throw std::runtime_error("IntegerMod<T>::operator+ : BN_add failed");
    return r;
}

template <ModulusType T>
inline const IntegerMod<T> operator-(const IntegerMod<T>& a, const IntegerMod<T>& b)
{
    IntegerMod<T> r(a);
    if (!BN_sub(&r.Value, &a.Value, &b.Value))
        throw std::runtime_error("IntegerMod<T>::operator- : BN_sub failed");
    return r;
}

template <ModulusType T>
inline const IntegerMod<T> operator-(const IntegerMod<T>& a)
{
    IntegerMod<T> r(a);
    BN_set_negative(&r.Value, !BN_is_negative(&r.Value));
    return r;
}

template <ModulusType T>
inline const IntegerMod<T> operator*(const IntegerMod<T>& a, const IntegerMod<T>& b)
{
    CAutoBN_CTX pctx;
    IntegerMod<T> r(a);
    if (!BN_mod_mul(&r.Value, &a.Value, &b.Value, &r.Mod, pctx))
        throw std::runtime_error("IntegerMod<T>::operator* : BN_mul failed");
    return r;
}
template <ModulusType T>
inline const IntegerMod<T> operator*(const CBigNum& a, const IntegerMod<T>& b)
{
    CAutoBN_CTX pctx;
    IntegerMod<T> r(b);
    if (!BN_mod_mul(&r.Value, &a, &b.Value, &r.Mod, pctx))
        throw std::runtime_error("IntegerMod<T>::operator* : BN_mul failed");
    return r;
}
template <ModulusType T>
inline const IntegerMod<T> operator*(const IntegerMod<T>& a, const CBigNum& b)
{
    CAutoBN_CTX pctx;
    IntegerMod<T> r(a);
    if (!BN_mod_mul(&r.Value, &a.Value, &b, &r.Mod, pctx))
        throw std::runtime_error("IntegerMod<T>::operator* : BN_mul failed");
    return r;
}

template <ModulusType T>
inline const IntegerMod<T> operator/(const IntegerMod<T>& a, const IntegerMod<T>& b)
{
    CAutoBN_CTX pctx;
    IntegerMod<T> r(a);
    IntegerMod<T> t(b);
    if (!BN_mod_inverse(&t.Value, &b.Value, &a.Mod, pctx))
        throw std::runtime_error("IntegerMod<T>::inverse*= :BN_mod_inverse");
    IntegerMod<T> ret = r * t;
    return ret;
}

template <ModulusType T>
inline const IntegerMod<T> operator%(const IntegerMod<T>& a, const IntegerMod<T>& b)
{
    CAutoBN_CTX pctx;
    IntegerMod<T> r(a);
    if (!BN_nnmod(&r.Value, &a.Value, &b.Value, pctx))
        throw std::runtime_error("IntegerMod<T>::operator% : BN_div failed");
    return r;
}

template <ModulusType T>
inline bool operator==(const IntegerMod<T>& a, const IntegerMod<T>& b)
{
    return (BN_cmp(&a.Value, &b.Value) == 0);
}
template <ModulusType T>
inline bool operator!=(const IntegerMod<T>& a, const IntegerMod<T>& b)
{
    return (BN_cmp(&a.Value, &b.Value) != 0);
}
template <ModulusType T>
inline bool operator<=(const IntegerMod<T>& a, const IntegerMod<T>& b)
{
    return (BN_cmp(&a.Value, &b.Value) <= 0);
}
template <ModulusType T>
inline bool operator>=(const IntegerMod<T>& a, const IntegerMod<T>& b)
{
    return (BN_cmp(&a.Value, &b.Value) >= 0);
}
template <ModulusType T>
inline bool operator<(const IntegerMod<T>& a, const IntegerMod<T>& b)
{
    return (BN_cmp(&a.Value, &b.Value) < 0);
}
template <ModulusType T>
inline bool operator>(const IntegerMod<T>& a, const IntegerMod<T>& b)
{
    return (BN_cmp(&a.Value, &b.Value) > 0);
}

template <ModulusType T>
inline bool operator==(const IntegerMod<T>& a, const CBigNum& b)
{
    return (BN_cmp(&a.Value, &b) == 0);
}
template <ModulusType T>
inline bool operator!=(const IntegerMod<T>& a, const CBigNum& b)
{
    return (BN_cmp(&a.Value, &b) != 0);
}
template <ModulusType T>
inline bool operator<=(const IntegerMod<T>& a, const CBigNum& b)
{
    return (BN_cmp(&a.Value, &b) <= 0);
}
template <ModulusType T>
inline bool operator>=(const IntegerMod<T>& a, const CBigNum& b)
{
    return (BN_cmp(&a.Value, &b) >= 0);
}
template <ModulusType T>
inline bool operator<(const IntegerMod<T>& a, const CBigNum& b)
{
    return (BN_cmp(&a.Value, &b) < 0);
}
template <ModulusType T>
inline bool operator>(const IntegerMod<T>& a, const CBigNum& b)
{
    return (BN_cmp(&a.Value, &b) > 0);
}

template <ModulusType T>
inline std::ostream& operator<<(std::ostream& strm, const IntegerMod<T>& b)
{
    return strm << b.Value.ToString(10);
}


 template <> const CBigNum IntegerMod<ACCUMULATOR_MODULUS>::Mod;
 template <> const CBigNum IntegerMod<COIN_COMMITMENT_MODULUS>::Mod;
 template <> const CBigNum IntegerMod<SERIAL_NUMBER_SOK_COMMITMENT_GROUP>::Mod;
 template <> const CBigNum IntegerMod<SERIAL_NUMBER_SOK_COMMITMENT_MODULUS>::Mod;
 template <> const CBigNum IntegerMod<ACCUMULATOR_POK_COMMITMENT_MODULUS>::Mod;
 template <> const CBigNum IntegerMod<ACCUMULATOR_POK_COMMITMENT_GROUP>::Mod;


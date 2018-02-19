/**
 * @file       Coin.h
 *
 * @brief      PublicCoin and PrivateCoin classes for the Zerocoin library.
 *
 * @author     Ian Miers, Christina Garman and Matthew Green
 * @date       June 2013
 *
 * @copyright  Copyright 2013 Ian Miers, Christina Garman and Matthew Green
 *  license    This project is released under the MIT license.
 **/
// Copyright (c) 2018 The PIVX developers
#pragma once

#include "Denominations.h"
#include "ZerocoinParams.h"
#include "amount.h"
#include "bignum.h"
#include "util.h"
namespace libzerocoin
{
/** A Public coin is the part of a coin that
 * is published to the network and what is handled
 * by other clients. It contains only the value
 * of commitment to a serial number and the
 * denomination of the coin.
 */
class PublicCoin
{
public:
    template <typename Stream> PublicCoin(Stream& strm) 
    {
        strm >> *this;
    }

    PublicCoin() {     denomination = ZQ_ERROR; }   // Assume this will get set by another method later

    /**Generates a public coin
     *
     * @param coin the value of the commitment.
     * @param denomination The denomination of the coin. 
     */
    PublicCoin(const CBigNum& coin, const CoinDenomination d) : value(coin), denomination(d) {
        if (denomination == 0) {
            throw std::runtime_error("Denomination does not exist");
        }
    }

    const CBigNum& getValue() const { return this->value; }
    CoinDenomination getDenomination() const { return this->denomination; }
    bool operator==(const PublicCoin& rhs) const
    {
        return ((this->value == rhs.value) && (this->denomination == rhs.denomination));
    }
    bool operator!=(const PublicCoin& rhs) const { return !(*this == rhs); }

    /** Checks that coin is prime and in the appropriate range given the parameters
     * @return true if valid
     */
    bool validate(const AccumulatorAndProofParams *p, int iterations) const {
        return (p->minCoinValue < value) && (value < p->maxCoinValue) && value.isPrime(iterations);
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(value);
        READWRITE(denomination);
    }

private:
    CBigNum value;
    CoinDenomination denomination;
};

}

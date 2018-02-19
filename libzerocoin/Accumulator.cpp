/**
 * @file       Accumulator.cpp
 *
 * @brief      Accumulator and AccumulatorWitness classes for the Zerocoin library.
 *
 * @author     Ian Miers, Christina Garman and Matthew Green
 * @date       June 2013
 *
 * @copyright  Copyright 2013 Ian Miers, Christina Garman and Matthew Green
 *  license    This project is released under the MIT license.
 **/
// Copyright (c) 2018 The PIVX developers

#include "Accumulator.h"
#include "ZerocoinDefines.h"
#include <iostream>
#include <sstream>

namespace libzerocoin
{
//Accumulator class
Accumulator::Accumulator(const AccumulatorAndProofParams* p, const CoinDenomination d, int iterations) : params(p)
{
    if (!(params->initialized)) {
        throw std::runtime_error("Invalid parameters for accumulator");
    }
    denomination = d;
    this->value = this->params->accumulatorBase;
    this->zkp_iterations = iterations;
}

Accumulator::Accumulator(const ZerocoinParams* p, const CoinDenomination d, const Bignum bnValue) : denomination(d)
{
    if (!(params->initialized)) {
        throw std::runtime_error("Invalid parameters for accumulator");
    }
    this->params = &(p->accumulatorParams);
    this->zkp_iterations = p->zkp_iterations;

    if (bnValue != 0)
        this->value = bnValue;
    else
        this->value = this->params->accumulatorBase;
}

void Accumulator::increment(const CBigNum& bnValue)
{
    // Compute new accumulator = "old accumulator"^{element} mod N
    this->value = this->value.pow_mod(bnValue, this->params->accumulatorModulus);
}

void Accumulator::accumulate(const PublicCoin& coin, int zkp_iterations)
{
    // Make sure we're initialized
    if (!(this->value)) {
        std::cout << "Accumulator is not initialized"
                  << "\n";
        throw std::runtime_error("Accumulator is not initialized");
    }

    if (this->denomination != coin.getDenomination()) {
        std::cout << "Wrong denomination for coin. Expected coins of denomination: ";
        std::cout << this->denomination;
        std::cout << ". Instead, got a coin of denomination: ";
        std::cout << coin.getDenomination();
        std::cout << "\n";
        throw std::runtime_error("Wrong denomination for coin");
    }

    if (coin.validate(params,zkp_iterations)) {
        increment(coin.getValue());
    } else {
        std::cout << "Coin not valid\n";
        throw std::runtime_error("Coin is not valid");
    }
}

Accumulator& Accumulator::operator+=(const PublicCoin& c)
{
    this->accumulate(c,zkp_iterations);
    return *this;
}

Accumulator& Accumulator::operator=(Accumulator rhs)
{
    if (this != &rhs) std::swap(*this, rhs);
    return *this;
}

bool Accumulator::operator==(const Accumulator rhs) const
{
    return this->value == rhs.value;
}

} /* namespace libzerocoin */
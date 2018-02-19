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

#include <sstream>
#include "AccumulatorWitness.h"
#include "ZerocoinDefines.h"

namespace libzerocoin {

void AccumulatorWitness::resetValue(const Accumulator& checkpoint, const PublicCoin coin) {
    this->witness.setValue(checkpoint.getValue());
    this->element = coin;
}

void AccumulatorWitness::AddElement(const PublicCoin& c) {
	if(element != c) {
		witness += c;
	}
}

bool AccumulatorWitness::VerifyWitness(const Accumulator& a, const PublicCoin &publicCoin) const {
	Accumulator temp(witness);
	temp += element;
	return (temp == a && this->element == publicCoin);
}

AccumulatorWitness& AccumulatorWitness::operator +=(
    const PublicCoin& rhs) {
	this->AddElement(rhs);
	return *this;
}

AccumulatorWitness& AccumulatorWitness::operator =(AccumulatorWitness rhs) {
    // Not pretty, but seems to work (SPOCK)
    if (&witness != &rhs.witness) this->witness = rhs.witness;
    if (&element != &rhs.element) std::swap(element, rhs.element);
	return *this;
}

} /* namespace libzerocoin */

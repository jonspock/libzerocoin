// Copyright (c) 2018 Jon Spock
#include "IntegerMod.h"
#include "ModulusType.h"

template <> const CBigNum IntegerMod<ACCUMULATOR_MODULUS>::Mod = IntegerModModulus<ACCUMULATOR_MODULUS>::getModulus();
template <>
const CBigNum IntegerMod<COIN_COMMITMENT_MODULUS>::Mod = IntegerModModulus<COIN_COMMITMENT_MODULUS>::getModulus();
template <>
const CBigNum IntegerMod<SERIAL_NUMBER_SOK_COMMITMENT_GROUP>::Mod =
    IntegerModModulus<SERIAL_NUMBER_SOK_COMMITMENT_GROUP>::getModulus();
template <>
const CBigNum IntegerMod<SERIAL_NUMBER_SOK_COMMITMENT_MODULUS>::Mod =
    IntegerModModulus<SERIAL_NUMBER_SOK_COMMITMENT_MODULUS>::getModulus();
template <>
const CBigNum IntegerMod<ACCUMULATOR_POK_COMMITMENT_MODULUS>::Mod =
    IntegerModModulus<ACCUMULATOR_POK_COMMITMENT_MODULUS>::getModulus();
template <>
const CBigNum IntegerMod<ACCUMULATOR_POK_COMMITMENT_GROUP>::Mod =
    IntegerModModulus<ACCUMULATOR_POK_COMMITMENT_GROUP>::getModulus();

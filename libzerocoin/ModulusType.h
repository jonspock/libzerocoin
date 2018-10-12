#pragma once
#include "bignum.h"

enum ModulusType {
  ACCUMULATOR_MODULUS,
  COIN_COMMITMENT_MODULUS,
  SERIAL_NUMBER_SOK_COMMITMENT_MODULUS,
  SERIAL_NUMBER_SOK_COMMITMENT_GROUP,
  ACCUMULATOR_POK_COMMITMENT_MODULUS,
  ACCUMULATOR_POK_COMMITMENT_GROUP
};

/// This is how to access the Modulus/Group values for each defined enum
/// If the below template class is not specialized for a value that is being used, you should get a compile error
template <ModulusType T> class IntegerModModulus {
 public:
  static CBigNum getModulus() {
    static_assert(true, " undefined Modulus type, please use one of ModulusType enum");
    throw std::runtime_error("Undefined default type");
  }
};

template <> inline CBigNum IntegerModModulus<ACCUMULATOR_MODULUS>::getModulus() {
  return CBigNum(
      "c7970ceedcc3b0754490201a7aa613cd73911081c790f5f1a8726f463550bb5b7ff0db8e1ea1189ec72f93d1650011bd721aeeacc2acde3"
      "2a04107f0648c2813a31f5b0b7765ff8b44b4b6ffc93384b646eb09c7cf5e8592d40ea33c80039f35b4f14a04b51f7bfd781be4d1673164"
      "ba8eb991c2c4d730bbbe35f592bdef524af7e8daefd26c66fc02c479af89d64d373f442709439de66ceb955f3ea37d5159f6135809f8533"
      "4b5cb1813addc80cd05609f10ac6a95ad65872c909525bdad32bc729592642920f24c61dc5b3c3b7923e56b16a4d9d373d8721f24a3fc0f"
      "1b3131f55615172866bccc30f95054c824e733a5eb6817f7bc16399d48c6361cc7e5");
}
template <> inline CBigNum IntegerModModulus<COIN_COMMITMENT_MODULUS>::getModulus() {
  return CBigNum(
      "e5f1c46cf7c4676f8e17f88373e340d6678a6054f55dc93694479a2844706f5e72ae264e793226cac0e59480a0d9037729a47201c7e67d8"
      "2894bbc986b1b478341649e3d59372cec09f9e2f5dd0815e9d4e93fd4918fe1dd2ec4ee9375ac0be438f82f715c6f5f1d673785d79c962c"
      "6097f7961d37c2508d2b933024723ba241");
}

template <> inline CBigNum IntegerModModulus<SERIAL_NUMBER_SOK_COMMITMENT_MODULUS>::getModulus() {
  return CBigNum(
      "10f4335b88c49b20599a0472b12b6167cee253da43974a35e62ec77db80bca3616b49713092f929c32f8ed52fbdc00216931ffe7e19d1e8"
      "0ffdf7587bce5a2e5cd724b2ac5f3f16fe73c4c9be0abf89d9d92b294cc3b7bc72ed2c5171f4d0f6073b34c7f7bb0b6234afc37fe45ab92"
      "859f346131677c73b068967a2cafec25968af");
}

template <> inline CBigNum IntegerModModulus<SERIAL_NUMBER_SOK_COMMITMENT_GROUP>::getModulus() {
  return CBigNum(
      "e5f1c46cf7c4676f8e17f88373e340d6678a6054f55dc93694479a2844706f5e72ae264e793226cac0e59480a0d9037729a47201c7e67d8"
      "2894bbc986b1b478341649e3d59372cec09f9e2f5dd0815e9d4e93fd4918fe1dd2ec4ee9375ac0be438f82f715c6f5f1d673785d79c962c"
      "6097f7961d37c2508d2b933024723ba241");
}

template <> inline CBigNum IntegerModModulus<ACCUMULATOR_POK_COMMITMENT_MODULUS>::getModulus() {
  return CBigNum(
      "f09b3785fa945027296dbbb660809191b7aaa73581fb6af13ffdfa319a597ffda256bfd0a922a442a80d48c9809849b001999e9e8da06f12"
      "ec9807996d550f10beda5d7b871");
}

template <> inline CBigNum IntegerModModulus<ACCUMULATOR_POK_COMMITMENT_GROUP>::getModulus() {
  return CBigNum("1723c6b5051557deef6da8dd8d3f3e48f68972cbbac1ca80d910777f72f38b25f");
}

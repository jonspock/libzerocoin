/**
 * @brief      Parameter class for Zerocoin.
 * @author     Ian Miers, Christina Garman and Matthew Green
 * @copyright  Copyright 2013 Ian Miers, Christina Garman and Matthew Green
 *  license    This project is released under the MIT license.
 **/
// Copyright (c) 2018 The PIVX developer
// Copyright (c) 2018 Jon Spock
#include "ZerocoinParams.h"
#include "AccumulatorAndProofParams.h"
#include "ModulusType.h"
#include "ParamGeneration.h"

namespace libzerocoin {

ZerocoinParams ZCParamsDec(ZEROCOIN_DEFAULT_SECURITYLEVEL);
ZerocoinParams* gpZerocoinParams = &ZCParamsDec;

ZerocoinParams::ZerocoinParams(uint32_t securityLevel) {
  this->zkp_hash_len = securityLevel;
  this->zkp_iterations = securityLevel;

  this->accumulatorParams.k_prime = ACCPROOF_KPRIME;
  this->accumulatorParams.k_dprime = ACCPROOF_KDPRIME;

  // Set the accumulator modulus to "N".
  this->accumulatorParams.accumulatorModulus = IntegerModModulus<ACCUMULATOR_MODULUS>::getModulus();

  //-------------------------

  this->coinCommitmentGroup.modulus = IntegerModModulus<COIN_COMMITMENT_MODULUS>::getModulus();
  this->coinCommitmentGroup.groupOrder.SetHex("a33a39fceb03fef51aa5f50322b557664a8364d7ad0ada150487fae8576af9e3");

  this->coinCommitmentGroup.g.SetHex(
      "9a7fd6508dfa79258e50019ab6cb59b4f91b2823dcd9250fb3ccf9fd8263b29a15b005c429915cec63e7d3eba1da337f45dd713246c41e39"
      "ac671cf2f87adfc6d45c842ae7ad21ed291e3a48b2a6e5d39381f6d4a9ab83d5aaa5031d17554df70cf5ecfe10096cf1a565d0f826b71eb4"
      "d105a3016afc445613f04ffbd0dd4162");
  this->coinCommitmentGroup.h.SetHex(
      "ccbbdd469de23cfba19728b625ee7b197b60389eebb7383ec63184fe6ddc94acf0e6e68eb49523acff5e4d0c6fd20b744df744c1a7b55414"
      "0d110e6398040425790fe3b9b32e87238f0338c4f52e3f9b84bef7bceace17f26ada12fa5e1ca0d992b79599f0ef29b66c323b88c1471d93"
      "67f991604a97414f99f748ead3d38622");

  //-------------------------

  this->serialNumberSoKCommitmentGroup.groupOrder = IntegerModModulus<SERIAL_NUMBER_SOK_COMMITMENT_GROUP>::getModulus();
  this->serialNumberSoKCommitmentGroup.modulus = IntegerModModulus<SERIAL_NUMBER_SOK_COMMITMENT_MODULUS>::getModulus();

  this->serialNumberSoKCommitmentGroup.g.SetHex(
      "755af74f335a187e660d329f9ff1f2186b8e087797b3043ce17dd4fe734359fa17d5aa2e4190afee489b0a1fee25c9fc08836cb658bdeb7e"
      "fe63fc75e67e3dc3514b2bed4685f82ed104c7ad7c19d171e8dbd589d4c8888e70eec79c5a2d72e6346c91d17e7af34482a5d446423059db"
      "a15e857d4020bcd5095429da2886990032");

  this->serialNumberSoKCommitmentGroup.h.SetHex(
      "5e257cc3861dfbbd85a95f16fdc867780188c0bc469a7744871f9fa79cfb942d3eb60642736d3e6db940f69fd05d19d57a2b1aa686ad8d26"
      "95b39fef8a4c6c92c99636a6172e5b2b9df49e113508185d15b18158f05d63fa4d6819c126f9065b01183043a17022f6c583735797f3e72c"
      "3c9c2485327127158e4cf0eb23391d739");

  //-------------------------

  this->accumulatorParams.accumulatorPoKCommitmentGroup.groupOrder =
      IntegerModModulus<ACCUMULATOR_POK_COMMITMENT_GROUP>::getModulus();
  this->accumulatorParams.accumulatorPoKCommitmentGroup.modulus =
      IntegerModModulus<ACCUMULATOR_POK_COMMITMENT_MODULUS>::getModulus();

  this->accumulatorParams.accumulatorPoKCommitmentGroup.g.SetHex(
      "1d5868e648ac6d41756e0409a510ff5a54bf1ebad22904ab359af54bb0d20a599324cba5ef004d7837cfa6b5904dbc221de6332101df5ef8"
      "d99992e8d5679969cc3221c0ba7");

  this->accumulatorParams.accumulatorPoKCommitmentGroup.h.SetHex(
      "408f47ff9a66729f6e6ad9796ce794e702995429c7f621294491042b63e1db4ed3cd9572e69fd82bd8cadf3e386dceb4064838be83105764"
      "60c3d4f40670b2d14b12ac23a86");

  //-------------------------

  this->accumulatorParams.accumulatorQRNCommitmentGroup.g.SetHex(
      "6324dc564e2b4afec9cabebb4d77daf9bac097fc2b72caffb0fd6db968ce1e4aff24137928fefecd636b8a987b5879e06b1ab52b1d69b2ca"
      "f5a0102328680fda67440691c7d36429560ead7490a21d90d92bd216e47a04ed810d3b4a79182dfbea9ff48d9631c6a5139c805d7a1ea5e3"
      "20b96b4fbc192ed8c957d5318fbc45b00bd911c58783b266ec71cad61a7b79236d2936024869d64dfb003173491932ba487dae5f6e7b0894"
      "45f0579297c221f7953c1143b4c9eae022f2c171401e6f49d89369ad0ef6ccd169f4b84a1e8cfe241bb2bacf362b18e967a379775b70f176"
      "f269a96c5e51f16b9c7e902336e293fcf45f1769d58d705075966ce97377f63a");

  this->accumulatorParams.accumulatorQRNCommitmentGroup.h.SetHex(
      "61a3be57d109c4bd2560ed3d52fb1b153612af0fecd25795ea0c64ef7fa730f081d2b2fd12d4d3f52b6d524fe47e07bed9397711491d4e01"
      "2b7d744ebd37b448726f38fd9252b16111a4454dbda10f0d4eb6b76b78cecfb37c1061d2cc7d75d73b5a59f508214caa29e0cc35f2196832"
      "4b39d477007905d345cbb262b2574550f146c7371b4ce09fb804eba9bfeae2c68d22a585e7264402accd7863c49e1073a0358a14f70e0066"
      "2591bbdc2d4dc6a4fae0390757f128ee320bd7cb51f50ed0f64721fe11b116f00234ec807f56a85ce1649a6026eb9179e97d00323cf6a210"
      "9e6a4b6d60b38a4f36744e2884442a62b48fd395dce9b6f7b5c564c6ef47d802");

  this->accumulatorParams.accumulatorBase = CBigNum(961);

  this->accumulatorParams.minCoinValue.SetHex(
      "8000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
      "00000000000000000");

  this->accumulatorParams.maxCoinValue = IntegerModModulus<COIN_COMMITMENT_MODULUS>::getModulus();

  this->accumulatorParams.initialized = true;
  this->initialized = true;
}

} /* namespace libzerocoin */

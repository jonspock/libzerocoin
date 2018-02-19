/**
* @file       Params.cpp
*
* @brief      Parameter class for Zerocoin.
*
* @author     Ian Miers, Christina Garman and Matthew Green
* @date       June 2013
*
* @copyright  Copyright 2013 Ian Miers, Christina Garman and Matthew Green
*  license    This project is released under the MIT license.
**/
// Copyright (c) 2018 The PIVX developers
#include "ZerocoinParams.h"
#include "AccumulatorAndProofParams.h"
#include "ParamGeneration.h"
#include "ModulusType.h"

namespace libzerocoin
{
ZerocoinParams::ZerocoinParams(CBigNum N, uint32_t securityLevel)
{
    this->zkp_hash_len = securityLevel;
    this->zkp_iterations = securityLevel;

    this->accumulatorParams.k_prime = ACCPROOF_KPRIME;
    this->accumulatorParams.k_dprime = ACCPROOF_KDPRIME;

    this->initialized = false;
    this->accumulatorParams.initialized = false;
    // Set the accumulator modulus to "N".
    this->accumulatorParams.accumulatorModulus = IntegerModModulus<ACCUMULATOR_MODULUS>::getModulus();

    //-------------------------
    
    this->coinCommitmentGroup.modulus = IntegerModModulus<COIN_COMMITMENT_MODULUS>::getModulus();
    this->coinCommitmentGroup.groupOrder.SetHex("cd4ca8386d4619718df572cc8ca31b009b044cb50edc79fe26e64324a0b9d2d3");

    this->coinCommitmentGroup.g.SetHex("166671ac5475e45b04ff52ab53020be557fe4c9ca9bf900c4a6d5a2f2c5455e4fb2d23e46cf0aa61b88a3a5567d196b1523155ec3e528a5af423ba4f1d76817aaa2c33de5facfe2b0ea366639bdf81cd2e288abf2160475f71b3315c1e37e9c7c7cf692823140443a2286317339a938299b74751995af2d6900bc282ab30bc08");
    this->coinCommitmentGroup.h.SetHex("77e0c7572b6f58057b7ccf67074720bdf8a4d4fc4450de932cb5f601e6cf99b6f417ad1e523904630ee65a35638d5b1c48784411dc142c3b70040b9ab37ab9dc5f81c4f14502b5aba7d8b79b227b84b7bdd8ffb68a89d72204a186df705502173ec13ae035abf6eff2a6878ba070ac5c733e694d6fc13f9eccdd7ae1fdf1ed7");

    //-------------------------
    
    this->serialNumberSoKCommitmentGroup.groupOrder =  IntegerModModulus<SERIAL_NUMBER_SOK_COMMITMENT_GROUP>::getModulus();
    this->serialNumberSoKCommitmentGroup.modulus = IntegerModModulus<SERIAL_NUMBER_SOK_COMMITMENT_MODULUS>::getModulus();

    this->serialNumberSoKCommitmentGroup.g.SetHex("cb944ee1c7fd61f05de75cfec4584bc0860af59875330d5841d36964cace014750eedd644d3db9fe7fe108a440bbed90b81f6f329af4e139d2c13ba7fbc4b57d638f9052409edc18835ab486fb3281e305d55394e1f68c501d8958f5db2ba76637f28b871793e3c12a4b970587c614a523de1ac890ff2c4c1cebd998c0e7a4e73");

    this->serialNumberSoKCommitmentGroup.h.SetHex("371817107be75218225abe8a00155bfd8762a18b3d8ae8e61f42503e75b18e8d8f681381319ffb60644bef2163df4036026524e290417bef415a7441173c519248e8db54c0cd6d8ea1d30a218a121babcda8705989d1e0a48198f096a12745ae3c5969c23d7b40fa8bfcf79acee4657a03574cbb04da1d9f7a2b88c2f48aeb00b6");

    // TBD - temporary HACK
    this->serialNumberSoKCommitmentGroup.g1.SetHex("371817107be75218225abe8a00155bfd8762a18b3d8ae8e61f42503e75b18e8d8f681381319ffb60644bef2163df4036026524e290417bef415a7441173c519248e8db54c0cd6d8ea1d30a218a121babcda8705989d1e0a48198f096a12745ae3c5969c23d7b40fa8bfcf79acee4657a03574cbb04da1d9f7a2b88c2f48aeb00b6");
    this->serialNumberSoKCommitmentGroup.g2.SetHex("371817107be75218225abe8a00155bfd8762a18b3d8ae8e61f42503e75b18e8d8f681381319ffb60644bef2163df4036026524e290417bef415a7441173c519248e8db54c0cd6d8ea1d30a218a121babcda8705989d1e0a48198f096a12745ae3c5969c23d7b40fa8bfcf79acee4657a03574cbb04da1d9f7a2b88c2f48aeb00b6");
    this->serialNumberSoKCommitmentGroup.g3.SetHex("371817107be75218225abe8a00155bfd8762a18b3d8ae8e61f42503e75b18e8d8f681381319ffb60644bef2163df4036026524e290417bef415a7441173c519248e8db54c0cd6d8ea1d30a218a121babcda8705989d1e0a48198f096a12745ae3c5969c23d7b40fa8bfcf79acee4657a03574cbb04da1d9f7a2b88c2f48aeb00b6");
    this->serialNumberSoKCommitmentGroup.g4.SetHex("371817107be75218225abe8a00155bfd8762a18b3d8ae8e61f42503e75b18e8d8f681381319ffb60644bef2163df4036026524e290417bef415a7441173c519248e8db54c0cd6d8ea1d30a218a121babcda8705989d1e0a48198f096a12745ae3c5969c23d7b40fa8bfcf79acee4657a03574cbb04da1d9f7a2b88c2f48aeb00b6");
    this->serialNumberSoKCommitmentGroup.g5.SetHex("371817107be75218225abe8a00155bfd8762a18b3d8ae8e61f42503e75b18e8d8f681381319ffb60644bef2163df4036026524e290417bef415a7441173c519248e8db54c0cd6d8ea1d30a218a121babcda8705989d1e0a48198f096a12745ae3c5969c23d7b40fa8bfcf79acee4657a03574cbb04da1d9f7a2b88c2f48aeb00b6");
    this->serialNumberSoKCommitmentGroup.g6.SetHex("371817107be75218225abe8a00155bfd8762a18b3d8ae8e61f42503e75b18e8d8f681381319ffb60644bef2163df4036026524e290417bef415a7441173c519248e8db54c0cd6d8ea1d30a218a121babcda8705989d1e0a48198f096a12745ae3c5969c23d7b40fa8bfcf79acee4657a03574cbb04da1d9f7a2b88c2f48aeb00b6");
    this->serialNumberSoKCommitmentGroup.g7.SetHex("371817107be75218225abe8a00155bfd8762a18b3d8ae8e61f42503e75b18e8d8f681381319ffb60644bef2163df4036026524e290417bef415a7441173c519248e8db54c0cd6d8ea1d30a218a121babcda8705989d1e0a48198f096a12745ae3c5969c23d7b40fa8bfcf79acee4657a03574cbb04da1d9f7a2b88c2f48aeb00b6");
    this->serialNumberSoKCommitmentGroup.g8.SetHex("371817107be75218225abe8a00155bfd8762a18b3d8ae8e61f42503e75b18e8d8f681381319ffb60644bef2163df4036026524e290417bef415a7441173c519248e8db54c0cd6d8ea1d30a218a121babcda8705989d1e0a48198f096a12745ae3c5969c23d7b40fa8bfcf79acee4657a03574cbb04da1d9f7a2b88c2f48aeb00b6");
    this->serialNumberSoKCommitmentGroup.g9.SetHex("371817107be75218225abe8a00155bfd8762a18b3d8ae8e61f42503e75b18e8d8f681381319ffb60644bef2163df4036026524e290417bef415a7441173c519248e8db54c0cd6d8ea1d30a218a121babcda8705989d1e0a48198f096a12745ae3c5969c23d7b40fa8bfcf79acee4657a03574cbb04da1d9f7a2b88c2f48aeb00b6");
    this->serialNumberSoKCommitmentGroup.ga.SetHex("371817107be75218225abe8a00155bfd8762a18b3d8ae8e61f42503e75b18e8d8f681381319ffb60644bef2163df4036026524e290417bef415a7441173c519248e8db54c0cd6d8ea1d30a218a121babcda8705989d1e0a48198f096a12745ae3c5969c23d7b40fa8bfcf79acee4657a03574cbb04da1d9f7a2b88c2f48aeb00b6");
    this->serialNumberSoKCommitmentGroup.gb.SetHex("371817107be75218225abe8a00155bfd8762a18b3d8ae8e61f42503e75b18e8d8f681381319ffb60644bef2163df4036026524e290417bef415a7441173c519248e8db54c0cd6d8ea1d30a218a121babcda8705989d1e0a48198f096a12745ae3c5969c23d7b40fa8bfcf79acee4657a03574cbb04da1d9f7a2b88c2f48aeb00b6");
    
    //-------------------------
    
    this->accumulatorParams.accumulatorPoKCommitmentGroup.groupOrder = IntegerModModulus<ACCUMULATOR_POK_COMMITMENT_GROUP>::getModulus();
    this->accumulatorParams.accumulatorPoKCommitmentGroup.modulus = IntegerModModulus<ACCUMULATOR_POK_COMMITMENT_MODULUS>::getModulus();

    this->accumulatorParams.accumulatorPoKCommitmentGroup.g.SetHex("7bc6a906dce53941560b042ebd59b8e9c954429747e44f949dbd2f0f42ca9c811437845a452ccdbedaada3c871c355096ba88a7e2ec403fb25d1f2e0454c8c91249ac0b2268");

    this->accumulatorParams.accumulatorPoKCommitmentGroup.h.SetHex("42f2e448bdb7fd45ad69571468c07ee70cd4759399b7aedaed0ad1f0d3799e3bead6c3cc9c6600c25d8ba709527e9de84b51362eae546b99a5c0dc6095e863cda07d3189a06");

    //-------------------------
    
    this->accumulatorParams.accumulatorQRNCommitmentGroup.g.SetHex("16c2edcc12d71c0657a7f8114ba021b1770e72aa3d7af7686b5a9a0461623445d7148117a4a197f273c489391882c569308b91d802b7c4e6aadda8ebdefb83dbff79593f49aa5f86386ce68b73db4b51ce274eb3ab8d8fee696754514cdc9ace987225c047ee432ac0a3d87a905c19e734e4bdc87a55c48f8e93cbdfcd69c10fe6f927ece8426a3ac70c36ab5fd2c66524564e07657d2d7572c6757f9f77654f6530643f1d611b323dcd083dafed6deca72fb91a703cd2ce8ec88a8a2a89c6530898f5f8eaae8c09a3bad9117f8b39df837ba3f03bb19675e9d9100a32fd9242f394957135dc65fe44888a348e6d71fb29cae39ba478ff0ea08c4e030e18b466be44f2f202de5cf042455ff79bc415ec9a9039e0bdf8f5f1ad54f7dbd9f316104104abf6b78cf13db7186e4447ca0481ffc79bdab");

    this->accumulatorParams.accumulatorQRNCommitmentGroup.h.SetHex("115967f5f81232cf81a590e8a6017eeea0fb2717d63728ccbb513211e769bb2825ea86fa7d280e4ebdf3697e62b0c842d94d1e83508db98bc0bc4fdd2581cae325702c08bce7ea54ba05229030df55e4b7aaea3c129ebb00ba543696fa46f7092438a1fb1a757bb3bd797c3df831ef7649b316e3423be24c3c2e94ab333507196ee781ee5c86ce3fda7b4f4c5a7481fe87cb0cee683630c0cc52197621a847d5997d703a8f6cdcbff199b16c6aa1dc346c3633a145909f1846012ef9c41a4fdd176f4d5e571b3d2697fd1487df2e91077d1615f383a1092d3d8e33a1594b11614c0542205d2931c0a27816345c4b27b535b185d9f2f86861a5c43215544d60501e0b53dcedf172bfdb05fd8272feb143fa92aca93eca6021735723fe621a1c53ac9384b7e48ca547fa30c48917bcc1022e81bff12");


    this->accumulatorParams.accumulatorBase = CBigNum(961);

    this->accumulatorParams.minCoinValue.SetHex("800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");

    this->accumulatorParams.maxCoinValue = IntegerModModulus<COIN_COMMITMENT_MODULUS>::getModulus();

    this->accumulatorParams.initialized = true;
    this->initialized = true;
}


} /* namespace libzerocoin */

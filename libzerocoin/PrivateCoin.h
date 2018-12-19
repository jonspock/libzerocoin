/**
 * @file       PrivateCoin.h
 *
 * @brief      PublicCoin and PrivateCoin classes for the Zerocoin library.
 *
 * @author     Ian Miers, Christina Garman and Matthew Green
 * @date       June 2013
 *
 * @copyright  Copyright 2013 Ian Miers, Christina Garman and Matthew Green
 *  license    This project is released under the MIT license.
 **/
// Copyright (c) 2018 The PIVX developer
// Copyright (c) 2018 Jon Spock
#pragma once
#include "Denominations.h"
#include "PublicCoin.h"
#include "ZerocoinParams.h"
#include "amount.h"
#include "bignum.h"
#include "key.h"
#include "util.h"

namespace libzerocoin {

bool GenerateKeyPair(const CBigNum& bnGroupOrder, const uint256& nPrivkey, CKey& key, CBigNum& bnSerial);

/**
 * A private coin. As the name implies, the content
 * of this should stay private except PublicCoin.
 *
 * Contains a coin's serial number, a commitment to it,
 * and opening randomness for the commitment.
 *
 * @warning Failure to keep this secret(or safe),
 * @warning will result in the theft of your coins and a TOTAL loss of anonymity.
 */
class PrivateCoin {
 public:
  PrivateCoin(const ZerocoinParams* p);
  
  //    PrivateCoin(const ZerocoinParams* p, const CoinDenomination denomination, bool fMintNew = true);
    PrivateCoin(const ZerocoinParams* p, const CoinDenomination denomination, const CBigNum& bnSerial, const CBigNum& bnRandomness);
  
    CBigNum CoinFromSeed(const uint512& seedZerocoin);
  
    const PublicCoin& getPublicCoin() const { return this->publicCoin; }
  // @return the coins serial number
  const CBigNum& getSerialNumber() const { return this->serialNumber; }
  const CBigNum& getRandomness() const { return this->randomness; }
  void getRandomnessBits(std::vector<int> &randomness_bits) const
    {
        randomness_bits.resize(ZKP_SERIALSIZE);
        std::string bin_string = this->randomness.ToString(2);
        unsigned int len = bin_string.length();
        for(unsigned int i=0; i<len; i++)
            randomness_bits[len-1-i] = (int)bin_string[i]-'0';
        for(unsigned int i=len; i<ZKP_SERIALSIZE; i++)
            randomness_bits[i] = 0;
    }
  
  const uint8_t& getVersion() const { return this->version; }
  const CSecretKey& getPrivKey() const { return this->privkey; }
  CPubKey getPubKey() const;

  void setPublicCoin(const PublicCoin& p) { publicCoin = p; }
  void setRandomness(const Bignum& n) { randomness = n; }
  void setSerialNumber(const Bignum& n) { serialNumber = n; }
  void setVersion(uint8_t nVersion) { this->version = nVersion; }
  void setPrivKey(const CSecretKey& privkey) { this->privkey = privkey; }
  void sign(const uint256& hash, std::vector<uint8_t>& vchSig) const;
  bool IsValid();

  ADD_SERIALIZE_METHODS
  template <typename Stream, typename Operation> inline void SerializationOp(Stream& s, Operation ser_action) {
      READWRITE(version);
      READWRITE(publicCoin);
      READWRITE(randomness);
      READWRITE(serialNumber);
      READWRITE(privkey);
  }

  static int const CURRENT_VERSION = 2;

 private:
  const ZerocoinParams* params;
  PublicCoin publicCoin;
  CBigNum randomness;
  CBigNum serialNumber;
  uint8_t version = 1;
  CSecretKey privkey;
};

} /* namespace libzerocoin */

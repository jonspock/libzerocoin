# Introduction

# This codebase has been migrated to 

* use libsodium and gmp
* stop using OpenSSL, Boost and secp256k1

For previous version based on OpenSSL/Boost/secp256k1, these were the timings on a particular machine

* Mint time Per coin: 		5.004 s
* Accumulator Per Element: 1.124 s
* Witness Per Element: 0.915 s

With Libsodium/gmp

* Mint time Per coin: 		0.154 s
* Accumulator Per Element: 0.039 s
* Witness Per Element: 0.038 s


### WARNING
Refactored Libzerocoin Code - Still not ready for PRODUCTION USE

libzerocoin is a C++ library that implements the core cryptographic routines of the Zerocoin protocol. Zerocoin is a distributed anonymous cash extension for Bitcoin-type (hash chain based) protocols. The protocol uses zero knowledge proofs to implement a fully decentralized coin laundry.

The Zerocoin protocol is provably secure and uses well-studied cryptographic primitives. For a complete description of the protocol, see the white paper published in the IEEE Security & Privacy Symposium (2013) and referenced below.

# Overview of the Library

libzerocoin implements the core cryptographic operations of Zerocoin. These include:

* Parameter generation
* Coin generation ("Minting")
* Coin spending (generation of a zero knowledge proof)
* Accumulator calculation
* Coin and spend proof verification

This library does _not_ implement the full Zerocoin protocol. In addition to the above cryptographic routines, a full Zerocoin implementation requires several specialized Zerocoin messages, double spending checks, and some additional coin redemption logic that must be supported by all clients in the network. libzerocoin does not provide routines to support these functions, although an overview is provided on the [Integrating with Bitcoin clients](https://github.com/Zerocoin/libzerocoin/wiki/Integrating-with-bitcoin-clients) page.

# Outside links

* [Zerocoin Project website](http://zerocoin.org/)
* [Zerocoin Paper](http://zerocoin.org/media/pdf/ZerocoinOakland.pdf)

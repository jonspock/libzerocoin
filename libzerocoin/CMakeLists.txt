# Copyright 2013 Corgan Labs
# Copyright 2017 PIVX Developers
# This file is part of the Zerocoin project
# See LICENSE file or http://opensource.org/licenses/MIT for terms

cmake_minimum_required(VERSION 2.8)
project(zerocoin)

# Just add Debug info for Debug builds
if (CMAKE_BUILD_TYPE MATCHES "DEBUG")
  add_definitions(-DZEROCOIN_DEBUG)
endif()

add_subdirectory(matrix)

#
# List of source files to compile
#
list(APPEND zerocoin_sources
  Accumulator.cpp
  AccumulatorWitness.cpp
  PrivateCoin.cpp
  Denominations.cpp
  CoinSpend.cpp
  IntegerMod.cpp
  CommitmentProofOfKnowledge.cpp
  ZerocoinParams.cpp
  SerialNumberSignatureOfKnowledge.cpp
  AccumulatorProofOfKnowledge.cpp
  IntegerMod.cpp
)

add_library(zerocoin ${ZEROCOIN_HEADERS} ${zerocoin_sources})


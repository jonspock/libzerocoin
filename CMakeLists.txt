CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "RELEASE")
ENDIF()

if(CMAKE_CXX_FLAGS_RELEASE MATCHES DNDEBUG)
  string(REPLACE "-DNDEBUG" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
  string(REPLACE "-DNDEBUG" "" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
endif()

if(CMAKE_CXX_FLAGS_RELWITHDEBINFO MATCHES DNDEBUG)
  string(REPLACE "-DNDEBUG" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE}")
  string(REPLACE "-DNDEBUG" "" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELEASE}")
endif()

project(ZKP)



# Add path for custom modules
set(CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH}
	${CMAKE_CURRENT_SOURCE_DIR}/modules
)

enable_testing()

#---------------------------------------------------------------------------------------------------------
# Code coverage
#---------------------------------------------------------------------------------------------------------
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
    # Setup Code Coverage for XCode when requested
    if ($ENV{COVERAGE})
        message(STATUS "********* Setting up Xcode compile for Code coverage **********")
        set(CMAKE_XCODE_ATTRIBUTE_GCC_INSTRUMENT_PROGRAM_FLOW_ARCS "YES")
        set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_TEST_COVERAGE_FILES "YES")
        # Works for Clang->
				#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -coverage")
    endif()
endif()

IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
else()
	add_definitions("-Wno-cpp -Wno-unused-function")
	set(ANL anl)
endif()

add_definitions("-Wall -Wextra -Wformat -Wformat-security -Wno-unused-parameter -fPIC -fvisibility=hidden -DHAVE_CONFIG_H")

SET(ARITH
	./external/arith_uint256.cpp
	./external/uint256.cpp
	./external/sha256.cpp
	./external/chacha20.cpp

	./external/sha512.cpp
	./external/pubkey.cpp
	./external/key.cpp
	./external/hash.cpp
  ./support/cleanse.cpp
  ./support/lockedpool.cpp

  ./external/ripemd160.cpp
  ./external/hmac_sha256.cpp
  ./external/hmac_sha512.cpp
  ./external/random.cpp
  ./external/utilstrencodings.cpp

	./secp256k1/src/secp256k1.c
	./secp256k1/src/gen_context.c

  )

IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(OPENSSL_VER "1.0.2o_2")
  set(OPENSSL_ROOT_DIR "/usr/local/Cellar/openssl/${OPENSSL_VER}/")
endif()

find_package(OpenSSL)
if (OPENSSL_FOUND)
  message(STATUS "Found OpenSSL")
endif()


find_package(gmp)
if (GMP_FOUND)
  message(STATUS "Found libgmp")
endif()


find_package( Boost COMPONENTS system filesystem )
link_directories ( ${Boost_LIBRARY_DIRS} )

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/external
  ${CMAKE_CURRENT_SOURCE_DIR}/secp256k1
  ${CMAKE_CURRENT_SOURCE_DIR}/secp256k1/include
  ${GMP_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${OPENSSL_INCLUDE_DIR}
  )

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
source_group("SrcHeaders" FILES ${HEADERS})

file(GLOB CRYPTO_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/crypto/*.h)
source_group("CryptoHeaders" FILES ${CRYPTO_HEADERS})

add_library(coin ${HEADERS} ${CRYPTO_HEADERS} ${ARITH})
set_property(TARGET coin PROPERTY CXX_STANDARD 11)

add_subdirectory(libzerocoin)

#---------------------------------------------------------------------------------------------------------
# Create a target in Xcode to setup coverage script
#---------------------------------------------------------------------------------------------------------
if ($ENV{COVERAGE})
    add_custom_target(xcov COMMAND ./XcodeCoverage/exportenv.sh )
endif()

add_subdirectory(test)

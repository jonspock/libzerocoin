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

include(CheckSymbolExists)
include(CheckCXXCompilerFlag)

check_cxx_compiler_flag(-std=c++17 HAVE_FLAG_STD_CXX17)
if (HAVE_FLAG_STD_CXX17)
  set(CMAKE_CXX_STANDARD 17)
  message(STATUS "Compiler supports C++17")
  add_definitions("-D_LIBCPP_ENABLE_CXX17_REMOVED_AUTO_PTR") # still needed?
else()
  message(ERROR "Compiler must at least support C++17")
endif()

SET(ARITH
	./external/arith_uint256.cpp
	./external/uint256.cpp
	./external/sha256.cpp
	./external/sha512.cpp

	./external/key.cpp
  ./support/cleanse.cpp
  ./support/lockedpool.cpp

  ./external/random.cpp
  ./external/rand_bignum.cpp
  ./external/utilstrencodings.cpp

  )

find_package(gmp)
if (GMP_FOUND)
  message(STATUS "Found libgmp")
endif()

find_package(sodium REQUIRED)
if (SODIUM_FOUND)
  message(STATUS "Found libsodium")
  message(STATUS "Sodium include dir = ${sodium_INCLUDE_DIR}")
endif()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/external
  ${GMP_INCLUDE_DIR}
  ${sodium_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/external/*.h)
source_group("ExtHeaders" FILES ${HEADERS})

file(GLOB LIB_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/libzerocoin/*.h)
source_group("LibHeaders" FILES ${LIB_HEADERS})

add_library(coin ${HEADERS} ${CRYPTO_HEADERS} ${ARITH})
add_subdirectory(libzerocoin)

#---------------------------------------------------------------------------------------------------------
# Create a target in Xcode to setup coverage script
#---------------------------------------------------------------------------------------------------------
if ($ENV{COVERAGE})
    add_custom_target(xcov COMMAND ./XcodeCoverage/exportenv.sh )
endif()

add_subdirectory(test)

# Copyright 2017 PIVX Developers

cmake_minimum_required(VERSION 2.8)
project(matrix)

include_directories(
  ${Boost_INCLUDE_DIRS}
  ${OPENSSL_INCLUDE_DIR}
  ..
)

# List of libraries to link in
#
list(APPEND matrix_libs
  ${Boost_LIBRARIES}
  ${OPENSSL_LIBRARIES}
)

FILE(GLOB MATRIX_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

#
# List of source files to compile
#
list(APPEND matrix_sources
  matrix_insts.cpp
)

add_library(matrix ${MATRIX_HEADERS} ${matrix_sources})
target_link_libraries(matrix ${matrix_libs})
set_property(TARGET matrix PROPERTY CXX_STANDARD 11)


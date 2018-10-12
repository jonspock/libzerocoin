CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project(tests)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/..")

include_directories(
	${include_directories} 
	${CMAKE_CURRENT_SOURCE_DIR}/../libzerocoin
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_executable(benchmark benchmark_zerocoin.cpp)
target_link_libraries(benchmark zerocoin coin sodium ${GMP_LIBRARIES} pthread)

add_test(NAME benchmark COMMAND benchmark)

#add_executable(test_p test_proveSOK.cpp)
#target_link_libraries(test_p zerocoin coin gmp ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} pthread)
#set_property(TARGET test_p PROPERTY CXX_STANDARD 11)



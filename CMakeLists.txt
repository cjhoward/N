
# Prevent in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "In-source builds prohibited. Call CMake from a build directory.")
endif()

cmake_minimum_required(VERSION 3.16.0)
project(N)
add_executable(n src/nterpreter.c src/sequence.c src/preprocess.c src/interpret.c)
add_executable(bin2n src/bin2n.c)
add_executable(n2c src/n2c.c src/preprocess.c)
add_executable(hello build/hello.n.c)

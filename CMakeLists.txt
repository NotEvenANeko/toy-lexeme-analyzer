cmake_minimum_required(VERSION 3.11)

project(Lexeme_Analyzer)

aux_source_directory(. DIR_TOOT_SRCS)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

add_executable(Lexeme_Analyzer ${DIR_TOOT_SRCS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
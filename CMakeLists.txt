cmake_minimum_required(VERSION 3.0)

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.101.tar.gz"
    SHA1 "ccb234f5ea9dc89989b28164cb38cb4ebb04237c"
)

# Project
project(protobuf C CXX)

hunter_add_package(ZLIB)

add_subdirectory(cmake)

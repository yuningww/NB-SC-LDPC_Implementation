cmake_minimum_required(VERSION 3.10)
project(ldpc CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

file(GLOB LDPC_SOURCES "${CMAKE_SOURCE_DIR}/LDPC/*.cpp")

add_executable(ldpc ${LDPC_SOURCES})
target_include_directories(ldpc PRIVATE "${CMAKE_SOURCE_DIR}/Include")

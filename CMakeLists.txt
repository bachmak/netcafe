cmake_minimum_required(VERSION 2.8)
project(netcafe)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
else()
    set(CXX_STANDARD_REQUIRED 17)
endif()

add_executable(main main.cpp)

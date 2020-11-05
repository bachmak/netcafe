cmake_minimum_required(VERSION 3.1)
project(netcafe)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
else()
    set(CXX_STANDARD_REQUIRED 17)
endif()

include_directories("sandbox")
file(GLOB SOURCES "sandbox/*.cpp")
add_executable(main main.cpp ${SOURCES})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
set(LIBRARIES Threads::Threads)

find_package(Boost 1.74 COMPONENTS system REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    set(LIBRARIES ${LIBRARIES} ${Boost_LIBRARIES})
    target_link_libraries(main ${LIBRARIES})
endif()
cmake_minimum_required(VERSION 3.16)

project(chunk)

include(CTest)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

set(CMAKE_EXE_LINKER_FLAGS "-static-pie -Wl,--whole-archive -lpthread -Wl,--no-whole-archive") 

set(CMAKE_CXX_FLAGS_DEBUG "-g")

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

find_package(Threads REQUIRED)

FetchContent_Declare(
        ctre
        GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
        GIT_TAG 331aebc79d715d1d0036d2d32021584eefecbcc9)

FetchContent_MakeAvailable(ctre)

add_executable(chunk
        ${CMAKE_CURRENT_SOURCE_DIR}/src/http.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/downloader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/socket.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_link_libraries(chunk
        PRIVATE 
        ctre)

add_subdirectory(tests)
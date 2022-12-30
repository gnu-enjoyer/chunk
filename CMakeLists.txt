cmake_minimum_required(VERSION 3.16)

project(chunk)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXE_LINKER_FLAGS "-static-pie")

find_package(Threads REQUIRED)

include(FetchContent)

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
        ctre
        ${CMAKE_THREAD_LIBS_INIT})

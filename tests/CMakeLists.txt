cmake_minimum_required(VERSION 3.16)

project(chunk_tests)

add_executable(chunk_tests
        ${CMAKE_SOURCE_DIR}/src/http.hpp
        ${CMAKE_SOURCE_DIR}/src/downloader.cpp
        ${CMAKE_SOURCE_DIR}/src/socket.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/smoke.cpp)

target_link_libraries(chunk_tests
        PRIVATE
        ctre)

target_include_directories(chunk_tests
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src)

add_test(NAME chunk_tests
        COMMAND chunk_tests)
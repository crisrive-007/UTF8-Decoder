cmake_minimum_required(VERSION 3.20)
project(UTF8-Decoder)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(utf8-decoder main.cpp decoder.cpp)
cmake_minimum_required(VERSION 3.20)
project(buckets)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -Wpedantic")

add_executable(buckets bucketing.cpp)

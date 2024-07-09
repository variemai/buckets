cmake_minimum_required(VERSION 3.20)
project(buckets)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -Wpedantic")

add_executable(buckets bucketing.cpp
        bench.h
        bench.cpp)
add_executable(constexpr_bench constexpr_bench.cpp
        bench.h
        bench.cpp)
add_executable(template_bench template_bench.cpp
        bench.h
        bench.cpp)

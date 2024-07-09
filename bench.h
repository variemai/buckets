#include <iostream>
#include <cstdint>
#include <array>
#include <vector>
#include <chrono>
#include <random>

#ifndef BUCKETS_BENCH_H
#define BUCKETS_BENCH_H

constexpr size_t NUM_BUCKETS = 9;
constexpr std::array<int64_t, NUM_BUCKETS - 1> thresholds = {64, 512, 4096, 32768, 131072, 1048576, 8388608, 33554432};
constexpr int num_values = 400000000;

std::vector<int64_t> init_bench();

void verify_bench(std::vector<int> reference, std::vector<int> indices);

#endif //BUCKETS_BENCH_H

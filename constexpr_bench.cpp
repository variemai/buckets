#include "bench.h"

constexpr int64_t get_bucket_constexpr(const int64_t value, size_t index = NUM_BUCKETS - 1) {

    //return ~((value - thresholds[index]) >> 63) + get_bucket_constexpr(value, index - 1);
    return index == 0
    ? ~((value - thresholds[0]) >> 63) : ~((value - thresholds[index - 1]) >> 63) + get_bucket_constexpr(value, index - 1);
}

void benchmark() {
    std::vector<int64_t> values;
    volatile int bucket;

    values = init_bench();

    // Measure the time taken to get the bucket index for each value
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& value : values) {
        bucket = get_bucket_constexpr(value);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> indexing_time = end - start;
    std::cout << "Time to compute bucket index with get_bucket_constexpr for " << num_values << " values: " << indexing_time.count() << " seconds\n";

}

int main() {

    benchmark();

    return 0;
}
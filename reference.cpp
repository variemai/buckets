#include "bench.h"

int
choose_bucket(const int64_t bytes) {
    int index;
    for (index = 0; index < NUM_BUCKETS-1; index++) {
        if ( thresholds[index] > bytes) {
            return index;
        }
    }
    return NUM_BUCKETS-1;
}

void benchmark() {
    std::vector<int64_t> values;
    volatile int bucket;

    values = init_bench();

    // Measure the time taken to get the bucket index for each value
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& value : values) {
        bucket = choose_bucket(value);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> indexing_time = end - start;
    std::cout << "Time to compute bucket index with reference implementation for " << num_values << " values: " << indexing_time.count() << " seconds\n";

}

int main() {

    benchmark();

    return 0;
}
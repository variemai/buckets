#include "bench.h"

int get_bin_index(const int64_t value) {
    int index;
    // Generate the masks without branches
    index = (value - thresholds[0]) >> 63 & 1;
    index += (value - thresholds[1]) >> 63 & 1;
    index += (value - thresholds[2]) >> 63 & 1;
    index += (value - thresholds[3]) >> 63 & 1;
    index += (value - thresholds[4]) >> 63 & 1;
    index += (value - thresholds[5]) >> 63 & 1;
    index += (value - thresholds[6]) >> 63 & 1;
    index += (value - thresholds[7]) >> 63 & 1;

    return index;
}

void benchmark() {
    std::vector<int64_t> values;
    volatile int bucket;

    values = init_bench();

    // Measure the time taken to get the bucket index for each value
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& value : values) {
        bucket = get_bin_index(value);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> indexing_time = end - start;
    std::cout << "Time to compute bucket index with the ideal for " << num_values << " values: " << indexing_time.count() << " seconds\n";

}

int main() {

    benchmark();

    return 0;
}
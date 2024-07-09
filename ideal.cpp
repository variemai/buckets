#include "bench.h"

int64_t get_bin_index(const int64_t value) {
    int64_t index;
    // Generate the masks without branches
    uint64_t mask0 = ~((value - thresholds[0]) >> 63);
    uint64_t mask1 = ~((value - thresholds[1]) >> 63);
    uint64_t mask2 = ~((value - thresholds[2]) >> 63);
    uint64_t mask3 = ~((value - thresholds[3]) >> 63);
    uint64_t mask4 = ~((value - thresholds[4]) >> 63);
    uint64_t mask5 = ~((value - thresholds[5]) >> 63);
    uint64_t mask6 = ~((value - thresholds[6]) >> 63);
    uint64_t mask7 = ~((value - thresholds[7]) >> 63);

    // Calculate the bucket index without branches
    index = (mask0) + (mask1) + (mask2) + (mask3) + (mask4) + (mask5) + (mask6) +
            (mask7);

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
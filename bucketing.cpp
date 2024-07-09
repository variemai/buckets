#include "bench.h"


int get_bin_index(int64_t value) {
    int index;
    // Generate the masks without branches
    uint64_t mask0 = ~((value - thresholds[0]) >> 63);
    uint64_t mask1 = ~((value - thresholds[1]) >> 63);
    uint64_t mask2 = ~((value - thresholds[2]) >> 63);
    uint64_t mask3 = ~((value - thresholds[3]) >> 63);
    uint64_t mask4 = ~((value - thresholds[4]) >> 63);
    uint64_t mask5 = ~((value - thresholds[5]) >> 63);
    uint64_t mask6 = ~((value - thresholds[6]) >> 63);
    uint64_t mask7 = ~((value - thresholds[7]) >> 63);
    uint64_t mask8 = ~((value - thresholds[8]) >> 63);

    // Calculate the bucket index without branches
    index = (mask0) + (mask1) + (mask2) + (mask3) + (mask4) + (mask5) + (mask6) +
            (mask7) + (mask8);

    return index;
}



int
choose_bucket(int64_t bytes) {
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
    std::chrono::duration<double> indexing_time;

    values = init_bench();

    // Measure the time taken to get the bucket index for each value
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& value : values) {
        bucket = get_bin_index(value);
        //indices1.push_back(bucket);
    }
    auto end = std::chrono::high_resolution_clock::now();
    indexing_time = end - start;
    std::cout << "Time to compute bucket index with get_bin_index for " << num_values << " values: " << indexing_time.count() << " seconds\n";


    // Measure the time taken to get the bucket index for each value
    start = std::chrono::high_resolution_clock::now();
    for (const auto& value : values) {
        bucket = choose_bucket(value); // Use 'volatile' to prevent compiler optimization
        //indices0.push_back(bucket);
    }
    end = std::chrono::high_resolution_clock::now();
    indexing_time = end - start;
    std::cout << "Time to compute bucket index with choose_bucket for " << num_values << " values: " << indexing_time.count() << " seconds\n";


}


int main() {

    benchmark();

    return 0;
}

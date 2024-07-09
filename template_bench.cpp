#include "bench.h"

// Utility to generate masks and index calculation at compile time
template <size_t N>
struct BucketIndexHelper {
    static int get_bucket_index(const int64_t value) {
        int64_t mask = ~((value - thresholds[N-1]) >> 63);
        return (mask & 1) + BucketIndexHelper<N-1>::get_bucket_index(value);
    }
};

template <>
struct BucketIndexHelper<1> {
    static int get_bucket_index(const int64_t value) {
        int64_t mask = ~((value - thresholds[0]) >> 63);
        return (mask & 1);
    }
};

template <>
struct BucketIndexHelper<0> {
    static int get_bucket_index(int64_t) {
        return 0;
    }
};

template <size_t N>
int get_bucket_index(const int64_t value) {
    //static_assert(N <= thresholds.size(), "N must not exceed the size of thresholds");
    return BucketIndexHelper<N>::get_bucket_index(value);
}


void benchmark() {
    std::vector<int64_t> values;
    volatile int bucket;

    values = init_bench();

    // Measure the time taken to get the bucket index for each value
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto &value: values) {
        bucket = get_bucket_index<thresholds.size()>(value);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> indexing_time = end - start;
    std::cout << "Time to compute bucket index with get_bucket_index for " << num_values << " values: "
              << indexing_time.count() << " seconds\n";
}

int main() {

    benchmark();

    return 0;
}
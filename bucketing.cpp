#include <iostream>
#include <cstdint>
#include <array>
#include <vector>
#include <chrono>
#include <random>

constexpr size_t NUM_BUCKETS = 10;
constexpr std::array<int64_t, NUM_BUCKETS - 1> thresholds = {8, 64, 512, 4096, 32768, 131072, 1048576, 8388608, 33554432};

constexpr int get_bucket_constexpr(const int64_t value, size_t index = NUM_BUCKETS - 1) {
    if (index == 0) {
        return ~((value - thresholds[0]) >> 63);  // Handle the first bucket explicitly
    } else {
        return ~((value - thresholds[index - 1]) >> 63) + get_bucket_constexpr(value, index - 1);
    }
}

// Utility to generate masks and index calculation at compile time
template <size_t N>
struct BucketIndexHelper {
    static int get_bucket_index(int64_t value) {
        int64_t mask = ~((value - thresholds[N-1]) >> 63);
        return mask + BucketIndexHelper<N-1>::get_bucket_index(value);
    }
};

template <>
struct BucketIndexHelper<1> {
    static int get_bucket_index(int64_t value) {
        int64_t mask = ~((value - thresholds[0]) >> 63);
        return mask;
    }
};

template <>
struct BucketIndexHelper<0> {
    static int get_bucket_index(int64_t) {
        return 0;
    }
};

template <size_t N>
int get_bucket_index(int64_t value) {
    //static_assert(N <= thresholds.size(), "N must not exceed the size of thresholds");
    return BucketIndexHelper<N>::get_bucket_index(value);
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

void benchmark() {
    constexpr size_t num_values = 200000000;
    std::vector<int64_t> values(num_values);
    //std::vector<int> indices0(num_values);
    //std::vector<int> indices1(num_values);
    volatile int bucket;

    // Seed for random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int64_t> dist(1, 100000000);
    std::chrono::duration<double> indexing_time;

    // Generate random values and measure the time taken
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < num_values; ++i) {
        values[i] = dist(gen);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> generation_time = end - start;
    std::cout << "Time to generate " << num_values << " values: " << generation_time.count() << " seconds\n";

    // Measure the time taken to get the bucket index for each value
    //start = std::chrono::high_resolution_clock::now();
    //for (const auto& value : values) {
    //    bucket = get_bin_index(value);
    //    //indices1.push_back(bucket);
    //}
    //end = std::chrono::high_resolution_clock::now();
    //indexing_time = end - start;
    //std::cout << "Time to compute bucket index with get_bin_index for " << num_values << " values: " << indexing_time.count() << " seconds\n";


    //// Measure the time taken to get the bucket index for each value
    //start = std::chrono::high_resolution_clock::now();
    //for (const auto& value : values) {
    //    bucket = choose_bucket(value); // Use 'volatile' to prevent compiler optimization
    //    //indices0.push_back(bucket);
    //}
    //end = std::chrono::high_resolution_clock::now();
    //indexing_time = end - start;
    //std::cout << "Time to compute bucket index with choose_bucket for " << num_values << " values: " << indexing_time.count() << " seconds\n";


    // Measure the time taken to get the bucket index for each value
    start = std::chrono::high_resolution_clock::now();
    for (const auto& value : values) {
        bucket = get_bucket_index<NUM_BUCKETS-1>(value);
        //indices1.push_back(bucket);
    }
    end = std::chrono::high_resolution_clock::now();
    indexing_time = end - start;
    std::cout << "Time to compute bucket index with get_bucket_index for " << num_values << " values: " << indexing_time.count() << " seconds\n";

    // Measure the time taken to get the bucket index for each value
    start = std::chrono::high_resolution_clock::now();
    for (const auto& value : values) {
        bucket = get_bucket_constexpr(value);
        //indices1.push_back(bucket);
    }
    end = std::chrono::high_resolution_clock::now();
    indexing_time = end - start;
    std::cout << "Time to compute bucket index with get_bucket_constexpr for " << num_values << " values: " << indexing_time.count() << " seconds\n";

     //Verify that the two methods produce the same results
     //for (size_t i = 0; i < num_values; ++i) {
     //    if (indices0[i] != indices1[i]) {
     //        std::cerr << "Mismatch at index " << i << ": " << indices0[i] << " != " << indices1[i] << std::endl;
     //        break;
     //    }
     //}
}


int main() {

    benchmark();

    return 0;
}

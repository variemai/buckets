#include "bench.h"
std::vector<int64_t> init_bench()
{
    std::vector<int64_t> values(num_values);
    // Seed for random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int64_t> dist(1, num_values);

    // Generate random values and measure the time taken
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < num_values; ++i) {
        values[i] = dist(gen);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> generation_time = end - start;
    std::cout << "Time to generate " << num_values << " values: " << generation_time.count() << " seconds\n";
    return values;
}


void verify_bench(std::vector<int> reference, std::vector<int> indices)
{
    // Verify that the two methods produce the same results
    for (size_t i = 0; i < num_values; ++i) {
        if (reference[i] != indices[i]) {
            std::cerr << "Mismatch at index " << i << ": " << reference[i] << " != " << indices[i] << std::endl;
            break;
        }
    }
}
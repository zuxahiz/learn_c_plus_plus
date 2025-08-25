// test_program.cpp - 综合性能测试程序
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <thread>
#include <mutex>
#include <cmath>
#include <numeric>

class PerformanceTestProgram {
private:
    std::vector<double> data;
    std::mt19937 rng;
    std::mutex data_mutex;
    
public:
    PerformanceTestProgram(size_t size = 1000000) : rng(42) {
        data.resize(size);
        std::generate(data.begin(), data.end(), 
                     [&]() { return static_cast<double>(rng()) / rng.max(); });
    }
    
    // CPU密集型测试
    void cpuIntensiveTest() {
        std::cout << "CPU密集型测试..." << std::endl;
        
        double result = 0.0;
        for (size_t i = 0; i < data.size(); ++i) {
            result += std::sin(data[i]) * std::cos(data[i]);
            result += std::sqrt(std::abs(data[i]));
            result += std::log(data[i] + 1.0);
        }
        
        std::cout << "CPU测试结果: " << result << std::endl;
    }
    
    // 内存密集型测试
    void memoryIntensiveTest() {
        std::cout << "内存密集型测试..." << std::endl;
        
        // 顺序访问
        double sum1 = 0.0;
        for (size_t i = 0; i < data.size(); ++i) {
            sum1 += data[i];
        }
        
        // 随机访问
        std::uniform_int_distribution<size_t> dist(0, data.size() - 1);
        double sum2 = 0.0;
        for (int i = 0; i < 100000; ++i) {
            size_t index = dist(rng);
            sum2 += data[index];
        }
        
        std::cout << "内存测试结果: sum1=" << sum1 << ", sum2=" << sum2 << std::endl;
    }
    
    // 多线程测试
    void multithreadTest() {
        std::cout << "多线程测试..." << std::endl;
        
        const int num_threads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;
        std::vector<double> results(num_threads);
        
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back([this, i, &results]() {
                double local_sum = 0.0;
                size_t start = i * data.size() / std::thread::hardware_concurrency();
                size_t end = (i + 1) * data.size() / std::thread::hardware_concurrency();
                
                for (size_t j = start; j < end; ++j) {
                    local_sum += std::sin(data[j]);
                }
                
                results[i] = local_sum;
            });
        }
        
        for (auto& t : threads) {
            t.join();
        }
        
        double total = std::accumulate(results.begin(), results.end(), 0.0);
        std::cout << "多线程测试结果: " << total << std::endl;
    }
    
    // 算法复杂度测试
    void algorithmComplexityTest() {
        std::cout << "算法复杂度测试..." << std::endl;
        
        std::vector<int> test_data(data.size());
        std::iota(test_data.begin(), test_data.end(), 1);
        
        auto start = std::chrono::high_resolution_clock::now();
        
        // O(n log n) 排序
        std::sort(test_data.begin(), test_data.end());
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "排序耗时: " << duration.count() << "ms" << std::endl;
        
        // O(n) 查找
        start = std::chrono::high_resolution_clock::now();
        
        auto it = std::find(test_data.begin(), test_data.end(), data.size() / 2);
        
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "查找耗时: " << duration.count() << "μs" << std::endl;
    }
};

int main(int argc, char* argv[]) {
    std::cout << "=== 性能测试程序开始 ===" << std::endl;
    
    // 根据命令行参数确定数据规模
    size_t data_size = 1000000;  // 默认100万
    if (argc > 1) {
        data_size = std::stoul(argv[1]);
    }
    
    std::string test_type = "all";
    if (argc > 2) {
        test_type = argv[2];
    }
    
    std::cout << "数据规模: " << data_size << std::endl;
    std::cout << "测试类型: " << test_type << std::endl;
    
    PerformanceTestProgram test(data_size);
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    if (test_type == "cpu" || test_type == "all") {
        test.cpuIntensiveTest();
    }
    
    if (test_type == "memory" || test_type == "all") {
        test.memoryIntensiveTest();
    }
    
    if (test_type == "thread" || test_type == "all") {
        test.multithreadTest();
    }
    
    if (test_type == "algorithm" || test_type == "all") {
        test.algorithmComplexityTest();
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    std::cout << "=== 测试完成，总耗时: " << total_duration.count() << "ms ===" << std::endl;
    
    return 0;
}

// cpu_intensive.cpp - CPU密集型测试程序
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>

class CPUIntensiveTest {
public:
    // 素数计算 - CPU密集型
    static std::vector<int> sieveOfEratosthenes(int limit) {
        std::vector<bool> is_prime(limit + 1, true);
        std::vector<int> primes;
        
        is_prime[0] = is_prime[1] = false;
        
        for (int i = 2; i * i <= limit; ++i) {
            if (is_prime[i]) {
                for (int j = i * i; j <= limit; j += i) {
                    is_prime[j] = false;
                }
            }
        }
        
        for (int i = 2; i <= limit; ++i) {
            if (is_prime[i]) {
                primes.push_back(i);
            }
        }
        
        return primes;
    }
    
    // 矩阵乘法 - CPU和内存密集型
    static std::vector<std::vector<double>> matrixMultiply(
        const std::vector<std::vector<double>>& a,
        const std::vector<std::vector<double>>& b) {
        
        size_t n = a.size();
        size_t m = b[0].size();
        size_t p = a[0].size();
        
        std::vector<std::vector<double>> result(n, std::vector<double>(m, 0.0));
        
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                for (size_t k = 0; k < p; ++k) {
                    result[i][j] += a[i][k] * b[k][j];
                }
            }
        }
        
        return result;
    }
    
    // 蒙特卡洛方法计算π
    static double calculatePiMonteCarlo(long long iterations) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(-1.0, 1.0);
        
        long long inside_circle = 0;
        
        for (long long i = 0; i < iterations; ++i) {
            double x = dis(gen);
            double y = dis(gen);
            
            if (x * x + y * y <= 1.0) {
                inside_circle++;
            }
        }
        
        return 4.0 * inside_circle / iterations;
    }
    
    // 递归斐波那契数列 - CPU密集且低效
    static long long fibonacciRecursive(int n) {
        if (n <= 1) return n;
        return fibonacciRecursive(n - 1) + fibonacciRecursive(n - 2);
    }
    
    // 动态规划斐波那契数列 - 高效版本
    static long long fibonacciDP(int n) {
        if (n <= 1) return n;
        
        std::vector<long long> dp(n + 1);
        dp[0] = 0;
        dp[1] = 1;
        
        for (int i = 2; i <= n; ++i) {
            dp[i] = dp[i-1] + dp[i-2];
        }
        
        return dp[n];
    }
    
    // 数值积分 - CPU密集型数学计算
    static double numericalIntegration(double (*func)(double), double a, double b, int n) {
        double h = (b - a) / n;
        double sum = 0.0;
        
        // 梯形法则
        for (int i = 0; i <= n; ++i) {
            double x = a + i * h;
            double weight = (i == 0 || i == n) ? 0.5 : 1.0;
            sum += weight * func(x);
        }
        
        return h * sum;
    }
    
    // 测试函数：f(x) = sin(x) * exp(-x/2)
    static double testFunction(double x) {
        return std::sin(x) * std::exp(-x / 2.0);
    }
    
    // 排序算法性能测试
    static void sortingBenchmark(size_t size) {
        std::cout << "排序算法性能测试 (数据规模: " << size << ")" << std::endl;
        
        // 生成随机数据
        std::vector<int> data(size);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::generate(data.begin(), data.end(), gen);
        
        // 测试std::sort
        auto data_copy = data;
        auto start = std::chrono::high_resolution_clock::now();
        std::sort(data_copy.begin(), data_copy.end());
        auto end = std::chrono::high_resolution_clock::now();
        auto sort_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "  std::sort: " << sort_time.count() << "ms" << std::endl;
        
        // 测试std::stable_sort
        data_copy = data;
        start = std::chrono::high_resolution_clock::now();
        std::stable_sort(data_copy.begin(), data_copy.end());
        end = std::chrono::high_resolution_clock::now();
        auto stable_sort_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "  std::stable_sort: " << stable_sort_time.count() << "ms" << std::endl;
        
        // 测试std::partial_sort (前10%元素)
        data_copy = data;
        start = std::chrono::high_resolution_clock::now();
        std::partial_sort(data_copy.begin(), data_copy.begin() + size/10, data_copy.end());
        end = std::chrono::high_resolution_clock::now();
        auto partial_sort_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "  std::partial_sort (10%): " << partial_sort_time.count() << "ms" << std::endl;
    }
};

// 基准测试辅助函数
template<typename Func>
void benchmark(const std::string& name, Func func) {
    std::cout << "执行 " << name << "..." << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    auto result = func();
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << name << " 完成，耗时: " << duration.count() << "ms" << std::endl;
    
    // 如果有返回值，尝试输出
    if constexpr (!std::is_void_v<decltype(result)>) {
        std::cout << "结果: " << result << std::endl;
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "=== CPU密集型测试程序 ===" << std::endl;
    
    // 可以通过命令行参数控制测试强度
    int intensity = 1;  // 默认强度
    if (argc > 1) {
        intensity = std::atoi(argv[1]);
        intensity = std::max(1, std::min(3, intensity));  // 限制在1-3之间
    }
    
    std::cout << "测试强度: " << intensity << " (1=轻度, 2=中度, 3=重度)" << std::endl;
    std::cout << std::endl;
    
    // 根据强度调整测试参数
    int prime_limit = 10000 * intensity;
    int matrix_size = 100 * intensity;
    long long pi_iterations = 1000000LL * intensity;
    int fibonacci_n = 30 + intensity * 5;
    int integration_steps = 1000000 * intensity;
    size_t sort_size = 1000000 * intensity;
    
    // 1. 素数计算测试
    benchmark("素数计算 (埃拉托斯特尼筛法)", [prime_limit]() {
        auto primes = CPUIntensiveTest::sieveOfEratosthenes(prime_limit);
        return primes.size();
    });
    
    // 2. 矩阵乘法测试
    benchmark("矩阵乘法", [matrix_size]() {
        // 创建随机矩阵
        std::vector<std::vector<double>> a(matrix_size, std::vector<double>(matrix_size));
        std::vector<std::vector<double>> b(matrix_size, std::vector<double>(matrix_size));
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(0.0, 1.0);
        
        for (int i = 0; i < matrix_size; ++i) {
            for (int j = 0; j < matrix_size; ++j) {
                a[i][j] = dis(gen);
                b[i][j] = dis(gen);
            }
        }
        
        auto result = CPUIntensiveTest::matrixMultiply(a, b);
        return result[0][0];  // 返回第一个元素作为验证
    });
    
    // 3. 蒙特卡洛计算π
    benchmark("蒙特卡洛法计算π", [pi_iterations]() {
        return CPUIntensiveTest::calculatePiMonteCarlo(pi_iterations);
    });
    
    // 4. 递归斐波那契数列（低效版本）
    if (fibonacci_n <= 40) {  // 避免过长时间
        benchmark("递归斐波那契数列", [fibonacci_n]() {
            return CPUIntensiveTest::fibonacciRecursive(fibonacci_n);
        });
    }
    
    // 5. 动态规划斐波那契数列（高效版本）
    benchmark("DP斐波那契数列", [fibonacci_n]() {
        return CPUIntensiveTest::fibonacciDP(fibonacci_n + 20);  // 可以计算更大的数
    });
    
    // 6. 数值积分
    benchmark("数值积分", [integration_steps]() {
        return CPUIntensiveTest::numericalIntegration(
            CPUIntensiveTest::testFunction, 0.0, 10.0, integration_steps);
    });
    
    // 7. 排序算法测试
    std::cout << "执行排序算法测试..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    CPUIntensiveTest::sortingBenchmark(sort_size);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "排序测试总耗时: " << duration.count() << "ms" << std::endl;
    std::cout << std::endl;
    
    std::cout << "=== CPU密集型测试完成 ===" << std::endl;
    
    return 0;
}

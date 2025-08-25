// io_intensive.cpp - I/O密集型测试程序
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <random>
#include <thread>
#include <future>
#include <filesystem>
#include <cstring>
#include <algorithm>
#include <numeric>

class IOIntensiveTest {
public:
    // 文件I/O性能测试
    static void fileIOPerformanceTest() {
        std::cout << "=== 文件I/O性能测试 ===" << std::endl;
        
        const size_t DATA_SIZE = 64 * 1024 * 1024;  // 64MB
        const std::string filename = "test_data.bin";
        
        // 生成测试数据
        std::vector<char> test_data(DATA_SIZE);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::generate(test_data.begin(), test_data.end(), [&]() { return gen() % 256; });
        
        // 1. 顺序写入测试
        auto start = std::chrono::high_resolution_clock::now();
        {
            std::ofstream file(filename, std::ios::binary);
            file.write(test_data.data(), test_data.size());
            file.flush();  // 确保数据写入磁盘
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto write_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // 2. 顺序读取测试
        std::vector<char> read_data(DATA_SIZE);
        start = std::chrono::high_resolution_clock::now();
        {
            std::ifstream file(filename, std::ios::binary);
            file.read(read_data.data(), read_data.size());
        }
        end = std::chrono::high_resolution_clock::now();
        auto read_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // 3. 随机访问测试
        const int RANDOM_READS = 10000;
        std::uniform_int_distribution<size_t> pos_dist(0, DATA_SIZE - 1024);
        
        start = std::chrono::high_resolution_clock::now();
        {
            std::ifstream file(filename, std::ios::binary);
            char buffer[1024];
            
            for (int i = 0; i < RANDOM_READS; ++i) {
                size_t pos = pos_dist(gen);
                file.seekg(pos);
                file.read(buffer, sizeof(buffer));
            }
        }
        end = std::chrono::high_resolution_clock::now();
        auto random_read_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // 计算带宽
        double write_bandwidth = (double)DATA_SIZE / (1024 * 1024) / (write_time.count() / 1000.0);
        double read_bandwidth = (double)DATA_SIZE / (1024 * 1024) / (read_time.count() / 1000.0);
        
        std::cout << "文件I/O性能结果:" << std::endl;
        std::cout << "  数据大小: " << DATA_SIZE / (1024 * 1024) << " MB" << std::endl;
        std::cout << "  顺序写入: " << write_time.count() << "ms (" << write_bandwidth << " MB/s)" << std::endl;
        std::cout << "  顺序读取: " << read_time.count() << "ms (" << read_bandwidth << " MB/s)" << std::endl;
        std::cout << "  随机读取 (" << RANDOM_READS << "次): " << random_read_time.count() << "ms" << std::endl;
        
        // 清理测试文件
        std::filesystem::remove(filename);
    }
    
    // 缓冲区大小对I/O性能的影响
    static void bufferSizeImpact() {
        std::cout << "\n=== 缓冲区大小对I/O性能的影响 ===" << std::endl;
        
        const size_t TOTAL_DATA = 16 * 1024 * 1024;  // 16MB
        const std::string filename = "buffer_test.bin";
        
        // 生成测试数据
        std::vector<char> source_data(TOTAL_DATA);
        std::iota(source_data.begin(), source_data.end(), 0);
        
        std::vector<size_t> buffer_sizes = {
            1024,         // 1KB
            4096,         // 4KB
            16384,        // 16KB
            65536,        // 64KB
            262144,       // 256KB
            1048576       // 1MB
        };
        
        for (size_t buffer_size : buffer_sizes) {
            // 写入测试
            auto start = std::chrono::high_resolution_clock::now();
            {
                std::ofstream file(filename, std::ios::binary);
                
                for (size_t pos = 0; pos < TOTAL_DATA; pos += buffer_size) {
                    size_t chunk_size = std::min(buffer_size, TOTAL_DATA - pos);
                    file.write(source_data.data() + pos, chunk_size);
                }
                file.flush();
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto write_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            
            // 读取测试
            std::vector<char> buffer(buffer_size);
            start = std::chrono::high_resolution_clock::now();
            {
                std::ifstream file(filename, std::ios::binary);
                
                for (size_t pos = 0; pos < TOTAL_DATA; pos += buffer_size) {
                    size_t chunk_size = std::min(buffer_size, TOTAL_DATA - pos);
                    file.read(buffer.data(), chunk_size);
                }
            }
            end = std::chrono::high_resolution_clock::now();
            auto read_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            
            double write_bandwidth = (double)TOTAL_DATA / (1024 * 1024) / (write_time.count() / 1000.0);
            double read_bandwidth = (double)TOTAL_DATA / (1024 * 1024) / (read_time.count() / 1000.0);
            
            std::cout << "缓冲区大小 " << buffer_size / 1024 << "KB:" << std::endl;
            std::cout << "  写入: " << write_time.count() << "ms (" << write_bandwidth << " MB/s)" << std::endl;
            std::cout << "  读取: " << read_time.count() << "ms (" << read_bandwidth << " MB/s)" << std::endl;
        }
        
        std::filesystem::remove(filename);
    }
    
    // 并发I/O测试
    static void concurrentIOTest() {
        std::cout << "\n=== 并发I/O测试 ===" << std::endl;
        
        const size_t FILE_SIZE = 8 * 1024 * 1024;  // 8MB per file
        const int NUM_FILES = 4;
        
        // 生成测试数据
        std::vector<char> test_data(FILE_SIZE);
        std::iota(test_data.begin(), test_data.end(), 0);
        
        // 1. 串行I/O测试
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < NUM_FILES; ++i) {
            std::string filename = "serial_test_" + std::to_string(i) + ".bin";
            std::ofstream file(filename, std::ios::binary);
            file.write(test_data.data(), test_data.size());
            file.flush();
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto serial_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // 2. 并行I/O测试
        start = std::chrono::high_resolution_clock::now();
        {
            std::vector<std::future<void>> futures;
            
            for (int i = 0; i < NUM_FILES; ++i) {
                futures.push_back(std::async(std::launch::async, [i, &test_data]() {
                    std::string filename = "parallel_test_" + std::to_string(i) + ".bin";
                    std::ofstream file(filename, std::ios::binary);
                    file.write(test_data.data(), test_data.size());
                    file.flush();
                }));
            }
            
            // 等待所有任务完成
            for (auto& future : futures) {
                future.wait();
            }
        }
        end = std::chrono::high_resolution_clock::now();
        auto parallel_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "并发I/O性能对比 (" << NUM_FILES << "个文件，每个" << FILE_SIZE / (1024*1024) << "MB):" << std::endl;
        std::cout << "  串行写入: " << serial_time.count() << "ms" << std::endl;
        std::cout << "  并行写入: " << parallel_time.count() << "ms" << std::endl;
        std::cout << "  性能提升: " << (double)serial_time.count() / parallel_time.count() << "x" << std::endl;
        
        // 清理测试文件
        for (int i = 0; i < NUM_FILES; ++i) {
            std::filesystem::remove("serial_test_" + std::to_string(i) + ".bin");
            std::filesystem::remove("parallel_test_" + std::to_string(i) + ".bin");
        }
    }
    
    // 内存映射文件测试
    static void memoryMappedFileTest() {
        std::cout << "\n=== 内存映射文件测试 ===" << std::endl;
        
        const size_t FILE_SIZE = 32 * 1024 * 1024;  // 32MB
        const std::string filename = "mmap_test.bin";
        
        // 创建测试文件
        {
            std::ofstream file(filename, std::ios::binary);
            std::vector<char> data(FILE_SIZE);
            std::iota(data.begin(), data.end(), 0);
            file.write(data.data(), data.size());
        }
        
        // 1. 传统文件I/O测试
        auto start = std::chrono::high_resolution_clock::now();
        {
            std::ifstream file(filename, std::ios::binary);
            std::vector<char> buffer(FILE_SIZE);
            file.read(buffer.data(), buffer.size());
            
            // 简单处理：计算校验和
            long long checksum = 0;
            for (char c : buffer) {
                checksum += static_cast<unsigned char>(c);
            }
            volatile long long temp = checksum;  // 防止优化
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto traditional_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // 注意：真正的内存映射需要系统调用，这里只是模拟概念
        // 在实际应用中，你会使用mmap()系统调用或平台特定的API
        
        std::cout << "I/O方式对比:" << std::endl;
        std::cout << "  传统文件I/O: " << traditional_time.count() << "ms" << std::endl;
        std::cout << "  注意: 真正的内存映射需要系统级API实现" << std::endl;
        
        std::filesystem::remove(filename);
    }
    
    // 不同I/O模式对比
    static void ioModeComparison() {
        std::cout << "\n=== I/O模式对比测试 ===" << std::endl;
        
        const size_t DATA_SIZE = 16 * 1024 * 1024;  // 16MB
        const std::string filename = "io_mode_test.bin";
        
        // 生成测试数据
        std::vector<char> test_data(DATA_SIZE);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::generate(test_data.begin(), test_data.end(), [&]() { return gen() % 256; });
        
        // 1. 带缓冲的I/O (默认)
        auto start = std::chrono::high_resolution_clock::now();
        {
            std::ofstream file(filename, std::ios::binary);
            file.write(test_data.data(), test_data.size());
            // 文件关闭时自动flush
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto buffered_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // 2. 无缓冲的I/O
        start = std::chrono::high_resolution_clock::now();
        {
            std::ofstream file(filename, std::ios::binary);
            file.rdbuf()->pubsetbuf(nullptr, 0);  // 禁用缓冲
            file.write(test_data.data(), test_data.size());
        }
        end = std::chrono::high_resolution_clock::now();
        auto unbuffered_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // 3. 大缓冲区I/O
        start = std::chrono::high_resolution_clock::now();
        {
            std::ofstream file(filename, std::ios::binary);
            std::vector<char> big_buffer(1024 * 1024);  // 1MB缓冲区
            file.rdbuf()->pubsetbuf(big_buffer.data(), big_buffer.size());
            file.write(test_data.data(), test_data.size());
        }
        end = std::chrono::high_resolution_clock::now();
        auto big_buffer_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // 4. C风格I/O对比
        start = std::chrono::high_resolution_clock::now();
        {
            FILE* file = fopen(filename.c_str(), "wb");
            if (file) {
                fwrite(test_data.data(), 1, test_data.size(), file);
                fclose(file);
            }
        }
        end = std::chrono::high_resolution_clock::now();
        auto c_style_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "I/O模式性能对比 (" << DATA_SIZE / (1024*1024) << "MB数据):" << std::endl;
        std::cout << "  C++ 带缓冲I/O: " << buffered_time.count() << "ms" << std::endl;
        std::cout << "  C++ 无缓冲I/O: " << unbuffered_time.count() << "ms" << std::endl;
        std::cout << "  C++ 大缓冲I/O: " << big_buffer_time.count() << "ms" << std::endl;
        std::cout << "  C风格I/O: " << c_style_time.count() << "ms" << std::endl;
        
        std::filesystem::remove(filename);
    }
    
    // 网络I/O模拟测试
    static void networkIOSimulation() {
        std::cout << "\n=== 网络I/O模拟测试 ===" << std::endl;
        
        // 模拟不同网络延迟和带宽的影响
        struct NetworkProfile {
            std::string name;
            int latency_ms;      // 延迟
            int bandwidth_mbps;  // 带宽 (Mbps)
        };
        
        std::vector<NetworkProfile> profiles = {
            {"局域网", 1, 1000},
            {"宽带", 20, 100},
            {"4G移动网", 50, 50},
            {"3G移动网", 100, 10},
            {"慢速连接", 200, 1}
        };
        
        const size_t DATA_SIZE = 1024 * 1024;  // 1MB数据包
        const int PACKET_COUNT = 10;
        
        for (const auto& profile : profiles) {
            auto start = std::chrono::high_resolution_clock::now();
            
            for (int i = 0; i < PACKET_COUNT; ++i) {
                // 模拟网络延迟
                std::this_thread::sleep_for(std::chrono::milliseconds(profile.latency_ms));
                
                // 模拟数据传输时间
                double transfer_time_ms = (double)DATA_SIZE * 8 / (profile.bandwidth_mbps * 1000);
                std::this_thread::sleep_for(std::chrono::milliseconds((int)transfer_time_ms));
                
                // 模拟数据处理
                volatile int dummy_work = 0;
                for (int j = 0; j < 10000; ++j) {
                    dummy_work += j;
                }
            }
            
            auto end = std::chrono::high_resolution_clock::now();
            auto total_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            
            double effective_bandwidth = (double)(DATA_SIZE * PACKET_COUNT) / (1024 * 1024) / (total_time.count() / 1000.0);
            
            std::cout << profile.name << " (" << profile.latency_ms << "ms延迟, " 
                      << profile.bandwidth_mbps << "Mbps带宽):" << std::endl;
            std::cout << "  传输" << PACKET_COUNT << "个1MB数据包: " << total_time.count() << "ms" << std::endl;
            std::cout << "  有效带宽: " << effective_bandwidth << " MB/s" << std::endl;
            std::cout << std::endl;
        }
    }
    
    // 大文件处理测试
    static void largeFileProcessing() {
        std::cout << "\n=== 大文件处理测试 ===" << std::endl;
        
        const size_t CHUNK_SIZE = 1024 * 1024;  // 1MB块
        const int NUM_CHUNKS = 100;  // 总共100MB
        const std::string filename = "large_file_test.bin";
        
        // 创建大文件
        std::cout << "创建大文件 (" << NUM_CHUNKS << "MB)..." << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        {
            std::ofstream file(filename, std::ios::binary);
            std::vector<char> chunk(CHUNK_SIZE);
            
            for (int i = 0; i < NUM_CHUNKS; ++i) {
                // 填充不同的数据模式
                std::fill(chunk.begin(), chunk.end(), i % 256);
                file.write(chunk.data(), chunk.size());
                
                // 显示进度
                if (i % 10 == 0) {
                    std::cout << "  进度: " << (i * 100 / NUM_CHUNKS) << "%" << std::endl;
                }
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto create_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // 分块读取处理
        std::cout << "分块处理大文件..." << std::endl;
        start = std::chrono::high_resolution_clock::now();
        {
            std::ifstream file(filename, std::ios::binary);
            std::vector<char> chunk(CHUNK_SIZE);
            long long total_checksum = 0;
            
            for (int i = 0; i < NUM_CHUNKS; ++i) {
                file.read(chunk.data(), chunk.size());
                
                // 模拟数据处理（计算校验和）
                long long chunk_checksum = 0;
                for (char c : chunk) {
                    chunk_checksum += static_cast<unsigned char>(c);
                }
                total_checksum += chunk_checksum;
                
                if (i % 10 == 0) {
                    std::cout << "  处理进度: " << (i * 100 / NUM_CHUNKS) << "%" << std::endl;
                }
            }
            
            volatile long long temp = total_checksum;  // 防止优化
        }
        end = std::chrono::high_resolution_clock::now();
        auto process_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // 获取文件大小确认
        auto file_size = std::filesystem::file_size(filename);
        
        std::cout << "大文件处理结果:" << std::endl;
        std::cout << "  文件大小: " << file_size / (1024*1024) << " MB" << std::endl;
        std::cout << "  创建时间: " << create_time.count() << "ms" << std::endl;
        std::cout << "  处理时间: " << process_time.count() << "ms" << std::endl;
        std::cout << "  创建速度: " << (double)file_size / (1024*1024) / (create_time.count() / 1000.0) << " MB/s" << std::endl;
        std::cout << "  处理速度: " << (double)file_size / (1024*1024) / (process_time.count() / 1000.0) << " MB/s" << std::endl;
        
        std::filesystem::remove(filename);
    }
};

int main(int argc, char* argv[]) {
    std::cout << "=== I/O密集型测试程序 ===" << std::endl;
    
    // 可以通过命令行参数选择测试类型
    std::string test_type = "all";
    if (argc > 1) {
        test_type = argv[1];
    }
    
    std::cout << "测试类型: " << test_type << std::endl;
    std::cout << "注意: I/O测试结果会受到磁盘类型、文件系统等因素影响" << std::endl;
    std::cout << std::endl;
    
    if (test_type == "file" || test_type == "all") {
        IOIntensiveTest::fileIOPerformanceTest();
    }
    
    if (test_type == "buffer" || test_type == "all") {
        IOIntensiveTest::bufferSizeImpact();
    }
    
    if (test_type == "concurrent" || test_type == "all") {
        IOIntensiveTest::concurrentIOTest();
    }
    
    if (test_type == "mmap" || test_type == "all") {
        IOIntensiveTest::memoryMappedFileTest();
    }
    
    if (test_type == "mode" || test_type == "all") {
        IOIntensiveTest::ioModeComparison();
    }
    
    if (test_type == "network" || test_type == "all") {
        IOIntensiveTest::networkIOSimulation();
    }
    
    if (test_type == "large" || test_type == "all") {
        IOIntensiveTest::largeFileProcessing();
    }
    
    std::cout << "=== I/O密集型测试完成 ===" << std::endl;
    
    return 0;
}

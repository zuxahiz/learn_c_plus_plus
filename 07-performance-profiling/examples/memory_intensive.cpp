// memory_intensive.cpp - 内存密集型测试程序
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <memory>
#include <list>
#include <deque>
#include <numeric>
#include <cstring>

class MemoryIntensiveTest {
public:
    // 测试不同的内存访问模式
    static void memoryAccessPatterns() {
        std::cout << "=== 内存访问模式测试 ===" << std::endl;
        
        const size_t SIZE = 64 * 1024 * 1024 / sizeof(int);  // 64MB数据
        std::vector<int> data(SIZE);
        std::iota(data.begin(), data.end(), 1);
        
        // 1. 顺序访问（缓存友好）
        auto start = std::chrono::high_resolution_clock::now();
        long long sum1 = 0;
        for (size_t i = 0; i < SIZE; ++i) {
            sum1 += data[i];
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto sequential_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // 2. 随机访问（缓存不友好）
        std::vector<size_t> indices(SIZE);
        std::iota(indices.begin(), indices.end(), 0);
        std::random_device rd;
        std::shuffle(indices.begin(), indices.end(), std::mt19937(rd()));
        
        start = std::chrono::high_resolution_clock::now();
        long long sum2 = 0;
        for (size_t i = 0; i < SIZE; ++i) {
            sum2 += data[indices[i]];
        }
        end = std::chrono::high_resolution_clock::now();
        auto random_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // 3. 步长访问
        start = std::chrono::high_resolution_clock::now();
        long long sum3 = 0;
        for (size_t i = 0; i < SIZE; i += 16) {  // 每16个元素访问一次
            sum3 += data[i];
        }
        end = std::chrono::high_resolution_clock::now();
        auto strided_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "内存访问模式对比:" << std::endl;
        std::cout << "  顺序访问: " << sequential_time.count() << "ms (sum=" << sum1 << ")" << std::endl;
        std::cout << "  随机访问: " << random_time.count() << "ms (sum=" << sum2 << ")" << std::endl;
        std::cout << "  步长访问: " << strided_time.count() << "ms (sum=" << sum3 << ")" << std::endl;
        std::cout << "  随机/顺序比: " << (double)random_time.count() / sequential_time.count() << "x" << std::endl;
    }
    
    // 测试容器的内存效率
    static void containerMemoryEfficiency() {
        std::cout << "\n=== 容器内存效率测试 ===" << std::endl;
        
        const int COUNT = 1000000;
        
        // vector - 连续内存
        auto start = std::chrono::high_resolution_clock::now();
        {
            std::vector<int> vec;
            vec.reserve(COUNT);  // 预分配
            for (int i = 0; i < COUNT; ++i) {
                vec.push_back(i);
            }
            
            long long sum = 0;
            for (int val : vec) {
                sum += val;
            }
            volatile long long temp = sum;  // 防止优化
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto vector_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // list - 链表结构
        start = std::chrono::high_resolution_clock::now();
        {
            std::list<int> lst;
            for (int i = 0; i < COUNT; ++i) {
                lst.push_back(i);
            }
            
            long long sum = 0;
            for (int val : lst) {
                sum += val;
            }
            volatile long long temp = sum;  // 防止优化
        }
        end = std::chrono::high_resolution_clock::now();
        auto list_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // deque - 分段数组
        start = std::chrono::high_resolution_clock::now();
        {
            std::deque<int> deq;
            for (int i = 0; i < COUNT; ++i) {
                deq.push_back(i);
            }
            
            long long sum = 0;
            for (int val : deq) {
                sum += val;
            }
            volatile long long temp = sum;  // 防止优化
        }
        end = std::chrono::high_resolution_clock::now();
        auto deque_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "容器性能对比 (" << COUNT << "个元素):" << std::endl;
        std::cout << "  vector: " << vector_time.count() << "ms" << std::endl;
        std::cout << "  list:   " << list_time.count() << "ms" << std::endl;
        std::cout << "  deque:  " << deque_time.count() << "ms" << std::endl;
        
        // 内存使用量估算
        size_t vector_memory = COUNT * sizeof(int);
        size_t list_memory = COUNT * (sizeof(int) + 2 * sizeof(void*));  // 数据 + 前后指针
        size_t deque_memory = COUNT * sizeof(int) + COUNT / 512 * sizeof(void*);  // 估算
        
        std::cout << "内存使用量估算:" << std::endl;
        std::cout << "  vector: " << vector_memory / 1024 << " KB" << std::endl;
        std::cout << "  list:   " << list_memory / 1024 << " KB" << std::endl;
        std::cout << "  deque:  " << deque_memory / 1024 << " KB" << std::endl;
    }
    
    // 内存分配和释放性能测试
    static void memoryAllocationTest() {
        std::cout << "\n=== 内存分配性能测试 ===" << std::endl;
        
        const int ITERATIONS = 100000;
        
        // 1. 频繁小对象分配
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < ITERATIONS; ++i) {
            int* ptr = new int(i);
            delete ptr;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto raw_alloc_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // 2. 智能指针分配
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < ITERATIONS; ++i) {
            auto ptr = std::make_unique<int>(i);
            // 自动释放
        }
        end = std::chrono::high_resolution_clock::now();
        auto smart_ptr_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // 3. 预分配策略
        start = std::chrono::high_resolution_clock::now();
        {
            std::vector<int> pool;
            pool.reserve(ITERATIONS);
            for (int i = 0; i < ITERATIONS; ++i) {
                pool.push_back(i);
            }
            // 批量释放
        }
        end = std::chrono::high_resolution_clock::now();
        auto pool_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // 4. 大块内存分配
        const size_t BIG_SIZE = 1024 * 1024;  // 1MB
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 100; ++i) {
            char* big_block = new char[BIG_SIZE];
            std::memset(big_block, i, BIG_SIZE);  // 确保真正使用内存
            delete[] big_block;
        }
        end = std::chrono::high_resolution_clock::now();
        auto big_alloc_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "内存分配性能对比:" << std::endl;
        std::cout << "  原始指针分配 (" << ITERATIONS << "次): " << raw_alloc_time.count() << "ms" << std::endl;
        std::cout << "  智能指针分配 (" << ITERATIONS << "次): " << smart_ptr_time.count() << "ms" << std::endl;
        std::cout << "  预分配策略 (" << ITERATIONS << "个): " << pool_time.count() << "ms" << std::endl;
        std::cout << "  大块内存分配 (100x1MB): " << big_alloc_time.count() << "ms" << std::endl;
    }
    
    // 缓存行为测试
    static void cachePerformanceTest() {
        std::cout << "\n=== 缓存性能测试 ===" << std::endl;
        
        // 创建不同大小的数据集来测试不同级别的缓存
        struct CacheTest {
            size_t size;
            std::string name;
        };
        
        std::vector<CacheTest> tests = {
            {8 * 1024, "L1 Cache (8KB)"},
            {256 * 1024, "L2 Cache (256KB)"},
            {8 * 1024 * 1024, "L3 Cache (8MB)"},
            {128 * 1024 * 1024, "Main Memory (128MB)"}
        };
        
        for (const auto& test : tests) {
            size_t count = test.size / sizeof(int);
            std::vector<int> data(count);
            std::iota(data.begin(), data.end(), 1);
            
            // 重复访问测试缓存效果
            auto start = std::chrono::high_resolution_clock::now();
            
            const int REPEATS = 100;
            volatile long long sum = 0;
            for (int rep = 0; rep < REPEATS; ++rep) {
                for (size_t i = 0; i < count; ++i) {
                    sum += data[i];
                }
            }
            
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            
            double bytes_per_second = (double)(count * sizeof(int) * REPEATS) / (duration.count() / 1000000.0);
            double gb_per_second = bytes_per_second / (1024 * 1024 * 1024);
            
            std::cout << test.name << ":" << std::endl;
            std::cout << "  数据大小: " << test.size / 1024 << " KB" << std::endl;
            std::cout << "  访问时间: " << duration.count() << " μs" << std::endl;
            std::cout << "  带宽: " << gb_per_second << " GB/s" << std::endl;
            std::cout << std::endl;
        }
    }
    
    // 内存碎片化测试
    static void memoryFragmentationTest() {
        std::cout << "\n=== 内存碎片化测试 ===" << std::endl;
        
        const int OBJECTS = 10000;
        std::vector<void*> pointers;
        pointers.reserve(OBJECTS);
        
        // 1. 分配大量不同大小的对象
        auto start = std::chrono::high_resolution_clock::now();
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> size_dist(16, 1024);
        
        for (int i = 0; i < OBJECTS; ++i) {
            size_t size = size_dist(gen);
            void* ptr = malloc(size);
            if (ptr) {
                pointers.push_back(ptr);
                // 写入数据确保真正分配
                memset(ptr, i % 256, size);
            }
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto alloc_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // 2. 随机释放一半对象（造成碎片）
        std::shuffle(pointers.begin(), pointers.end(), gen);
        
        start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < pointers.size() / 2; ++i) {
            free(pointers[i]);
            pointers[i] = nullptr;
        }
        end = std::chrono::high_resolution_clock::now();
        auto free_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // 3. 尝试分配大块连续内存（可能因碎片化失败）
        start = std::chrono::high_resolution_clock::now();
        const size_t BIG_SIZE = 1024 * 1024;  // 1MB
        void* big_block = malloc(BIG_SIZE);
        end = std::chrono::high_resolution_clock::now();
        auto big_alloc_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "内存碎片化测试结果:" << std::endl;
        std::cout << "  小对象分配 (" << OBJECTS << "个): " << alloc_time.count() << "ms" << std::endl;
        std::cout << "  随机释放 (" << OBJECTS/2 << "个): " << free_time.count() << "ms" << std::endl;
        std::cout << "  大块分配 (1MB): " << big_alloc_time.count() << "μs" << std::endl;
        std::cout << "  大块分配" << (big_block ? "成功" : "失败") << std::endl;
        
        // 清理剩余内存
        for (void* ptr : pointers) {
            if (ptr) free(ptr);
        }
        if (big_block) free(big_block);
    }
    
    // 数据结构对齐测试
    static void dataAlignmentTest() {
        std::cout << "\n=== 数据结构对齐测试 ===" << std::endl;
        
        // 未对齐的结构体
        struct UnalignedStruct {
            char a;      // 1 byte
            int b;       // 4 bytes
            char c;      // 1 byte  
            double d;    // 8 bytes
        };
        
        // 手动对齐的结构体
        struct AlignedStruct {
            double d;    // 8 bytes (最大对齐要求)
            int b;       // 4 bytes
            char a;      // 1 byte
            char c;      // 1 byte
            // 编译器会自动填充到8字节边界
        };
        
        // 使用alignas强制对齐
        struct alignas(64) CacheAlignedStruct {  // 64字节对齐（典型缓存行大小）
            double d;
            int b;
            char a;
            char c;
        };
        
        const int COUNT = 1000000;
        
        std::cout << "结构体大小对比:" << std::endl;
        std::cout << "  未对齐结构体: " << sizeof(UnalignedStruct) << " bytes" << std::endl;
        std::cout << "  已对齐结构体: " << sizeof(AlignedStruct) << " bytes" << std::endl;
        std::cout << "  缓存行对齐结构体: " << sizeof(CacheAlignedStruct) << " bytes" << std::endl;
        
        // 测试访问性能
        std::vector<UnalignedStruct> unaligned_data(COUNT);
        std::vector<AlignedStruct> aligned_data(COUNT);
        std::vector<CacheAlignedStruct> cache_aligned_data(COUNT);
        
        // 未对齐结构体访问测试
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < COUNT; ++i) {
            unaligned_data[i].b = i;
            unaligned_data[i].d = i * 1.5;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto unaligned_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // 已对齐结构体访问测试
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < COUNT; ++i) {
            aligned_data[i].b = i;
            aligned_data[i].d = i * 1.5;
        }
        end = std::chrono::high_resolution_clock::now();
        auto aligned_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // 缓存行对齐结构体访问测试
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < COUNT; ++i) {
            cache_aligned_data[i].b = i;
            cache_aligned_data[i].d = i * 1.5;
        }
        end = std::chrono::high_resolution_clock::now();
        auto cache_aligned_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "访问性能对比:" << std::endl;
        std::cout << "  未对齐访问: " << unaligned_time.count() << " μs" << std::endl;
        std::cout << "  已对齐访问: " << aligned_time.count() << " μs" << std::endl;
        std::cout << "  缓存行对齐访问: " << cache_aligned_time.count() << " μs" << std::endl;
        
        // 内存使用效率
        size_t unaligned_memory = COUNT * sizeof(UnalignedStruct);
        size_t aligned_memory = COUNT * sizeof(AlignedStruct);
        size_t cache_aligned_memory = COUNT * sizeof(CacheAlignedStruct);
        
        std::cout << "内存使用量:" << std::endl;
        std::cout << "  未对齐: " << unaligned_memory / 1024 << " KB" << std::endl;
        std::cout << "  已对齐: " << aligned_memory / 1024 << " KB" << std::endl;
        std::cout << "  缓存行对齐: " << cache_aligned_memory / 1024 << " KB" << std::endl;
    }
};

int main(int argc, char* argv[]) {
    std::cout << "=== 内存密集型测试程序 ===" << std::endl;
    
    // 可以通过命令行参数选择测试类型
    std::string test_type = "all";
    if (argc > 1) {
        test_type = argv[1];
    }
    
    std::cout << "测试类型: " << test_type << std::endl;
    std::cout << std::endl;
    
    if (test_type == "access" || test_type == "all") {
        MemoryIntensiveTest::memoryAccessPatterns();
    }
    
    if (test_type == "container" || test_type == "all") {
        MemoryIntensiveTest::containerMemoryEfficiency();
    }
    
    if (test_type == "allocation" || test_type == "all") {
        MemoryIntensiveTest::memoryAllocationTest();
    }
    
    if (test_type == "cache" || test_type == "all") {
        MemoryIntensiveTest::cachePerformanceTest();
    }
    
    if (test_type == "fragmentation" || test_type == "all") {
        MemoryIntensiveTest::memoryFragmentationTest();
    }
    
    if (test_type == "alignment" || test_type == "all") {
        MemoryIntensiveTest::dataAlignmentTest();
    }
    
    std::cout << "=== 内存密集型测试完成 ===" << std::endl;
    
    return 0;
}

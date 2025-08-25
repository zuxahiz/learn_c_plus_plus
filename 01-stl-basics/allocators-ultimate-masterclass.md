# C++ STL 分配器终极大师级教程

## 🎯 分配器的核心哲学

分配器是STL中最**底层**但最**重要**的组件，它实现了**内存管理的抽象化**，将内存分配与对象构造分离，是**零开销抽象**和**可定制化**的完美体现。

```
分配器设计架构
├── 内存管理抽象 (Memory Management Abstraction)
│   ├── 分配 (allocate) - 原始内存分配
│   ├── 释放 (deallocate) - 原始内存释放
│   ├── 构造 (construct) - 对象就地构造
│   └── 析构 (destroy) - 对象就地析构
│
├── 类型系统 (Type System)
│   ├── value_type - 分配的对象类型
│   ├── pointer/const_pointer - 指针类型
│   ├── reference/const_reference - 引用类型
│   ├── size_type - 大小类型
│   └── difference_type - 差值类型
│
├── 分配器特性 (Allocator Traits)
│   ├── rebind - 类型转换能力
│   ├── propagate_on_* - 传播策略
│   ├── is_always_equal - 相等性判断
│   └── select_on_* - 选择策略
│
└── 高级特性 (Advanced Features)
    ├── 内存池 (Memory Pool)
    ├── 对齐分配 (Aligned Allocation)
    ├── 调试支持 (Debug Support)
    └── 性能优化 (Performance Optimization)

分配器设计原则:
✓ 分离关注点 - 内存分配与对象生命周期分离
✓ 可定制性 - 支持不同的内存管理策略
✓ 零开销抽象 - 不引入额外运行时开销
✓ 类型安全 - 编译时类型检查
```

---

## 🔬 标准分配器深度解析

### 💻 std::allocator 内部机制

```cpp
#include <memory>
#include <iostream>
#include <vector>
#include <chrono>
#include <type_traits>

class StandardAllocatorMasterclass {
public:
    // 1. std::allocator的基本原理
    static void allocatorBasicPrinciples() {
        std::cout << "=== std::allocator基本原理 ===" << std::endl;
        
        std::allocator<int> alloc;
        
        std::cout << "1. 分配器类型信息:" << std::endl;
        std::cout << "value_type: " << typeid(decltype(alloc)::value_type).name() << std::endl;
        std::cout << "size_type: " << sizeof(decltype(alloc)::size_type) << " bytes" << std::endl;
        std::cout << "difference_type: " << sizeof(decltype(alloc)::difference_type) << " bytes" << std::endl;
        
        // 内存分配演示
        std::cout << "\n2. 内存分配和释放:" << std::endl;
        
        const size_t n = 10;
        
        // 分配原始内存（不构造对象）
        int* raw_memory = alloc.allocate(n);
        std::cout << "分配了 " << n << " 个int的内存空间" << std::endl;
        std::cout << "内存地址: " << raw_memory << std::endl;
        
        // 在分配的内存中构造对象
        for(size_t i = 0; i < n; ++i) {
            std::allocator_traits<decltype(alloc)>::construct(alloc, raw_memory + i, i * 10);
        }
        
        std::cout << "构造的对象: ";
        for(size_t i = 0; i < n; ++i) {
            std::cout << raw_memory[i] << " ";
        }
        std::cout << std::endl;
        
        // 析构对象
        for(size_t i = 0; i < n; ++i) {
            std::allocator_traits<decltype(alloc)>::destroy(alloc, raw_memory + i);
        }
        
        // 释放内存
        alloc.deallocate(raw_memory, n);
        std::cout << "内存已释放" << std::endl;
        
        std::cout << "\n3. 与new/delete的对比:" << std::endl;
        
        // new/delete - 分配+构造一体化
        auto start = std::chrono::high_resolution_clock::now();
        
        std::vector<int*> ptrs;
        for(int i = 0; i < 10000; ++i) {
            ptrs.push_back(new int(i));
        }
        
        for(int* ptr : ptrs) {
            delete ptr;
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto new_delete_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // allocator - 分配和构造分离
        start = std::chrono::high_resolution_clock::now();
        
        std::vector<int*> alloc_ptrs;
        for(int i = 0; i < 10000; ++i) {
            int* ptr = alloc.allocate(1);
            std::allocator_traits<decltype(alloc)>::construct(alloc, ptr, i);
            alloc_ptrs.push_back(ptr);
        }
        
        for(int* ptr : alloc_ptrs) {
            std::allocator_traits<decltype(alloc)>::destroy(alloc, ptr);
            alloc.deallocate(ptr, 1);
        }
        
        end = std::chrono::high_resolution_clock::now();
        auto allocator_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "new/delete时间: " << new_delete_time.count() << "μs" << std::endl;
        std::cout << "allocator时间: " << allocator_time.count() << "μs" << std::endl;
    }
    
    // 2. allocator_traits的威力
    static void allocatorTraitsFeatures() {
        std::cout << "\n=== allocator_traits的威力 ===" << std::endl;
        
        std::allocator<std::string> string_alloc;
        
        std::cout << "1. allocator_traits特性检测:" << std::endl;
        
        using Traits = std::allocator_traits<decltype(string_alloc)>;
        
        std::cout << "propagate_on_container_copy_assignment: " 
                  << Traits::propagate_on_container_copy_assignment::value << std::endl;
        std::cout << "propagate_on_container_move_assignment: " 
                  << Traits::propagate_on_container_move_assignment::value << std::endl;
        std::cout << "propagate_on_container_swap: " 
                  << Traits::propagate_on_container_swap::value << std::endl;
        std::cout << "is_always_equal: " 
                  << Traits::is_always_equal::value << std::endl;
        
        // rebind功能演示
        std::cout << "\n2. rebind功能:" << std::endl;
        
        using IntAlloc = decltype(string_alloc);
        using CharAlloc = typename Traits::template rebind_alloc<char>;
        using DoubleAlloc = typename Traits::template rebind_alloc<double>;
        
        std::cout << "原始分配器类型: " << typeid(IntAlloc::value_type).name() << std::endl;
        std::cout << "rebind到char: " << typeid(CharAlloc::value_type).name() << std::endl;
        std::cout << "rebind到double: " << typeid(DoubleAlloc::value_type).name() << std::endl;
        
        // 使用rebind的实际例子
        CharAlloc char_alloc = Traits::template rebind_alloc<char>(string_alloc);
        char* char_memory = char_alloc.allocate(100);
        
        const char* message = "Hello, Allocator!";
        for(size_t i = 0; message[i]; ++i) {
            std::allocator_traits<CharAlloc>::construct(char_alloc, char_memory + i, message[i]);
        }
        
        std::cout << "使用rebind分配器构造的字符串: ";
        for(size_t i = 0; message[i]; ++i) {
            std::cout << char_memory[i];
        }
        std::cout << std::endl;
        
        // 清理
        for(size_t i = 0; message[i]; ++i) {
            std::allocator_traits<CharAlloc>::destroy(char_alloc, char_memory + i);
        }
        char_alloc.deallocate(char_memory, 100);
    }
    
    // 3. 分配器的容器集成
    static void allocatorContainerIntegration() {
        std::cout << "\n=== 分配器的容器集成 ===" << std::endl;
        
        // 自定义分配器的简单实现
        template<typename T>
        class LoggingAllocator {
        public:
            using value_type = T;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;
            
            LoggingAllocator() = default;
            
            template<typename U>
            LoggingAllocator(const LoggingAllocator<U>&) {}
            
            T* allocate(size_type n) {
                std::cout << "[Alloc] 分配 " << n << " 个 " << typeid(T).name() 
                          << " 对象 (" << n * sizeof(T) << " bytes)" << std::endl;
                
                return static_cast<T*>(::operator new(n * sizeof(T)));
            }
            
            void deallocate(T* p, size_type n) {
                std::cout << "[Alloc] 释放 " << n << " 个 " << typeid(T).name() 
                          << " 对象 (" << n * sizeof(T) << " bytes)" << std::endl;
                
                ::operator delete(p);
            }
            
            template<typename U>
            bool operator==(const LoggingAllocator<U>&) const { return true; }
            
            template<typename U>
            bool operator!=(const LoggingAllocator<U>&) const { return false; }
        };
        
        std::cout << "使用自定义分配器的vector:" << std::endl;
        
        std::vector<int, LoggingAllocator<int>> logged_vector;
        
        std::cout << "\n插入元素:" << std::endl;
        for(int i = 1; i <= 5; ++i) {
            logged_vector.push_back(i * 10);
        }
        
        std::cout << "\nvector内容: ";
        for(int x : logged_vector) {
            std::cout << x << " ";
        }
        std::cout << std::endl;
        
        std::cout << "\n预留空间:" << std::endl;
        logged_vector.reserve(20);
        
        std::cout << "\n清空vector:" << std::endl;
        logged_vector.clear();
        
        std::cout << "\n析构vector:" << std::endl;
        // vector析构时会自动调用分配器的deallocate
    }
};
```

---

## 🏗️ 自定义分配器设计

### 🎯 内存池分配器

```cpp
class MemoryPoolAllocatorMasterclass {
public:
    // 1. 内存池分配器的基本实现
    static void basicMemoryPoolAllocator() {
        std::cout << "\n=== 内存池分配器基本实现 ===" << std::endl;
        
        // 简单的内存池分配器
        template<typename T, size_t PoolSize = 4096>
        class PoolAllocator {
        private:
            struct FreeBlock {
                FreeBlock* next;
            };
            
            static char memory_pool[PoolSize];
            static FreeBlock* free_list;
            static size_t allocated_bytes;
            static bool initialized;
            
            static void initialize() {
                if(initialized) return;
                
                // 将内存池分割成T大小的块
                const size_t block_size = sizeof(T) >= sizeof(FreeBlock*) ? sizeof(T) : sizeof(FreeBlock*);
                const size_t num_blocks = PoolSize / block_size;
                
                char* current = memory_pool;
                free_list = reinterpret_cast<FreeBlock*>(current);
                
                FreeBlock* block = free_list;
                for(size_t i = 1; i < num_blocks; ++i) {
                    current += block_size;
                    block->next = reinterpret_cast<FreeBlock*>(current);
                    block = block->next;
                }
                block->next = nullptr;
                
                initialized = true;
                
                std::cout << "[Pool] 初始化内存池: " << PoolSize << " bytes, " 
                          << num_blocks << " 块" << std::endl;
            }
            
        public:
            using value_type = T;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using const_pointer = const T*;
            using reference = T&;
            using const_reference = const T&;
            
            template<typename U>
            struct rebind {
                using other = PoolAllocator<U, PoolSize>;
            };
            
            PoolAllocator() {
                initialize();
            }
            
            template<typename U>
            PoolAllocator(const PoolAllocator<U, PoolSize>&) {
                initialize();
            }
            
            T* allocate(size_type n) {
                if(n != 1) {
                    // 对于多个对象的分配，回退到标准分配器
                    std::cout << "[Pool] 大块分配，使用标准分配器: " << n << " 对象" << std::endl;
                    return static_cast<T*>(::operator new(n * sizeof(T)));
                }
                
                if(!free_list) {
                    std::cout << "[Pool] 内存池耗尽，使用标准分配器" << std::endl;
                    return static_cast<T*>(::operator new(sizeof(T)));
                }
                
                FreeBlock* block = free_list;
                free_list = free_list->next;
                allocated_bytes += sizeof(T);
                
                std::cout << "[Pool] 从池分配: " << sizeof(T) << " bytes (总分配: " 
                          << allocated_bytes << ")" << std::endl;
                
                return reinterpret_cast<T*>(block);
            }
            
            void deallocate(T* p, size_type n) {
                if(!p) return;
                
                // 检查是否在内存池范围内
                char* ptr = reinterpret_cast<char*>(p);
                if(ptr >= memory_pool && ptr < memory_pool + PoolSize) {
                    // 返回到自由链表
                    FreeBlock* block = reinterpret_cast<FreeBlock*>(p);
                    block->next = free_list;
                    free_list = block;
                    allocated_bytes -= sizeof(T);
                    
                    std::cout << "[Pool] 返回到池: " << sizeof(T) << " bytes (剩余分配: " 
                              << allocated_bytes << ")" << std::endl;
                } else {
                    // 使用标准释放
                    std::cout << "[Pool] 标准释放: " << n * sizeof(T) << " bytes" << std::endl;
                    ::operator delete(p);
                }
            }
            
            template<typename U, typename... Args>
            void construct(U* p, Args&&... args) {
                new(p) U(std::forward<Args>(args)...);
            }
            
            template<typename U>
            void destroy(U* p) {
                p->~U();
            }
            
            template<typename U>
            bool operator==(const PoolAllocator<U, PoolSize>&) const { return true; }
            
            template<typename U>
            bool operator!=(const PoolAllocator<U, PoolSize>&) const { return false; }
            
            // 获取池统计信息
            static void printStats() {
                std::cout << "[Pool] 统计信息:" << std::endl;
                std::cout << "  池大小: " << PoolSize << " bytes" << std::endl;
                std::cout << "  已分配: " << allocated_bytes << " bytes" << std::endl;
                std::cout << "  剩余: " << (PoolSize - allocated_bytes) << " bytes" << std::endl;
            }
        };
        
        // 静态成员定义
        template<typename T, size_t PoolSize>
        char PoolAllocator<T, PoolSize>::memory_pool[PoolSize];
        
        template<typename T, size_t PoolSize>
        typename PoolAllocator<T, PoolSize>::FreeBlock* PoolAllocator<T, PoolSize>::free_list = nullptr;
        
        template<typename T, size_t PoolSize>
        size_t PoolAllocator<T, PoolSize>::allocated_bytes = 0;
        
        template<typename T, size_t PoolSize>
        bool PoolAllocator<T, PoolSize>::initialized = false;
        
        // 测试内存池分配器
        std::cout << "测试内存池分配器:" << std::endl;
        
        {
            std::vector<int, PoolAllocator<int, 1024>> pool_vector;
            
            std::cout << "\n插入元素到pool_vector:" << std::endl;
            for(int i = 1; i <= 10; ++i) {
                pool_vector.push_back(i * 10);
            }
            
            PoolAllocator<int, 1024>::printStats();
            
            std::cout << "\nvector内容: ";
            for(int x : pool_vector) {
                std::cout << x << " ";
            }
            std::cout << std::endl;
            
            std::cout << "\n清空vector:" << std::endl;
            pool_vector.clear();
            
            PoolAllocator<int, 1024>::printStats();
        }
        
        std::cout << "\nvector析构完成" << std::endl;
        PoolAllocator<int, 1024>::printStats();
    }
    
    // 2. 高级内存池分配器
    static void advancedMemoryPoolAllocator() {
        std::cout << "\n=== 高级内存池分配器 ===" << std::endl;
        
        // 支持多种大小的内存池分配器
        template<typename T>
        class AdvancedPoolAllocator {
        private:
            struct PoolBlock {
                size_t size;
                char* memory;
                std::vector<bool> used;
                size_t next_free;
                
                PoolBlock(size_t block_size, size_t count) 
                    : size(block_size), next_free(0) {
                    memory = static_cast<char*>(::operator new(block_size * count));
                    used.resize(count, false);
                }
                
                ~PoolBlock() {
                    ::operator delete(memory);
                }
                
                void* allocate() {
                    for(size_t i = next_free; i < used.size(); ++i) {
                        if(!used[i]) {
                            used[i] = true;
                            next_free = i + 1;
                            return memory + i * size;
                        }
                    }
                    
                    // 从头开始查找
                    for(size_t i = 0; i < next_free; ++i) {
                        if(!used[i]) {
                            used[i] = true;
                            next_free = i + 1;
                            return memory + i * size;
                        }
                    }
                    
                    return nullptr;  // 池已满
                }
                
                bool deallocate(void* ptr) {
                    char* p = static_cast<char*>(ptr);
                    if(p < memory || p >= memory + size * used.size()) {
                        return false;  // 不在此池中
                    }
                    
                    size_t index = (p - memory) / size;
                    if(index < used.size() && used[index]) {
                        used[index] = false;
                        if(index < next_free) {
                            next_free = index;
                        }
                        return true;
                    }
                    
                    return false;
                }
                
                size_t getUsedCount() const {
                    return std::count(used.begin(), used.end(), true);
                }
            };
            
            static std::vector<std::unique_ptr<PoolBlock>> pools;
            static std::mutex pool_mutex;
            
            static PoolBlock* findOrCreatePool(size_t size) {
                std::lock_guard<std::mutex> lock(pool_mutex);
                
                // 查找合适的池
                for(auto& pool : pools) {
                    if(pool->size >= size) {
                        return pool.get();
                    }
                }
                
                // 创建新池
                size_t block_size = std::max(size, sizeof(T));
                size_t block_count = 4096 / block_size;
                if(block_count == 0) block_count = 1;
                
                pools.push_back(std::make_unique<PoolBlock>(block_size, block_count));
                
                std::cout << "[AdvancedPool] 创建新池: " << block_size 
                          << " bytes/block, " << block_count << " blocks" << std::endl;
                
                return pools.back().get();
            }
            
        public:
            using value_type = T;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;
            
            template<typename U>
            struct rebind {
                using other = AdvancedPoolAllocator<U>;
            };
            
            AdvancedPoolAllocator() = default;
            
            template<typename U>
            AdvancedPoolAllocator(const AdvancedPoolAllocator<U>&) {}
            
            T* allocate(size_type n) {
                if(n == 1) {
                    PoolBlock* pool = findOrCreatePool(sizeof(T));
                    void* ptr = pool->allocate();
                    
                    if(ptr) {
                        std::cout << "[AdvancedPool] 池分配: " << sizeof(T) << " bytes" << std::endl;
                        return static_cast<T*>(ptr);
                    }
                }
                
                // 回退到标准分配
                std::cout << "[AdvancedPool] 标准分配: " << n * sizeof(T) << " bytes" << std::endl;
                return static_cast<T*>(::operator new(n * sizeof(T)));
            }
            
            void deallocate(T* p, size_type n) {
                if(!p) return;
                
                std::lock_guard<std::mutex> lock(pool_mutex);
                
                // 尝试在池中释放
                for(auto& pool : pools) {
                    if(pool->deallocate(p)) {
                        std::cout << "[AdvancedPool] 池释放: " << sizeof(T) << " bytes" << std::endl;
                        return;
                    }
                }
                
                // 标准释放
                std::cout << "[AdvancedPool] 标准释放: " << n * sizeof(T) << " bytes" << std::endl;
                ::operator delete(p);
            }
            
            template<typename U>
            bool operator==(const AdvancedPoolAllocator<U>&) const { return true; }
            
            template<typename U>
            bool operator!=(const AdvancedPoolAllocator<U>&) const { return false; }
            
            static void printPoolStats() {
                std::lock_guard<std::mutex> lock(pool_mutex);
                
                std::cout << "[AdvancedPool] 池统计:" << std::endl;
                for(size_t i = 0; i < pools.size(); ++i) {
                    auto& pool = pools[i];
                    std::cout << "  池" << i << ": " << pool->size << " bytes/block, "
                              << pool->getUsedCount() << "/" << pool->used.size() << " 已使用" << std::endl;
                }
            }
        };
        
        // 静态成员定义
        template<typename T>
        std::vector<std::unique_ptr<typename AdvancedPoolAllocator<T>::PoolBlock>> 
        AdvancedPoolAllocator<T>::pools;
        
        template<typename T>
        std::mutex AdvancedPoolAllocator<T>::pool_mutex;
        
        // 测试高级内存池分配器
        std::cout << "测试高级内存池分配器:" << std::endl;
        
        {
            std::vector<std::string, AdvancedPoolAllocator<std::string>> string_vector;
            std::vector<double, AdvancedPoolAllocator<double>> double_vector;
            
            std::cout << "\n分配不同类型的对象:" << std::endl;
            
            for(int i = 0; i < 5; ++i) {
                string_vector.push_back("string_" + std::to_string(i));
                double_vector.push_back(i * 3.14);
            }
            
            AdvancedPoolAllocator<std::string>::printPoolStats();
            
            std::cout << "\nstring_vector: ";
            for(const auto& s : string_vector) {
                std::cout << s << " ";
            }
            std::cout << std::endl;
            
            std::cout << "double_vector: ";
            for(double d : double_vector) {
                std::cout << d << " ";
            }
            std::cout << std::endl;
        }
        
        std::cout << "\n容器析构后:" << std::endl;
        AdvancedPoolAllocator<std::string>::printPoolStats();
    }
};
```

### 🛡️ 调试分配器

```cpp
class DebuggingAllocatorMasterclass {
public:
    // 1. 内存泄漏检测分配器
    static void memoryLeakDetectionAllocator() {
        std::cout << "\n=== 内存泄漏检测分配器 ===" << std::endl;
        
        template<typename T>
        class LeakDetectionAllocator {
        private:
            struct AllocationRecord {
                size_t size;
                std::string file;
                int line;
                std::chrono::steady_clock::time_point timestamp;
                
                AllocationRecord(size_t s, const std::string& f, int l)
                    : size(s), file(f), line(l), timestamp(std::chrono::steady_clock::now()) {}
            };
            
            static std::map<void*, AllocationRecord> allocations;
            static std::mutex allocation_mutex;
            static size_t total_allocated;
            static size_t total_deallocated;
            static size_t peak_memory;
            
        public:
            using value_type = T;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;
            
            template<typename U>
            struct rebind {
                using other = LeakDetectionAllocator<U>;
            };
            
            LeakDetectionAllocator() = default;
            
            template<typename U>
            LeakDetectionAllocator(const LeakDetectionAllocator<U>&) {}
            
            T* allocate(size_type n, const std::string& file = __FILE__, int line = __LINE__) {
                size_t bytes = n * sizeof(T);
                T* ptr = static_cast<T*>(::operator new(bytes));
                
                std::lock_guard<std::mutex> lock(allocation_mutex);
                
                allocations.emplace(ptr, AllocationRecord(bytes, file, line));
                total_allocated += bytes;
                
                if(total_allocated - total_deallocated > peak_memory) {
                    peak_memory = total_allocated - total_deallocated;
                }
                
                std::cout << "[LeakDetect] 分配: " << bytes << " bytes at " << ptr 
                          << " (" << file << ":" << line << ")" << std::endl;
                
                return ptr;
            }
            
            void deallocate(T* p, size_type n) {
                if(!p) return;
                
                std::lock_guard<std::mutex> lock(allocation_mutex);
                
                auto it = allocations.find(p);
                if(it != allocations.end()) {
                    size_t bytes = it->second.size;
                    total_deallocated += bytes;
                    
                    std::cout << "[LeakDetect] 释放: " << bytes << " bytes at " << p 
                              << " (存活时间: " 
                              << std::chrono::duration_cast<std::chrono::milliseconds>(
                                     std::chrono::steady_clock::now() - it->second.timestamp).count()
                              << "ms)" << std::endl;
                    
                    allocations.erase(it);
                } else {
                    std::cout << "[LeakDetect] 警告: 释放未记录的内存 " << p << std::endl;
                }
                
                ::operator delete(p);
            }
            
            template<typename U>
            bool operator==(const LeakDetectionAllocator<U>&) const { return true; }
            
            template<typename U>
            bool operator!=(const LeakDetectionAllocator<U>&) const { return false; }
            
            static void printLeakReport() {
                std::lock_guard<std::mutex> lock(allocation_mutex);
                
                std::cout << "\n[LeakDetect] 内存报告:" << std::endl;
                std::cout << "总分配: " << total_allocated << " bytes" << std::endl;
                std::cout << "总释放: " << total_deallocated << " bytes" << std::endl;
                std::cout << "峰值使用: " << peak_memory << " bytes" << std::endl;
                std::cout << "当前泄漏: " << (total_allocated - total_deallocated) << " bytes" << std::endl;
                
                if(!allocations.empty()) {
                    std::cout << "\n未释放的分配:" << std::endl;
                    for(const auto& [ptr, record] : allocations) {
                        std::cout << "  " << ptr << ": " << record.size << " bytes ("
                                  << record.file << ":" << record.line << ")" << std::endl;
                    }
                } else {
                    std::cout << "✓ 无内存泄漏检测到!" << std::endl;
                }
            }
        };
        
        // 静态成员定义
        template<typename T>
        std::map<void*, typename LeakDetectionAllocator<T>::AllocationRecord> 
        LeakDetectionAllocator<T>::allocations;
        
        template<typename T>
        std::mutex LeakDetectionAllocator<T>::allocation_mutex;
        
        template<typename T>
        size_t LeakDetectionAllocator<T>::total_allocated = 0;
        
        template<typename T>
        size_t LeakDetectionAllocator<T>::total_deallocated = 0;
        
        template<typename T>
        size_t LeakDetectionAllocator<T>::peak_memory = 0;
        
        // 测试内存泄漏检测
        std::cout << "测试内存泄漏检测分配器:" << std::endl;
        
        {
            std::vector<int, LeakDetectionAllocator<int>> test_vector;
            
            for(int i = 0; i < 5; ++i) {
                test_vector.push_back(i * 10);
            }
            
            LeakDetectionAllocator<int>::printLeakReport();
            
            // 模拟内存泄漏
            LeakDetectionAllocator<int> leak_alloc;
            int* leaked_memory = leak_alloc.allocate(10);  // 故意不释放
            
            std::cout << "\nvector内容: ";
            for(int x : test_vector) {
                std::cout << x << " ";
            }
            std::cout << std::endl;
        }
        
        std::cout << "\nvector析构后的泄漏检测:" << std::endl;
        LeakDetectionAllocator<int>::printLeakReport();
    }
    
    // 2. 边界检查分配器
    static void boundaryCheckingAllocator() {
        std::cout << "\n=== 边界检查分配器 ===" << std::endl;
        
        template<typename T>
        class BoundaryCheckAllocator {
        private:
            static constexpr size_t GUARD_SIZE = 16;
            static constexpr uint8_t GUARD_PATTERN = 0xAB;
            static constexpr uint8_t FREED_PATTERN = 0xDE;
            
            struct AllocationHeader {
                size_t size;
                uint32_t magic;
                static constexpr uint32_t MAGIC_VALUE = 0x12345678;
            };
            
            static void setGuardPattern(void* ptr, size_t size) {
                std::memset(ptr, GUARD_PATTERN, size);
            }
            
            static bool checkGuardPattern(void* ptr, size_t size) {
                uint8_t* bytes = static_cast<uint8_t*>(ptr);
                for(size_t i = 0; i < size; ++i) {
                    if(bytes[i] != GUARD_PATTERN) {
                        return false;
                    }
                }
                return true;
            }
            
        public:
            using value_type = T;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;
            
            template<typename U>
            struct rebind {
                using other = BoundaryCheckAllocator<U>;
            };
            
            BoundaryCheckAllocator() = default;
            
            template<typename U>
            BoundaryCheckAllocator(const BoundaryCheckAllocator<U>&) {}
            
            T* allocate(size_type n) {
                size_t total_size = sizeof(AllocationHeader) + n * sizeof(T) + 2 * GUARD_SIZE;
                char* raw_memory = static_cast<char*>(::operator new(total_size));
                
                // 设置头部
                AllocationHeader* header = reinterpret_cast<AllocationHeader*>(raw_memory);
                header->size = n * sizeof(T);
                header->magic = AllocationHeader::MAGIC_VALUE;
                
                // 设置前后保护区
                char* front_guard = raw_memory + sizeof(AllocationHeader);
                char* user_memory = front_guard + GUARD_SIZE;
                char* back_guard = user_memory + n * sizeof(T);
                
                setGuardPattern(front_guard, GUARD_SIZE);
                setGuardPattern(back_guard, GUARD_SIZE);
                
                std::cout << "[BoundaryCheck] 分配: " << n * sizeof(T) << " bytes with guards" << std::endl;
                
                return reinterpret_cast<T*>(user_memory);
            }
            
            void deallocate(T* p, size_type n) {
                if(!p) return;
                
                char* user_memory = reinterpret_cast<char*>(p);
                char* front_guard = user_memory - GUARD_SIZE;
                char* raw_memory = front_guard - sizeof(AllocationHeader);
                
                // 检查头部
                AllocationHeader* header = reinterpret_cast<AllocationHeader*>(raw_memory);
                if(header->magic != AllocationHeader::MAGIC_VALUE) {
                    std::cout << "[BoundaryCheck] 错误: 损坏的分配头部!" << std::endl;
                    return;
                }
                
                // 检查保护区
                bool front_ok = checkGuardPattern(front_guard, GUARD_SIZE);
                bool back_ok = checkGuardPattern(user_memory + header->size, GUARD_SIZE);
                
                if(!front_ok) {
                    std::cout << "[BoundaryCheck] 错误: 前保护区被破坏!" << std::endl;
                }
                
                if(!back_ok) {
                    std::cout << "[BoundaryCheck] 错误: 后保护区被破坏!" << std::endl;
                }
                
                if(front_ok && back_ok) {
                    std::cout << "[BoundaryCheck] 释放: " << header->size << " bytes (保护区完整)" << std::endl;
                } else {
                    std::cout << "[BoundaryCheck] 释放: " << header->size << " bytes (检测到越界!)" << std::endl;
                }
                
                // 用特殊模式填充已释放的内存
                std::memset(user_memory, FREED_PATTERN, header->size);
                
                ::operator delete(raw_memory);
            }
            
            template<typename U>
            bool operator==(const BoundaryCheckAllocator<U>&) const { return true; }
            
            template<typename U>
            bool operator!=(const BoundaryCheckAllocator<U>&) const { return false; }
        };
        
        // 测试边界检查分配器
        std::cout << "测试边界检查分配器:" << std::endl;
        
        BoundaryCheckAllocator<int> boundary_alloc;
        
        // 正常使用
        std::cout << "\n正常使用测试:" << std::endl;
        int* normal_ptr = boundary_alloc.allocate(5);
        for(int i = 0; i < 5; ++i) {
            normal_ptr[i] = i * 10;
        }
        
        std::cout << "正常数据: ";
        for(int i = 0; i < 5; ++i) {
            std::cout << normal_ptr[i] << " ";
        }
        std::cout << std::endl;
        
        boundary_alloc.deallocate(normal_ptr, 5);
        
        // 模拟越界写入（注意：这是不安全的，仅用于演示）
        std::cout << "\n越界写入测试:" << std::endl;
        int* overflow_ptr = boundary_alloc.allocate(3);
        
        for(int i = 0; i < 3; ++i) {
            overflow_ptr[i] = i;
        }
        
        // 故意越界写入（破坏后保护区）
        // 注意：这是危险操作，仅用于演示边界检查功能
        volatile char* dangerous_ptr = reinterpret_cast<char*>(overflow_ptr + 3);
        *dangerous_ptr = 0xFF;  // 破坏保护区
        
        boundary_alloc.deallocate(overflow_ptr, 3);  // 应该检测到越界
    }
};
```

现在让我更新TODO并创建仿函数的终极教程：

<function_calls>
<invoke name="todo_write">
<parameter name="merge">true

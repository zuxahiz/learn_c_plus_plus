# C++ STL 分配器详解

## 🎯 什么是分配器？

分配器(Allocator)是STL中负责内存分配和释放的组件，它封装了内存管理的细节，使得容器可以独立于具体的内存分配策略。分配器提供了统一的内存分配接口，支持自定义内存管理策略。

## 🔧 分配器的作用

### 1. 核心功能
- **内存分配**: 分配指定大小的内存块
- **内存释放**: 释放已分配的内存
- **内存构造**: 在分配的内存上构造对象
- **内存销毁**: 销毁对象并释放内存

### 2. 设计优势
- **解耦**: 容器与内存管理策略分离
- **定制**: 支持自定义内存分配策略
- **效率**: 可以针对特定场景优化内存管理
- **调试**: 便于内存泄漏检测和性能分析

## 📚 标准分配器

### 1. std::allocator
```cpp
#include <memory>
#include <vector>
using namespace std;

// 使用默认分配器
vector<int> v1;                           // 使用std::allocator<int>

// 显式指定分配器
vector<int, allocator<int>> v2;           // 等价于v1

// 分配器基本操作
allocator<int> alloc;
int* ptr = alloc.allocate(10);            // 分配10个int的空间

// 在分配的内存上构造对象
for (int i = 0; i < 10; ++i) {
    alloc.construct(ptr + i, i);          // 构造对象
}

// 销毁对象
for (int i = 0; i < 10; ++i) {
    alloc.destroy(ptr + i);               // 销毁对象
}

// 释放内存
alloc.deallocate(ptr, 10);
```

### 2. 分配器接口
```cpp
template<typename T>
class MyAllocator {
public:
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    
    // 分配内存
    pointer allocate(size_type n) {
        return static_cast<pointer>(::operator new(n * sizeof(T)));
    }
    
    // 释放内存
    void deallocate(pointer p, size_type n) noexcept {
        ::operator delete(p);
    }
    
    // 构造对象
    template<typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        ::new(p) U(std::forward<Args>(args)...);
    }
    
    // 销毁对象
    template<typename U>
    void destroy(U* p) {
        p->~U();
    }
    
    // 获取最大分配大小
    size_type max_size() const noexcept {
        return size_type(-1) / sizeof(T);
    }
};
```

## 📚 自定义分配器

### 1. 池分配器 (Pool Allocator)
```cpp
#include <vector>
#include <cstddef>
#include <new>

template<typename T, size_t BlockSize = 1024>
class PoolAllocator {
private:
    struct Block {
        Block* next;
        char data[BlockSize];
    };
    
    struct FreeNode {
        FreeNode* next;
    };
    
    Block* currentBlock;
    FreeNode* freeList;
    size_t currentPos;
    
public:
    using value_type = T;
    using size_type = size_t;
    using pointer = T*;
    
    PoolAllocator() : currentBlock(nullptr), freeList(nullptr), currentPos(0) {}
    
    ~PoolAllocator() {
        while (currentBlock) {
            Block* next = currentBlock->next;
            delete currentBlock;
            currentBlock = next;
        }
    }
    
    pointer allocate(size_type n) {
        if (n != 1) {
            // 对于非单个对象，使用标准分配器
            return static_cast<pointer>(::operator new(n * sizeof(T)));
        }
        
        // 检查空闲列表
        if (freeList) {
            pointer result = reinterpret_cast<pointer>(freeList);
            freeList = freeList->next;
            return result;
        }
        
        // 分配新块
        if (currentPos >= BlockSize / sizeof(T)) {
            Block* newBlock = new Block;
            newBlock->next = currentBlock;
            currentBlock = newBlock;
            currentPos = 0;
        }
        
        return reinterpret_cast<pointer>(&currentBlock->data[currentPos++ * sizeof(T)]);
    }
    
    void deallocate(pointer p, size_type n) noexcept {
        if (n != 1) {
            ::operator delete(p);
            return;
        }
        
        // 将释放的内存加入空闲列表
        FreeNode* node = reinterpret_cast<FreeNode*>(p);
        node->next = freeList;
        freeList = node;
    }
    
    template<typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        ::new(p) U(std::forward<Args>(args)...);
    }
    
    template<typename U>
    void destroy(U* p) {
        p->~U();
    }
};

// 使用示例
vector<int, PoolAllocator<int>> v;
for (int i = 0; i < 1000; ++i) {
    v.push_back(i);
}
```

### 2. 调试分配器 (Debug Allocator)
```cpp
#include <iostream>
#include <map>
#include <cassert>

template<typename T>
class DebugAllocator {
private:
    static std::map<void*, size_t> allocatedBlocks;
    static size_t totalAllocated;
    
public:
    using value_type = T;
    using size_type = size_t;
    using pointer = T*;
    
    pointer allocate(size_type n) {
        size_t bytes = n * sizeof(T);
        pointer p = static_cast<pointer>(::operator new(bytes));
        
        allocatedBlocks[p] = bytes;
        totalAllocated += bytes;
        
        std::cout << "Allocated " << bytes << " bytes at " << p 
                  << " (total: " << totalAllocated << ")" << std::endl;
        
        return p;
    }
    
    void deallocate(pointer p, size_type n) noexcept {
        size_t bytes = n * sizeof(T);
        
        auto it = allocatedBlocks.find(p);
        if (it != allocatedBlocks.end()) {
            allocatedBlocks.erase(it);
            totalAllocated -= bytes;
            
            std::cout << "Deallocated " << bytes << " bytes at " << p 
                      << " (total: " << totalAllocated << ")" << std::endl;
        } else {
            std::cerr << "Attempting to deallocate unallocated memory at " << p << std::endl;
            assert(false);
        }
        
        ::operator delete(p);
    }
    
    template<typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        ::new(p) U(std::forward<Args>(args)...);
    }
    
    template<typename U>
    void destroy(U* p) {
        p->~U();
    }
    
    // 静态方法
    static void printStats() {
        std::cout << "Allocated blocks: " << allocatedBlocks.size() << std::endl;
        std::cout << "Total allocated: " << totalAllocated << " bytes" << std::endl;
    }
    
    static void checkLeaks() {
        if (!allocatedBlocks.empty()) {
            std::cerr << "Memory leak detected! " << allocatedBlocks.size() 
                      << " blocks not deallocated" << std::endl;
            for (const auto& [ptr, size] : allocatedBlocks) {
                std::cerr << "  " << ptr << ": " << size << " bytes" << std::endl;
            }
        }
    }
};

// 静态成员定义
template<typename T>
std::map<void*, size_t> DebugAllocator<T>::allocatedBlocks;

template<typename T>
size_t DebugAllocator<T>::totalAllocated = 0;

// 使用示例
void testDebugAllocator() {
    {
        vector<int, DebugAllocator<int>> v;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
        
        DebugAllocator<int>::printStats();
    } // 作用域结束，v被销毁
    
    DebugAllocator<int>::checkLeaks();
}
```

### 3. 对齐分配器 (Aligned Allocator)
```cpp
#include <cstddef>
#include <cstdint>

template<typename T, size_t Alignment = alignof(T)>
class AlignedAllocator {
public:
    using value_type = T;
    using size_type = size_t;
    using pointer = T*;
    
    pointer allocate(size_type n) {
        size_t bytes = n * sizeof(T);
        size_t alignedBytes = (bytes + Alignment - 1) & ~(Alignment - 1);
        
        void* ptr = nullptr;
        if (posix_memalign(&ptr, Alignment, alignedBytes) != 0) {
            throw std::bad_alloc();
        }
        
        return static_cast<pointer>(ptr);
    }
    
    void deallocate(pointer p, size_type) noexcept {
        free(p);
    }
    
    template<typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        ::new(p) U(std::forward<Args>(args)...);
    }
    
    template<typename U>
    void destroy(U* p) {
        p->~U();
    }
    
    // 获取对齐要求
    static constexpr size_t alignment() noexcept {
        return Alignment;
    }
};

// 使用示例
vector<int, AlignedAllocator<int, 64>> alignedVector;  // 64字节对齐
```

## 📚 分配器特性

### 1. 分配器传播
```cpp
#include <type_traits>

template<typename T, typename Alloc>
class MyContainer {
private:
    Alloc alloc;
    
public:
    // 分配器传播类型
    using allocator_type = Alloc;
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
    
    // 分配器传播构造函数
    template<typename UAlloc>
    MyContainer(const MyContainer<T, UAlloc>& other, const Alloc& a)
        : alloc(a) {
        // 复制构造逻辑
    }
    
    // 分配器传播赋值操作符
    MyContainer& operator=(const MyContainer& other) {
        if (std::allocator_traits<Alloc>::propagate_on_container_copy_assignment::value) {
            alloc = other.alloc;
        }
        // 赋值逻辑
        return *this;
    }
};
```

### 2. 分配器比较
```cpp
template<typename T>
class MyAllocator {
public:
    // 分配器比较
    template<typename U>
    bool operator==(const MyAllocator<U>&) const noexcept {
        return true;  // 所有实例都相等
    }
    
    template<typename U>
    bool operator!=(const MyAllocator<U>& other) const noexcept {
        return !(*this == other);
    }
    
    // 分配器选择
    template<typename U>
    struct rebind {
        using other = MyAllocator<U>;
    };
};
```

## 💡 分配器使用技巧

### 1. 选择合适的分配器
```cpp
// 标准场景：使用默认分配器
vector<int> standardVector;

// 内存池场景：使用池分配器
vector<int, PoolAllocator<int>> poolVector;

// 调试场景：使用调试分配器
vector<int, DebugAllocator<int>> debugVector;

// 性能关键场景：使用对齐分配器
vector<int, AlignedAllocator<int, 64>> alignedVector;
```

### 2. 分配器性能优化
```cpp
// 预分配空间避免频繁分配
vector<int> v;
v.reserve(1000);                          // 预分配1000个元素的空间

// 使用自定义分配器优化特定场景
template<typename T>
class FastAllocator {
    // 针对频繁分配释放的场景优化
    // 使用内存池或对象池
};

vector<int, FastAllocator<int>> fastVector;
```

### 3. 分配器调试和监控
```cpp
// 使用调试分配器监控内存使用
void monitorMemoryUsage() {
    DebugAllocator<int>::printStats();
    
    // 在程序结束时检查内存泄漏
    std::atexit([]() {
        DebugAllocator<int>::checkLeaks();
    });
}
```

## 🎯 分配器应用场景

### 1. 内存池管理
```cpp
// 对象池分配器
template<typename T, size_t PoolSize = 1000>
class ObjectPoolAllocator {
    // 预分配对象池
    // 快速分配和释放
    // 减少内存碎片
};

// 使用对象池
vector<MyClass, ObjectPoolAllocator<MyClass>> objectVector;
```

### 2. 性能优化
```cpp
// 缓存友好的分配器
template<typename T>
class CacheFriendlyAllocator {
    // 分配连续内存块
    // 减少缓存未命中
    // 提高访问性能
};

// 使用缓存友好分配器
vector<int, CacheFriendlyAllocator<int>> cacheFriendlyVector;
```

### 3. 内存监控
```cpp
// 内存使用统计分配器
template<typename T>
class StatisticsAllocator {
    // 记录分配次数和大小
    // 监控内存使用模式
    // 提供性能分析数据
};

// 使用统计分配器
vector<int, StatisticsAllocator<int>> monitoredVector;
```

## ⚡ 性能考虑

### 1. 分配器开销
- **标准分配器**: 通用但有一定开销
- **池分配器**: 快速但内存利用率可能较低
- **自定义分配器**: 针对特定场景优化

### 2. 内存碎片
- **连续分配**: 减少内存碎片
- **块分配**: 平衡性能和碎片
- **智能合并**: 自动合并相邻空闲块

### 3. 缓存性能
- **内存局部性**: 提高缓存命中率
- **预取优化**: 减少缓存未命中
- **对齐访问**: 优化内存访问模式

## 🚀 下一步
- 学习 [仿函数详解](./functors-guide.md)
- 了解 [算法详解](./algorithms-guide.md)
- 掌握 [容器详解](./containers-guide.md)

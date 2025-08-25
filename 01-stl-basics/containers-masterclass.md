# C++ STL 容器大师级教程

## 🎯 教程目标

这是一份**世界级标准**的C++ STL容器教程，将为您提供：
- 🔬 **深度理论分析** - 每个容器的内部实现原理
- 💻 **完整代码实战** - 工业级别的使用示例
- ⚡ **性能基准测试** - 详细的性能对比和选择指导
- 🎯 **实际应用场景** - 真实项目中的最佳实践
- 🚀 **高级优化技巧** - 专家级的使用技巧

---

## 📊 STL容器生态系统概览

```
STL容器架构
├── 序列容器 (Sequence Containers)
│   ├── vector          ← 动态数组，连续内存
│   ├── deque           ← 双端队列，分段连续
│   ├── list            ← 双向链表，非连续
│   ├── forward_list    ← 单向链表，内存优化
│   └── array           ← 固定数组，编译时大小
│
├── 关联容器 (Associative Containers) - 基于红黑树
│   ├── set             ← 有序唯一集合
│   ├── multiset        ← 有序可重复集合  
│   ├── map             ← 有序键值对，唯一键
│   └── multimap        ← 有序键值对，重复键
│
├── 无序关联容器 (Unordered Associative) - 基于哈希表
│   ├── unordered_set   ← 哈希集合，唯一
│   ├── unordered_multiset ← 哈希集合，重复
│   ├── unordered_map   ← 哈希映射，唯一键
│   └── unordered_multimap ← 哈希映射，重复键
│
└── 容器适配器 (Container Adapters)
    ├── stack           ← 栈，LIFO
    ├── queue           ← 队列，FIFO
    └── priority_queue  ← 优先级队列，堆
```

---

## 🚀 序列容器深度解析

### Vector - 动态数组的艺术

#### 🔬 内部实现原理

Vector是基于**连续内存块**的动态数组，具有以下特征：
- **内存布局**: 连续的内存地址，类似C语言数组
- **扩容策略**: 通常按2倍增长（GCC实现），保证摊销O(1)插入
- **迭代器类型**: 随机访问迭代器，支持所有STL算法

```cpp
// Vector内部结构示意（简化版）
template<typename T>
class vector {
private:
    T* data;        // 指向数据的指针
    size_t size_;   // 当前元素数量
    size_t capacity_; // 当前容量
    
public:
    // 核心操作的复杂度
    // 访问: O(1)
    // 尾部插入: 摊销O(1)
    // 中间插入: O(n)
    // 删除: O(n)
};
```

#### 💻 完整使用指南

```cpp
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <memory>

class VectorMasterclass {
public:
    // 1. 构造函数详解
    static void constructorDemo() {
        std::cout << "=== Vector构造函数全解析 ===" << std::endl;
        
        // 默认构造
        std::vector<int> v1;
        std::cout << "v1 size: " << v1.size() << ", capacity: " << v1.capacity() << std::endl;
        
        // 预分配大小
        std::vector<int> v2(100);  // 100个0
        std::cout << "v2 size: " << v2.size() << ", capacity: " << v2.capacity() << std::endl;
        
        // 预分配大小并初始化
        std::vector<int> v3(50, 42);  // 50个42
        std::cout << "v3[0]: " << v3[0] << ", v3[49]: " << v3[49] << std::endl;
        
        // 列表初始化 (C++11)
        std::vector<int> v4 = {1, 2, 3, 4, 5};
        std::cout << "v4: ";
        for(int x : v4) std::cout << x << " ";
        std::cout << std::endl;
        
        // 范围构造
        std::vector<int> v5(v4.begin(), v4.end());
        std::cout << "v5 (copy of v4): ";
        for(int x : v5) std::cout << x << " ";
        std::cout << std::endl;
        
        // 移动构造 (C++11)
        std::vector<int> v6 = std::move(v5);
        std::cout << "v6 (moved from v5): ";
        for(int x : v6) std::cout << x << " ";
        std::cout << std::endl;
        std::cout << "v5 after move: size = " << v5.size() << std::endl;
    }
    
    // 2. 容量管理精讲
    static void capacityManagement() {
        std::cout << "\n=== Vector容量管理精讲 ===" << std::endl;
        
        std::vector<int> v;
        
        // 观察扩容过程
        std::cout << "扩容过程观察:" << std::endl;
        for(int i = 0; i < 20; ++i) {
            auto old_capacity = v.capacity();
            v.push_back(i);
            if(v.capacity() != old_capacity) {
                std::cout << "扩容: " << old_capacity << " -> " << v.capacity() 
                         << " (增长率: " << (double)v.capacity() / old_capacity << ")" << std::endl;
            }
        }
        
        // reserve vs resize 的区别
        std::vector<int> v1, v2;
        
        v1.reserve(1000);  // 只分配内存，不构造对象
        std::cout << "\nreserve(1000) -> size: " << v1.size() << ", capacity: " << v1.capacity() << std::endl;
        
        v2.resize(1000);   // 分配内存并构造对象
        std::cout << "resize(1000) -> size: " << v2.size() << ", capacity: " << v2.capacity() << std::endl;
        
        // shrink_to_fit - 减少内存使用
        v2.clear();
        std::cout << "clear() -> size: " << v2.size() << ", capacity: " << v2.capacity() << std::endl;
        
        v2.shrink_to_fit();
        std::cout << "shrink_to_fit() -> size: " << v2.size() << ", capacity: " << v2.capacity() << std::endl;
    }
    
    // 3. 插入删除操作详解
    static void insertDeleteOperations() {
        std::cout << "\n=== Vector插入删除操作详解 ===" << std::endl;
        
        std::vector<int> v = {1, 2, 3, 4, 5};
        
        // 各种插入方式
        v.push_back(6);                              // 尾部插入
        v.insert(v.begin(), 0);                      // 头部插入
        v.insert(v.begin() + 3, 99);                 // 中间插入
        v.insert(v.end(), {7, 8, 9});               // 尾部批量插入
        
        std::cout << "插入后: ";
        for(int x : v) std::cout << x << " ";
        std::cout << std::endl;
        
        // emplace 系列 - 原地构造
        std::vector<std::pair<int, std::string>> pairs;
        pairs.emplace_back(1, "one");               // 直接构造，避免临时对象
        pairs.push_back({2, "two"});                // 先构造临时对象再移动
        
        // 删除操作
        v.pop_back();                               // 删除尾部
        v.erase(v.begin());                         // 删除头部
        v.erase(v.begin() + 2, v.begin() + 4);     // 删除范围
        
        std::cout << "删除后: ";
        for(int x : v) std::cout << x << " ";
        std::cout << std::endl;
        
        // 条件删除
        v.erase(std::remove_if(v.begin(), v.end(), 
                              [](int x) { return x > 5; }), v.end());
        
        std::cout << "条件删除后: ";
        for(int x : v) std::cout << x << " ";
        std::cout << std::endl;
    }
    
    // 4. 访问元素和迭代器
    static void accessAndIterators() {
        std::cout << "\n=== Vector访问和迭代器 ===" << std::endl;
        
        std::vector<int> v = {10, 20, 30, 40, 50};
        
        // 各种访问方式
        std::cout << "operator[]: " << v[2] << std::endl;        // 不检查边界
        std::cout << "at(): " << v.at(2) << std::endl;          // 检查边界
        std::cout << "front(): " << v.front() << std::endl;     // 第一个元素
        std::cout << "back(): " << v.back() << std::endl;       // 最后一个元素
        std::cout << "data(): " << *v.data() << std::endl;      // 底层数组指针
        
        // 边界检查演示
        try {
            std::cout << v.at(100);  // 抛出异常
        } catch(const std::out_of_range& e) {
            std::cout << "边界检查异常: " << e.what() << std::endl;
        }
        
        // 迭代器类型演示
        std::vector<int>::iterator it = v.begin();
        std::vector<int>::const_iterator cit = v.cbegin();
        std::vector<int>::reverse_iterator rit = v.rbegin();
        
        // 随机访问迭代器特性
        std::cout << "随机访问: " << *(v.begin() + 2) << std::endl;
        std::cout << "迭代器算术: " << (v.end() - v.begin()) << std::endl;
        std::cout << "迭代器比较: " << (v.begin() < v.end()) << std::endl;
        
        // 范围for循环 (C++11)
        std::cout << "范围for: ";
        for(const auto& element : v) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }
    
    // 5. 性能特性和基准测试
    static void performanceAnalysis() {
        std::cout << "\n=== Vector性能分析 ===" << std::endl;
        
        const size_t N = 1000000;
        
        // 测试1: 预分配 vs 动态扩容
        auto start = std::chrono::high_resolution_clock::now();
        
        std::vector<int> v1;
        for(size_t i = 0; i < N; ++i) {
            v1.push_back(i);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        start = std::chrono::high_resolution_clock::now();
        
        std::vector<int> v2;
        v2.reserve(N);  // 预分配
        for(size_t i = 0; i < N; ++i) {
            v2.push_back(i);
        }
        
        end = std::chrono::high_resolution_clock::now();
        auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "无预分配: " << duration1.count() << "μs" << std::endl;
        std::cout << "预分配: " << duration2.count() << "μs" << std::endl;
        std::cout << "性能提升: " << (double)duration1.count() / duration2.count() << "倍" << std::endl;
        
        // 测试2: 不同访问方式的性能
        std::vector<int> v3(N);
        std::iota(v3.begin(), v3.end(), 0);
        
        // operator[] 访问
        start = std::chrono::high_resolution_clock::now();
        long long sum1 = 0;
        for(size_t i = 0; i < N; ++i) {
            sum1 += v3[i];
        }
        end = std::chrono::high_resolution_clock::now();
        auto duration3 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // 迭代器访问
        start = std::chrono::high_resolution_clock::now();
        long long sum2 = 0;
        for(auto it = v3.begin(); it != v3.end(); ++it) {
            sum2 += *it;
        }
        end = std::chrono::high_resolution_clock::now();
        auto duration4 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // 范围for访问
        start = std::chrono::high_resolution_clock::now();
        long long sum3 = 0;
        for(const auto& element : v3) {
            sum3 += element;
        }
        end = std::chrono::high_resolution_clock::now();
        auto duration5 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "\n访问方式性能对比:" << std::endl;
        std::cout << "operator[]: " << duration3.count() << "μs" << std::endl;
        std::cout << "迭代器: " << duration4.count() << "μs" << std::endl;
        std::cout << "范围for: " << duration5.count() << "μs" << std::endl;
    }
};
```

#### 🎯 Vector最佳实践

```cpp
class VectorBestPractices {
public:
    // 1. 内存管理最佳实践
    static void memoryBestPractices() {
        std::cout << "\n=== Vector内存管理最佳实践 ===" << std::endl;
        
        // ✅ 好的做法: 预分配内存
        std::vector<int> good_vector;
        good_vector.reserve(1000);  // 如果知道大概大小，提前分配
        
        // ❌ 避免: 频繁扩容
        std::vector<int> bad_vector;
        // 没有预分配，频繁触发扩容
        
        // ✅ 好的做法: 使用emplace避免不必要的拷贝
        std::vector<std::pair<int, std::string>> pairs;
        pairs.emplace_back(1, "one");  // 直接构造
        
        // ❌ 避免: 不必要的临时对象
        // pairs.push_back(std::pair<int, std::string>(1, "one"));
        
        // ✅ 好的做法: 使用移动语义
        std::vector<std::vector<int>> matrix;
        std::vector<int> row = {1, 2, 3, 4, 5};
        matrix.push_back(std::move(row));  // 移动而不是拷贝
        
        // ✅ 好的做法: 及时释放内存
        std::vector<int> large_vector(1000000);
        // 使用完毕后
        large_vector.clear();
        large_vector.shrink_to_fit();  // 释放内存
    }
    
    // 2. 异常安全
    static void exceptionSafety() {
        std::cout << "\n=== Vector异常安全 ===" << std::endl;
        
        class ThrowingClass {
        public:
            static int instance_count;
            int value;
            
            ThrowingClass(int v) : value(v) {
                ++instance_count;
                if(instance_count > 5) {
                    throw std::runtime_error("Too many instances");
                }
            }
            
            ~ThrowingClass() { --instance_count; }
            
            // 拷贝构造可能抛异常
            ThrowingClass(const ThrowingClass& other) : value(other.value) {
                ++instance_count;
                if(instance_count > 5) {
                    throw std::runtime_error("Copy failed");
                }
            }
        };
        
        try {
            std::vector<ThrowingClass> v;
            for(int i = 0; i < 10; ++i) {
                v.emplace_back(i);  // 可能抛异常
            }
        } catch(const std::exception& e) {
            std::cout << "异常捕获: " << e.what() << std::endl;
            std::cout << "剩余实例: " << ThrowingClass::instance_count << std::endl;
        }
        
        // Vector提供强异常安全保证
        // 如果操作失败，vector保持原状态
    }
    
    // 3. 性能优化技巧
    static void performanceOptimization() {
        std::cout << "\n=== Vector性能优化技巧 ===" << std::endl;
        
        // 技巧1: 使用reserve避免重复分配
        std::vector<int> v1;
        v1.reserve(1000);  // 一次性分配足够内存
        
        // 技巧2: 使用resize而不是循环push_back（如果知道确切大小）
        std::vector<int> v2(1000);  // 直接创建1000个元素
        std::iota(v2.begin(), v2.end(), 0);
        
        // 技巧3: 使用swap trick释放内存
        std::vector<int> v3(1000000);
        // 使用后想释放内存
        std::vector<int>().swap(v3);  // 或者 v3.shrink_to_fit()
        
        // 技巧4: 批量操作而不是单个操作
        std::vector<int> source = {1, 2, 3, 4, 5};
        std::vector<int> target;
        
        // ✅ 好的做法: 批量插入
        target.insert(target.end(), source.begin(), source.end());
        
        // ❌ 避免: 循环单个插入
        // for(int x : source) target.push_back(x);
        
        // 技巧5: 预计算capacity避免重复扩容
        std::vector<int> result;
        std::vector<std::vector<int>> inputs = {{1,2}, {3,4,5}, {6,7,8,9}};
        
        size_t total_size = 0;
        for(const auto& input : inputs) {
            total_size += input.size();
        }
        result.reserve(total_size);
        
        for(const auto& input : inputs) {
            result.insert(result.end(), input.begin(), input.end());
        }
    }
};

// 静态成员定义
int VectorBestPractices::ThrowingClass::instance_count = 0;
```

### Array - 编译时固定数组

#### 🔬 内部实现原理

```cpp
#include <array>
#include <iostream>

class ArrayMasterclass {
public:
    static void basicUsage() {
        std::cout << "\n=== Array基础用法 ===" << std::endl;
        
        // 编译时大小确定
        std::array<int, 5> arr1 = {1, 2, 3, 4, 5};
        std::array<int, 5> arr2{}; // 零初始化
        
        // 大小是模板参数，编译时确定
        constexpr size_t size = arr1.size();  // 编译时常量
        std::cout << "Array size (compile-time): " << size << std::endl;
        
        // 与C数组的互操作
        int c_array[5] = {1, 2, 3, 4, 5};
        std::array<int, 5> std_array;
        std::copy(std::begin(c_array), std::end(c_array), std_array.begin());
        
        // 获取底层数组
        int* raw_ptr = std_array.data();
        std::cout << "Raw pointer access: " << raw_ptr[0] << std::endl;
        
        // 编译时访问
        constexpr std::array<int, 3> const_arr = {10, 20, 30};
        constexpr int value = const_arr[1];  // 编译时计算
        std::cout << "Compile-time value: " << value << std::endl;
    }
    
    static void performanceComparison() {
        std::cout << "\n=== Array vs Vector性能对比 ===" << std::endl;
        
        const size_t N = 1000000;
        
        // Array (栈分配)
        auto start = std::chrono::high_resolution_clock::now();
        std::array<int, 1000> arr;
        std::iota(arr.begin(), arr.end(), 0);
        long long sum1 = 0;
        for(int x : arr) sum1 += x;
        auto end = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        
        // Vector (堆分配)
        start = std::chrono::high_resolution_clock::now();
        std::vector<int> vec(1000);
        std::iota(vec.begin(), vec.end(), 0);
        long long sum2 = 0;
        for(int x : vec) sum2 += x;
        end = std::chrono::high_resolution_clock::now();
        auto duration2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        
        std::cout << "Array: " << duration1.count() << "ns" << std::endl;
        std::cout << "Vector: " << duration2.count() << "ns" << std::endl;
        std::cout << "Array性能优势: " << (double)duration2.count() / duration1.count() << "倍" << std::endl;
    }
};
```

---

### Deque - 双端队列的智慧

#### 🔬 内部实现原理

Deque（Double-Ended Queue）是一个复杂而精巧的数据结构：

```cpp
#include <deque>
#include <iostream>
#include <algorithm>

class DequeMasterclass {
public:
    // 1. Deque的独特优势
    static void dequeAdvantages() {
        std::cout << "\n=== Deque独特优势演示 ===" << std::endl;
        
        std::deque<int> dq;
        
        // 双端高效操作
        auto start = std::chrono::high_resolution_clock::now();
        
        for(int i = 0; i < 10000; ++i) {
            if(i % 2 == 0) {
                dq.push_back(i);   // 后端插入
            } else {
                dq.push_front(i);  // 前端插入
            }
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "双端插入10000个元素: " << duration.count() << "μs" << std::endl;
        
        // 随机访问
        std::cout << "中间元素: " << dq[5000] << std::endl;
        
        // 内存布局分析
        std::cout << "Deque大小: " << dq.size() << std::endl;
        std::cout << "首元素地址: " << &dq.front() << std::endl;
        std::cout << "末元素地址: " << &dq.back() << std::endl;
        std::cout << "地址差: " << &dq.back() - &dq.front() << std::endl;
        
        // 证明deque不是连续内存
        if(&dq.back() - &dq.front() != (long)(dq.size() - 1)) {
            std::cout << "验证: Deque确实不是连续内存!" << std::endl;
        }
    }
    
    // 2. Deque vs Vector vs List 性能对比
    static void performanceComparison() {
        std::cout << "\n=== Deque性能全面对比 ===" << std::endl;
        
        const size_t N = 100000;
        
        // 测试1: 前端插入性能
        auto testFrontInsertion = [](const std::string& name, auto& container) {
            auto start = std::chrono::high_resolution_clock::now();
            for(size_t i = 0; i < N; ++i) {
                container.push_front(i);
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << name << " 前端插入: " << duration.count() << "μs" << std::endl;
        };
        
        std::deque<int> dq;
        std::list<int> lst;
        
        testFrontInsertion("Deque", dq);
        testFrontInsertion("List", lst);
        
        // Vector前端插入（为了对比）
        std::vector<int> vec;
        auto start = std::chrono::high_resolution_clock::now();
        for(size_t i = 0; i < 1000; ++i) {  // 更少的元素，因为太慢了
            vec.insert(vec.begin(), i);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Vector 前端插入(1000个): " << duration.count() << "μs" << std::endl;
        
        // 测试2: 随机访问性能
        dq.clear();
        vec.clear();
        for(size_t i = 0; i < N; ++i) {
            dq.push_back(i);
            vec.push_back(i);
        }
        
        auto testRandomAccess = [&](const std::string& name, auto& container) {
            start = std::chrono::high_resolution_clock::now();
            long long sum = 0;
            for(size_t i = 0; i < N; ++i) {
                sum += container[i];
            }
            end = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << name << " 随机访问: " << duration.count() << "μs (sum: " << sum << ")" << std::endl;
        };
        
        testRandomAccess("Deque", dq);
        testRandomAccess("Vector", vec);
    }
    
    // 3. Deque高级用法
    static void advancedUsage() {
        std::cout << "\n=== Deque高级用法 ===" << std::endl;
        
        // 用作滑动窗口
        std::deque<int> window;
        std::vector<int> data = {1, 3, -1, -3, 5, 3, 6, 7};
        int k = 3;  // 窗口大小
        
        std::cout << "滑动窗口最大值:" << std::endl;
        for(size_t i = 0; i < data.size(); ++i) {
            // 移除窗口外的元素
            while(!window.empty() && window.front() <= (int)i - k) {
                window.pop_front();
            }
            
            // 维护递减队列
            while(!window.empty() && data[window.back()] < data[i]) {
                window.pop_back();
            }
            
            window.push_back(i);
            
            if(i >= k - 1) {
                std::cout << "窗口[" << i - k + 1 << ", " << i << "]: " 
                         << data[window.front()] << std::endl;
            }
        }
        
        // 双端队列作为缓冲区
        std::deque<std::string> buffer;
        const size_t max_buffer_size = 5;
        
        auto addToBuffer = [&](const std::string& item) {
            buffer.push_back(item);
            if(buffer.size() > max_buffer_size) {
                std::cout << "缓冲区满，移除: " << buffer.front() << std::endl;
                buffer.pop_front();
            }
        };
        
        std::cout << "\n缓冲区演示:" << std::endl;
        for(int i = 1; i <= 8; ++i) {
            addToBuffer("item" + std::to_string(i));
            std::cout << "当前缓冲区大小: " << buffer.size() << std::endl;
        }
    }
};
```

---

### List - 双向链表的灵活性

#### 🔬 内部实现原理

```cpp
#include <list>
#include <iostream>
#include <algorithm>

class ListMasterclass {
public:
    // 1. List的核心优势
    static void listAdvantages() {
        std::cout << "\n=== List核心优势演示 ===" << std::endl;
        
        std::list<int> lst = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        
        // 迭代器稳定性演示
        auto it1 = lst.begin();
        std::advance(it1, 3);  // 指向第4个元素
        auto it2 = lst.begin();
        std::advance(it2, 7);  // 指向第8个元素
        
        std::cout << "删除前: it1=" << *it1 << ", it2=" << *it2 << std::endl;
        
        // 删除中间的一些元素
        auto it_start = lst.begin();
        std::advance(it_start, 1);
        auto it_end = lst.begin();
        std::advance(it_end, 3);
        lst.erase(it_start, it_end);  // 删除第2、3个元素
        
        std::cout << "删除后: it1=" << *it1 << ", it2=" << *it2 << std::endl;
        std::cout << "迭代器依然有效!" << std::endl;
        
        // 高效的任意位置插入删除
        auto middle = lst.begin();
        std::advance(middle, lst.size() / 2);
        
        auto start = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < 1000; ++i) {
            lst.insert(middle, i);  // O(1)操作
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "中间插入1000个元素: " << duration.count() << "μs" << std::endl;
    }
    
    // 2. List特有操作详解
    static void specialOperations() {
        std::cout << "\n=== List特有操作详解 ===" << std::endl;
        
        std::list<int> lst1 = {1, 3, 5, 7, 9};
        std::list<int> lst2 = {2, 4, 6, 8, 10};
        
        std::cout << "初始状态:" << std::endl;
        std::cout << "lst1: ";
        for(int x : lst1) std::cout << x << " ";
        std::cout << std::endl;
        std::cout << "lst2: ";
        for(int x : lst2) std::cout << x << " ";
        std::cout << std::endl;
        
        // splice操作 - 高效移动元素
        auto it = lst1.begin();
        std::advance(it, 2);  // 移动到第3个位置
        
        lst1.splice(it, lst2, lst2.begin());  // 移动lst2的第一个元素到lst1
        
        std::cout << "\nsplice后:" << std::endl;
        std::cout << "lst1: ";
        for(int x : lst1) std::cout << x << " ";
        std::cout << std::endl;
        std::cout << "lst2: ";
        for(int x : lst2) std::cout << x << " ";
        std::cout << std::endl;
        
        // merge操作 - 合并有序链表
        lst1.sort();
        lst2.sort();
        
        std::cout << "\n排序后:" << std::endl;
        std::cout << "lst1: ";
        for(int x : lst1) std::cout << x << " ";
        std::cout << std::endl;
        std::cout << "lst2: ";
        for(int x : lst2) std::cout << x << " ";
        std::cout << std::endl;
        
        lst1.merge(lst2);  // 合并两个有序链表
        
        std::cout << "\nmerge后:" << std::endl;
        std::cout << "lst1: ";
        for(int x : lst1) std::cout << x << " ";
        std::cout << std::endl;
        std::cout << "lst2 size: " << lst2.size() << std::endl;
        
        // unique操作 - 去除连续重复元素
        lst1.insert(lst1.end(), {10, 10, 10, 11, 11, 12});
        std::cout << "\n添加重复元素后: ";
        for(int x : lst1) std::cout << x << " ";
        std::cout << std::endl;
        
        lst1.unique();
        std::cout << "unique后: ";
        for(int x : lst1) std::cout << x << " ";
        std::cout << std::endl;
        
        // remove_if操作 - 条件删除
        lst1.remove_if([](int x) { return x % 2 == 0; });
        std::cout << "删除偶数后: ";
        for(int x : lst1) std::cout << x << " ";
        std::cout << std::endl;
    }
    
    // 3. 实际应用场景
    static void realWorldApplications() {
        std::cout << "\n=== List实际应用场景 ===" << std::endl;
        
        // 场景1: LRU缓存实现
        class LRUCache {
        private:
            size_t capacity;
            std::list<std::pair<int, int>> cache;  // key-value对
            std::unordered_map<int, std::list<std::pair<int, int>>::iterator> map;
            
        public:
            LRUCache(size_t cap) : capacity(cap) {}
            
            int get(int key) {
                auto it = map.find(key);
                if(it == map.end()) {
                    return -1;  // 未找到
                }
                
                // 移动到前面（最近使用）
                cache.splice(cache.begin(), cache, it->second);
                return it->second->second;
            }
            
            void put(int key, int value) {
                auto it = map.find(key);
                if(it != map.end()) {
                    // 更新现有key
                    it->second->second = value;
                    cache.splice(cache.begin(), cache, it->second);
                    return;
                }
                
                // 新key
                if(cache.size() >= capacity) {
                    // 删除最久未使用的
                    map.erase(cache.back().first);
                    cache.pop_back();
                }
                
                cache.emplace_front(key, value);
                map[key] = cache.begin();
            }
            
            void print() const {
                std::cout << "LRU Cache: ";
                for(const auto& p : cache) {
                    std::cout << "[" << p.first << ":" << p.second << "] ";
                }
                std::cout << std::endl;
            }
        };
        
        LRUCache lru(3);
        lru.put(1, 10);
        lru.put(2, 20);
        lru.put(3, 30);
        lru.print();
        
        lru.get(1);  // 访问1，使其成为最近使用
        lru.print();
        
        lru.put(4, 40);  // 添加新元素，应该删除2
        lru.print();
        
        // 场景2: 撤销/重做功能
        class UndoRedoManager {
        private:
            std::list<std::string> actions;
            std::list<std::string>::iterator current;
            
        public:
            UndoRedoManager() {
                current = actions.end();
            }
            
            void execute(const std::string& action) {
                // 删除current之后的所有动作
                actions.erase(current, actions.end());
                
                // 添加新动作
                actions.push_back(action);
                current = actions.end();
                
                std::cout << "执行: " << action << std::endl;
            }
            
            bool undo() {
                if(current != actions.begin()) {
                    --current;
                    std::cout << "撤销: " << *current << std::endl;
                    return true;
                }
                return false;
            }
            
            bool redo() {
                if(current != actions.end()) {
                    std::cout << "重做: " << *current << std::endl;
                    ++current;
                    return true;
                }
                return false;
            }
            
            void printState() const {
                std::cout << "动作历史: ";
                for(auto it = actions.begin(); it != actions.end(); ++it) {
                    if(it == current) std::cout << "[" << *it << "] ";
                    else std::cout << *it << " ";
                }
                std::cout << std::endl;
            }
        };
        
        std::cout << "\n撤销/重做演示:" << std::endl;
        UndoRedoManager manager;
        manager.execute("创建文件");
        manager.execute("写入内容");
        manager.execute("保存文件");
        manager.printState();
        
        manager.undo();
        manager.undo();
        manager.printState();
        
        manager.redo();
        manager.printState();
        
        manager.execute("修改内容");
        manager.printState();
    }
};
```

---

### Forward_list - 内存效率之王

#### 🔬 内部实现原理

```cpp
#include <forward_list>
#include <iostream>
#include <memory>

class ForwardListMasterclass {
public:
    // 1. 内存效率对比
    static void memoryEfficiency() {
        std::cout << "\n=== Forward_list内存效率分析 ===" << std::endl;
        
        // 理论分析
        std::cout << "理论内存开销分析:" << std::endl;
        std::cout << "int大小: " << sizeof(int) << " bytes" << std::endl;
        std::cout << "指针大小: " << sizeof(void*) << " bytes" << std::endl;
        
        // List节点：data + prev指针 + next指针
        struct ListNode {
            int data;
            ListNode* prev;
            ListNode* next;
        };
        std::cout << "List节点大小: " << sizeof(ListNode) << " bytes" << std::endl;
        
        // Forward_list节点：data + next指针
        struct ForwardListNode {
            int data;
            ForwardListNode* next;
        };
        std::cout << "Forward_list节点大小: " << sizeof(ForwardListNode) << " bytes" << std::endl;
        
        double memory_saving = (double)(sizeof(ListNode) - sizeof(ForwardListNode)) / sizeof(ListNode) * 100;
        std::cout << "内存节省: " << memory_saving << "%" << std::endl;
        
        // 实际测试
        const size_t N = 100000;
        
        auto measureMemory = [](const std::string& name, auto createContainer) {
            auto container = createContainer();
            
            // 简单的内存使用估算
            size_t theoretical_memory = 0;
            if(name == "list") {
                theoretical_memory = N * sizeof(ListNode);
            } else {
                theoretical_memory = N * sizeof(ForwardListNode);
            }
            
            std::cout << name << " 理论内存使用: " << theoretical_memory / 1024 << " KB" << std::endl;
        };
        
        measureMemory("list", []() {
            std::list<int> lst;
            for(size_t i = 0; i < N; ++i) {
                lst.push_back(i);
            }
            return lst;
        });
        
        measureMemory("forward_list", []() {
            std::forward_list<int> fl;
            for(size_t i = 0; i < N; ++i) {
                fl.push_front(i);
            }
            return fl;
        });
    }
    
    // 2. Forward_list特有操作技巧
    static void specialTechniques() {
        std::cout << "\n=== Forward_list特有操作技巧 ===" << std::endl;
        
        std::forward_list<int> fl = {1, 2, 3, 4, 5};
        
        // 技巧1: 在指定元素后插入
        auto insert_after_value = [](std::forward_list<int>& list, int target, int value) {
            auto prev = list.before_begin();
            for(auto it = list.begin(); it != list.end(); ++it) {
                if(*it == target) {
                    list.insert_after(it, value);
                    return true;
                }
                prev = it;
            }
            return false;
        };
        
        std::cout << "插入前: ";
        for(int x : fl) std::cout << x << " ";
        std::cout << std::endl;
        
        insert_after_value(fl, 3, 99);
        
        std::cout << "在3后插入99: ";
        for(int x : fl) std::cout << x << " ";
        std::cout << std::endl;
        
        // 技巧2: 删除指定值
        auto remove_value = [](std::forward_list<int>& list, int value) {
            auto prev = list.before_begin();
            auto current = list.begin();
            
            while(current != list.end()) {
                if(*current == value) {
                    current = list.erase_after(prev);
                } else {
                    prev = current;
                    ++current;
                }
            }
        };
        
        remove_value(fl, 2);
        
        std::cout << "删除2后: ";
        for(int x : fl) std::cout << x << " ";
        std::cout << std::endl;
        
        // 技巧3: 反转链表
        fl.reverse();
        std::cout << "反转后: ";
        for(int x : fl) std::cout << x << " ";
        std::cout << std::endl;
        
        // 技巧4: 合并两个有序链表
        std::forward_list<int> fl1 = {1, 3, 5};
        std::forward_list<int> fl2 = {2, 4, 6};
        
        std::cout << "\n合并前:" << std::endl;
        std::cout << "fl1: ";
        for(int x : fl1) std::cout << x << " ";
        std::cout << std::endl;
        std::cout << "fl2: ";
        for(int x : fl2) std::cout << x << " ";
        std::cout << std::endl;
        
        fl1.merge(fl2);
        
        std::cout << "合并后:" << std::endl;
        std::cout << "fl1: ";
        for(int x : fl1) std::cout << x << " ";
        std::cout << std::endl;
    }
    
    // 3. 实际应用：单链表算法
    static void algorithmApplications() {
        std::cout << "\n=== Forward_list算法应用 ===" << std::endl;
        
        // 应用1: 检测环
        auto has_cycle = [](const std::forward_list<int>& fl) {
            if(fl.empty()) return false;
            
            auto slow = fl.begin();
            auto fast = fl.begin();
            
            while(fast != fl.end() && std::next(fast) != fl.end()) {
                slow = std::next(slow);
                fast = std::next(std::next(fast));
                
                if(slow == fast) {
                    return true;
                }
            }
            return false;
        };
        
        std::forward_list<int> normal_list = {1, 2, 3, 4, 5};
        std::cout << "正常链表有环: " << (has_cycle(normal_list) ? "是" : "否") << std::endl;
        
        // 应用2: 找到中间节点
        auto find_middle = [](const std::forward_list<int>& fl) {
            if(fl.empty()) return fl.end();
            
            auto slow = fl.begin();
            auto fast = fl.begin();
            
            while(fast != fl.end() && std::next(fast) != fl.end()) {
                slow = std::next(slow);
                fast = std::next(std::next(fast));
            }
            
            return slow;
        };
        
        auto middle = find_middle(normal_list);
        if(middle != normal_list.end()) {
            std::cout << "中间节点值: " << *middle << std::endl;
        }
        
        // 应用3: 删除倒数第k个节点
        auto remove_kth_from_end = [](std::forward_list<int>& fl, int k) {
            if(k <= 0) return;
            
            auto dummy = fl.before_begin();
            auto slow = dummy;
            auto fast = fl.begin();
            
            // fast先走k步
            for(int i = 0; i < k && fast != fl.end(); ++i) {
                fast = std::next(fast);
            }
            
            // 同时移动slow和fast
            while(fast != fl.end()) {
                slow = std::next(slow);
                fast = std::next(fast);
            }
            
            // 删除slow的下一个节点
            if(std::next(slow) != fl.end()) {
                fl.erase_after(slow);
            }
        };
        
        std::forward_list<int> test_list = {1, 2, 3, 4, 5, 6, 7};
        std::cout << "\n删除前: ";
        for(int x : test_list) std::cout << x << " ";
        std::cout << std::endl;
        
        remove_kth_from_end(test_list, 3);  // 删除倒数第3个
        
        std::cout << "删除倒数第3个后: ";
        for(int x : test_list) std::cout << x << " ";
        std::cout << std::endl;
    }
};
```

---

## 🎯 序列容器选择指南

```cpp
class ContainerSelector {
public:
    static void selectionGuide() {
        std::cout << "\n=== 序列容器选择指南 ===" << std::endl;
        
        std::cout << "容器特性对比表:" << std::endl;
        std::cout << "┌─────────────┬────────┬────────┬────────┬────────┬────────┐" << std::endl;
        std::cout << "│    操作     │ vector │ deque  │  list  │ f_list │ array  │" << std::endl;
        std::cout << "├─────────────┼────────┼────────┼────────┼────────┼────────┤" << std::endl;
        std::cout << "│ 随机访问    │  O(1)  │  O(1)  │  O(n)  │  O(n)  │  O(1)  │" << std::endl;
        std::cout << "│ 前端插入    │  O(n)  │  O(1)  │  O(1)  │  O(1)  │   无   │" << std::endl;
        std::cout << "│ 后端插入    │  O(1)* │  O(1)  │  O(1)  │  O(n)  │   无   │" << std::endl;
        std::cout << "│ 中间插入    │  O(n)  │  O(n)  │  O(1)  │  O(1)  │   无   │" << std::endl;
        std::cout << "│ 内存效率    │   高   │   中   │   低   │  最高  │  最高  │" << std::endl;
        std::cout << "│ 缓存友好    │  最好  │   好   │   差   │   差   │  最好  │" << std::endl;
        std::cout << "│ 迭代器稳定  │   否   │   否   │   是   │   是   │   是   │" << std::endl;
        std::cout << "└─────────────┴────────┴────────┴────────┴────────┴────────┘" << std::endl;
        std::cout << "* 摊销复杂度" << std::endl;
        
        std::cout << "\n选择建议:" << std::endl;
        std::cout << "🎯 vector: 默认选择，90%的情况下使用" << std::endl;
        std::cout << "   - 需要随机访问" << std::endl;
        std::cout << "   - 主要在末尾操作" << std::endl;
        std::cout << "   - 内存和缓存效率重要" << std::endl;
        
        std::cout << "\n🎯 deque: 需要双端操作时" << std::endl;
        std::cout << "   - 需要在前端和后端插入删除" << std::endl;
        std::cout << "   - 需要随机访问" << std::endl;
        std::cout << "   - 用作queue的底层容器" << std::endl;
        
        std::cout << "\n🎯 list: 需要频繁中间操作时" << std::endl;
        std::cout << "   - 频繁在中间插入删除" << std::endl;
        std::cout << "   - 需要迭代器稳定性" << std::endl;
        std::cout << "   - 需要splice等特殊操作" << std::endl;
        
        std::cout << "\n🎯 forward_list: 内存极度受限时" << std::endl;
        std::cout << "   - 最小的内存开销" << std::endl;
        std::cout << "   - 只需要单向遍历" << std::endl;
        std::cout << "   - 实现某些特殊算法" << std::endl;
        
        std::cout << "\n🎯 array: 编译时固定大小" << std::endl;
        std::cout << "   - 大小在编译时确定" << std::endl;
        std::cout << "   - 需要最佳性能" << std::endl;
        std::cout << "   - 替代C风格数组" << std::endl;
    }
};
```

这只是开始！接下来我将继续完善关联容器、无序容器、适配器等部分，确保每个部分都达到世界级标准！您觉得这个深度如何？需要我继续完善哪个部分？

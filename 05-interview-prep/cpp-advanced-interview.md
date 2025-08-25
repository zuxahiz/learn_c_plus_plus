# C++ 高级面试题详解

## 🎯 C++高级面试概述

C++作为系统级编程语言，面试中不仅考察语法知识，更注重对内存管理、性能优化、设计模式等高级特性的深度理解。本指南覆盖了各大公司C++岗位的核心面试内容。

### 面试重点领域
- **内存管理**: 智能指针、RAII、内存泄漏
- **现代C++**: C++11/14/17/20新特性
- **性能优化**: 移动语义、编译器优化
- **设计模式**: 单例、工厂、观察者等
- **并发编程**: 多线程、锁机制、原子操作
- **模板元编程**: 类型萃取、SFINAE

## 📚 1. 内存管理深度解析

### 1.1 智能指针实现原理

**面试问题**: "请实现一个shared_ptr，并解释其工作原理"

```cpp
template<typename T>
class MySharedPtr {
private:
    T* ptr;
    size_t* refCount;
    
    void cleanup() {
        if (refCount && --(*refCount) == 0) {
            delete ptr;
            delete refCount;
        }
        ptr = nullptr;
        refCount = nullptr;
    }
    
public:
    // 构造函数
    explicit MySharedPtr(T* p = nullptr) 
        : ptr(p), refCount(p ? new size_t(1) : nullptr) {}
    
    // 拷贝构造函数
    MySharedPtr(const MySharedPtr& other) 
        : ptr(other.ptr), refCount(other.refCount) {
        if (refCount) ++(*refCount);
    }
    
    // 移动构造函数
    MySharedPtr(MySharedPtr&& other) noexcept
        : ptr(other.ptr), refCount(other.refCount) {
        other.ptr = nullptr;
        other.refCount = nullptr;
    }
    
    // 拷贝赋值操作符
    MySharedPtr& operator=(const MySharedPtr& other) {
        if (this != &other) {
            cleanup();
            ptr = other.ptr;
            refCount = other.refCount;
            if (refCount) ++(*refCount);
        }
        return *this;
    }
    
    // 移动赋值操作符
    MySharedPtr& operator=(MySharedPtr&& other) noexcept {
        if (this != &other) {
            cleanup();
            ptr = other.ptr;
            refCount = other.refCount;
            other.ptr = nullptr;
            other.refCount = nullptr;
        }
        return *this;
    }
    
    // 析构函数
    ~MySharedPtr() {
        cleanup();
    }
    
    // 解引用操作符
    T& operator*() const {
        if (!ptr) throw std::runtime_error("Dereferencing null pointer");
        return *ptr;
    }
    
    T* operator->() const {
        if (!ptr) throw std::runtime_error("Accessing null pointer");
        return ptr;
    }
    
    // 获取原始指针
    T* get() const { return ptr; }
    
    // 获取引用计数
    size_t use_count() const { return refCount ? *refCount : 0; }
    
    // 重置指针
    void reset(T* p = nullptr) {
        cleanup();
        ptr = p;
        refCount = p ? new size_t(1) : nullptr;
    }
    
    // 判断是否为空
    explicit operator bool() const { return ptr != nullptr; }
};
```

**关键知识点**:
1. **引用计数机制**: 多个shared_ptr共享同一个引用计数
2. **RAII原则**: 构造时获取资源，析构时释放资源
3. **异常安全**: 使用RAII确保异常情况下的资源释放
4. **移动语义**: 提高性能，避免不必要的拷贝

### 1.2 循环引用问题与解决方案

**面试问题**: "shared_ptr的循环引用问题是什么？如何解决？"

```cpp
// 问题演示：循环引用导致内存泄漏
class Node {
public:
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> prev;
    int data;
    
    Node(int val) : data(val) {
        std::cout << "Node " << data << " created" << std::endl;
    }
    
    ~Node() {
        std::cout << "Node " << data << " destroyed" << std::endl;
    }
};

// 解决方案：使用weak_ptr
class NodeFixed {
public:
    std::shared_ptr<NodeFixed> next;
    std::weak_ptr<NodeFixed> prev; // 使用weak_ptr避免循环引用
    int data;
    
    NodeFixed(int val) : data(val) {
        std::cout << "NodeFixed " << data << " created" << std::endl;
    }
    
    ~NodeFixed() {
        std::cout << "NodeFixed " << data << " destroyed" << std::endl;
    }
    
    void printPrev() {
        if (auto prevNode = prev.lock()) {
            std::cout << "Previous node data: " << prevNode->data << std::endl;
        } else {
            std::cout << "Previous node is expired" << std::endl;
        }
    }
};
```

### 1.3 自定义内存分配器

**面试问题**: "如何实现一个高效的内存池分配器？"

```cpp
template<typename T, size_t BlockSize = 4096>
class MemoryPool {
private:
    struct Block {
        alignas(T) char data[BlockSize];
        Block* next;
        
        Block() : next(nullptr) {}
    };
    
    struct FreeNode {
        FreeNode* next;
    };
    
    Block* currentBlock;
    FreeNode* freeList;
    char* currentPos;
    char* endPos;
    size_t allocatedBlocks;
    
    void allocateNewBlock() {
        Block* newBlock = static_cast<Block*>(std::malloc(sizeof(Block)));
        if (!newBlock) {
            throw std::bad_alloc();
        }
        
        new(newBlock) Block();
        newBlock->next = currentBlock;
        currentBlock = newBlock;
        currentPos = newBlock->data;
        endPos = newBlock->data + BlockSize;
        allocatedBlocks++;
    }
    
public:
    MemoryPool() : currentBlock(nullptr), freeList(nullptr), 
                   currentPos(nullptr), endPos(nullptr), allocatedBlocks(0) {
        static_assert(sizeof(T) >= sizeof(FreeNode), 
                     "Type too small for memory pool");
        allocateNewBlock();
    }
    
    ~MemoryPool() {
        while (currentBlock) {
            Block* next = currentBlock->next;
            std::free(currentBlock);
            currentBlock = next;
        }
    }
    
    T* allocate() {
        // 首先检查空闲列表
        if (freeList) {
            T* result = reinterpret_cast<T*>(freeList);
            freeList = freeList->next;
            return result;
        }
        
        // 检查当前块是否有足够空间
        if (currentPos + sizeof(T) > endPos) {
            allocateNewBlock();
        }
        
        T* result = reinterpret_cast<T*>(currentPos);
        currentPos += sizeof(T);
        return result;
    }
    
    void deallocate(T* ptr) {
        if (!ptr) return;
        
        // 将释放的内存添加到空闲列表
        FreeNode* node = reinterpret_cast<FreeNode*>(ptr);
        node->next = freeList;
        freeList = node;
    }
    
    template<typename... Args>
    T* construct(Args&&... args) {
        T* ptr = allocate();
        try {
            new(ptr) T(std::forward<Args>(args)...);
            return ptr;
        } catch (...) {
            deallocate(ptr);
            throw;
        }
    }
    
    void destroy(T* ptr) {
        if (!ptr) return;
        ptr->~T();
        deallocate(ptr);
    }
    
    size_t getBlockCount() const { return allocatedBlocks; }
};
```

## 📚 2. 现代C++特性深度解析

### 2.1 移动语义和完美转发

**面试问题**: "解释移动语义的原理，并实现一个支持移动语义的容器类"

```cpp
template<typename T>
class MyVector {
private:
    T* data;
    size_t size_;
    size_t capacity_;
    
    void reallocate(size_t newCapacity) {
        T* newData = static_cast<T*>(std::malloc(newCapacity * sizeof(T)));
        if (!newData && newCapacity > 0) {
            throw std::bad_alloc();
        }
        
        // 移动构造元素到新位置
        for (size_t i = 0; i < size_; i++) {
            new(newData + i) T(std::move(data[i]));
            data[i].~T();
        }
        
        std::free(data);
        data = newData;
        capacity_ = newCapacity;
    }
    
public:
    // 默认构造函数
    MyVector() : data(nullptr), size_(0), capacity_(0) {}
    
    // 带容量的构造函数
    explicit MyVector(size_t capacity) : size_(0), capacity_(capacity) {
        data = static_cast<T*>(std::malloc(capacity * sizeof(T)));
    }
    
    // 拷贝构造函数
    MyVector(const MyVector& other) : size_(other.size_), capacity_(other.capacity_) {
        data = static_cast<T*>(std::malloc(capacity_ * sizeof(T)));
        for (size_t i = 0; i < size_; i++) {
            new(data + i) T(other.data[i]);
        }
    }
    
    // 移动构造函数
    MyVector(MyVector&& other) noexcept 
        : data(other.data), size_(other.size_), capacity_(other.capacity_) {
        other.data = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    
    // 拷贝赋值操作符
    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            clear();
            size_ = other.size_;
            capacity_ = other.capacity_;
            data = static_cast<T*>(std::malloc(capacity_ * sizeof(T)));
            
            for (size_t i = 0; i < size_; i++) {
                new(data + i) T(other.data[i]);
            }
        }
        return *this;
    }
    
    // 移动赋值操作符
    MyVector& operator=(MyVector&& other) noexcept {
        if (this != &other) {
            clear();
            
            data = other.data;
            size_ = other.size_;
            capacity_ = other.capacity_;
            
            other.data = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }
    
    // 析构函数
    ~MyVector() {
        clear();
    }
    
    // 清空容器
    void clear() {
        for (size_t i = 0; i < size_; i++) {
            data[i].~T();
        }
        std::free(data);
        data = nullptr;
        size_ = 0;
        capacity_ = 0;
    }
    
    // 添加元素（拷贝版本）
    void push_back(const T& value) {
        if (size_ >= capacity_) {
            reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        new(data + size_) T(value);
        size_++;
    }
    
    // 添加元素（移动版本）
    void push_back(T&& value) {
        if (size_ >= capacity_) {
            reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        new(data + size_) T(std::move(value));
        size_++;
    }
    
    // 原地构造元素
    template<typename... Args>
    void emplace_back(Args&&... args) {
        if (size_ >= capacity_) {
            reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        new(data + size_) T(std::forward<Args>(args)...);
        size_++;
    }
    
    // 访问元素
    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }
    
    // 获取大小和容量
    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }
    
    // 迭代器支持
    T* begin() { return data; }
    T* end() { return data + size_; }
    const T* begin() const { return data; }
    const T* end() const { return data + size_; }
};
```

### 2.2 Lambda表达式和函数对象

**面试问题**: "解释Lambda表达式的捕获机制，并实现一个函数式编程风格的算法"

```cpp
class LambdaExamples {
public:
    // 演示各种捕获方式
    void demonstrateCaptures() {
        int localVar = 42;
        static int staticVar = 100;
        
        // 1. 值捕获
        auto lambda1 = [localVar]() {
            std::cout << "值捕获: " << localVar << std::endl;
        };
        
        // 2. 引用捕获
        auto lambda2 = [&localVar]() {
            std::cout << "引用捕获: " << localVar << std::endl;
            localVar++;
        };
        
        // 3. 可变Lambda
        auto lambda3 = [localVar]() mutable {
            std::cout << "可变Lambda: " << localVar << std::endl;
            localVar++;
        };
        
        // 4. 捕获所有（值）
        auto lambda4 = [=]() {
            std::cout << "捕获所有（值）: " << localVar << ", " << staticVar << std::endl;
        };
        
        // 5. 捕获所有（引用）
        auto lambda5 = [&]() {
            std::cout << "捕获所有（引用）: " << localVar << ", " << staticVar << std::endl;
            localVar += 10;
        };
        
        // 执行测试
        lambda1();
        lambda2();
        lambda3();
        lambda4();
        lambda5();
    }
};
```

## 💡 面试技巧和注意事项

### 1. 代码质量标准
- **RAII原则**: 构造时获取资源，析构时释放
- **异常安全**: 使用智能指针和RAII确保异常安全
- **const正确性**: 合理使用const关键字
- **移动语义**: 利用std::move和完美转发提高性能

### 2. 常见面试陷阱
- **内存泄漏**: 使用智能指针避免
- **数据竞争**: 合理使用同步机制
- **异常处理**: 考虑异常情况下的资源管理
- **性能优化**: 避免过早优化，先保证正确性

### 3. 回答策略
- **由浅入深**: 先说基本概念，再深入细节
- **举例说明**: 用具体例子解释抽象概念
- **对比分析**: 比较不同方案的优缺点
- **实际应用**: 结合实际项目经验

## 🔥 高频面试题快速复习

### 基础概念 (5分钟准备)
1. 指针与引用的区别
2. const关键字的各种用法
3. 静态成员和静态函数
4. 虚函数和纯虚函数
5. 构造函数和析构函数

### 内存管理 (10分钟准备)
1. 智能指针的实现原理
2. RAII原则和应用
3. 内存泄漏的原因和避免
4. 循环引用问题和解决
5. 自定义内存分配器

### 现代C++ (15分钟准备)
1. 移动语义和右值引用
2. Lambda表达式和捕获
3. auto关键字和类型推导
4. 模板元编程和SFINAE
5. constexpr和编译期计算

### 并发编程 (15分钟准备)
1. 线程创建和管理
2. 互斥锁和条件变量
3. 原子操作和内存序
4. 读写锁和共享锁
5. future/promise异步编程

### 设计模式 (10分钟准备)
1. 单例模式的线程安全实现
2. 工厂模式的应用场景
3. 观察者模式的实现
4. 策略模式的使用
5. SOLID设计原则

## 🚀 下一步学习建议

1. **深入学习现代C++**: 关注C++20/23的新特性
2. **实践项目开发**: 用学到的知识做实际项目
3. **阅读优秀代码**: 学习开源项目的设计思想
4. **性能分析工具**: 学习使用性能分析工具
5. **持续关注技术发展**: 跟进C++社区的最新动态

---

**总结**: C++面试不仅考察语法知识，更注重对底层原理的理解和实际应用能力。通过系统学习和大量练习，掌握这些高级特性，能够显著提高面试成功率。记住，面试官更看重你的思考过程和解决问题的能力！🎯
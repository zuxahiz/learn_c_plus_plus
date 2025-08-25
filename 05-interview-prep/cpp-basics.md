# C++ 基础面试题详解

## 🎯 C++基础概述

C++基础面试题主要考察对C++核心概念的理解，包括指针、引用、const、static、面向对象编程、内存管理等。这些是C++程序员必须掌握的基础知识。

## 📚 指针和引用

### 1. 指针和引用的区别
**问题**: 请解释指针和引用的区别，并说明它们的适用场景。

**答案**:
```cpp
// 指针和引用的基本区别
int x = 10;
int* ptr = &x;      // 指针：可以指向nullptr，可以重新赋值
int& ref = x;       // 引用：必须初始化，不能重新绑定

// 1. 初始化
int* p1;            // 合法：指针可以不初始化
// int& r1;         // 错误：引用必须初始化

// 2. 重新赋值
int y = 20;
ptr = &y;           // 合法：指针可以重新指向
// ref = y;         // 这是赋值，不是重新绑定

// 3. 空值
int* p2 = nullptr;  // 合法：指针可以为空
// int& r2 = nullptr; // 错误：引用不能为空

// 4. 多级引用
int** pp = &ptr;    // 合法：指针的指针
// int&& rr = ref;  // 错误：引用的引用（C++11中的右值引用除外）
```

**关键区别**:
- **指针**: 可以为空，可以重新指向，支持多级指针
- **引用**: 必须初始化，不能重新绑定，不能为空
- **适用场景**: 指针用于可能为空的情况，引用用于确保非空的情况

### 2. 指针的常见问题
**问题**: 以下代码有什么问题？如何修复？

```cpp
int* createArray() {
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    return arr;  // 问题：返回局部数组的地址
}

int main() {
    int* ptr = createArray();
    cout << ptr[0] << endl;  // 未定义行为
    return 0;
}
```

**修复方案**:
```cpp
// 方案1：动态分配内存
int* createArray() {
    int* arr = new int[10];
    for (int i = 0; i < 10; ++i) {
        arr[i] = i + 1;
    }
    return arr;
}

// 方案2：使用智能指针
unique_ptr<int[]> createArray() {
    auto arr = make_unique<int[]>(10);
    for (int i = 0; i < 10; ++i) {
        arr[i] = i + 1;
    }
    return arr;
}

// 方案3：传入数组作为参数
void fillArray(int* arr, int size) {
    for (int i = 0; i < size; ++i) {
        arr[i] = i + 1;
    }
}
```

### 3. 引用作为函数参数
**问题**: 什么时候使用引用作为函数参数？请举例说明。

**答案**:
```cpp
// 1. 避免拷贝大对象
struct BigObject {
    vector<int> data;
    string name;
    // ... 其他成员
};

// 使用引用避免拷贝
void processObject(const BigObject& obj) {
    // 只读访问，不需要拷贝
    cout << "Processing: " << obj.name << endl;
}

// 2. 需要修改原对象
void modifyObject(BigObject& obj) {
    obj.name = "Modified";
    obj.data.push_back(100);
}

// 3. 返回值优化
const string& getLongString() {
    static string longString = "This is a very long string...";
    return longString;  // 避免拷贝
}

// 4. 在循环中使用引用
void processVector(const vector<int>& vec) {
    for (const auto& item : vec) {  // 避免拷贝每个元素
        cout << item << " ";
    }
}
```

## 📚 const关键字

### 1. const的各种用法
**问题**: 请解释const关键字的各种用法和含义。

**答案**:
```cpp
class Example {
private:
    int data;
    mutable int counter;  // 即使在const函数中也可以修改
    
public:
    // 1. const成员函数
    int getData() const {
        // counter++;  // 合法：mutable成员可以修改
        // data = 100; // 错误：不能修改非mutable成员
        return data;
    }
    
    // 2. const对象只能调用const成员函数
    void nonConstFunc() {
        data = 100;  // 可以修改成员
    }
};

// 3. const指针和指向const的指针
void constPointerExample() {
    int x = 10;
    int y = 20;
    
    const int* ptr1 = &x;    // 指向const int的指针
    // *ptr1 = 30;           // 错误：不能通过ptr1修改x
    ptr1 = &y;               // 合法：可以重新指向
    
    int* const ptr2 = &x;    // const指针
    *ptr2 = 30;              // 合法：可以修改指向的值
    // ptr2 = &y;            // 错误：不能重新指向
    
    const int* const ptr3 = &x;  // 指向const int的const指针
    // *ptr3 = 30;           // 错误：不能修改值
    // ptr3 = &y;            // 错误：不能重新指向
}

// 4. const引用
void constReferenceExample() {
    int x = 10;
    const int& ref = x;      // const引用
    // ref = 20;             // 错误：不能通过ref修改x
    x = 20;                  // 合法：可以通过原变量修改
}
```

### 2. const和函数重载
**问题**: 请解释const如何影响函数重载。

**答案**:
```cpp
class String {
private:
    char* data;
    
public:
    // 1. const和非const成员函数可以重载
    char& operator[](size_t index) {
        return data[index];  // 返回非const引用，可以修改
    }
    
    const char& operator[](size_t index) const {
        return data[index];  // 返回const引用，不能修改
    }
    
    // 2. 基于const的函数重载
    void print() {
        cout << "Non-const print" << endl;
    }
    
    void print() const {
        cout << "Const print" << endl;
    }
};

void testConstOverload() {
    String str;
    const String constStr;
    
    str[0] = 'A';           // 调用非const版本
    char c = constStr[0];   // 调用const版本
    
    str.print();             // 调用非const版本
    constStr.print();        // 调用const版本
}
```

## 📚 static关键字

### 1. static的各种用法
**问题**: 请解释static关键字在C++中的各种用法。

**答案**:
```cpp
// 1. 静态局部变量
void functionWithStatic() {
    static int count = 0;    // 只在第一次调用时初始化
    count++;
    cout << "Called " << count << " times" << endl;
}

// 2. 静态全局变量
static int globalVar = 100;  // 只在当前文件可见

// 3. 静态成员变量
class Counter {
private:
    static int totalCount;   // 所有对象共享
    
public:
    Counter() {
        totalCount++;
    }
    
    ~Counter() {
        totalCount--;
    }
    
    static int getTotalCount() {
        return totalCount;
    }
};

// 静态成员变量定义
int Counter::totalCount = 0;

// 4. 静态成员函数
class MathUtils {
public:
    static int add(int a, int b) {
        return a + b;
    }
    
    static double PI;        // 静态常量
};

double MathUtils::PI = 3.14159;

// 5. 静态函数
static void internalFunction() {
    // 只在当前文件可见
    cout << "Internal function" << endl;
}
```

### 2. static在单例模式中的应用
**问题**: 请实现一个线程安全的单例模式。

**答案**:
```cpp
// 方案1：局部静态变量（C++11及以后线程安全）
class Singleton {
private:
    Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    
public:
    static Singleton& getInstance() {
        static Singleton instance;  // C++11保证线程安全
        return instance;
    }
    
    void doSomething() {
        cout << "Singleton working..." << endl;
    }
};

// 方案2：双重检查锁定
class ThreadSafeSingleton {
private:
    static ThreadSafeSingleton* instance;
    static mutex mtx;
    
    ThreadSafeSingleton() = default;
    ThreadSafeSingleton(const ThreadSafeSingleton&) = delete;
    ThreadSafeSingleton& operator=(const ThreadSafeSingleton&) = delete;
    
public:
    static ThreadSafeSingleton* getInstance() {
        if (instance == nullptr) {
            lock_guard<mutex> lock(mtx);
            if (instance == nullptr) {
                instance = new ThreadSafeSingleton();
            }
        }
        return instance;
    }
    
    void doSomething() {
        cout << "Thread-safe singleton working..." << endl;
    }
    
    ~ThreadSafeSingleton() {
        delete instance;
    }
};

ThreadSafeSingleton* ThreadSafeSingleton::instance = nullptr;
mutex ThreadSafeSingleton::mtx;
```

## 📚 面向对象编程

### 1. 继承和多态
**问题**: 请解释虚函数、纯虚函数和抽象类的概念。

**答案**:
```cpp
// 1. 虚函数
class Base {
public:
    virtual void virtualFunction() {
        cout << "Base virtual function" << endl;
    }
    
    virtual ~Base() = default;  // 虚析构函数
};

class Derived : public Base {
public:
    void virtualFunction() override {
        cout << "Derived virtual function" << endl;
    }
};

// 2. 纯虚函数和抽象类
class AbstractShape {
public:
    virtual double area() const = 0;      // 纯虚函数
    virtual double perimeter() const = 0; // 纯虚函数
    
    virtual void print() const {          // 虚函数（有默认实现）
        cout << "Shape" << endl;
    }
    
    virtual ~AbstractShape() = default;
};

// 3. 具体类实现
class Circle : public AbstractShape {
private:
    double radius;
    
public:
    Circle(double r) : radius(r) {}
    
    double area() const override {
        return 3.14159 * radius * radius;
    }
    
    double perimeter() const override {
        return 2 * 3.14159 * radius;
    }
    
    void print() const override {
        cout << "Circle with radius " << radius << endl;
    }
};

// 4. 多态使用
void testPolymorphism() {
    Base* basePtr = new Derived();
    basePtr->virtualFunction();  // 调用Derived版本
    
    AbstractShape* shape = new Circle(5.0);
    shape->print();              // 调用Circle版本
    cout << "Area: " << shape->area() << endl;
    
    delete basePtr;
    delete shape;
}
```

### 2. 访问控制
**问题**: 请解释public、protected、private的区别。

**答案**:
```cpp
class Base {
public:
    int publicVar;      // 任何地方都可以访问
    
protected:
    int protectedVar;   // 本类和派生类可以访问
    
private:
    int privateVar;     // 只有本类可以访问
    
public:
    void publicFunc() {
        publicVar = 1;      // 合法
        protectedVar = 2;   // 合法
        privateVar = 3;     // 合法
    }
};

class Derived : public Base {
public:
    void derivedFunc() {
        publicVar = 10;     // 合法
        protectedVar = 20;  // 合法
        // privateVar = 30; // 错误：不能访问基类的private成员
    }
};

class Unrelated {
    void unrelatedFunc() {
        Base base;
        base.publicVar = 100;     // 合法
        // base.protectedVar = 200; // 错误：不能访问protected成员
        // base.privateVar = 300;   // 错误：不能访问private成员
    }
};
```

## 📚 内存管理

### 1. new和delete
**问题**: 请解释new和delete的用法，以及如何避免内存泄漏。

**答案**:
```cpp
// 1. 基本用法
void basicMemoryManagement() {
    // 动态分配单个对象
    int* ptr1 = new int(42);
    delete ptr1;
    
    // 动态分配数组
    int* arr = new int[10];
    delete[] arr;  // 注意：数组用delete[]
    
    // 避免内存泄漏的最佳实践
    try {
        int* ptr2 = new int(100);
        // 使用ptr2
        delete ptr2;
    } catch (...) {
        // 异常处理
        delete ptr2;  // 确保释放内存
        throw;
    }
}

// 2. 使用智能指针（推荐）
void smartPointerUsage() {
    // unique_ptr：独占所有权
    unique_ptr<int> ptr1 = make_unique<int>(42);
    // auto ptr2 = ptr1;  // 错误：不能复制
    
    // shared_ptr：共享所有权
    shared_ptr<int> ptr3 = make_shared<int>(100);
    shared_ptr<int> ptr4 = ptr3;  // 引用计数+1
    
    // weak_ptr：弱引用，不增加引用计数
    weak_ptr<int> ptr5 = ptr3;
    
    // 自动管理内存，无需手动delete
}

// 3. RAII原则
class ResourceManager {
private:
    int* resource;
    
public:
    ResourceManager() : resource(new int(0)) {}
    
    ~ResourceManager() {
        delete resource;  // 析构函数自动释放资源
    }
    
    // 禁用拷贝构造和赋值
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    
    // 支持移动语义
    ResourceManager(ResourceManager&& other) noexcept 
        : resource(other.resource) {
        other.resource = nullptr;
    }
    
    ResourceManager& operator=(ResourceManager&& other) noexcept {
        if (this != &other) {
            delete resource;
            resource = other.resource;
            other.resource = nullptr;
        }
        return *this;
    }
};
```

### 2. 内存泄漏检测
**问题**: 如何检测和避免内存泄漏？

**答案**:
```cpp
// 1. 使用智能指针
class SafeClass {
private:
    unique_ptr<int[]> data;
    
public:
    SafeClass(size_t size) : data(make_unique<int[]>(size)) {}
    // 无需析构函数，智能指针自动管理内存
};

// 2. 自定义内存泄漏检测
class MemoryTracker {
private:
    static map<void*, size_t> allocatedBlocks;
    static size_t totalAllocated;
    
public:
    static void* allocate(size_t size) {
        void* ptr = malloc(size);
        if (ptr) {
            allocatedBlocks[ptr] = size;
            totalAllocated += size;
            cout << "Allocated " << size << " bytes at " << ptr << endl;
        }
        return ptr;
    }
    
    static void deallocate(void* ptr) {
        if (ptr && allocatedBlocks.count(ptr)) {
            size_t size = allocatedBlocks[ptr];
            allocatedBlocks.erase(ptr);
            totalAllocated -= size;
            cout << "Deallocated " << size << " bytes at " << ptr << endl;
            free(ptr);
        }
    }
    
    static void checkLeaks() {
        if (!allocatedBlocks.empty()) {
            cout << "Memory leak detected!" << endl;
            for (const auto& [ptr, size] : allocatedBlocks) {
                cout << "  " << ptr << ": " << size << " bytes" << endl;
            }
        } else {
            cout << "No memory leaks detected." << endl;
        }
    }
};

// 3. 重载new和delete操作符
void* operator new(size_t size) {
    return MemoryTracker::allocate(size);
}

void operator delete(void* ptr) noexcept {
    MemoryTracker::deallocate(ptr);
}
```

## 📚 模板编程

### 1. 函数模板
**问题**: 请实现一个通用的交换函数模板。

**答案**:
```cpp
// 1. 基本函数模板
template<typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// 2. 特化版本
template<>
void swap<int>(int& a, int& b) {
    // 针对int类型的优化实现
    a = a ^ b;
    b = a ^ b;
    a = a ^ b;
}

// 3. 可变参数模板
template<typename T, typename... Args>
T sum(T first, Args... args) {
    if constexpr (sizeof...(args) == 0) {
        return first;
    } else {
        return first + sum(args...);
    }
}

// 4. 类型推导
template<typename T>
auto getValue(T value) -> decltype(value * 2) {
    return value * 2;
}

// 5. 约束模板（C++20）
template<typename T>
requires requires(T t) { t.size(); }
auto getSize(const T& container) {
    return container.size();
}
```

### 2. 类模板
**问题**: 请实现一个简单的栈类模板。

**答案**:
```cpp
template<typename T, size_t MaxSize = 100>
class Stack {
private:
    T data[MaxSize];
    size_t top_index;
    
public:
    Stack() : top_index(0) {}
    
    void push(const T& value) {
        if (top_index < MaxSize) {
            data[top_index++] = value;
        }
    }
    
    T pop() {
        if (top_index > 0) {
            return data[--top_index];
        }
        throw runtime_error("Stack is empty");
    }
    
    T& top() {
        if (top_index > 0) {
            return data[top_index - 1];
        }
        throw runtime_error("Stack is empty");
    }
    
    bool empty() const {
        return top_index == 0;
    }
    
    size_t size() const {
        return top_index;
    }
    
    // 友元函数模板
    template<typename U, size_t N>
    friend bool operator==(const Stack<U, N>& lhs, const Stack<U, N>& rhs);
};

// 友元函数实现
template<typename T, size_t MaxSize>
bool operator==(const Stack<T, MaxSize>& lhs, const Stack<T, MaxSize>& rhs) {
    if (lhs.size() != rhs.size()) return false;
    
    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs.data[i] != rhs.data[i]) return false;
    }
    return true;
}
```

## 💡 面试技巧

### 1. 回答策略
- **STAR方法**: Situation（情况）、Task（任务）、Action（行动）、Result（结果）
- **举例说明**: 用具体例子解释抽象概念
- **代码演示**: 在纸上或白板上写代码

### 2. 常见陷阱
- **指针和引用的混淆**: 明确区分概念
- **内存管理**: 强调RAII和智能指针的使用
- **const正确性**: 说明const的重要性

### 3. 进阶话题
- **移动语义**: 理解右值引用和移动构造函数
- **lambda表达式**: 掌握现代C++特性
- **异常安全**: 了解异常安全保证级别

## 🚀 下一步
- 学习 [数据结构面试题](./data-structure-interview.md)
- 了解 [算法面试题](./algorithm-interview.md)
- 掌握 [系统设计面试题](./system-design.md)

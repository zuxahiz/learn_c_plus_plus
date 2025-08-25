# 现代C++设计模式详解

现代C++（C++11及以后）引入了许多新特性，这些特性催生了新的设计模式，并改进了传统设计模式的实现方式。

## 📋 目录
1. [RAII模式](#raii模式)
2. [CRTP模式](#crtp模式)
3. [移动语义模式](#移动语义模式)
4. [SFINAE模式](#sfinae模式)
5. [模板元编程模式](#模板元编程模式)
6. [线程安全模式](#线程安全模式)
7. [类型擦除模式](#类型擦除模式)

---

## RAII模式

### 🎯 模式意图
Resource Acquisition Is Initialization - 资源获取即初始化。通过对象的生命周期来管理资源，确保资源的自动获取和释放。

### 🤔 为什么需要？
- **自动资源管理** - 避免忘记释放资源
- **异常安全** - 即使发生异常也能正确释放资源
- **简化代码** - 不需要显式的清理代码

### 🏗️ 实现示例

```cpp
#include <iostream>
#include <memory>
#include <fstream>
#include <mutex>
#include <chrono>

// 文件资源RAII管理
class FileHandler {
private:
    std::unique_ptr<std::ifstream> file;
    std::string filename;
    
public:
    explicit FileHandler(const std::string& filename) : filename(filename) {
        file = std::make_unique<std::ifstream>(filename);
        if (!file->is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }
        std::cout << "File opened: " << filename << std::endl;
    }
    
    ~FileHandler() {
        if (file && file->is_open()) {
            file->close();
            std::cout << "File closed: " << filename << std::endl;
        }
    }
    
    // 禁止拷贝
    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;
    
    // 支持移动
    FileHandler(FileHandler&& other) noexcept 
        : file(std::move(other.file)), filename(std::move(other.filename)) {}
    
    FileHandler& operator=(FileHandler&& other) noexcept {
        if (this != &other) {
            file = std::move(other.file);
            filename = std::move(other.filename);
        }
        return *this;
    }
    
    std::string readLine() {
        std::string line;
        if (file && std::getline(*file, line)) {
            return line;
        }
        return "";
    }
    
    bool isOpen() const {
        return file && file->is_open();
    }
};

// 锁管理RAII
class LockGuard {
private:
    std::mutex& mtx;
    
public:
    explicit LockGuard(std::mutex& m) : mtx(m) {
        mtx.lock();
        std::cout << "Mutex locked" << std::endl;
    }
    
    ~LockGuard() {
        mtx.unlock();
        std::cout << "Mutex unlocked" << std::endl;
    }
    
    // 禁止拷贝和移动
    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;
    LockGuard(LockGuard&&) = delete;
    LockGuard& operator=(LockGuard&&) = delete;
};

// 计时器RAII
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start;
    std::string name;
    
public:
    explicit Timer(const std::string& name) : name(name) {
        start = std::chrono::high_resolution_clock::now();
        std::cout << "Timer '" << name << "' started" << std::endl;
    }
    
    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Timer '" << name << "' finished: " 
                  << duration.count() << "ms" << std::endl;
    }
};

// 自定义资源RAII包装器
template<typename Resource, typename Deleter>
class RAIIWrapper {
private:
    Resource resource;
    Deleter deleter;
    bool valid;
    
public:
    RAIIWrapper(Resource res, Deleter del) 
        : resource(res), deleter(del), valid(true) {}
    
    ~RAIIWrapper() {
        if (valid) {
            deleter(resource);
        }
    }
    
    // 禁止拷贝
    RAIIWrapper(const RAIIWrapper&) = delete;
    RAIIWrapper& operator=(const RAIIWrapper&) = delete;
    
    // 支持移动
    RAIIWrapper(RAIIWrapper&& other) noexcept
        : resource(other.resource), deleter(std::move(other.deleter)), valid(other.valid) {
        other.valid = false;
    }
    
    Resource get() const { return resource; }
    Resource release() {
        valid = false;
        return resource;
    }
};

// 使用示例
void demonstrateRAII() {
    std::cout << "=== RAII Pattern Demo ===" << std::endl;
    
    // 文件资源管理
    try {
        Timer timer("File Processing");
        FileHandler file("example.txt");
        
        if (file.isOpen()) {
            std::string line = file.readLine();
            std::cout << "Read: " << line << std::endl;
        }
        // 文件会在作用域结束时自动关闭
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    
    // 自定义资源管理
    auto handle = RAIIWrapper<int*, void(*)(int*)>(
        new int(42),
        [](int* ptr) { 
            std::cout << "Deleting resource: " << *ptr << std::endl;
            delete ptr; 
        }
    );
    
    std::cout << "Resource value: " << *handle.get() << std::endl;
}
```

---

## CRTP模式

### 🎯 模式意图
Curiously Recurring Template Pattern - 奇异递归模板模式。基类是以派生类为模板参数的模板类，实现编译时多态。

### 🤔 为什么需要？
- **编译时多态** - 避免虚函数的运行时开销
- **代码复用** - 在基类中实现通用功能
- **类型安全** - 编译时检查类型

### 🏗️ 实现示例

```cpp
#include <iostream>
#include <vector>
#include <type_traits>

// CRTP基类
template<typename Derived>
class Shape {
public:
    double area() const {
        return static_cast<const Derived*>(this)->areaImpl();
    }
    
    double perimeter() const {
        return static_cast<const Derived*>(this)->perimeterImpl();
    }
    
    void draw() const {
        static_cast<const Derived*>(this)->drawImpl();
    }
    
    // 提供默认实现的接口
    std::string getInfo() const {
        return "Shape: area=" + std::to_string(area()) + 
               ", perimeter=" + std::to_string(perimeter());
    }
    
    // 编译时检查派生类是否实现了必要的方法
    static_assert(std::is_base_of_v<Shape<Derived>, Derived>, 
                  "Derived must inherit from Shape<Derived>");

protected:
    Shape() = default;
    ~Shape() = default;
};

// 派生类：圆形
class Circle : public Shape<Circle> {
private:
    double radius;
    
public:
    Circle(double r) : radius(r) {}
    
    double areaImpl() const {
        return 3.14159 * radius * radius;
    }
    
    double perimeterImpl() const {
        return 2 * 3.14159 * radius;
    }
    
    void drawImpl() const {
        std::cout << "Drawing Circle with radius " << radius << std::endl;
    }
};

// 派生类：矩形
class Rectangle : public Shape<Rectangle> {
private:
    double width, height;
    
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    
    double areaImpl() const {
        return width * height;
    }
    
    double perimeterImpl() const {
        return 2 * (width + height);
    }
    
    void drawImpl() const {
        std::cout << "Drawing Rectangle " << width << "x" << height << std::endl;
    }
};

// CRTP计数器 - 统计每个类的实例数量
template<typename T>
class Counter {
private:
    static inline int count = 0;
    
protected:
    Counter() { ++count; }
    Counter(const Counter&) { ++count; }
    Counter(Counter&&) { ++count; }
    ~Counter() { --count; }
    
public:
    static int getCount() { return count; }
};

// 使用计数器的类
class Widget : public Counter<Widget> {
private:
    std::string name;
    
public:
    Widget(const std::string& n) : name(n) {
        std::cout << "Widget '" << name << "' created. Total: " << getCount() << std::endl;
    }
    
    ~Widget() {
        std::cout << "Widget '" << name << "' destroyed. Remaining: " << getCount() - 1 << std::endl;
    }
};

// CRTP单例模式
template<typename T>
class Singleton {
protected:
    Singleton() = default;
    ~Singleton() = default;
    
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;
    
    static T& getInstance() {
        static T instance;
        return instance;
    }
};

// 使用CRTP单例的类
class Logger : public Singleton<Logger> {
    friend class Singleton<Logger>;  // 允许基类访问私有构造函数
    
private:
    Logger() = default;
    
public:
    void log(const std::string& message) {
        std::cout << "[LOG] " << message << std::endl;
    }
};

// 模板函数：处理任何Shape派生类
template<typename ShapeType>
void processShape(const Shape<ShapeType>& shape) {
    shape.draw();
    std::cout << shape.getInfo() << std::endl;
    std::cout << std::endl;
}

// 使用示例
void demonstrateCRTP() {
    std::cout << "=== CRTP Pattern Demo ===" << std::endl;
    
    // 形状示例
    Circle circle(5.0);
    Rectangle rectangle(4.0, 6.0);
    
    processShape(circle);
    processShape(rectangle);
    
    // 计数器示例
    std::cout << "Widget count: " << Widget::getCount() << std::endl;
    {
        Widget w1("First");
        Widget w2("Second");
        std::cout << "Widget count: " << Widget::getCount() << std::endl;
        
        {
            Widget w3("Third");
            std::cout << "Widget count: " << Widget::getCount() << std::endl;
        }
        
        std::cout << "Widget count after scope: " << Widget::getCount() << std::endl;
    }
    std::cout << "Final widget count: " << Widget::getCount() << std::endl;
    
    // 单例示例
    Logger& logger = Logger::getInstance();
    logger.log("CRTP Singleton working!");
    
    Logger& logger2 = Logger::getInstance();
    std::cout << "Same instance: " << (&logger == &logger2) << std::endl;
}
```

---

## 移动语义模式

### 🎯 模式意图
利用C++11引入的移动语义，避免不必要的深拷贝，提高性能。

### 🏗️ 实现示例

```cpp
#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <string>

// 移动优化的资源管理类
class Buffer {
private:
    std::unique_ptr<char[]> data;
    size_t size;
    std::string name;
    
public:
    // 构造函数
    Buffer(size_t size, const std::string& name) 
        : data(std::make_unique<char[]>(size)), size(size), name(name) {
        std::cout << "Buffer '" << name << "' created with size " << size << std::endl;
    }
    
    // 拷贝构造函数（深拷贝）
    Buffer(const Buffer& other) 
        : data(std::make_unique<char[]>(other.size)), size(other.size), 
          name(other.name + "_copy") {
        std::copy(other.data.get(), other.data.get() + size, data.get());
        std::cout << "Buffer copied: " << other.name << " -> " << name << std::endl;
    }
    
    // 移动构造函数（资源转移）
    Buffer(Buffer&& other) noexcept 
        : data(std::move(other.data)), size(other.size), name(std::move(other.name)) {
        other.size = 0;
        std::cout << "Buffer moved: " << name << std::endl;
    }
    
    // 拷贝赋值操作符
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            data = std::make_unique<char[]>(other.size);
            size = other.size;
            name = other.name + "_assigned";
            std::copy(other.data.get(), other.data.get() + size, data.get());
            std::cout << "Buffer copy assigned: " << other.name << " -> " << name << std::endl;
        }
        return *this;
    }
    
    // 移动赋值操作符
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
            size = other.size;
            name = std::move(other.name);
            other.size = 0;
            std::cout << "Buffer move assigned: " << name << std::endl;
        }
        return *this;
    }
    
    ~Buffer() {
        if (data) {
            std::cout << "Buffer '" << name << "' destroyed" << std::endl;
        }
    }
    
    size_t getSize() const { return size; }
    const std::string& getName() const { return name; }
    char* getData() const { return data.get(); }
};

// 工厂函数（返回移动语义）
class BufferFactory {
public:
    static Buffer createBuffer(size_t size, const std::string& name) {
        return Buffer(size, name);  // 移动返回
    }
    
    static std::vector<Buffer> createBuffers(const std::vector<size_t>& sizes) {
        std::vector<Buffer> buffers;
        buffers.reserve(sizes.size());  // 预分配空间避免重新分配
        
        for (size_t i = 0; i < sizes.size(); ++i) {
            buffers.emplace_back(sizes[i], "buffer_" + std::to_string(i));
        }
        
        return buffers;  // 移动返回
    }
};

// 完美转发示例
template<typename T>
class Wrapper {
private:
    T value;
    
public:
    template<typename U>
    Wrapper(U&& val) : value(std::forward<U>(val)) {
        std::cout << "Wrapper created with perfect forwarding" << std::endl;
    }
    
    T& get() { return value; }
    const T& get() const { return value; }
};

// 移动语义的容器包装器
template<typename Container>
class MoveOptimizedContainer {
private:
    Container container;
    
public:
    // 完美转发构造
    template<typename... Args>
    MoveOptimizedContainer(Args&&... args) 
        : container(std::forward<Args>(args)...) {}
    
    // 移动插入
    template<typename T>
    void push_back(T&& item) {
        container.push_back(std::forward<T>(item));
    }
    
    // 原地构造
    template<typename... Args>
    void emplace_back(Args&&... args) {
        container.emplace_back(std::forward<Args>(args)...);
    }
    
    // 移动访问
    Container&& move_container() {
        return std::move(container);
    }
    
    size_t size() const { return container.size(); }
    
    auto begin() { return container.begin(); }
    auto end() { return container.end(); }
    auto begin() const { return container.begin(); }
    auto end() const { return container.end(); }
};

// 使用示例
void demonstrateMoveSemantics() {
    std::cout << "=== Move Semantics Pattern Demo ===" << std::endl;
    
    // 基本移动语义
    std::cout << "\n1. Basic Move Semantics:" << std::endl;
    Buffer buffer1(1024, "original");
    Buffer buffer2 = std::move(buffer1);  // 移动构造
    
    std::cout << "\n2. Factory with Move:" << std::endl;
    auto buffer3 = BufferFactory::createBuffer(2048, "factory_created");
    
    std::cout << "\n3. Container with Move:" << std::endl;
    auto buffers = BufferFactory::createBuffers({512, 1024, 2048});
    std::cout << "Created " << buffers.size() << " buffers" << std::endl;
    
    std::cout << "\n4. Perfect Forwarding:" << std::endl;
    Wrapper<Buffer> wrapper1(Buffer(256, "wrapped"));  // 移动构造
    Wrapper<std::string> wrapper2(std::string("Hello"));  // 移动构造
    
    std::cout << "\n5. Move-Optimized Container:" << std::endl;
    MoveOptimizedContainer<std::vector<Buffer>> container;
    container.emplace_back(128, "container_item1");
    container.push_back(Buffer(256, "container_item2"));
    
    std::cout << "Container size: " << container.size() << std::endl;
    
    // 移动整个容器
    auto moved_container = container.move_container();
    std::cout << "Moved container size: " << moved_container.size() << std::endl;
}
```

---

## SFINAE模式

### 🎯 模式意图
Substitution Failure Is Not An Error - 替换失败不是错误。利用模板参数推导失败时编译器不报错的特性，实现编译时条件编译。

### 🤔 为什么需要？
- **条件编译** - 根据类型特性选择不同的实现
- **类型检测** - 检测类型是否具有某些特性
- **重载解析** - 基于类型特性进行函数重载

### 🏗️ 实现示例

```cpp
#include <iostream>
#include <type_traits>
#include <vector>

// 检测类型是否有begin()方法
template<typename T, typename = void>
struct has_begin : std::false_type {};

template<typename T>
struct has_begin<T, std::void_t<decltype(std::declval<T>().begin())>> : std::true_type {};

// 使用SFINAE的函数重载
template<typename T>
std::enable_if_t<has_begin<T>::value, void>
print_container(const T& container) {
    std::cout << "Container with " << container.size() << " elements" << std::endl;
}

template<typename T>
std::enable_if_t<!has_begin<T>::value, void>
print_container(const T& item) {
    std::cout << "Single item: " << item << std::endl;
}

void demonstrateSFINAE() {
    std::vector<int> vec{1, 2, 3};
    print_container(vec);
    print_container(42);
}
```

---

## 模板元编程模式

### 🎯 模式意图
使用模板在编译时进行计算和代码生成，实现高效的编译时编程。

### 🏗️ 实现示例

```cpp
// 编译时阶乘计算
template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

void demonstrateMetaprogramming() {
    constexpr int fact5 = Factorial<5>::value;
    std::cout << "5! = " << fact5 << std::endl;
}
```

---

## 线程安全模式

### 🎯 模式意图
在多线程环境下确保数据的安全访问和操作。

### 🏗️ 实现示例

```cpp
#include <mutex>
#include <atomic>

class ThreadSafeCounter {
private:
    std::atomic<int> counter_{0};
    
public:
    int increment() { return counter_.fetch_add(1) + 1; }
    int get() const { return counter_.load(); }
};

void demonstrateThreadSafety() {
    ThreadSafeCounter counter;
    std::cout << "Counter: " << counter.increment() << std::endl;
}
```

---

## 类型擦除模式

### 🎯 模式意图
隐藏具体类型信息，提供统一的接口。

### 🏗️ 实现示例

```cpp
#include <memory>

class Any {
private:
    struct Base {
        virtual ~Base() = default;
        virtual std::unique_ptr<Base> clone() const = 0;
    };
    
    template<typename T>
    struct Derived : Base {
        T value;
        Derived(T v) : value(std::move(v)) {}
        std::unique_ptr<Base> clone() const override {
            return std::make_unique<Derived<T>>(value);
        }
    };
    
    std::unique_ptr<Base> ptr_;
    
public:
    template<typename T>
    Any(T value) : ptr_(std::make_unique<Derived<T>>(std::move(value))) {}
    
    template<typename T>
    T& cast() {
        auto derived = dynamic_cast<Derived<T>*>(ptr_.get());
        if (!derived) throw std::bad_cast();
        return derived->value;
    }
};

void demonstrateTypeErasure() {
    Any any_int(42);
    Any any_string(std::string("Hello"));
    
    std::cout << "Int: " << any_int.cast<int>() << std::endl;
    std::cout << "String: " << any_string.cast<std::string>() << std::endl;
}
```

这些现代C++模式充分利用了C++11及以后版本的语言特性，为我们提供了更高效、更安全的编程方式！

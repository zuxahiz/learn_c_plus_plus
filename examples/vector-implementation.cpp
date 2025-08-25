/**
 * @file vector-implementation.cpp
 * @brief 自定义Vector容器的完整实现
 * @author C++ Learning Guide
 * @date 2024
 * 
 * 这个文件实现了一个简化版的std::vector，包含主要功能：
 * - 动态内存管理
 * - 自动扩容
 * - 迭代器支持
 * - 异常安全
 */

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>
#include <memory>

template<typename T>
class Vector {
public:
    // 类型定义
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    
    // 迭代器类型
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    T* data_;           // 数据指针
    size_type size_;    // 当前元素数量
    size_type capacity_; // 容量大小

    // 内存管理辅助函数
    void deallocate() {
        if (data_) {
            // 调用析构函数
            for (size_type i = 0; i < size_; ++i) {
                data_[i].~T();
            }
            // 释放内存
            operator delete(data_);
            data_ = nullptr;
        }
    }

    void reallocate(size_type new_capacity) {
        // 分配新内存
        T* new_data = static_cast<T*>(operator new(new_capacity * sizeof(T)));
        
        size_type new_size = std::min(size_, new_capacity);
        
        try {
            // 移动或复制旧元素到新内存
            for (size_type i = 0; i < new_size; ++i) {
                new (new_data + i) T(std::move(data_[i]));
            }
        } catch (...) {
            // 异常安全：释放新分配的内存
            operator delete(new_data);
            throw;
        }
        
        // 释放旧内存
        deallocate();
        
        // 更新指针和容量
        data_ = new_data;
        size_ = new_size;
        capacity_ = new_capacity;
    }

public:
    // 构造函数
    Vector() : data_(nullptr), size_(0), capacity_(0) {}
    
    explicit Vector(size_type count) : data_(nullptr), size_(0), capacity_(0) {
        resize(count);
    }
    
    Vector(size_type count, const T& value) : data_(nullptr), size_(0), capacity_(0) {
        assign(count, value);
    }
    
    Vector(std::initializer_list<T> init) : data_(nullptr), size_(0), capacity_(0) {
        assign(init);
    }
    
    template<typename InputIt>
    Vector(InputIt first, InputIt last) : data_(nullptr), size_(0), capacity_(0) {
        assign(first, last);
    }

    // 拷贝构造函数
    Vector(const Vector& other) : data_(nullptr), size_(0), capacity_(0) {
        assign(other.begin(), other.end());
    }

    // 移动构造函数
    Vector(Vector&& other) noexcept 
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    // 析构函数
    ~Vector() {
        deallocate();
    }

    // 赋值操作符
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            assign(other.begin(), other.end());
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            deallocate();
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    Vector& operator=(std::initializer_list<T> init) {
        assign(init);
        return *this;
    }

    // assign 方法
    void assign(size_type count, const T& value) {
        clear();
        if (count > capacity_) {
            reallocate(count);
        }
        
        for (size_type i = 0; i < count; ++i) {
            new (data_ + i) T(value);
        }
        size_ = count;
    }

    void assign(std::initializer_list<T> init) {
        assign(init.begin(), init.end());
    }

    template<typename InputIt>
    void assign(InputIt first, InputIt last) {
        clear();
        
        // 计算距离（如果是随机访问迭代器）
        size_type count = std::distance(first, last);
        if (count > capacity_) {
            reallocate(count);
        }
        
        size_type i = 0;
        for (auto it = first; it != last; ++it, ++i) {
            new (data_ + i) T(*it);
        }
        size_ = count;
    }

    // 元素访问
    reference at(size_type pos) {
        if (pos >= size_) {
            throw std::out_of_range("Vector::at: index out of range");
        }
        return data_[pos];
    }

    const_reference at(size_type pos) const {
        if (pos >= size_) {
            throw std::out_of_range("Vector::at: index out of range");
        }
        return data_[pos];
    }

    reference operator[](size_type pos) {
        return data_[pos];
    }

    const_reference operator[](size_type pos) const {
        return data_[pos];
    }

    reference front() {
        if (empty()) {
            throw std::runtime_error("Vector::front: container is empty");
        }
        return data_[0];
    }

    const_reference front() const {
        if (empty()) {
            throw std::runtime_error("Vector::front: container is empty");
        }
        return data_[0];
    }

    reference back() {
        if (empty()) {
            throw std::runtime_error("Vector::back: container is empty");
        }
        return data_[size_ - 1];
    }

    const_reference back() const {
        if (empty()) {
            throw std::runtime_error("Vector::back: container is empty");
        }
        return data_[size_ - 1];
    }

    T* data() noexcept {
        return data_;
    }

    const T* data() const noexcept {
        return data_;
    }

    // 迭代器
    iterator begin() noexcept {
        return data_;
    }

    const_iterator begin() const noexcept {
        return data_;
    }

    const_iterator cbegin() const noexcept {
        return data_;
    }

    iterator end() noexcept {
        return data_ + size_;
    }

    const_iterator end() const noexcept {
        return data_ + size_;
    }

    const_iterator cend() const noexcept {
        return data_ + size_;
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(begin());
    }

    // 容量
    bool empty() const noexcept {
        return size_ == 0;
    }

    size_type size() const noexcept {
        return size_;
    }

    size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

    void reserve(size_type new_cap) {
        if (new_cap > capacity_) {
            reallocate(new_cap);
        }
    }

    size_type capacity() const noexcept {
        return capacity_;
    }

    void shrink_to_fit() {
        if (size_ < capacity_) {
            reallocate(size_);
        }
    }

    // 修改器
    void clear() noexcept {
        for (size_type i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = 0;
    }

    iterator insert(const_iterator pos, const T& value) {
        return insert(pos, 1, value);
    }

    iterator insert(const_iterator pos, T&& value) {
        size_type index = pos - begin();
        
        if (size_ == capacity_) {
            size_type new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
            reallocate(new_capacity);
        }
        
        // 移动元素
        for (size_type i = size_; i > index; --i) {
            new (data_ + i) T(std::move(data_[i - 1]));
            data_[i - 1].~T();
        }
        
        new (data_ + index) T(std::move(value));
        ++size_;
        
        return begin() + index;
    }

    iterator insert(const_iterator pos, size_type count, const T& value) {
        if (count == 0) {
            return const_cast<iterator>(pos);
        }
        
        size_type index = pos - begin();
        
        if (size_ + count > capacity_) {
            size_type new_capacity = std::max(capacity_ * 2, size_ + count);
            reallocate(new_capacity);
        }
        
        // 移动现有元素
        for (size_type i = size_ + count - 1; i >= index + count; --i) {
            new (data_ + i) T(std::move(data_[i - count]));
            data_[i - count].~T();
        }
        
        // 插入新元素
        for (size_type i = 0; i < count; ++i) {
            new (data_ + index + i) T(value);
        }
        
        size_ += count;
        return begin() + index;
    }

    template<typename InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last) {
        size_type count = std::distance(first, last);
        if (count == 0) {
            return const_cast<iterator>(pos);
        }
        
        size_type index = pos - begin();
        
        if (size_ + count > capacity_) {
            size_type new_capacity = std::max(capacity_ * 2, size_ + count);
            reallocate(new_capacity);
        }
        
        // 移动现有元素
        for (size_type i = size_ + count - 1; i >= index + count; --i) {
            new (data_ + i) T(std::move(data_[i - count]));
            data_[i - count].~T();
        }
        
        // 插入新元素
        size_type i = index;
        for (auto it = first; it != last; ++it, ++i) {
            new (data_ + i) T(*it);
        }
        
        size_ += count;
        return begin() + index;
    }

    iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
        return insert(pos, ilist.begin(), ilist.end());
    }

    template<typename... Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        size_type index = pos - begin();
        
        if (size_ == capacity_) {
            size_type new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
            reallocate(new_capacity);
        }
        
        // 移动元素
        for (size_type i = size_; i > index; --i) {
            new (data_ + i) T(std::move(data_[i - 1]));
            data_[i - 1].~T();
        }
        
        new (data_ + index) T(std::forward<Args>(args)...);
        ++size_;
        
        return begin() + index;
    }

    iterator erase(const_iterator pos) {
        return erase(pos, pos + 1);
    }

    iterator erase(const_iterator first, const_iterator last) {
        size_type start_index = first - begin();
        size_type end_index = last - begin();
        size_type count = end_index - start_index;
        
        if (count == 0) {
            return const_cast<iterator>(first);
        }
        
        // 析构要删除的元素
        for (size_type i = start_index; i < end_index; ++i) {
            data_[i].~T();
        }
        
        // 移动后面的元素
        for (size_type i = end_index; i < size_; ++i) {
            new (data_ + i - count) T(std::move(data_[i]));
            data_[i].~T();
        }
        
        size_ -= count;
        return begin() + start_index;
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            size_type new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
            reallocate(new_capacity);
        }
        
        new (data_ + size_) T(value);
        ++size_;
    }

    void push_back(T&& value) {
        if (size_ == capacity_) {
            size_type new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
            reallocate(new_capacity);
        }
        
        new (data_ + size_) T(std::move(value));
        ++size_;
    }

    template<typename... Args>
    reference emplace_back(Args&&... args) {
        if (size_ == capacity_) {
            size_type new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
            reallocate(new_capacity);
        }
        
        new (data_ + size_) T(std::forward<Args>(args)...);
        ++size_;
        
        return back();
    }

    void pop_back() {
        if (empty()) {
            throw std::runtime_error("Vector::pop_back: container is empty");
        }
        
        --size_;
        data_[size_].~T();
    }

    void resize(size_type count) {
        resize(count, T());
    }

    void resize(size_type count, const T& value) {
        if (count > capacity_) {
            reallocate(count);
        }
        
        if (count > size_) {
            // 构造新元素
            for (size_type i = size_; i < count; ++i) {
                new (data_ + i) T(value);
            }
        } else if (count < size_) {
            // 析构多余元素
            for (size_type i = count; i < size_; ++i) {
                data_[i].~T();
            }
        }
        
        size_ = count;
    }

    void swap(Vector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }
};

// 非成员函数
template<typename T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs) {
    return lhs.size() == rhs.size() && 
           std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(lhs == rhs);
}

template<typename T>
bool operator<(const Vector<T>& lhs, const Vector<T>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                      rhs.begin(), rhs.end());
}

template<typename T>
bool operator<=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(rhs < lhs);
}

template<typename T>
bool operator>(const Vector<T>& lhs, const Vector<T>& rhs) {
    return rhs < lhs;
}

template<typename T>
bool operator>=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(lhs < rhs);
}

template<typename T>
void swap(Vector<T>& lhs, Vector<T>& rhs) noexcept {
    lhs.swap(rhs);
}

// 测试函数
void testVector() {
    std::cout << "=== Vector Implementation Test ===" << std::endl;
    
    // 测试构造函数
    std::cout << "\n1. Testing constructors:" << std::endl;
    Vector<int> v1;
    std::cout << "Default constructor - size: " << v1.size() << ", capacity: " << v1.capacity() << std::endl;
    
    Vector<int> v2(5, 10);
    std::cout << "Fill constructor - size: " << v2.size() << ", values: ";
    for (const auto& val : v2) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    Vector<int> v3{1, 2, 3, 4, 5};
    std::cout << "Initializer list - size: " << v3.size() << ", values: ";
    for (const auto& val : v3) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 测试拷贝构造
    Vector<int> v4(v3);
    std::cout << "Copy constructor - size: " << v4.size() << ", values: ";
    for (const auto& val : v4) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 测试push_back和自动扩容
    std::cout << "\n2. Testing push_back and auto-expansion:" << std::endl;
    Vector<int> v5;
    for (int i = 0; i < 10; ++i) {
        v5.push_back(i);
        std::cout << "After push_back(" << i << ") - size: " << v5.size() 
                  << ", capacity: " << v5.capacity() << std::endl;
    }
    
    // 测试元素访问
    std::cout << "\n3. Testing element access:" << std::endl;
    std::cout << "v5[0] = " << v5[0] << std::endl;
    std::cout << "v5.at(5) = " << v5.at(5) << std::endl;
    std::cout << "v5.front() = " << v5.front() << std::endl;
    std::cout << "v5.back() = " << v5.back() << std::endl;
    
    // 测试insert
    std::cout << "\n4. Testing insert:" << std::endl;
    auto it = v5.insert(v5.begin() + 2, 100);
    std::cout << "After insert(100) at position 2: ";
    for (const auto& val : v5) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 测试erase
    std::cout << "\n5. Testing erase:" << std::endl;
    v5.erase(v5.begin() + 2);
    std::cout << "After erase at position 2: ";
    for (const auto& val : v5) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 测试pop_back
    std::cout << "\n6. Testing pop_back:" << std::endl;
    v5.pop_back();
    std::cout << "After pop_back: ";
    for (const auto& val : v5) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 测试resize
    std::cout << "\n7. Testing resize:" << std::endl;
    v5.resize(15, 99);
    std::cout << "After resize(15, 99): ";
    for (const auto& val : v5) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 测试迭代器
    std::cout << "\n8. Testing iterators:" << std::endl;
    std::cout << "Forward iteration: ";
    for (auto it = v5.begin(); it != v5.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Reverse iteration: ";
    for (auto it = v5.rbegin(); it != v5.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // 测试clear
    std::cout << "\n9. Testing clear:" << std::endl;
    v5.clear();
    std::cout << "After clear - size: " << v5.size() << ", capacity: " << v5.capacity() << std::endl;
    
    // 测试异常情况
    std::cout << "\n10. Testing exceptions:" << std::endl;
    try {
        Vector<int> empty_vec;
        empty_vec.at(0);  // 应该抛出异常
    } catch (const std::exception& e) {
        std::cout << "Caught expected exception: " << e.what() << std::endl;
    }
    
    std::cout << "\n=== Vector Test Completed ===" << std::endl;
}

// 性能测试
void performanceTest() {
    std::cout << "\n=== Performance Test ===" << std::endl;
    
    const size_t N = 1000000;
    
    // 测试push_back性能
    auto start = std::chrono::high_resolution_clock::now();
    
    Vector<int> v;
    for (size_t i = 0; i < N; ++i) {
        v.push_back(static_cast<int>(i));
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Push back " << N << " elements took: " << duration.count() << " ms" << std::endl;
    std::cout << "Final size: " << v.size() << ", capacity: " << v.capacity() << std::endl;
    
    // 测试随机访问性能
    start = std::chrono::high_resolution_clock::now();
    
    long long sum = 0;
    for (size_t i = 0; i < N; ++i) {
        sum += v[i];
    }
    
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Random access " << N << " elements took: " << duration.count() << " ms" << std::endl;
    std::cout << "Sum: " << sum << std::endl;
}

int main() {
    try {
        testVector();
        performanceTest();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

# C++ 栈和队列实现详解

## 🎯 什么是栈和队列？

### 栈 (Stack)
- **LIFO原则**: 后进先出(Last In First Out)
- **基本操作**: push(入栈)、pop(出栈)、top(查看栈顶)、empty(判空)、size(大小)
- **应用场景**: 函数调用栈、括号匹配、深度优先搜索、撤销操作

### 队列 (Queue)
- **FIFO原则**: 先进先出(First In First Out)
- **基本操作**: push(入队)、pop(出队)、front(查看队首)、back(查看队尾)、empty(判空)、size(大小)
- **应用场景**: 广度优先搜索、任务调度、缓冲区、打印队列

## 📚 栈的实现

### 1. 基于数组的栈实现
```cpp
#include <iostream>
#include <stdexcept>
using namespace std;

template<typename T>
class ArrayStack {
private:
    T* data;                    // 数据数组
    size_t capacity;            // 容量
    size_t top_index;           // 栈顶索引
    
public:
    // 构造函数
    ArrayStack(size_t initial_capacity = 10) 
        : capacity(initial_capacity), top_index(0) {
        data = new T[capacity];
    }
    
    // 拷贝构造函数
    ArrayStack(const ArrayStack& other) 
        : capacity(other.capacity), top_index(other.top_index) {
        data = new T[capacity];
        for (size_t i = 0; i < top_index; ++i) {
            data[i] = other.data[i];
        }
    }
    
    // 赋值操作符
    ArrayStack& operator=(const ArrayStack& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            top_index = other.top_index;
            data = new T[capacity];
            for (size_t i = 0; i < top_index; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    
    // 析构函数
    ~ArrayStack() {
        delete[] data;
    }
    
    // 入栈
    void push(const T& value) {
        if (top_index >= capacity) {
            expand();
        }
        data[top_index++] = value;
    }
    
    // 出栈
    void pop() {
        if (empty()) {
            throw runtime_error("Stack is empty");
        }
        --top_index;
    }
    
    // 查看栈顶元素
    T& top() {
        if (empty()) {
            throw runtime_error("Stack is empty");
        }
        return data[top_index - 1];
    }
    
    const T& top() const {
        if (empty()) {
            throw runtime_error("Stack is empty");
        }
        return data[top_index - 1];
    }
    
    // 检查是否为空
    bool empty() const {
        return top_index == 0;
    }
    
    // 获取大小
    size_t size() const {
        return top_index;
    }
    
    // 获取容量
    size_t getCapacity() const {
        return capacity;
    }
    
    // 清空栈
    void clear() {
        top_index = 0;
    }
    
private:
    // 扩容
    void expand() {
        size_t new_capacity = capacity * 2;
        T* new_data = new T[new_capacity];
        
        for (size_t i = 0; i < top_index; ++i) {
            new_data[i] = data[i];
        }
        
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }
};
```

### 2. 基于链表的栈实现
```cpp
#include <iostream>
#include <stdexcept>
using namespace std;

template<typename T>
struct StackNode {
    T data;
    StackNode<T>* next;
    
    StackNode(const T& value) : data(value), next(nullptr) {}
};

template<typename T>
class LinkedListStack {
private:
    StackNode<T>* top_node;
    size_t size_;
    
public:
    // 构造函数
    LinkedListStack() : top_node(nullptr), size_(0) {}
    
    // 拷贝构造函数
    LinkedListStack(const LinkedListStack& other) : top_node(nullptr), size_(0) {
        if (!other.empty()) {
            StackNode<T>* current = other.top_node;
            StackNode<T>* prev = nullptr;
            
            while (current != nullptr) {
                StackNode<T>* new_node = new StackNode<T>(current->data);
                if (prev == nullptr) {
                    top_node = new_node;
                } else {
                    prev->next = new_node;
                }
                prev = new_node;
                current = current->next;
                ++size_;
            }
        }
    }
    
    // 赋值操作符
    LinkedListStack& operator=(const LinkedListStack& other) {
        if (this != &other) {
            clear();
            if (!other.empty()) {
                StackNode<T>* current = other.top_node;
                StackNode<T>* prev = nullptr;
                
                while (current != nullptr) {
                    StackNode<T>* new_node = new StackNode<T>(current->data);
                    if (prev == nullptr) {
                        top_node = new_node;
                    } else {
                        prev->next = new_node;
                    }
                    prev = new_node;
                    current = current->next;
                    ++size_;
                }
            }
        }
        return *this;
    }
    
    // 析构函数
    ~LinkedListStack() {
        clear();
    }
    
    // 入栈
    void push(const T& value) {
        StackNode<T>* new_node = new StackNode<T>(value);
        new_node->next = top_node;
        top_node = new_node;
        ++size_;
    }
    
    // 出栈
    void pop() {
        if (empty()) {
            throw runtime_error("Stack is empty");
        }
        
        StackNode<T>* temp = top_node;
        top_node = top_node->next;
        delete temp;
        --size_;
    }
    
    // 查看栈顶元素
    T& top() {
        if (empty()) {
            throw runtime_error("Stack is empty");
        }
        return top_node->data;
    }
    
    const T& top() const {
        if (empty()) {
            throw runtime_error("Stack is empty");
        }
        return top_node->data;
    }
    
    // 检查是否为空
    bool empty() const {
        return top_node == nullptr;
    }
    
    // 获取大小
    size_t size() const {
        return size_;
    }
    
    // 清空栈
    void clear() {
        while (!empty()) {
            pop();
        }
    }
};
```

## 📚 队列的实现

### 1. 基于数组的循环队列实现
```cpp
#include <iostream>
#include <stdexcept>
using namespace std;

template<typename T>
class CircularQueue {
private:
    T* data;                    // 数据数组
    size_t capacity;            // 容量
    size_t front_index;         // 队首索引
    size_t back_index;          // 队尾索引
    size_t size_;               // 当前大小
    bool is_full;               // 是否满的标志
    
public:
    // 构造函数
    CircularQueue(size_t initial_capacity = 10) 
        : capacity(initial_capacity), front_index(0), back_index(0), 
          size_(0), is_full(false) {
        data = new T[capacity];
    }
    
    // 拷贝构造函数
    CircularQueue(const CircularQueue& other) 
        : capacity(other.capacity), front_index(other.front_index), 
          back_index(other.back_index), size_(other.size_), is_full(other.is_full) {
        data = new T[capacity];
        for (size_t i = 0; i < capacity; ++i) {
            data[i] = other.data[i];
        }
    }
    
    // 赋值操作符
    CircularQueue& operator=(const CircularQueue& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            front_index = other.front_index;
            back_index = other.back_index;
            size_ = other.size_;
            is_full = other.is_full;
            
            data = new T[capacity];
            for (size_t i = 0; i < capacity; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    
    // 析构函数
    ~CircularQueue() {
        delete[] data;
    }
    
    // 入队
    void push(const T& value) {
        if (is_full) {
            expand();
        }
        
        data[back_index] = value;
        back_index = (back_index + 1) % capacity;
        ++size_;
        
        if (back_index == front_index) {
            is_full = true;
        }
    }
    
    // 出队
    void pop() {
        if (empty()) {
            throw runtime_error("Queue is empty");
        }
        
        front_index = (front_index + 1) % capacity;
        --size_;
        is_full = false;
    }
    
    // 查看队首元素
    T& front() {
        if (empty()) {
            throw runtime_error("Queue is empty");
        }
        return data[front_index];
    }
    
    const T& front() const {
        if (empty()) {
            throw runtime_error("Queue is empty");
        }
        return data[front_index];
    }
    
    // 查看队尾元素
    T& back() {
        if (empty()) {
            throw runtime_error("Queue is empty");
        }
        size_t last_index = (back_index - 1 + capacity) % capacity;
        return data[last_index];
    }
    
    const T& back() const {
        if (empty()) {
            throw runtime_error("Queue is empty");
        }
        size_t last_index = (back_index - 1 + capacity) % capacity;
        return data[last_index];
    }
    
    // 检查是否为空
    bool empty() const {
        return size_ == 0;
    }
    
    // 检查是否满
    bool full() const {
        return is_full;
    }
    
    // 获取大小
    size_t size() const {
        return size_;
    }
    
    // 获取容量
    size_t getCapacity() const {
        return capacity;
    }
    
    // 清空队列
    void clear() {
        front_index = 0;
        back_index = 0;
        size_ = 0;
        is_full = false;
    }
    
private:
    // 扩容
    void expand() {
        size_t new_capacity = capacity * 2;
        T* new_data = new T[new_capacity];
        
        size_t new_index = 0;
        size_t current = front_index;
        
        for (size_t i = 0; i < size_; ++i) {
            new_data[new_index++] = data[current];
            current = (current + 1) % capacity;
        }
        
        delete[] data;
        data = new_data;
        capacity = new_capacity;
        front_index = 0;
        back_index = new_index;
        is_full = false;
    }
};
```

### 2. 基于链表的队列实现
```cpp
#include <iostream>
#include <stdexcept>
using namespace std;

template<typename T>
struct QueueNode {
    T data;
    QueueNode<T>* next;
    
    QueueNode(const T& value) : data(value), next(nullptr) {}
};

template<typename T>
class LinkedListQueue {
private:
    QueueNode<T>* front_node;
    QueueNode<T>* back_node;
    size_t size_;
    
public:
    // 构造函数
    LinkedListQueue() : front_node(nullptr), back_node(nullptr), size_(0) {}
    
    // 拷贝构造函数
    LinkedListQueue(const LinkedListQueue& other) : front_node(nullptr), back_node(nullptr), size_(0) {
        if (!other.empty()) {
            QueueNode<T>* current = other.front_node;
            while (current != nullptr) {
                push(current->data);
                current = current->next;
            }
        }
    }
    
    // 赋值操作符
    LinkedListQueue& operator=(const LinkedListQueue& other) {
        if (this != &other) {
            clear();
            if (!other.empty()) {
                QueueNode<T>* current = other.front_node;
                while (current != nullptr) {
                    push(current->data);
                    current = current->next;
                }
            }
        }
        return *this;
    }
    
    // 析构函数
    ~LinkedListQueue() {
        clear();
    }
    
    // 入队
    void push(const T& value) {
        QueueNode<T>* new_node = new QueueNode<T>(value);
        
        if (empty()) {
            front_node = back_node = new_node;
        } else {
            back_node->next = new_node;
            back_node = new_node;
        }
        
        ++size_;
    }
    
    // 出队
    void pop() {
        if (empty()) {
            throw runtime_error("Queue is empty");
        }
        
        QueueNode<T>* temp = front_node;
        front_node = front_node->next;
        
        if (front_node == nullptr) {
            back_node = nullptr;
        }
        
        delete temp;
        --size_;
    }
    
    // 查看队首元素
    T& front() {
        if (empty()) {
            throw runtime_error("Queue is empty");
        }
        return front_node->data;
    }
    
    const T& front() const {
        if (empty()) {
            throw runtime_error("Queue is empty");
        }
        return front_node->data;
    }
    
    // 查看队尾元素
    T& back() {
        if (empty()) {
            throw runtime_error("Queue is empty");
        }
        return back_node->data;
    }
    
    const T& back() const {
        if (empty()) {
            throw runtime_error("Queue is empty");
        }
        return back_node->data;
    }
    
    // 检查是否为空
    bool empty() const {
        return front_node == nullptr;
    }
    
    // 获取大小
    size_t size() const {
        return size_;
    }
    
    // 清空队列
    void clear() {
        while (!empty()) {
            pop();
        }
    }
};
```

## 📚 双端队列 (Deque) 实现

### 基于数组的双端队列
```cpp
#include <iostream>
#include <stdexcept>
using namespace std;

template<typename T>
class ArrayDeque {
private:
    T* data;                    // 数据数组
    size_t capacity;            // 容量
    size_t front_index;         // 队首索引
    size_t back_index;          // 队尾索引
    size_t size_;               // 当前大小
    
public:
    // 构造函数
    ArrayDeque(size_t initial_capacity = 10) 
        : capacity(initial_capacity), front_index(0), back_index(0), size_(0) {
        data = new T[capacity];
    }
    
    // 拷贝构造函数
    ArrayDeque(const ArrayDeque& other) 
        : capacity(other.capacity), front_index(other.front_index), 
          back_index(other.back_index), size_(other.size_) {
        data = new T[capacity];
        for (size_t i = 0; i < capacity; ++i) {
            data[i] = other.data[i];
        }
    }
    
    // 赋值操作符
    ArrayDeque& operator=(const ArrayDeque& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            front_index = other.front_index;
            back_index = other.back_index;
            size_ = other.size_;
            
            data = new T[capacity];
            for (size_t i = 0; i < capacity; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    
    // 析构函数
    ~ArrayDeque() {
        delete[] data;
    }
    
    // 前端入队
    void push_front(const T& value) {
        if (size_ >= capacity) {
            expand();
        }
        
        front_index = (front_index - 1 + capacity) % capacity;
        data[front_index] = value;
        ++size_;
    }
    
    // 后端入队
    void push_back(const T& value) {
        if (size_ >= capacity) {
            expand();
        }
        
        data[back_index] = value;
        back_index = (back_index + 1) % capacity;
        ++size_;
    }
    
    // 前端出队
    void pop_front() {
        if (empty()) {
            throw runtime_error("Deque is empty");
        }
        
        front_index = (front_index + 1) % capacity;
        --size_;
    }
    
    // 后端出队
    void pop_back() {
        if (empty()) {
            throw runtime_error("Deque is empty");
        }
        
        back_index = (back_index - 1 + capacity) % capacity;
        --size_;
    }
    
    // 查看前端元素
    T& front() {
        if (empty()) {
            throw runtime_error("Deque is empty");
        }
        return data[front_index];
    }
    
    const T& front() const {
        if (empty()) {
            throw runtime_error("Deque is empty");
        }
        return data[front_index];
    }
    
    // 查看后端元素
    T& back() {
        if (empty()) {
            throw runtime_error("Deque is empty");
        }
        size_t last_index = (back_index - 1 + capacity) % capacity;
        return data[last_index];
    }
    
    const T& back() const {
        if (empty()) {
            throw runtime_error("Deque is empty");
        }
        size_t last_index = (back_index - 1 + capacity) % capacity;
        return data[last_index];
    }
    
    // 随机访问
    T& at(size_t index) {
        if (index >= size_) {
            throw out_of_range("Index out of range");
        }
        size_t actual_index = (front_index + index) % capacity;
        return data[actual_index];
    }
    
    const T& at(size_t index) const {
        if (index >= size_) {
            throw out_of_range("Index out of range");
        }
        size_t actual_index = (front_index + index) % capacity;
        return data[actual_index];
    }
    
    // 检查是否为空
    bool empty() const {
        return size_ == 0;
    }
    
    // 获取大小
    size_t size() const {
        return size_;
    }
    
    // 获取容量
    size_t getCapacity() const {
        return capacity;
    }
    
    // 清空双端队列
    void clear() {
        front_index = 0;
        back_index = 0;
        size_ = 0;
    }
    
private:
    // 扩容
    void expand() {
        size_t new_capacity = capacity * 2;
        T* new_data = new T[new_capacity];
        
        size_t new_index = 0;
        size_t current = front_index;
        
        for (size_t i = 0; i < size_; ++i) {
            new_data[new_index++] = data[current];
            current = (current + 1) % capacity;
        }
        
        delete[] data;
        data = new_data;
        capacity = new_capacity;
        front_index = 0;
        back_index = new_index;
    }
};
```

## 💡 实现选择指南

### 1. 栈的实现选择
- **数组实现**: 适合固定大小或可预测大小的栈，缓存友好
- **链表实现**: 适合动态大小，内存分配灵活

### 2. 队列的实现选择
- **循环队列**: 适合固定大小的队列，内存利用率高
- **链表队列**: 适合动态大小的队列，无容量限制

### 3. 性能对比
| 实现方式 | 入栈/入队 | 出栈/出队 | 随机访问 | 内存效率 |
|----------|-----------|-----------|----------|----------|
| 数组栈 | O(1) | O(1) | O(1) | 高 |
| 链表栈 | O(1) | O(1) | O(n) | 中 |
| 循环队列 | O(1) | O(1) | O(1) | 高 |
| 链表队列 | O(1) | O(1) | O(n) | 中 |

## 🎯 实际应用示例

### 1. 括号匹配
```cpp
bool isValidParentheses(const string& s) {
    ArrayStack<char> stack;
    
    for (char c : s) {
        if (c == '(' || c == '{' || c == '[') {
            stack.push(c);
        } else {
            if (stack.empty()) return false;
            
            char top = stack.top();
            if ((c == ')' && top == '(') ||
                (c == '}' && top == '{') ||
                (c == ']' && top == '[')) {
                stack.pop();
            } else {
                return false;
            }
        }
    }
    
    return stack.empty();
}
```

### 2. 广度优先搜索
```cpp
void bfs(const vector<vector<int>>& graph, int start) {
    CircularQueue<int> queue;
    vector<bool> visited(graph.size(), false);
    
    queue.push(start);
    visited[start] = true;
    
    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();
        
        cout << "访问节点: " << current << endl;
        
        for (int neighbor : graph[current]) {
            if (!visited[neighbor]) {
                queue.push(neighbor);
                visited[neighbor] = true;
            }
        }
    }
}
```

### 3. 滑动窗口最大值
```cpp
vector<int> maxSlidingWindow(const vector<int>& nums, int k) {
    ArrayDeque<int> dq;
    vector<int> result;
    
    for (int i = 0; i < nums.size(); ++i) {
        // 移除超出窗口的元素
        if (!dq.empty() && dq.front() == i - k) {
            dq.pop_front();
        }
        
        // 维护单调递减队列
        while (!dq.empty() && nums[dq.back()] < nums[i]) {
            dq.pop_back();
        }
        
        dq.push_back(i);
        
        if (i >= k - 1) {
            result.push_back(nums[dq.front()]);
        }
    }
    
    return result;
}
```

## 🚀 下一步
- 学习 [二叉树实现](./binary-tree.md)
- 了解 [堆实现](./heap.md)
- 掌握 [哈希表实现](./hash-table.md)

# C++ 链表实现详解

## 🎯 什么是链表？

链表是一种线性数据结构，由一系列节点组成，每个节点包含数据和指向下一个节点的指针。链表的主要优势是插入和删除操作的高效性，但随机访问较慢。

## 🔧 链表类型

### 1. 单链表
- 每个节点只有一个指向下一个节点的指针
- 最后一个节点指向nullptr
- 只能从头节点开始遍历

### 2. 双链表
- 每个节点有两个指针：指向前一个和后一个节点
- 可以双向遍历
- 插入和删除操作更灵活

### 3. 循环链表
- 最后一个节点指向头节点
- 可以循环遍历
- 适合需要循环访问的场景

## 📚 单链表实现

### 节点结构
```cpp
template<typename T>
struct Node {
    T data;                    // 数据
    Node<T>* next;            // 指向下一个节点的指针
    
    Node(const T& value) : data(value), next(nullptr) {}
};
```

### 单链表类
```cpp
template<typename T>
class LinkedList {
private:
    Node<T>* head;            // 头节点指针
    Node<T>* tail;            // 尾节点指针
    size_t size_;             // 链表大小
    
public:
    // 构造函数
    LinkedList() : head(nullptr), tail(nullptr), size_(0) {}
    
    // 析构函数
    ~LinkedList() {
        clear();
    }
    
    // 拷贝构造函数
    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), size_(0) {
        Node<T>* current = other.head;
        while (current != nullptr) {
            push_back(current->data);
            current = current->next;
        }
    }
    
    // 赋值操作符
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            Node<T>* current = other.head;
            while (current != nullptr) {
                push_back(current->data);
                current = current->next;
            }
        }
        return *this;
    }
    
    // 清空链表
    void clear() {
        while (head != nullptr) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        head = tail = nullptr;
        size_ = 0;
    }
    
    // 检查是否为空
    bool empty() const {
        return head == nullptr;
    }
    
    // 获取大小
    size_t size() const {
        return size_;
    }
    
    // 在头部插入元素
    void push_front(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        newNode->next = head;
        head = newNode;
        if (tail == nullptr) {
            tail = head;
        }
        size_++;
    }
    
    // 在尾部插入元素
    void push_back(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        size_++;
    }
    
    // 在指定位置插入元素
    void insert(size_t position, const T& value) {
        if (position > size_) {
            throw std::out_of_range("Position out of range");
        }
        
        if (position == 0) {
            push_front(value);
            return;
        }
        
        if (position == size_) {
            push_back(value);
            return;
        }
        
        Node<T>* current = head;
        for (size_t i = 0; i < position - 1; ++i) {
            current = current->next;
        }
        
        Node<T>* newNode = new Node<T>(value);
        newNode->next = current->next;
        current->next = newNode;
        size_++;
    }
    
    // 删除头部元素
    void pop_front() {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }
        
        Node<T>* temp = head;
        head = head->next;
        delete temp;
        
        if (head == nullptr) {
            tail = nullptr;
        }
        size_--;
    }
    
    // 删除尾部元素
    void pop_back() {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }
        
        if (head == tail) {
            delete head;
            head = tail = nullptr;
        } else {
            Node<T>* current = head;
            while (current->next != tail) {
                current = current->next;
            }
            delete tail;
            tail = current;
            tail->next = nullptr;
        }
        size_--;
    }
    
    // 删除指定位置的元素
    void remove(size_t position) {
        if (position >= size_) {
            throw std::out_of_range("Position out of range");
        }
        
        if (position == 0) {
            pop_front();
            return;
        }
        
        Node<T>* current = head;
        for (size_t i = 0; i < position - 1; ++i) {
            current = current->next;
        }
        
        Node<T>* temp = current->next;
        current->next = temp->next;
        
        if (temp == tail) {
            tail = current;
        }
        
        delete temp;
        size_--;
    }
    
    // 获取指定位置的元素
    T& at(size_t position) {
        if (position >= size_) {
            throw std::out_of_range("Position out of range");
        }
        
        Node<T>* current = head;
        for (size_t i = 0; i < position; ++i) {
            current = current->next;
        }
        
        return current->data;
    }
    
    // 查找元素
    Node<T>* find(const T& value) {
        Node<T>* current = head;
        while (current != nullptr) {
            if (current->data == value) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }
    
    // 反转链表
    void reverse() {
        if (size_ <= 1) return;
        
        Node<T>* prev = nullptr;
        Node<T>* current = head;
        Node<T>* next = nullptr;
        
        tail = head;  // 更新尾节点
        
        while (current != nullptr) {
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        
        head = prev;
    }
    
    // 打印链表
    void print() const {
        Node<T>* current = head;
        while (current != nullptr) {
            std::cout << current->data;
            if (current->next != nullptr) {
                std::cout << " -> ";
            }
            current = current->next;
        }
        std::cout << " -> nullptr" << std::endl;
    }
};
```

## 📚 双链表实现

### 双链表节点
```cpp
template<typename T>
struct DoublyNode {
    T data;
    DoublyNode<T>* prev;
    DoublyNode<T>* next;
    
    DoublyNode(const T& value) : data(value), prev(nullptr), next(nullptr) {}
};
```

### 双链表类
```cpp
template<typename T>
class DoublyLinkedList {
private:
    DoublyNode<T>* head;
    DoublyNode<T>* tail;
    size_t size_;
    
public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), size_(0) {}
    
    ~DoublyLinkedList() {
        clear();
    }
    
    void clear() {
        while (head != nullptr) {
            DoublyNode<T>* temp = head;
            head = head->next;
            delete temp;
        }
        head = tail = nullptr;
        size_ = 0;
    }
    
    bool empty() const {
        return head == nullptr;
    }
    
    size_t size() const {
        return size_;
    }
    
    void push_front(const T& value) {
        DoublyNode<T>* newNode = new DoublyNode<T>(value);
        newNode->next = head;
        if (head != nullptr) {
            head->prev = newNode;
        }
        head = newNode;
        if (tail == nullptr) {
            tail = head;
        }
        size_++;
    }
    
    void push_back(const T& value) {
        DoublyNode<T>* newNode = new DoublyNode<T>(value);
        newNode->prev = tail;
        if (tail != nullptr) {
            tail->next = newNode;
        }
        tail = newNode;
        if (head == nullptr) {
            head = tail;
        }
        size_++;
    }
    
    void pop_front() {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }
        
        DoublyNode<T>* temp = head;
        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }
        delete temp;
        size_--;
    }
    
    void pop_back() {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }
        
        DoublyNode<T>* temp = tail;
        tail = tail->prev;
        if (tail != nullptr) {
            tail->next = nullptr;
        } else {
            head = nullptr;
        }
        delete temp;
        size_--;
    }
    
    void print() const {
        DoublyNode<T>* current = head;
        std::cout << "Forward: ";
        while (current != nullptr) {
            std::cout << current->data;
            if (current->next != nullptr) {
                std::cout << " <-> ";
            }
            current = current->next;
        }
        std::cout << " -> nullptr" << std::endl;
        
        std::cout << "Backward: ";
        current = tail;
        while (current != nullptr) {
            std::cout << current->data;
            if (current->prev != nullptr) {
                std::cout << " <-> ";
            }
            current = current->prev;
        }
        std::cout << " -> nullptr" << std::endl;
    }
};
```

## ⚡ 性能分析

### 时间复杂度
- **访问**: O(n) - 需要遍历到指定位置
- **查找**: O(n) - 线性搜索
- **插入**: O(1) - 头部/尾部插入，O(n) - 中间插入
- **删除**: O(1) - 头部/尾部删除，O(n) - 中间删除

### 空间复杂度
- **存储**: O(n) - 每个节点存储数据和指针
- **额外空间**: O(1) - 除了存储数据外无额外开销

## 💡 使用技巧

### 1. 虚拟头节点
```cpp
// 使用虚拟头节点简化操作
template<typename T>
class LinkedListWithDummy {
private:
    Node<T>* dummyHead;  // 虚拟头节点
    
public:
    LinkedListWithDummy() {
        dummyHead = new Node<T>(T());  // 创建虚拟头节点
    }
    
    void push_front(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        newNode->next = dummyHead->next;
        dummyHead->next = newNode;
    }
    
    // 其他操作类似...
};
```

### 2. 快慢指针技巧
```cpp
// 检测链表是否有环
template<typename T>
bool hasCycle(Node<T>* head) {
    if (head == nullptr || head->next == nullptr) {
        return false;
    }
    
    Node<T>* slow = head;
    Node<T>* fast = head->next;
    
    while (slow != fast) {
        if (fast == nullptr || fast->next == nullptr) {
            return false;
        }
        slow = slow->next;
        fast = fast->next->next;
    }
    
    return true;
}

// 找到链表的中间节点
template<typename T>
Node<T>* findMiddle(Node<T>* head) {
    if (head == nullptr) return nullptr;
    
    Node<T>* slow = head;
    Node<T>* fast = head;
    
    while (fast->next != nullptr && fast->next->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    return slow;
}
```

## 🎯 适用场景

### 适用场景
- **频繁插入/删除**: 不需要移动其他元素
- **动态大小**: 大小不固定的数据
- **内存分散**: 不需要连续内存空间
- **实现栈/队列**: 作为底层数据结构

### 不适用场景
- **随机访问**: 需要O(1)访问时间
- **缓存友好**: 需要良好的缓存性能
- **内存敏感**: 指针开销较大

## 🔍 常见错误

### 1. 内存泄漏
```cpp
// 错误：没有正确删除节点
void remove(Node<T>* node) {
    node->next = node->next->next;  // 丢失了要删除的节点
}

// 正确做法
void remove(Node<T>* node) {
    Node<T>* temp = node->next;
    node->next = temp->next;
    delete temp;
}
```

### 2. 空指针访问
```cpp
// 错误：没有检查空指针
void print(Node<T>* head) {
    while (head->next != nullptr) {  // 如果head为nullptr会崩溃
        cout << head->data << " ";
        head = head->next;
    }
}

// 正确做法
void print(Node<T>* head) {
    while (head != nullptr) {
        cout << head->data << " ";
        head = head->next;
    }
}
```

## 📝 实际应用示例

### 1. LRU缓存实现
```cpp
template<typename K, typename V>
class LRUCache {
private:
    struct CacheNode {
        K key;
        V value;
        CacheNode* prev;
        CacheNode* next;
        CacheNode(K k, V v) : key(k), value(v), prev(nullptr), next(nullptr) {}
    };
    
    unordered_map<K, CacheNode*> cache;
    CacheNode* head;
    CacheNode* tail;
    int capacity;
    
public:
    LRUCache(int cap) : capacity(cap) {
        head = new CacheNode(K(), V());
        tail = new CacheNode(K(), V());
        head->next = tail;
        tail->prev = head;
    }
    
    V get(K key) {
        if (cache.find(key) == cache.end()) {
            return V();  // 返回默认值
        }
        
        // 移动到头部
        moveToHead(cache[key]);
        return cache[key]->value;
    }
    
    void put(K key, V value) {
        if (cache.find(key) != cache.end()) {
            cache[key]->value = value;
            moveToHead(cache[key]);
        } else {
            if (cache.size() >= capacity) {
                removeTail();
            }
            CacheNode* newNode = new CacheNode(key, value);
            cache[key] = newNode;
            addToHead(newNode);
        }
    }
    
private:
    void moveToHead(CacheNode* node) {
        removeNode(node);
        addToHead(node);
    }
    
    void addToHead(CacheNode* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }
    
    void removeNode(CacheNode* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    
    void removeTail() {
        CacheNode* last = tail->prev;
        removeNode(last);
        cache.erase(last->key);
        delete last;
    }
};
```

## 📝 练习题目
1. 实现链表的反转
2. 检测链表是否有环
3. 找到链表的中间节点
4. 合并两个有序链表
5. 实现LRU缓存

## 🚀 下一步
- 学习 [栈和队列实现](./stack-queue.md)
- 了解 [二叉树实现](./binary-tree.md)
- 准备 [LeetCode刷题](../04-leetcode-guide/)

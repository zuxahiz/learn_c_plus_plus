# 堆数据结构实现详解

## 🎯 堆的基本概念

堆(Heap)是一种特殊的完全二叉树，分为最大堆和最小堆。堆是实现优先队列的经典数据结构，在排序、图算法、任务调度等场景中应用广泛。

### 堆的性质
1. **完全二叉树**: 除最后一层外，其他层都是满的，最后一层从左到右填充
2. **堆序性质**: 
   - 最大堆：父节点≥子节点
   - 最小堆：父节点≤子节点

### 堆的优势
- **插入和删除**: O(log n)
- **查找最值**: O(1)
- **原地排序**: 堆排序空间复杂度O(1)

## 📚 堆的基本实现

### 基础堆结构

```cpp
#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <functional>

template<typename T, typename Compare = std::less<T>>
class Heap {
private:
    std::vector<T> data;
    Compare comp;
    
    // 获取父节点索引
    size_t parent(size_t i) const {
        return (i - 1) / 2;
    }
    
    // 获取左子节点索引
    size_t leftChild(size_t i) const {
        return 2 * i + 1;
    }
    
    // 获取右子节点索引
    size_t rightChild(size_t i) const {
        return 2 * i + 2;
    }
    
    // 向上调整（用于插入）
    void heapifyUp(size_t index) {
        while (index > 0) {
            size_t parentIndex = parent(index);
            if (!comp(data[parentIndex], data[index])) {
                break;
            }
            std::swap(data[index], data[parentIndex]);
            index = parentIndex;
        }
    }
    
    // 向下调整（用于删除）
    void heapifyDown(size_t index) {
        size_t size = data.size();
        
        while (leftChild(index) < size) {
            size_t maxChild = leftChild(index);
            size_t rightIdx = rightChild(index);
            
            // 找到较大的子节点
            if (rightIdx < size && comp(data[maxChild], data[rightIdx])) {
                maxChild = rightIdx;
            }
            
            // 如果当前节点已经满足堆性质，停止
            if (!comp(data[index], data[maxChild])) {
                break;
            }
            
            std::swap(data[index], data[maxChild]);
            index = maxChild;
        }
    }
    
public:
    // 构造函数
    Heap() = default;
    
    explicit Heap(const Compare& cmp) : comp(cmp) {}
    
    // 从数组构造堆
    explicit Heap(const std::vector<T>& arr, const Compare& cmp = Compare()) 
        : data(arr), comp(cmp) {
        buildHeap();
    }
    
    // 插入元素
    void push(const T& value) {
        data.push_back(value);
        heapifyUp(data.size() - 1);
    }
    
    void push(T&& value) {
        data.push_back(std::move(value));
        heapifyUp(data.size() - 1);
    }
    
    // 删除堆顶元素
    void pop() {
        if (empty()) {
            throw std::runtime_error("Heap is empty");
        }
        
        // 将最后一个元素移到堆顶
        data[0] = std::move(data.back());
        data.pop_back();
        
        // 向下调整
        if (!empty()) {
            heapifyDown(0);
        }
    }
    
    // 获取堆顶元素
    const T& top() const {
        if (empty()) {
            throw std::runtime_error("Heap is empty");
        }
        return data[0];
    }
    
    T& top() {
        if (empty()) {
            throw std::runtime_error("Heap is empty");
        }
        return data[0];
    }
    
    // 检查是否为空
    bool empty() const {
        return data.empty();
    }
    
    // 获取大小
    size_t size() const {
        return data.size();
    }
    
    // 清空堆
    void clear() {
        data.clear();
    }
    
    // 从数组构建堆（O(n)）
    void buildHeap() {
        if (data.size() <= 1) return;
        
        // 从最后一个非叶子节点开始向下调整
        for (int i = static_cast<int>(parent(data.size() - 1)); i >= 0; --i) {
            heapifyDown(static_cast<size_t>(i));
        }
    }
    
    // 堆排序
    std::vector<T> heapSort() const {
        std::vector<T> result = data;
        Heap<T, Compare> tempHeap(result, comp);
        result.clear();
        
        while (!tempHeap.empty()) {
            result.push_back(tempHeap.top());
            tempHeap.pop();
        }
        
        return result;
    }
    
    // 获取内部数据（用于调试）
    const std::vector<T>& getData() const {
        return data;
    }
    
    // 验证堆性质
    bool isValidHeap() const {
        return isValidHeapHelper(0);
    }
    
private:
    bool isValidHeapHelper(size_t index) const {
        size_t size = data.size();
        size_t left = leftChild(index);
        size_t right = rightChild(index);
        
        // 检查左子节点
        if (left < size) {
            if (comp(data[index], data[left])) {
                return false;
            }
            if (!isValidHeapHelper(left)) {
                return false;
            }
        }
        
        // 检查右子节点
        if (right < size) {
            if (comp(data[index], data[right])) {
                return false;
            }
            if (!isValidHeapHelper(right)) {
                return false;
            }
        }
        
        return true;
    }
    
public:
    // 打印堆结构
    void printHeap() const {
        if (empty()) {
            std::cout << "堆为空" << std::endl;
            return;
        }
        
        std::cout << "堆结构（数组表示）: ";
        for (size_t i = 0; i < data.size(); ++i) {
            std::cout << data[i];
            if (i < data.size() - 1) std::cout << " ";
        }
        std::cout << std::endl;
        
        // 层次打印
        printLevelOrder();
    }
    
private:
    void printLevelOrder() const {
        if (empty()) return;
        
        size_t level = 0;
        size_t levelSize = 1;
        size_t index = 0;
        
        std::cout << "层次结构:" << std::endl;
        
        while (index < data.size()) {
            std::cout << "第" << level << "层: ";
            
            for (size_t i = 0; i < levelSize && index < data.size(); ++i, ++index) {
                std::cout << data[index] << " ";
            }
            
            std::cout << std::endl;
            level++;
            levelSize *= 2;
        }
    }
};

// 类型别名
template<typename T>
using MaxHeap = Heap<T, std::less<T>>;

template<typename T>
using MinHeap = Heap<T, std::greater<T>>;
```

## 📚 高级堆实现

### 可更新优先队列

```cpp
template<typename T, typename Compare = std::less<T>>
class IndexedHeap {
private:
    std::vector<T> data;
    std::vector<size_t> heap;      // 堆中存储的是数据的索引
    std::vector<size_t> position;  // position[i] 表示数据i在堆中的位置
    Compare comp;
    
    size_t parent(size_t i) const { return (i - 1) / 2; }
    size_t leftChild(size_t i) const { return 2 * i + 1; }
    size_t rightChild(size_t i) const { return 2 * i + 2; }
    
    void heapifyUp(size_t heapIndex) {
        while (heapIndex > 0) {
            size_t parentIndex = parent(heapIndex);
            size_t currentDataIndex = heap[heapIndex];
            size_t parentDataIndex = heap[parentIndex];
            
            if (!comp(data[parentDataIndex], data[currentDataIndex])) {
                break;
            }
            
            // 交换堆中的索引
            std::swap(heap[heapIndex], heap[parentIndex]);
            // 更新position数组
            position[currentDataIndex] = parentIndex;
            position[parentDataIndex] = heapIndex;
            
            heapIndex = parentIndex;
        }
    }
    
    void heapifyDown(size_t heapIndex) {
        while (leftChild(heapIndex) < heap.size()) {
            size_t maxChild = leftChild(heapIndex);
            size_t rightIdx = rightChild(heapIndex);
            
            if (rightIdx < heap.size() && 
                comp(data[heap[maxChild]], data[heap[rightIdx]])) {
                maxChild = rightIdx;
            }
            
            size_t currentDataIndex = heap[heapIndex];
            size_t maxChildDataIndex = heap[maxChild];
            
            if (!comp(data[currentDataIndex], data[maxChildDataIndex])) {
                break;
            }
            
            std::swap(heap[heapIndex], heap[maxChild]);
            position[currentDataIndex] = maxChild;
            position[maxChildDataIndex] = heapIndex;
            
            heapIndex = maxChild;
        }
    }
    
public:
    IndexedHeap(size_t capacity, const Compare& cmp = Compare()) 
        : data(capacity), position(capacity), comp(cmp) {}
    
    // 插入元素到指定索引
    void insert(size_t index, const T& value) {
        if (index >= data.size()) {
            throw std::out_of_range("Index out of range");
        }
        
        data[index] = value;
        heap.push_back(index);
        position[index] = heap.size() - 1;
        heapifyUp(heap.size() - 1);
    }
    
    // 更新指定索引的值
    void update(size_t index, const T& newValue) {
        if (index >= data.size()) {
            throw std::out_of_range("Index out of range");
        }
        
        T oldValue = data[index];
        data[index] = newValue;
        size_t heapPos = position[index];
        
        if (comp(oldValue, newValue)) {
            heapifyUp(heapPos);
        } else {
            heapifyDown(heapPos);
        }
    }
    
    // 获取堆顶元素的索引和值
    std::pair<size_t, T> top() const {
        if (heap.empty()) {
            throw std::runtime_error("Heap is empty");
        }
        size_t topIndex = heap[0];
        return {topIndex, data[topIndex]};
    }
    
    // 删除堆顶元素
    void pop() {
        if (heap.empty()) {
            throw std::runtime_error("Heap is empty");
        }
        
        size_t topIndex = heap[0];
        size_t lastIndex = heap.back();
        
        heap[0] = lastIndex;
        position[lastIndex] = 0;
        heap.pop_back();
        
        if (!heap.empty()) {
            heapifyDown(0);
        }
    }
    
    bool empty() const { return heap.empty(); }
    size_t size() const { return heap.size(); }
    
    // 检查某个索引是否在堆中
    bool contains(size_t index) const {
        if (index >= data.size()) return false;
        size_t pos = position[index];
        return pos < heap.size() && heap[pos] == index;
    }
};
```

### 多路归并堆

```cpp
template<typename T>
class MergeableHeap {
private:
    struct Node {
        T data;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        
        Node(const T& val) : data(val), left(nullptr), right(nullptr) {}
    };
    
    using NodePtr = std::shared_ptr<Node>;
    NodePtr root;
    
    // 合并两个堆
    NodePtr merge(NodePtr h1, NodePtr h2) {
        if (!h1) return h2;
        if (!h2) return h1;
        
        // 确保h1的根节点小于等于h2的根节点
        if (h1->data > h2->data) {
            std::swap(h1, h2);
        }
        
        // 随机选择左右子树进行合并（保持平衡）
        if (rand() % 2) {
            h1->left = merge(h1->left, h2);
        } else {
            h1->right = merge(h1->right, h2);
        }
        
        return h1;
    }
    
public:
    MergeableHeap() : root(nullptr) {}
    
    // 插入元素
    void insert(const T& value) {
        NodePtr newNode = std::make_shared<Node>(value);
        root = merge(root, newNode);
    }
    
    // 获取最小元素
    T getMin() const {
        if (!root) {
            throw std::runtime_error("Heap is empty");
        }
        return root->data;
    }
    
    // 删除最小元素
    void deleteMin() {
        if (!root) {
            throw std::runtime_error("Heap is empty");
        }
        
        NodePtr left = root->left;
        NodePtr right = root->right;
        root = merge(left, right);
    }
    
    // 合并另一个堆
    void merge(MergeableHeap& other) {
        root = merge(root, other.root);
        other.root = nullptr;
    }
    
    bool empty() const {
        return root == nullptr;
    }
    
    // 计算堆的大小
    size_t size() const {
        return sizeHelper(root);
    }
    
private:
    size_t sizeHelper(NodePtr node) const {
        if (!node) return 0;
        return 1 + sizeHelper(node->left) + sizeHelper(node->right);
    }
};
```

## 📚 堆的应用示例

### 1. 堆排序实现

```cpp
class HeapSort {
public:
    template<typename Iterator>
    static void heapSort(Iterator begin, Iterator end) {
        using ValueType = typename std::iterator_traits<Iterator>::value_type;
        
        // 构建最大堆
        buildMaxHeap(begin, end);
        
        // 反复取出最大元素
        for (auto it = end - 1; it > begin; --it) {
            std::swap(*begin, *it);
            heapifyDown(begin, it, begin);
        }
    }
    
private:
    template<typename Iterator>
    static void buildMaxHeap(Iterator begin, Iterator end) {
        auto size = std::distance(begin, end);
        
        // 从最后一个非叶子节点开始向下调整
        for (auto i = size / 2 - 1; i >= 0; --i) {
            heapifyDown(begin, end, begin + i);
        }
    }
    
    template<typename Iterator>
    static void heapifyDown(Iterator begin, Iterator end, Iterator current) {
        auto size = std::distance(begin, end);
        auto currentIndex = std::distance(begin, current);
        
        while (true) {
            auto largest = currentIndex;
            auto leftChild = 2 * currentIndex + 1;
            auto rightChild = 2 * currentIndex + 2;
            
            if (leftChild < size && *(begin + leftChild) > *(begin + largest)) {
                largest = leftChild;
            }
            
            if (rightChild < size && *(begin + rightChild) > *(begin + largest)) {
                largest = rightChild;
            }
            
            if (largest == currentIndex) {
                break;
            }
            
            std::swap(*(begin + currentIndex), *(begin + largest));
            currentIndex = largest;
        }
    }
};
```

### 2. Top K 问题解决

```cpp
template<typename T>
class TopKFinder {
private:
    MinHeap<T> heap;
    size_t k;
    
public:
    explicit TopKFinder(size_t k) : k(k) {}
    
    // 添加元素
    void add(const T& value) {
        if (heap.size() < k) {
            heap.push(value);
        } else if (value > heap.top()) {
            heap.pop();
            heap.push(value);
        }
    }
    
    // 获取Top K元素
    std::vector<T> getTopK() const {
        auto tempHeap = heap;
        std::vector<T> result;
        
        while (!tempHeap.empty()) {
            result.push_back(tempHeap.top());
            tempHeap.pop();
        }
        
        std::reverse(result.begin(), result.end());
        return result;
    }
    
    // 批量处理
    std::vector<T> findTopK(const std::vector<T>& data) {
        for (const auto& item : data) {
            add(item);
        }
        return getTopK();
    }
};
```

### 3. 任务调度器

```cpp
template<typename T>
class TaskScheduler {
private:
    struct Task {
        T data;
        int priority;
        std::chrono::system_clock::time_point schedule_time;
        
        Task(const T& d, int p, std::chrono::system_clock::time_point t)
            : data(d), priority(p), schedule_time(t) {}
        
        bool operator<(const Task& other) const {
            if (schedule_time != other.schedule_time) {
                return schedule_time > other.schedule_time; // 最小堆：早的时间优先
            }
            return priority < other.priority; // 优先级高的优先
        }
    };
    
    Heap<Task> taskHeap;
    
public:
    // 添加任务
    void scheduleTask(const T& task, int priority, 
                     std::chrono::milliseconds delay = std::chrono::milliseconds(0)) {
        auto schedule_time = std::chrono::system_clock::now() + delay;
        taskHeap.push(Task(task, priority, schedule_time));
    }
    
    // 获取下一个要执行的任务
    std::optional<T> getNextTask() {
        auto now = std::chrono::system_clock::now();
        
        if (!taskHeap.empty() && taskHeap.top().schedule_time <= now) {
            T task = taskHeap.top().data;
            taskHeap.pop();
            return task;
        }
        
        return std::nullopt;
    }
    
    // 检查是否有待执行的任务
    bool hasPendingTasks() const {
        return !taskHeap.empty();
    }
    
    // 获取下一个任务的执行时间
    std::optional<std::chrono::system_clock::time_point> getNextExecutionTime() const {
        if (taskHeap.empty()) {
            return std::nullopt;
        }
        return taskHeap.top().schedule_time;
    }
};
```

## 📊 性能分析和优化

### 性能特征

| 操作 | 时间复杂度 | 空间复杂度 | 说明 |
|------|------------|------------|------|
| 插入 | O(log n) | O(1) | 需要向上调整 |
| 删除堆顶 | O(log n) | O(1) | 需要向下调整 |
| 查看堆顶 | O(1) | O(1) | 直接访问 |
| 构建堆 | O(n) | O(1) | 自底向上构建 |
| 堆排序 | O(n log n) | O(1) | 原地排序 |

### 优化技巧

```cpp
// 1. 缓存友好的堆实现
template<typename T>
class CacheFriendlyHeap {
private:
    std::vector<T> data;
    static constexpr size_t BLOCK_SIZE = 64; // 缓存行大小
    
    // 预分配内存减少重新分配
    void reserve(size_t capacity) {
        data.reserve(capacity);
    }
    
public:
    // 批量插入优化
    template<typename Iterator>
    void batchInsert(Iterator begin, Iterator end) {
        size_t oldSize = data.size();
        data.insert(data.end(), begin, end);
        
        // 从最后一个非叶子节点开始调整
        if (data.size() > 1) {
            for (int i = static_cast<int>(parent(data.size() - 1)); i >= static_cast<int>(oldSize); --i) {
                heapifyDown(static_cast<size_t>(i));
            }
        }
    }
};

// 2. 内存池优化的堆
template<typename T>
class PooledHeap {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        
        Node(const T& val) : data(val), left(nullptr), right(nullptr) {}
    };
    
    // 简单的内存池
    std::vector<std::unique_ptr<Node[]>> pools;
    std::vector<Node*> freeNodes;
    static constexpr size_t POOL_SIZE = 1024;
    
    Node* allocateNode(const T& value) {
        if (freeNodes.empty()) {
            auto pool = std::make_unique<Node[]>(POOL_SIZE);
            for (size_t i = 0; i < POOL_SIZE; ++i) {
                freeNodes.push_back(&pool[i]);
            }
            pools.push_back(std::move(pool));
        }
        
        Node* node = freeNodes.back();
        freeNodes.pop_back();
        new(node) Node(value);
        return node;
    }
    
    void deallocateNode(Node* node) {
        node->~Node();
        freeNodes.push_back(node);
    }
};
```

## 🧪 测试和验证

```cpp
void testHeapImplementations() {
    std::cout << "=== 堆实现测试 ===" << std::endl;
    
    // 测试基本堆操作
    std::cout << "\n1. 基本最大堆测试:" << std::endl;
    MaxHeap<int> maxHeap;
    
    std::vector<int> testData = {4, 1, 7, 3, 8, 5, 2, 6};
    std::cout << "插入数据: ";
    for (int val : testData) {
        std::cout << val << " ";
        maxHeap.push(val);
    }
    std::cout << std::endl;
    
    maxHeap.printHeap();
    std::cout << "堆有效性: " << (maxHeap.isValidHeap() ? "有效" : "无效") << std::endl;
    
    std::cout << "\n取出元素: ";
    while (!maxHeap.empty()) {
        std::cout << maxHeap.top() << " ";
        maxHeap.pop();
    }
    std::cout << std::endl;
    
    // 测试堆排序
    std::cout << "\n2. 堆排序测试:" << std::endl;
    std::vector<int> sortData = {64, 34, 25, 12, 22, 11, 90};
    std::cout << "原始数据: ";
    for (int val : sortData) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    HeapSort::heapSort(sortData.begin(), sortData.end());
    std::cout << "排序结果: ";
    for (int val : sortData) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 测试Top K
    std::cout << "\n3. Top K测试:" << std::endl;
    TopKFinder<int> topKFinder(3);
    std::vector<int> data = {1, 5, 3, 9, 2, 8, 4, 7, 6};
    auto topK = topKFinder.findTopK(data);
    
    std::cout << "数据: ";
    for (int val : data) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Top 3: ";
    for (int val : topK) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 测试任务调度器
    std::cout << "\n4. 任务调度器测试:" << std::endl;
    TaskScheduler<std::string> scheduler;
    
    scheduler.scheduleTask("低优先级任务", 1, std::chrono::milliseconds(100));
    scheduler.scheduleTask("高优先级任务", 5, std::chrono::milliseconds(50));
    scheduler.scheduleTask("立即执行任务", 3, std::chrono::milliseconds(0));
    
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    
    while (scheduler.hasPendingTasks()) {
        auto task = scheduler.getNextTask();
        if (task) {
            std::cout << "执行任务: " << *task << std::endl;
        }
    }
}
```

## 💡 堆的应用场景

### 1. 优先队列
- **任务调度**: 操作系统中的进程调度
- **网络路由**: Dijkstra算法中的最短路径
- **事件驱动**: 事件按时间优先级处理

### 2. 排序算法
- **堆排序**: 原地排序，最坏情况O(n log n)
- **部分排序**: 只需要前K个最大/最小元素

### 3. 算法优化
- **贪心算法**: 霍夫曼编码树构建
- **图算法**: Prim最小生成树算法
- **搜索算法**: A*算法中的开放列表

### 4. 数据流处理
- **中位数维护**: 使用两个堆维护动态中位数
- **滑动窗口**: 维护窗口内的最值
- **实时统计**: 在线维护Top K统计

## 🔧 实现注意事项

### 1. 边界条件处理
- 空堆操作的异常处理
- 单元素堆的特殊情况
- 索引越界检查

### 2. 内存管理
- 使用RAII原则管理资源
- 注意深拷贝和浅拷贝
- 避免内存泄漏

### 3. 性能优化
- 减少不必要的比较操作
- 利用缓存局部性优化
- 考虑使用内存池

### 4. 线程安全
- 如需要，添加适当的同步机制
- 考虑无锁数据结构
- 注意并发访问的数据一致性

## 🚀 进阶学习建议

1. **研究STL中的priority_queue实现**
2. **学习二项堆、斐波那契堆等高级堆结构**
3. **了解堆在分布式系统中的应用**
4. **掌握堆的并发实现技术**

堆是一种极其重要和实用的数据结构，掌握其实现原理和应用技巧对于算法设计和系统优化都有重要意义！🎯

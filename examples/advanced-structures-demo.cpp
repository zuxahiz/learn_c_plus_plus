#include <iostream>
#include <forward_list>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <algorithm>
#include <chrono>

// 演示forward_list的使用
void demonstrateForwardList() {
    std::cout << "=== Forward List 演示 ===" << std::endl;
    
    // 创建和初始化
    std::forward_list<int> fl1;
    std::forward_list<int> fl2 = {1, 2, 3, 4, 5};
    std::forward_list<std::string> fl3 = {"apple", "banana", "cherry"};
    
    // 前端操作
    fl1.push_front(10);
    fl1.push_front(20);
    fl1.push_front(30);
    
    std::cout << "fl1 after push_front operations: ";
    for (const auto& val : fl1) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 插入操作
    auto it = fl2.begin();
    fl2.insert_after(it, 99);  // 在第一个元素后插入
    
    std::cout << "fl2 after insert_after: ";
    for (const auto& val : fl2) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 删除操作
    fl2.remove(3);  // 删除所有值为3的元素
    
    std::cout << "fl2 after remove(3): ";
    for (const auto& val : fl2) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 链表操作
    std::forward_list<int> fl4 = {100, 200};
    fl2.splice_after(fl2.begin(), fl4);  // 拼接链表
    
    std::cout << "fl2 after splice_after: ";
    for (const auto& val : fl2) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 排序和去重
    fl2.sort();
    fl2.unique();
    
    std::cout << "fl2 after sort and unique: ";
    for (const auto& val : fl2) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 性能比较：forward_list vs vector
    std::cout << "\n--- 性能比较 ---" << std::endl;
    const int n = 100000;
    
    // forward_list 前端插入
    auto start = std::chrono::high_resolution_clock::now();
    std::forward_list<int> fl_perf;
    for (int i = 0; i < n; ++i) {
        fl_perf.push_front(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_fl = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // vector 前端插入
    start = std::chrono::high_resolution_clock::now();
    std::vector<int> vec_perf;
    for (int i = 0; i < n; ++i) {
        vec_perf.insert(vec_perf.begin(), i);
    }
    end = std::chrono::high_resolution_clock::now();
    auto duration_vec = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "插入 " << n << " 个元素到前端:" << std::endl;
    std::cout << "forward_list: " << duration_fl.count() << " μs" << std::endl;
    std::cout << "vector: " << duration_vec.count() << " μs" << std::endl;
    std::cout << "forward_list 快了 " << (double)duration_vec.count() / duration_fl.count() << " 倍" << std::endl;
}

// 简化的堆实现演示
class MaxHeap {
private:
    std::vector<int> heap;
    
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[parent] >= heap[index]) break;
            std::swap(heap[parent], heap[index]);
            index = parent;
        }
    }
    
    void heapifyDown(int index) {
        int size = heap.size();
        while (true) {
            int largest = index;
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            
            if (left < size && heap[left] > heap[largest]) {
                largest = left;
            }
            if (right < size && heap[right] > heap[largest]) {
                largest = right;
            }
            
            if (largest == index) break;
            
            std::swap(heap[index], heap[largest]);
            index = largest;
        }
    }
    
public:
    void push(int value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }
    
    int pop() {
        if (heap.empty()) throw std::runtime_error("Heap is empty");
        
        int result = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) {
            heapifyDown(0);
        }
        return result;
    }
    
    int top() const {
        if (heap.empty()) throw std::runtime_error("Heap is empty");
        return heap[0];
    }
    
    bool empty() const { return heap.empty(); }
    size_t size() const { return heap.size(); }
    
    void print() const {
        std::cout << "Heap: ";
        for (int val : heap) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
};

// 演示堆的使用
void demonstrateHeap() {
    std::cout << "\n=== 堆数据结构演示 ===" << std::endl;
    
    // 自定义最大堆
    MaxHeap maxHeap;
    std::vector<int> data = {4, 10, 3, 5, 1, 8, 7, 6, 2, 9};
    
    std::cout << "插入数据: ";
    for (int val : data) {
        std::cout << val << " ";
        maxHeap.push(val);
    }
    std::cout << std::endl;
    
    maxHeap.print();
    
    std::cout << "逐个取出最大值: ";
    while (!maxHeap.empty()) {
        std::cout << maxHeap.pop() << " ";
    }
    std::cout << std::endl;
    
    // STL priority_queue 演示
    std::cout << "\n--- STL priority_queue 演示 ---" << std::endl;
    
    // 最大堆（默认）
    std::priority_queue<int> pq_max;
    for (int val : data) {
        pq_max.push(val);
    }
    
    std::cout << "最大堆输出: ";
    while (!pq_max.empty()) {
        std::cout << pq_max.top() << " ";
        pq_max.pop();
    }
    std::cout << std::endl;
    
    // 最小堆
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq_min;
    for (int val : data) {
        pq_min.push(val);
    }
    
    std::cout << "最小堆输出: ";
    while (!pq_min.empty()) {
        std::cout << pq_min.top() << " ";
        pq_min.pop();
    }
    std::cout << std::endl;
    
    // 堆排序演示
    std::cout << "\n--- 堆排序演示 ---" << std::endl;
    std::vector<int> unsorted = {64, 34, 25, 12, 22, 11, 90, 88, 76, 50, 42};
    
    std::cout << "原始数组: ";
    for (int val : unsorted) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 使用STL的make_heap和sort_heap
    std::make_heap(unsorted.begin(), unsorted.end());
    std::cout << "建堆后: ";
    for (int val : unsorted) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    std::sort_heap(unsorted.begin(), unsorted.end());
    std::cout << "堆排序后: ";
    for (int val : unsorted) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

// 演示红黑树和AVL树（通过STL的set/map）
void demonstrateBalancedTrees() {
    std::cout << "\n=== 平衡树演示（STL set/map基于红黑树） ===" << std::endl;
    
    // set演示（内部使用红黑树）
    std::set<int> rb_set;
    std::vector<int> data = {20, 10, 30, 5, 15, 25, 35, 1, 7, 12, 18, 22, 28, 33, 40};
    
    std::cout << "插入数据到set: ";
    for (int val : data) {
        std::cout << val << " ";
        rb_set.insert(val);
    }
    std::cout << std::endl;
    
    std::cout << "set中序遍历（自动排序）: ";
    for (int val : rb_set) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 查找演示
    std::cout << "\n--- 查找演示 ---" << std::endl;
    std::vector<int> search_vals = {15, 50, 25, 100};
    for (int val : search_vals) {
        auto it = rb_set.find(val);
        if (it != rb_set.end()) {
            std::cout << "找到 " << val << std::endl;
        } else {
            std::cout << "未找到 " << val << std::endl;
        }
    }
    
    // map演示
    std::cout << "\n--- map演示（键值对存储） ---" << std::endl;
    std::map<std::string, int> student_scores;
    student_scores["Alice"] = 95;
    student_scores["Bob"] = 87;
    student_scores["Charlie"] = 92;
    student_scores["David"] = 89;
    student_scores["Eve"] = 96;
    
    std::cout << "学生成绩（按姓名排序）:" << std::endl;
    for (const auto& pair : student_scores) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
    
    // 查找操作
    std::string search_name = "Charlie";
    auto it = student_scores.find(search_name);
    if (it != student_scores.end()) {
        std::cout << search_name << " 的成绩是: " << it->second << std::endl;
    }
    
    // 性能比较：插入操作
    std::cout << "\n--- 性能比较：有序插入 ---" << std::endl;
    const int n = 100000;
    
    // set（红黑树）插入
    auto start = std::chrono::high_resolution_clock::now();
    std::set<int> perf_set;
    for (int i = 0; i < n; ++i) {
        perf_set.insert(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_set = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // vector 插入并排序
    start = std::chrono::high_resolution_clock::now();
    std::vector<int> perf_vec;
    for (int i = 0; i < n; ++i) {
        perf_vec.push_back(i);
    }
    std::sort(perf_vec.begin(), perf_vec.end());
    end = std::chrono::high_resolution_clock::now();
    auto duration_vec = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "插入 " << n << " 个有序元素:" << std::endl;
    std::cout << "set (红黑树): " << duration_set.count() << " μs" << std::endl;
    std::cout << "vector + sort: " << duration_vec.count() << " μs" << std::endl;
}

// 演示各种数据结构的应用场景
void demonstrateUseCases() {
    std::cout << "\n=== 数据结构应用场景演示 ===" << std::endl;
    
    // 场景1：Top K问题 - 使用最小堆
    std::cout << "\n--- 场景1：找出数组中最大的K个元素 ---" << std::endl;
    std::vector<int> nums = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9};
    int k = 5;
    
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;
    
    for (int num : nums) {
        if (minHeap.size() < k) {
            minHeap.push(num);
        } else if (num > minHeap.top()) {
            minHeap.pop();
            minHeap.push(num);
        }
    }
    
    std::cout << "原数组: ";
    for (int num : nums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    std::cout << "最大的 " << k << " 个元素: ";
    std::vector<int> result;
    while (!minHeap.empty()) {
        result.push_back(minHeap.top());
        minHeap.pop();
    }
    std::reverse(result.begin(), result.end());
    for (int num : result) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    // 场景2：LRU缓存 - 使用链表的特性
    std::cout << "\n--- 场景2：LRU缓存模拟（使用链表特性） ---" << std::endl;
    std::forward_list<std::pair<int, std::string>> lru_cache;
    int cache_size = 3;
    
    auto addToCache = [&](int key, const std::string& value) {
        // 检查是否已存在
        auto prev = lru_cache.before_begin();
        for (auto it = lru_cache.begin(); it != lru_cache.end(); ++it) {
            if (it->first == key) {
                // 删除旧项
                lru_cache.erase_after(prev);
                break;
            }
            ++prev;
        }
        
        // 添加到前面
        lru_cache.push_front({key, value});
        
        // 检查大小限制
        int count = 0;
        prev = lru_cache.before_begin();
        for (auto it = lru_cache.begin(); it != lru_cache.end() && count < cache_size; ++it, ++count) {
            ++prev;
        }
        
        // 删除超出大小的项
        while (std::next(prev) != lru_cache.end()) {
            lru_cache.erase_after(prev);
        }
        
        std::cout << "缓存状态: ";
        for (const auto& item : lru_cache) {
            std::cout << "[" << item.first << ":" << item.second << "] ";
        }
        std::cout << std::endl;
    };
    
    addToCache(1, "page1");
    addToCache(2, "page2");
    addToCache(3, "page3");
    addToCache(1, "page1");  // 访问已存在的
    addToCache(4, "page4");  // 超出容量，应该删除最久未使用的
}

int main() {
    std::cout << "=== C++ 高级数据结构演示程序 ===" << std::endl;
    
    try {
        demonstrateForwardList();
        demonstrateHeap();
        demonstrateBalancedTrees();
        demonstrateUseCases();
        
        std::cout << "\n=== 演示程序结束 ===" << std::endl;
        std::cout << "\n💡 总结:" << std::endl;
        std::cout << "1. forward_list 适合频繁前端插入的场景" << std::endl;
        std::cout << "2. 堆适合优先级队列和Top K问题" << std::endl;
        std::cout << "3. 红黑树（set/map）适合需要有序且频繁查找的场景" << std::endl;
        std::cout << "4. 选择合适的数据结构对性能至关重要！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

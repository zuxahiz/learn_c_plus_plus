# C++ Vector 容器详解

## 🎯 什么是Vector？
`vector` 是C++ STL中最常用的序列容器，它是一个动态数组，能够自动管理内存，支持随机访问。

## 🔧 基本特性
- **动态扩容**: 自动管理内存，无需手动分配
- **随机访问**: 支持O(1)时间复杂度的索引访问
- **连续存储**: 元素在内存中连续存储，缓存友好
- **自动扩容**: 当空间不足时自动扩容（通常扩容为原来的2倍）

## 📚 常用操作

### 1. 创建和初始化
```cpp
#include <vector>
using namespace std;

// 创建空vector
vector<int> v1;

// 创建指定大小的vector
vector<int> v2(10);           // 10个元素，初始值为0
vector<int> v3(10, 5);        // 10个元素，初始值为5

// 从数组初始化
int arr[] = {1, 2, 3, 4, 5};
vector<int> v4(arr, arr + 5);

// 列表初始化(C++11)
vector<int> v5 = {1, 2, 3, 4, 5};
```

### 2. 元素访问
```cpp
vector<int> v = {1, 2, 3, 4, 5};

// 下标访问
int first = v[0];              // 不检查边界
int second = v.at(1);          // 检查边界，越界抛出异常

// 首尾元素
int front = v.front();         // 第一个元素
int back = v.back();           // 最后一个元素

// 迭代器访问
for (auto it = v.begin(); it != v.end(); ++it) {
    cout << *it << " ";
}

// 范围for循环(C++11)
for (const auto& element : v) {
    cout << element << " ";
}
```

### 3. 容量操作
```cpp
vector<int> v = {1, 2, 3};

// 容量查询
cout << "Size: " << v.size() << endl;           // 当前元素个数
cout << "Capacity: " << v.capacity() << endl;   // 当前容量
cout << "Empty: " << v.empty() << endl;         // 是否为空

// 容量调整
v.reserve(100);                // 预留100个元素的空间
v.resize(10);                  // 调整大小为10，新元素初始化为0
v.resize(10, 5);              // 调整大小为10，新元素初始化为5
```

### 4. 元素操作
```cpp
vector<int> v = {1, 2, 3};

// 添加元素
v.push_back(4);                // 在末尾添加元素
v.insert(v.begin() + 1, 10);  // 在指定位置插入元素

// 删除元素
v.pop_back();                  // 删除最后一个元素
v.erase(v.begin() + 1);       // 删除指定位置的元素
v.clear();                     // 清空所有元素

// 交换
vector<int> v2 = {5, 6, 7};
v.swap(v2);                   // 交换两个vector的内容
```

## ⚡ 性能分析

### 时间复杂度
- **随机访问**: O(1)
- **末尾插入/删除**: 平均O(1)，最坏O(n)（需要扩容）
- **中间插入/删除**: O(n)
- **查找**: O(n)

### 空间复杂度
- **存储**: O(n)
- **额外空间**: 通常预留一些额外空间用于扩容

## 💡 使用技巧

### 1. 预分配空间
```cpp
// 如果知道大概需要多少元素，提前预留空间
vector<int> v;
v.reserve(1000);  // 避免频繁扩容
for (int i = 0; i < 1000; ++i) {
    v.push_back(i);
}
```

### 2. 避免频繁的中间插入/删除
```cpp
// 不好的做法：频繁在中间插入
vector<int> v = {1, 2, 3, 4, 5};
for (int i = 0; i < 1000; ++i) {
    v.insert(v.begin() + 2, i);  // 每次都要移动元素
}

// 好的做法：批量操作后一次性插入
vector<int> toInsert = {/* 要插入的元素 */};
v.insert(v.begin() + 2, toInsert.begin(), toInsert.end());
```

### 3. 使用emplace_back提高效率
```cpp
// 对于复杂对象，emplace_back更高效
vector<pair<int, string>> v;
v.emplace_back(1, "hello");  // 直接在容器中构造对象
// 等价于 v.push_back(make_pair(1, "hello"));
```

## 🎯 适用场景
- **需要随机访问**: 数组下标访问
- **动态大小**: 不知道最终需要多少元素
- **频繁末尾操作**: 主要在末尾添加/删除元素
- **缓存友好**: 需要良好的缓存性能

## ❌ 不适用场景
- **频繁中间插入/删除**: 考虑使用list或deque
- **固定大小**: 考虑使用普通数组
- **需要频繁排序**: 考虑使用set或multiset

## 🔍 常见错误
```cpp
// 错误1：越界访问
vector<int> v = {1, 2, 3};
int x = v[10];  // 未定义行为

// 错误2：迭代器失效
vector<int> v = {1, 2, 3, 4, 5};
for (auto it = v.begin(); it != v.end(); ++it) {
    if (*it == 3) {
        v.erase(it);  // 错误！迭代器失效
    }
}

// 正确做法
for (auto it = v.begin(); it != v.end();) {
    if (*it == 3) {
        it = v.erase(it);  // erase返回下一个有效迭代器
    } else {
        ++it;
    }
}
```

## 📝 练习题目
1. 实现一个简单的vector类
2. 使用vector解决数组相关问题
3. 练习vector的各种操作

## 🚀 下一步
- 学习 [set和map](./set-map-guide.md)
- 了解 [STL算法](./algorithms-guide.md)
- 开始 [数据结构实现](../02-data-structures/)

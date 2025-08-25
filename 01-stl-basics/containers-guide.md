# C++ STL 容器详解

## 🎯 什么是STL容器？

STL容器是用于存储和管理数据的类模板，它们提供了统一的操作接口，隐藏了内部实现的复杂性。容器是STL的核心组件之一，为算法提供了数据存储的基础。

## 🔧 容器分类

### 1. 序列容器 (Sequence Containers)
- **vector**: 动态数组，支持随机访问
- **deque**: 双端队列，支持两端快速插入删除
- **list**: 双向链表，支持任意位置插入删除
- **forward_list**: 单向链表，节省内存

### 2. 关联容器 (Associative Containers)
- **set**: 有序集合，不允许重复
- **multiset**: 有序集合，允许重复
- **map**: 有序映射，键值对，键唯一
- **multimap**: 有序映射，键值对，键可重复

### 3. 无序关联容器 (Unordered Associative Containers)
- **unordered_set**: 无序集合，基于哈希表
- **unordered_multiset**: 无序集合，允许重复
- **unordered_map**: 无序映射，基于哈希表
- **unordered_multimap**: 无序映射，键可重复

### 4. 容器适配器 (Container Adapters)
- **stack**: 栈，后进先出
- **queue**: 队列，先进先出
- **priority_queue**: 优先级队列

## 📚 序列容器详解

### Vector（动态数组）
```cpp
#include <vector>
using namespace std;

// 创建和初始化
vector<int> v1;                           // 空vector
vector<int> v2(10);                       // 10个元素，初始值为0
vector<int> v3(10, 5);                    // 10个元素，初始值为5
vector<int> v4 = {1, 2, 3, 4, 5};        // 列表初始化

// 基本操作
v1.push_back(10);                         // 末尾添加元素
v1.insert(v1.begin() + 1, 20);           // 指定位置插入
v1.pop_back();                            // 删除末尾元素
v1.erase(v1.begin() + 1);                // 删除指定位置元素

// 访问元素
int first = v4[0];                        // 下标访问
int second = v4.at(1);                    // 边界检查访问
int front = v4.front();                   // 第一个元素
int back = v4.back();                     // 最后一个元素

// 容量操作
v1.reserve(100);                          // 预留空间
v1.resize(20);                            // 调整大小
cout << "Size: " << v1.size() << endl;    // 当前大小
cout << "Capacity: " << v1.capacity() << endl;  // 当前容量
```

**特性**:
- 连续内存存储，支持随机访问
- 动态扩容，自动管理内存
- 末尾操作高效，中间操作较慢
- 缓存友好，适合频繁访问

### Deque（双端队列）
```cpp
#include <deque>
using namespace std;

deque<int> dq = {1, 2, 3, 4, 5};

// 双端操作
dq.push_front(0);                         // 前端添加
dq.push_back(6);                          // 后端添加
dq.pop_front();                           // 前端删除
dq.pop_back();                            // 后端删除

// 随机访问
int first = dq[0];
int last = dq[dq.size() - 1];
```

**特性**:
- 支持两端高效插入删除
- 支持随机访问
- 内存不连续，但分段连续
- 比vector稍慢，但更灵活

### List（双向链表）
```cpp
#include <list>
using namespace std;

list<int> lst = {1, 2, 3, 4, 5};

// 任意位置操作
auto it = lst.begin();
advance(it, 2);                           // 移动到第3个位置
lst.insert(it, 10);                       // 在指定位置插入
lst.erase(it);                            // 删除指定位置

// 链表特有操作
lst.splice(lst.begin(), lst, it);         // 移动元素
lst.reverse();                            // 反转链表
lst.sort();                               // 排序
lst.unique();                             // 去重
```

**特性**:
- 任意位置插入删除高效
- 不支持随机访问
- 内存不连续
- 迭代器稳定，插入删除不影响其他迭代器

## 📚 关联容器详解

### Set（有序集合）
```cpp
#include <set>
using namespace std;

set<int> s = {3, 1, 4, 1, 5, 9, 2, 6};  // 自动排序去重

// 基本操作
s.insert(7);                              // 插入元素
s.erase(3);                               // 删除元素
auto it = s.find(4);                      // 查找元素

// 边界操作
auto lower = s.lower_bound(4);            // 第一个不小于4的元素
auto upper = s.upper_bound(4);            // 第一个大于4的元素
auto range = s.equal_range(4);            // 等于4的元素范围

// 集合运算
set<int> s2 = {4, 5, 6, 7, 8};
set<int> result;
set_union(s.begin(), s.end(), s2.begin(), s2.end(), 
          inserter(result, result.begin()));  // 并集
```

**特性**:
- 基于红黑树实现
- 自动排序，不允许重复
- 查找、插入、删除都是O(log n)
- 支持范围查询和集合运算

### Map（有序映射）
```cpp
#include <map>
using namespace std;

map<string, int> m = {
    {"apple", 1},
    {"banana", 2},
    {"cherry", 3}
};

// 基本操作
m["orange"] = 4;                          // 插入或修改
m.insert({"grape", 5});                   // 插入
m.erase("banana");                        // 删除

// 访问元素
int value = m["apple"];                   // 如果不存在会创建
auto it = m.find("pear");                 // 安全查找
if (it != m.end()) {
    cout << it->first << ": " << it->second << endl;
}

// 遍历
for (const auto& [key, val] : m) {       // C++17结构化绑定
    cout << key << ": " << val << endl;
}
```

**特性**:
- 键值对存储，键唯一
- 基于红黑树，自动按键排序
- 支持范围查询和有序遍历
- 查找、插入、删除都是O(log n)

## 📚 无序关联容器详解

### Unordered Set/Map
```cpp
#include <unordered_set>
#include <unordered_map>
using namespace std;

unordered_set<int> us = {1, 2, 3, 4, 5};
unordered_map<string, int> um = {
    {"apple", 1},
    {"banana", 2}
};

// 基本操作与有序容器类似
us.insert(6);
um["cherry"] = 3;

// 哈希表特有操作
us.reserve(100);                          // 预留空间
cout << "Load factor: " << us.load_factor() << endl;
us.max_load_factor(0.5);                  // 设置最大负载因子
```

**特性**:
- 基于哈希表实现
- 平均O(1)的查找、插入、删除
- 不支持排序和范围查询
- 内存效率较高

## 📚 容器适配器详解

### Stack（栈）
```cpp
#include <stack>
using namespace std;

stack<int> st;
st.push(1);                               // 入栈
st.push(2);
st.push(3);

cout << "Top: " << st.top() << endl;      // 查看栈顶
st.pop();                                 // 出栈
cout << "Size: " << st.size() << endl;    // 大小
cout << "Empty: " << st.empty() << endl;  // 是否为空
```

### Queue（队列）
```cpp
#include <queue>
using namespace std;

queue<int> q;
q.push(1);                                // 入队
q.push(2);
q.push(3);

cout << "Front: " << q.front() << endl;   // 队首
cout << "Back: " << q.back() << endl;     // 队尾
q.pop();                                  // 出队
```

### Priority Queue（优先级队列）
```cpp
#include <queue>
using namespace std;

priority_queue<int> pq;                   // 默认最大堆
pq.push(3);
pq.push(1);
pq.push(5);
pq.push(2);

cout << "Top: " << pq.top() << endl;      // 最大元素
pq.pop();                                 // 删除最大元素

// 最小堆
priority_queue<int, vector<int>, greater<int>> min_pq;
```

## ⚡ 容器选择指南

### 选择标准
1. **访问模式**: 随机访问选vector，顺序访问选list
2. **插入删除**: 频繁中间操作选list，频繁末尾操作选vector
3. **查找需求**: 需要排序选set/map，只需要存在性检查选unordered_set/map
4. **内存要求**: 内存敏感选list，缓存友好选vector
5. **性能要求**: 对性能要求高选unordered容器，需要有序选有序容器

### 性能对比
| 容器 | 随机访问 | 插入删除 | 查找 | 内存效率 |
|------|----------|----------|------|----------|
| vector | O(1) | O(n) | O(n) | 高 |
| deque | O(1) | O(1) | O(n) | 中 |
| list | O(n) | O(1) | O(n) | 低 |
| set/map | O(log n) | O(log n) | O(log n) | 中 |
| unordered_set/map | O(1) | O(1) | O(1) | 中 |

## 💡 使用技巧

### 1. 预分配空间
```cpp
vector<int> v;
v.reserve(1000);                          // 避免频繁扩容
for (int i = 0; i < 1000; ++i) {
    v.push_back(i);
}
```

### 2. 选择合适的容器
```cpp
// 需要频繁查找
unordered_set<int> us;
for (int i = 0; i < 1000; ++i) {
    us.insert(i);
}

// 需要有序遍历
set<int> s;
for (int i = 0; i < 1000; ++i) {
    s.insert(i);
}
```

### 3. 避免常见错误
```cpp
// 错误：在遍历时修改容器
vector<int> v = {1, 2, 3, 4, 5};
for (auto it = v.begin(); it != v.end(); ++it) {
    if (*it % 2 == 0) {
        v.erase(it);                      // 错误！迭代器失效
    }
}

// 正确做法
v.erase(remove_if(v.begin(), v.end(), 
                  [](int x) { return x % 2 == 0; }), v.end());
```

## 🎯 适用场景总结

### Vector适用场景
- 需要随机访问
- 大小不固定
- 主要在末尾操作
- 需要良好的缓存性能

### List适用场景
- 频繁在任意位置插入删除
- 不需要随机访问
- 迭代器稳定性要求高

### Set/Map适用场景
- 需要自动排序
- 需要去重
- 需要范围查询
- 查找操作频繁

### Unordered容器适用场景
- 不需要排序
- 需要O(1)的查找性能
- 内存效率要求高

## 🚀 下一步
- 学习 [迭代器详解](./iterators-guide.md)
- 了解 [算法详解](./algorithms-guide.md)
- 掌握 [分配器详解](./allocators-guide.md)

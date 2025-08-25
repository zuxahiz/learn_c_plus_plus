# C++ Unordered 容器详解

## 🎯 什么是Unordered容器？

### unordered_set
- **无序集合**: 元素不按顺序存储，基于哈希表实现
- **快速查找**: O(1)平均时间复杂度的查找、插入、删除
- **唯一性**: 不允许重复元素

### unordered_map
- **无序映射**: 键值对容器，基于哈希表实现
- **快速访问**: O(1)平均时间复杂度的查找和修改
- **键唯一**: 每个key只能对应一个value

## 🔧 基本特性

### 优势
- **快速操作**: 平均O(1)时间复杂度
- **内存效率**: 比红黑树容器更节省内存
- **简单实现**: 基于哈希函数，实现相对简单

### 劣势
- **无序性**: 元素不按顺序存储
- **最坏情况**: 最坏情况下可能退化到O(n)
- **哈希冲突**: 需要处理哈希冲突

## 📚 常用操作

### 1. unordered_set基本操作
```cpp
#include <unordered_set>
using namespace std;

// 创建和初始化
unordered_set<int> us1;
unordered_set<int> us2 = {3, 1, 4, 1, 5};  // 自动去重

// 插入元素
us1.insert(10);
us1.insert({20, 30, 40});

// 查找元素
if (us1.find(20) != us1.end()) {
    cout << "Found 20" << endl;
}

// 检查元素存在性
if (us1.count(20)) {
    cout << "20 exists" << endl;
}

// 删除元素
us1.erase(20);

// 遍历（无序）
for (const auto& element : us1) {
    cout << element << " ";
}
```

### 2. unordered_map基本操作
```cpp
#include <unordered_map>
using namespace std;

// 创建和初始化
unordered_map<string, int> um1;
unordered_map<string, int> um2 = {
    {"apple", 1},
    {"banana", 2}
};

// 插入和访问
um1["orange"] = 3;
um1.insert({"grape", 4});

// 安全访问
auto it = um1.find("pear");
if (it != um1.end()) {
    cout << "Value: " << it->second << endl;
}

// 修改值
um1["orange"] = 10;

// 删除元素
um1.erase("orange");

// 遍历
for (const auto& [key, value] : um1) {
    cout << key << ": " << value << endl;
}
```

## ⚡ 性能分析

### 时间复杂度
- **平均情况**: O(1) - 查找、插入、删除
- **最坏情况**: O(n) - 哈希冲突严重时
- **遍历**: O(n)

### 空间复杂度
- **存储**: O(n)
- **哈希表开销**: 额外的哈希表结构

## 💡 使用技巧

### 1. 自定义哈希函数
```cpp
// 为自定义类型定义哈希函数
struct Point {
    int x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

struct PointHash {
    size_t operator()(const Point& p) const {
        return hash<int>()(p.x) ^ (hash<int>()(p.y) << 1);
    }
};

unordered_set<Point, PointHash> pointSet;
```

### 2. 性能优化
```cpp
// 预分配空间
unordered_set<int> us;
us.reserve(1000);  // 避免频繁重新哈希

// 设置负载因子
us.max_load_factor(0.5);  // 降低负载因子提高性能
```

## 🎯 适用场景

### 适用场景
- **快速查找**: 需要O(1)平均时间复杂度的查找
- **不需要排序**: 元素顺序不重要
- **频繁插入/删除**: 动态数据操作
- **内存敏感**: 相比红黑树更节省内存

### 不适用场景
- **需要有序**: 需要按顺序访问元素
- **范围查询**: 需要范围操作（如lower_bound）
- **稳定性要求**: 需要稳定的性能保证

## 🔍 常见错误

### 1. 哈希冲突处理
```cpp
// 注意：unordered容器不保证元素顺序
unordered_set<int> us = {1, 2, 3, 4, 5};
// 遍历顺序可能每次都不一样
```

### 2. 性能退化
```cpp
// 避免插入过多重复元素导致哈希冲突
unordered_set<int> us;
for (int i = 0; i < 10000; ++i) {
    us.insert(i % 100);  // 大量重复值可能导致性能问题
}
```

## 📝 实际应用示例

### 1. 快速去重
```cpp
vector<int> numbers = {3, 1, 4, 1, 5, 9, 2, 6};
unordered_set<int> uniqueNumbers(numbers.begin(), numbers.end());
// 比set更快，但结果无序
```

### 2. 缓存实现
```cpp
unordered_map<string, int> cache;
string key = "expensive_calculation";

if (cache.find(key) == cache.end()) {
    cache[key] = expensive_function();
}
return cache[key];
```

## 🚀 下一步
- 学习 [容器适配器](./adapters-guide.md)
- 了解 [STL算法](./algorithms-guide.md)
- 开始 [数据结构实现](../02-data-structures/)

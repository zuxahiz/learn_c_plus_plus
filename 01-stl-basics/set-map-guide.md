# C++ Set 和 Map 容器详解

## 🎯 什么是Set和Map？

### Set（集合）
- **有序集合**: 元素自动排序，不允许重复
- **基于红黑树**: 内部使用红黑树实现，保证O(log n)的操作复杂度
- **唯一性**: 每个元素在集合中只能出现一次

### Map（映射）
- **键值对容器**: 存储key-value对，key唯一
- **有序存储**: 按key自动排序
- **快速查找**: 基于key的快速查找和访问

## 🔧 基本特性

### Set特性
- **自动排序**: 元素按升序排列
- **唯一性**: 不允许重复元素
- **不可修改**: 不能直接修改元素值
- **迭代器稳定**: 插入/删除不会使其他迭代器失效

### Map特性
- **键值对**: 每个元素是一个pair<key, value>
- **键唯一**: 每个key只能对应一个value
- **自动排序**: 按key升序排列
- **快速查找**: O(log n)时间复杂度的查找

## 📚 常用操作

### 1. Set的基本操作
```cpp
#include <set>
using namespace std;

// 创建和初始化
set<int> s1;                           // 空集合
set<int> s2 = {3, 1, 4, 1, 5};        // 自动去重和排序：{1, 3, 4, 5}

// 插入元素
s1.insert(10);                         // 插入单个元素
s1.insert({20, 30, 40});              // 插入多个元素

// 查找元素
auto it = s1.find(20);                 // 返回迭代器，未找到返回end()
if (it != s1.end()) {
    cout << "Found: " << *it << endl;
}

// 检查元素是否存在
if (s1.count(20)) {                    // 返回0或1
    cout << "20 exists" << endl;
}

// 删除元素
s1.erase(20);                          // 删除值为20的元素
s1.erase(it);                          // 删除迭代器指向的元素

// 遍历
for (const auto& element : s1) {
    cout << element << " ";
}
```

### 2. Map的基本操作
```cpp
#include <map>
using namespace std;

// 创建和初始化
map<string, int> m1;                   // 空映射
map<string, int> m2 = {
    {"apple", 1},
    {"banana", 2},
    {"cherry", 3}
};

// 插入元素
m1["orange"] = 4;                      // 使用下标操作符
m1.insert({"grape", 5});               // 使用insert方法
m1.insert({"orange", 6});              // 不会覆盖已存在的key

// 访问元素
int value = m1["orange"];              // 如果key不存在，会创建并初始化为0
int value2 = m1.at("banana");          // 如果key不存在，抛出异常

// 安全访问
auto it = m1.find("pear");
if (it != m1.end()) {
    cout << "Value: " << it->second << endl;
}

// 修改元素
m1["orange"] = 10;                     // 修改已存在的值

// 删除元素
m1.erase("orange");                    // 删除指定key的元素

// 遍历
for (const auto& pair : m1) {
    cout << pair.first << ": " << pair.second << endl;
}

// 使用结构化绑定(C++17)
for (const auto& [key, value] : m1) {
    cout << key << ": " << value << endl;
}
```

## ⚡ 性能分析

### 时间复杂度
- **查找**: O(log n)
- **插入**: O(log n)
- **删除**: O(log n)
- **遍历**: O(n)

### 空间复杂度
- **存储**: O(n)
- **每个节点**: 额外存储平衡信息

## 💡 使用技巧

### 1. 自定义比较函数
```cpp
// 自定义比较函数
struct Compare {
    bool operator()(const string& a, const string& b) const {
        return a.length() < b.length();  // 按长度排序
    }
};

set<string, Compare> s;
s.insert({"apple", "banana", "cherry"});  // 按长度排序

// 对于map
map<string, int, Compare> m;
```

### 2. 使用emplace提高效率
```cpp
// 对于复杂对象，emplace更高效
set<pair<int, string>> s;
s.emplace(1, "hello");  // 直接在容器中构造对象

map<int, string> m;
m.emplace(1, "world");  // 直接在容器中构造对象
```

### 3. 边界操作
```cpp
set<int> s = {1, 3, 5, 7, 9};

// 找到第一个不小于给定值的元素
auto it = s.lower_bound(4);  // 返回指向5的迭代器

// 找到第一个大于给定值的元素
auto it2 = s.upper_bound(4); // 返回指向5的迭代器

// 找到等于给定值的范围
auto range = s.equal_range(5); // 返回pair<iterator, iterator>
```

## 🎯 适用场景

### Set适用场景
- **需要去重**: 去除重复元素
- **需要排序**: 自动维护有序性
- **快速查找**: 频繁的查找操作
- **集合运算**: 交集、并集、差集等

### Map适用场景
- **键值映射**: 需要建立key-value关系
- **字典功能**: 类似Python的dict
- **计数统计**: 统计元素出现次数
- **缓存实现**: 存储计算结果

## ❌ 不适用场景
- **频繁插入/删除**: 考虑使用unordered容器
- **不需要排序**: 考虑使用unordered容器
- **内存敏感**: 红黑树有额外的内存开销

## 🔍 常见错误

### 1. 迭代器失效
```cpp
// 错误：在遍历时删除元素
set<int> s = {1, 2, 3, 4, 5};
for (auto it = s.begin(); it != s.end(); ++it) {
    if (*it % 2 == 0) {
        s.erase(it);  // 错误！迭代器失效
    }
}

// 正确做法
for (auto it = s.begin(); it != s.end();) {
    if (*it % 2 == 0) {
        it = s.erase(it);  // erase返回下一个有效迭代器
    } else {
        ++it;
    }
}
```

### 2. 访问不存在的key
```cpp
map<string, int> m;
int value = m["nonexistent"];  // 会创建key并初始化为0

// 安全做法
auto it = m.find("nonexistent");
if (it != m.end()) {
    value = it->second;
}
```

## 📝 实际应用示例

### 1. 统计单词频率
```cpp
map<string, int> wordCount;
string word;

while (cin >> word) {
    wordCount[word]++;
}

// 输出结果
for (const auto& [word, count] : wordCount) {
    cout << word << ": " << count << endl;
}
```

### 2. 去重并排序
```cpp
vector<int> numbers = {3, 1, 4, 1, 5, 9, 2, 6};
set<int> uniqueNumbers(numbers.begin(), numbers.end());

// 转换回vector
vector<int> result(uniqueNumbers.begin(), uniqueNumbers.end());
```

### 3. 查找最近的值
```cpp
set<int> s = {1, 3, 5, 7, 9};
int target = 4;

auto it = s.lower_bound(target);
if (it != s.begin()) {
    auto prev = prev(it);
    int closest = (target - *prev < *it - target) ? *prev : *it;
    cout << "Closest: " << closest << endl;
}
```

## 📝 练习题目
1. 实现一个简单的set类
2. 使用map统计字符频率
3. 实现集合的交集、并集、差集操作

## 🚀 下一步
- 学习 [unordered容器](./unordered-guide.md)
- 了解 [容器适配器](./adapters-guide.md)
- 开始 [数据结构实现](../02-data-structures/)

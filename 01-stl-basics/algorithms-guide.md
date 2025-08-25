# C++ STL 算法详解

## 🎯 什么是STL算法？

STL算法是一组通用的函数模板，可以对容器中的元素进行各种操作：
- **非修改算法**: 不改变容器内容，如查找、计数
- **修改算法**: 改变容器内容，如排序、复制、替换
- **排序算法**: 对元素进行排序和查找
- **数值算法**: 进行数值计算

## 🔧 核心特性

### 优势
- **通用性**: 可以用于任何满足要求的容器
- **高效性**: 经过优化的实现
- **一致性**: 统一的接口和命名规范
- **可组合**: 可以组合使用实现复杂功能

### 设计原则
- **迭代器抽象**: 通过迭代器访问容器元素
- **函数对象**: 支持自定义比较和操作函数
- **范围操作**: 操作指定范围内的元素

## 📚 常用算法分类

### 1. 查找算法
```cpp
#include <algorithm>
#include <vector>
using namespace std;

vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// 线性查找
auto it = find(v.begin(), v.end(), 5);
if (it != v.end()) {
    cout << "Found 5 at position: " << (it - v.begin()) << endl;
}

// 条件查找
auto it2 = find_if(v.begin(), v.end(), [](int x) { return x > 7; });
if (it2 != v.end()) {
    cout << "First element > 7: " << *it2 << endl;
}

// 二分查找（要求容器已排序）
if (binary_search(v.begin(), v.end(), 6)) {
    cout << "6 found using binary search" << endl;
}

// 查找第一个不满足条件的元素
auto it3 = find_if_not(v.begin(), v.end(), [](int x) { return x < 5; });
cout << "First element >= 5: " << *it3 << endl;
```

### 2. 排序算法
```cpp
vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};

// 排序
sort(v.begin(), v.end());                    // 升序排序
sort(v.begin(), v.end(), greater<int>());   // 降序排序

// 部分排序
partial_sort(v.begin(), v.begin() + 3, v.end());  // 前3个元素排序

// 稳定排序
stable_sort(v.begin(), v.end());            // 保持相等元素的相对顺序

// 堆操作
make_heap(v.begin(), v.end());              // 构建最大堆
sort_heap(v.begin(), v.end());              // 堆排序

// 检查是否已排序
if (is_sorted(v.begin(), v.end())) {
    cout << "Vector is sorted" << endl;
}
```

### 3. 修改算法
```cpp
vector<int> v = {1, 2, 3, 4, 5};

// 复制
vector<int> dest(5);
copy(v.begin(), v.end(), dest.begin());

// 填充
fill(v.begin(), v.end(), 0);               // 填充0
fill_n(v.begin(), 3, 10);                  // 前3个元素填充10

// 替换
replace(v.begin(), v.end(), 3, 30);        // 替换所有3为30
replace_if(v.begin(), v.end(), 
          [](int x) { return x % 2 == 0; }, 0);  // 替换所有偶数为0

// 移除
auto newEnd = remove(v.begin(), v.end(), 0);      // 移除所有0
v.erase(newEnd, v.end());                         // 调整大小

// 变换
transform(v.begin(), v.end(), v.begin(), 
          [](int x) { return x * 2; });           // 所有元素乘2
```

### 4. 数值算法
```cpp
#include <numeric>
vector<int> v = {1, 2, 3, 4, 5};

// 累加
int sum = accumulate(v.begin(), v.end(), 0);
int product = accumulate(v.begin(), v.end(), 1, multiplies<int>());

// 内积
vector<int> v2 = {2, 3, 4, 5, 6};
int innerProduct = inner_product(v.begin(), v.end(), v2.begin(), 0);

// 部分和
vector<int> partialSums(v.size());
partial_sum(v.begin(), v.end(), partialSums.begin());

// 相邻差
vector<int> differences(v.size() - 1);
adjacent_difference(v.begin(), v.end(), differences.begin());
```

### 5. 集合算法
```cpp
vector<int> v1 = {1, 2, 3, 4, 5};
vector<int> v2 = {3, 4, 5, 6, 7};
vector<int> result;

// 并集
set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), 
          back_inserter(result));

// 交集
result.clear();
set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), 
                 back_inserter(result));

// 差集
result.clear();
set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), 
               back_inserter(result));

// 对称差集
result.clear();
set_symmetric_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), 
                        back_inserter(result));
```

## 💡 使用技巧

### 1. 使用函数对象和lambda表达式
```cpp
vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// 使用函数对象
struct IsEven {
    bool operator()(int x) const { return x % 2 == 0; }
};
auto it = find_if(v.begin(), v.end(), IsEven());

// 使用lambda表达式（更简洁）
auto it2 = find_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; });

// 自定义比较函数
sort(v.begin(), v.end(), [](int a, int b) {
    return abs(a) < abs(b);  // 按绝对值排序
});
```

### 2. 使用迭代器适配器
```cpp
vector<int> v = {1, 2, 3, 4, 5};

// 反向迭代器
for (auto it = v.rbegin(); it != v.rend(); ++it) {
    cout << *it << " ";  // 5 4 3 2 1
}

// 插入迭代器
vector<int> dest;
copy(v.begin(), v.end(), back_inserter(dest));  // 自动扩展容器

// 流迭代器
copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
```

### 3. 算法组合使用
```cpp
vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// 找出所有偶数的平方
vector<int> result;
copy_if(v.begin(), v.end(), back_inserter(result), 
        [](int x) { return x % 2 == 0; });
transform(result.begin(), result.end(), result.begin(), 
          [](int x) { return x * x; });

// 一行代码实现
vector<int> result2;
transform(
    v.begin(), v.end(), back_inserter(result2),
    [](int x) { return x * x; },
    [](int x) { return x % 2 == 0; }
);
```

## 🎯 适用场景

### 查找算法适用场景
- **线性查找**: 小数据集或无序数据
- **二分查找**: 大数据集且已排序
- **条件查找**: 需要复杂条件判断

### 排序算法适用场景
- **快速排序**: 一般情况下的排序
- **稳定排序**: 需要保持相等元素顺序
- **部分排序**: 只需要前K个元素排序

### 修改算法适用场景
- **数据清洗**: 移除无效数据
- **数据转换**: 批量修改数据
- **数据复制**: 在不同容器间转移数据

## 🔍 常见错误

### 1. 迭代器失效
```cpp
vector<int> v = {1, 2, 3, 4, 5};

// 错误：在遍历时修改容器
for (auto it = v.begin(); it != v.end(); ++it) {
    if (*it % 2 == 0) {
        v.erase(it);  // 错误！迭代器失效
    }
}

// 正确做法：使用remove + erase
v.erase(remove_if(v.begin(), v.end(), 
                  [](int x) { return x % 2 == 0; }), v.end());
```

### 2. 未排序容器使用二分查找
```cpp
vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};

// 错误：未排序容器使用binary_search
if (binary_search(v.begin(), v.end(), 6)) {  // 未定义行为
    cout << "Found 6" << endl;
}

// 正确做法：先排序
sort(v.begin(), v.end());
if (binary_search(v.begin(), v.end(), 6)) {
    cout << "Found 6" << endl;
}
```

## 📝 实际应用示例

### 1. 查找重复元素
```cpp
vector<int> v = {1, 2, 3, 2, 4, 5, 3, 6};

// 方法1：使用sort + adjacent_find
sort(v.begin(), v.end());
auto it = adjacent_find(v.begin(), v.end());
if (it != v.end()) {
    cout << "First duplicate: " << *it << endl;
}

// 方法2：使用unordered_set
unordered_set<int> seen;
for (int x : v) {
    if (seen.count(x)) {
        cout << "Duplicate found: " << x << endl;
        break;
    }
    seen.insert(x);
}
```

### 2. 实现自定义排序
```cpp
struct Person {
    string name;
    int age;
    double salary;
};

vector<Person> people = {
    {"Alice", 25, 50000},
    {"Bob", 30, 60000},
    {"Charlie", 25, 55000}
};

// 按年龄升序，年龄相同时按薪资降序
sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
    if (a.age != b.age) return a.age < b.age;
    return a.salary > b.salary;
});
```

## 📝 练习题目
1. 实现一个通用的查找函数
2. 使用STL算法实现去重功能
3. 实现自定义比较器的排序
4. 组合使用多个算法解决复杂问题

## 🚀 下一步
- 开始 [数据结构实现](../02-data-structures/)
- 准备 [LeetCode刷题](../04-leetcode-guide/)
- 学习 [面试准备](../05-interview-prep/)

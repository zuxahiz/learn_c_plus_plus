# C++ STL 迭代器详解

## 🎯 什么是迭代器？

迭代器是STL的核心概念，它提供了一种统一的方式来访问容器中的元素，无论容器的内部实现如何。迭代器是算法和容器之间的桥梁，使得算法可以独立于具体的容器类型。

## 🔧 迭代器分类

### 1. 按功能分类
- **输入迭代器 (Input Iterator)**: 只读，只能向前移动
- **输出迭代器 (Output Iterator)**: 只写，只能向前移动
- **前向迭代器 (Forward Iterator)**: 可读写，只能向前移动
- **双向迭代器 (Bidirectional Iterator)**: 可读写，可向前向后移动
- **随机访问迭代器 (Random Access Iterator)**: 可读写，可任意位置跳转

### 2. 按容器支持分类
- **vector, deque**: 随机访问迭代器
- **list, set, map**: 双向迭代器
- **forward_list**: 前向迭代器
- **stack, queue, priority_queue**: 不支持迭代器

## 📚 迭代器基本操作

### 1. 迭代器创建和基本操作
```cpp
#include <vector>
#include <list>
#include <set>
using namespace std;

vector<int> v = {1, 2, 3, 4, 5};

// 获取迭代器
auto begin_it = v.begin();                 // 开始迭代器
auto end_it = v.end();                     // 结束迭代器
auto cbegin_it = v.cbegin();               // 常量开始迭代器
auto cend_it = v.cend();                   // 常量结束迭代器

// 反向迭代器
auto rbegin_it = v.rbegin();               // 反向开始迭代器
auto rend_it = v.rend();                   // 反向结束迭代器

// 基本操作
cout << "第一个元素: " << *begin_it << endl;  // 解引用
++begin_it;                                // 向前移动
--begin_it;                                // 向后移动（双向迭代器）
begin_it += 2;                             // 随机访问（随机访问迭代器）
```

### 2. 迭代器遍历
```cpp
vector<int> v = {1, 2, 3, 4, 5};

// 方法1：传统for循环
for (auto it = v.begin(); it != v.end(); ++it) {
    cout << *it << " ";
}
cout << endl;

// 方法2：范围for循环（C++11）
for (const auto& element : v) {
    cout << element << " ";
}
cout << endl;

// 方法3：反向遍历
for (auto it = v.rbegin(); it != v.rend(); ++it) {
    cout << *it << " ";
}
cout << endl;

// 方法4：使用迭代器距离
for (auto it = v.begin(); it != v.begin() + v.size(); ++it) {
    cout << *it << " ";
}
cout << endl;
```

## 📚 迭代器类型详解

### 1. 输入迭代器 (Input Iterator)
```cpp
#include <iostream>
#include <iterator>
using namespace std;

// 输入迭代器示例：从标准输入读取
istream_iterator<int> input_it(cin);
istream_iterator<int> end_it;

vector<int> numbers;
while (input_it != end_it) {
    numbers.push_back(*input_it);
    ++input_it;
}

// 输入迭代器只能使用一次
// 不能保存状态，不能比较两个输入迭代器
```

**特性**:
- 只读访问
- 只能向前移动
- 不能保存状态
- 典型应用：输入流、算法中的只读遍历

### 2. 输出迭代器 (Output Iterator)
```cpp
#include <iterator>
using namespace std;

vector<int> v = {1, 2, 3, 4, 5};

// 输出迭代器示例：向标准输出写入
ostream_iterator<int> output_it(cout, " ");
copy(v.begin(), v.end(), output_it);

// 输出迭代器只能写入，不能读取
// 不能比较两个输出迭代器
```

**特性**:
- 只写访问
- 只能向前移动
- 不能保存状态
- 典型应用：输出流、算法中的写入操作

### 3. 前向迭代器 (Forward Iterator)
```cpp
#include <forward_list>
using namespace std;

forward_list<int> fl = {1, 2, 3, 4, 5};

// 前向迭代器可以多次遍历
auto it1 = fl.begin();
auto it2 = it1;                            // 可以复制

// 向前移动
++it1;
++it2;

// 可以比较
if (it1 == it2) {
    cout << "迭代器相等" << endl;
}
```

**特性**:
- 可读写访问
- 只能向前移动
- 可以保存状态
- 可以多次遍历
- 典型应用：forward_list、单链表

### 4. 双向迭代器 (Bidirectional Iterator)
```cpp
#include <list>
#include <set>
using namespace std;

list<int> lst = {1, 2, 3, 4, 5};

auto it = lst.begin();
++it;                                       // 向前移动
--it;                                       // 向后移动

// 双向迭代器支持所有前向迭代器的操作
// 还可以向后移动
```

**特性**:
- 可读写访问
- 可向前向后移动
- 可以保存状态
- 可以多次遍历
- 典型应用：list、set、map

### 5. 随机访问迭代器 (Random Access Iterator)
```cpp
#include <vector>
#include <deque>
using namespace std;

vector<int> v = {1, 2, 3, 4, 5};

auto it = v.begin();

// 随机访问操作
it += 3;                                   // 向前跳转3个位置
it -= 1;                                   // 向后跳转1个位置
it = it + 2;                               // 算术运算
it = it - 1;

// 比较操作
if (it > v.begin()) {
    cout << "it在begin之后" << endl;
}

// 下标访问
cout << "第3个元素: " << it[2] << endl;
```

**特性**:
- 可读写访问
- 可任意位置跳转
- 支持算术运算
- 支持比较操作
- 支持下标访问
- 典型应用：vector、deque、普通数组

## 📚 迭代器适配器

### 1. 插入迭代器 (Insert Iterators)
```cpp
#include <iterator>
#include <vector>
#include <list>
using namespace std;

vector<int> v1 = {1, 2, 3};
vector<int> v2 = {4, 5, 6};

// back_inserter: 在末尾插入
copy(v2.begin(), v2.end(), back_inserter(v1));

// front_inserter: 在开头插入（需要容器支持push_front）
list<int> lst = {1, 2, 3};
copy(v2.begin(), v2.end(), front_inserter(lst));

// inserter: 在指定位置插入
auto it = v1.begin() + 1;
copy(v2.begin(), v2.end(), inserter(v1, it));
```

### 2. 流迭代器 (Stream Iterators)
```cpp
#include <iterator>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> v = {1, 2, 3, 4, 5};

// 输出流迭代器
ostream_iterator<int> output_it(cout, " ");
copy(v.begin(), v.end(), output_it);

// 输入流迭代器
vector<int> input_numbers;
istream_iterator<int> input_it(cin);
istream_iterator<int> end_it;

copy(input_it, end_it, back_inserter(input_numbers));
```

### 3. 反向迭代器 (Reverse Iterators)
```cpp
#include <vector>
#include <algorithm>
using namespace std;

vector<int> v = {1, 2, 3, 4, 5};

// 反向迭代器
for (auto it = v.rbegin(); it != v.rend(); ++it) {
    cout << *it << " ";
}

// 反向迭代器与正向迭代器的关系
auto forward_it = v.begin() + 2;
auto reverse_it = reverse_iterator<vector<int>::iterator>(forward_it);
cout << "正向位置2: " << *forward_it << endl;
cout << "反向位置2: " << *reverse_it << endl;
```

### 4. 移动迭代器 (Move Iterators)
```cpp
#include <iterator>
#include <vector>
#include <string>
using namespace std;

vector<string> v1 = {"hello", "world"};
vector<string> v2;

// 使用移动迭代器避免拷贝
copy(make_move_iterator(v1.begin()), 
     make_move_iterator(v1.end()), 
     back_inserter(v2));

// v1中的字符串现在处于"移动后"状态
cout << "v1[0]: '" << v1[0] << "'" << endl;  // 可能是空字符串
```

## 💡 迭代器使用技巧

### 1. 迭代器失效处理
```cpp
vector<int> v = {1, 2, 3, 4, 5};

// 错误：在遍历时修改容器
for (auto it = v.begin(); it != v.end(); ++it) {
    if (*it % 2 == 0) {
        v.erase(it);                      // 错误！迭代器失效
    }
}

// 正确做法1：使用erase的返回值
for (auto it = v.begin(); it != v.end();) {
    if (*it % 2 == 0) {
        it = v.erase(it);                 // erase返回下一个有效迭代器
    } else {
        ++it;
    }
}

// 正确做法2：使用remove + erase
v.erase(remove_if(v.begin(), v.end(), 
                  [](int x) { return x % 2 == 0; }), v.end());
```

### 2. 迭代器距离和范围
```cpp
vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// 计算迭代器距离
auto it1 = v.begin();
auto it2 = v.begin() + 5;
ptrdiff_t distance = it2 - it1;           // 距离为5

// 使用迭代器范围
auto start = v.begin() + 2;
auto end = v.begin() + 7;
for (auto it = start; it != end; ++it) {
    cout << *it << " ";                   // 输出3 4 5 6 7
}
```

### 3. 自定义迭代器
```cpp
#include <iterator>
using namespace std;

// 简单的自定义迭代器
template<typename T>
class ArrayIterator {
private:
    T* ptr;
    
public:
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    
    ArrayIterator(T* p) : ptr(p) {}
    
    // 解引用
    reference operator*() { return *ptr; }
    pointer operator->() { return ptr; }
    
    // 递增递减
    ArrayIterator& operator++() { ++ptr; return *this; }
    ArrayIterator& operator--() { --ptr; return *this; }
    
    // 算术运算
    ArrayIterator operator+(difference_type n) { return ArrayIterator(ptr + n); }
    ArrayIterator operator-(difference_type n) { return ArrayIterator(ptr - n); }
    
    // 比较
    bool operator==(const ArrayIterator& other) { return ptr == other.ptr; }
    bool operator!=(const ArrayIterator& other) { return ptr != other.ptr; }
    
    // 下标访问
    reference operator[](difference_type n) { return *(ptr + n); }
};

// 使用示例
int arr[] = {1, 2, 3, 4, 5};
ArrayIterator<int> begin_it(arr);
ArrayIterator<int> end_it(arr + 5);

for (auto it = begin_it; it != end_it; ++it) {
    cout << *it << " ";
}
```

## 🎯 迭代器应用场景

### 1. 算法中使用
```cpp
#include <algorithm>
#include <vector>
using namespace std;

vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};

// 查找
auto it = find(v.begin(), v.end(), 5);
if (it != v.end()) {
    cout << "找到5，位置: " << (it - v.begin()) << endl;
}

// 排序
sort(v.begin(), v.end());

// 复制
vector<int> v2;
copy(v.begin(), v.end(), back_inserter(v2));

// 变换
transform(v.begin(), v.end(), v.begin(), 
          [](int x) { return x * 2; });
```

### 2. 容器操作中使用
```cpp
vector<int> v = {1, 2, 3, 4, 5};

// 插入
v.insert(v.begin() + 2, 10);

// 删除
v.erase(v.begin() + 1);

// 范围构造
vector<int> v2(v.begin() + 1, v.begin() + 4);  // {2, 3, 4}
```

### 3. 自定义算法中使用
```cpp
// 自定义查找算法
template<typename Iterator, typename T>
Iterator my_find(Iterator first, Iterator last, const T& value) {
    while (first != last) {
        if (*first == value) {
            return first;
        }
        ++first;
    }
    return last;
}

// 使用示例
vector<int> v = {1, 2, 3, 4, 5};
auto it = my_find(v.begin(), v.end(), 3);
if (it != v.end()) {
    cout << "找到3" << endl;
}
```

## ⚡ 性能考虑

### 1. 迭代器类型对性能的影响
```cpp
// 随机访问迭代器：O(1)访问
vector<int> v = {1, 2, 3, 4, 5};
auto it = v.begin() + 3;                  // 直接跳转

// 双向迭代器：O(n)访问
list<int> lst = {1, 2, 3, 4, 5};
auto it = lst.begin();
advance(it, 3);                           // 需要遍历
```

### 2. 迭代器失效的影响
- **vector**: 插入删除可能导致所有迭代器失效
- **list**: 插入删除不会影响其他迭代器
- **set/map**: 插入删除不会影响其他迭代器
- **unordered容器**: 插入删除可能导致所有迭代器失效

## 🚀 下一步
- 学习 [算法详解](./algorithms-guide.md)
- 了解 [分配器详解](./allocators-guide.md)
- 掌握 [仿函数详解](./functors-guide.md)

# C++ STL 仿函数详解

## 🎯 什么是仿函数？

仿函数(Functor)是STL中可调用对象的统称，它是一个行为类似函数的对象。仿函数通过重载`operator()`操作符来实现函数调用，可以像函数一样被调用，同时还可以携带状态和数据。

## 🔧 仿函数的分类

### 1. 按功能分类
- **算术仿函数**: 进行算术运算
- **关系仿函数**: 进行关系比较
- **逻辑仿函数**: 进行逻辑运算
- **自定义仿函数**: 用户定义的特定功能

### 2. 按参数数量分类
- **一元仿函数**: 接受一个参数
- **二元仿函数**: 接受两个参数
- **多元仿函数**: 接受多个参数

## 📚 标准仿函数

### 1. 算术仿函数
```cpp
#include <functional>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> v = {1, 2, 3, 4, 5};

// 加法仿函数
plus<int> add;
int result = add(3, 5);                    // 等价于 3 + 5

// 减法仿函数
minus<int> sub;
result = sub(10, 3);                       // 等价于 10 - 3

// 乘法仿函数
multiplies<int> mul;
result = mul(4, 6);                        // 等价于 4 * 6

// 除法仿函数
divides<int> div;
result = div(15, 3);                       // 等价于 15 / 3

// 取模仿函数
modulus<int> mod;
result = mod(17, 5);                       // 等价于 17 % 5

// 取反仿函数
negate<int> neg;
result = neg(10);                          // 等价于 -10

// 在算法中使用
vector<int> v2;
transform(v.begin(), v.end(), back_inserter(v2), negate<int>());
// v2 = {-1, -2, -3, -4, -5}
```

### 2. 关系仿函数
```cpp
#include <functional>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};

// 等于仿函数
equal_to<int> eq;
bool isEqual = eq(5, 5);                   // true

// 不等于仿函数
not_equal_to<int> neq;
bool isNotEqual = neq(5, 3);               // true

// 大于仿函数
greater<int> gt;
bool isGreater = gt(10, 5);                // true

// 大于等于仿函数
greater_equal<int> ge;
bool isGreaterEqual = ge(5, 5);            // true

// 小于仿函数
less<int> lt;
bool isLess = lt(3, 7);                    // true

// 小于等于仿函数
less_equal<int> le;
bool isLessEqual = le(5, 5);               // true

// 在算法中使用
sort(v.begin(), v.end(), greater<int>());  // 降序排序
// v = {9, 6, 5, 4, 3, 2, 1, 1}

// 查找第一个大于5的元素
auto it = find_if(v.begin(), v.end(), bind2nd(greater<int>(), 5));
```

### 3. 逻辑仿函数
```cpp
#include <functional>
#include <vector>
#include <algorithm>
using namespace std;

vector<bool> v = {true, false, true, false};

// 逻辑与仿函数
logical_and<bool> land;
bool result = land(true, false);            // false

// 逻辑或仿函数
logical_or<bool> lor;
result = lor(true, false);                  // true

// 逻辑非仿函数
logical_not<bool> lnot;
result = lnot(true);                        // false

// 在算法中使用
// 检查是否所有元素都为true
bool allTrue = all_of(v.begin(), v.end(), logical_not<bool>());
// 等价于 !all_of(v.begin(), v.end(), identity<bool>())
```

## 📚 自定义仿函数

### 1. 基本仿函数
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 一元仿函数：计算平方
class Square {
public:
    int operator()(int x) const {
        return x * x;
    }
};

// 二元仿函数：计算幂
class Power {
private:
    int exponent;
    
public:
    Power(int exp) : exponent(exp) {}
    
    int operator()(int base) const {
        int result = 1;
        for (int i = 0; i < exponent; ++i) {
            result *= base;
        }
        return result;
    }
};

// 状态仿函数：累加器
class Accumulator {
private:
    int sum;
    
public:
    Accumulator() : sum(0) {}
    
    int operator()(int x) {
        sum += x;
        return sum;
    }
    
    int getSum() const { return sum; }
    void reset() { sum = 0; }
};

// 使用示例
void testBasicFunctors() {
    vector<int> v = {1, 2, 3, 4, 5};
    
    // 使用Square仿函数
    Square square;
    cout << "5的平方: " << square(5) << endl;
    
    // 使用Power仿函数
    Power power3(3);
    cout << "2的3次方: " << power3(2) << endl;
    
    // 使用Accumulator仿函数
    Accumulator acc;
    for (int x : v) {
        cout << "累加" << x << "后: " << acc(x) << endl;
    }
    cout << "最终总和: " << acc.getSum() << endl;
}
```

### 2. 模板仿函数
```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// 通用比较仿函数
template<typename T>
class Less {
public:
    bool operator()(const T& a, const T& b) const {
        return a < b;
    }
};

// 通用转换仿函数
template<typename T, typename U>
class Converter {
public:
    U operator()(const T& value) const {
        return static_cast<U>(value);
    }
};

// 通用过滤器仿函数
template<typename T>
class Filter {
private:
    T threshold;
    
public:
    Filter(const T& thresh) : threshold(thresh) {}
    
    bool operator()(const T& value) const {
        return value > threshold;
    }
};

// 使用示例
void testTemplateFunctors() {
    vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 使用Less仿函数排序
    Less<int> less;
    sort(numbers.begin(), numbers.end(), less);
    
    // 使用Converter仿函数转换类型
    Converter<int, double> intToDouble;
    vector<double> doubles;
    transform(numbers.begin(), numbers.end(), back_inserter(doubles), intToDouble);
    
    // 使用Filter仿函数过滤
    Filter<int> filter(5);
    vector<int> filtered;
    copy_if(numbers.begin(), numbers.end(), back_inserter(filtered), filter);
    
    cout << "过滤后的元素: ";
    for (int x : filtered) {
        cout << x << " ";
    }
    cout << endl;
}
```

### 3. 函数对象适配器
```cpp
#include <functional>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 使用bind1st和bind2nd（C++11之前）
void testOldBinders() {
    vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 查找第一个大于5的元素
    auto it1 = find_if(v.begin(), v.end(), bind2nd(greater<int>(), 5));
    
    // 查找第一个小于3的元素
    auto it2 = find_if(v.begin(), v.end(), bind1st(greater<int>(), 3));
    
    // 将所有元素乘以2
    transform(v.begin(), v.end(), v.begin(), bind2nd(multiplies<int>(), 2));
}

// 使用bind（C++11及以后）
void testModernBind() {
    vector<int> v = {1, 2, 3, 4, 5};
    
    // 使用bind绑定参数
    auto greaterThan3 = bind(greater<int>(), placeholders::_1, 3);
    auto multiplyBy2 = bind(multiplies<int>(), placeholders::_1, 2);
    
    // 查找第一个大于3的元素
    auto it = find_if(v.begin(), v.end(), greaterThan3);
    
    // 将所有元素乘以2
    transform(v.begin(), v.end(), v.begin(), multiplyBy2);
}

// 使用mem_fn
void testMemFn() {
    vector<string> strings = {"hello", "world", "cpp", "stl"};
    
    // 调用string的size()方法
    auto sizeFunc = mem_fn(&string::size);
    vector<size_t> sizes;
    transform(strings.begin(), strings.end(), back_inserter(sizes), sizeFunc);
    
    // 调用string的empty()方法
    auto emptyFunc = mem_fn(&string::empty);
    auto it = find_if(strings.begin(), strings.end(), emptyFunc);
}
```

## 📚 高级仿函数技巧

### 1. 函数组合
```cpp
#include <functional>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 函数组合器
template<typename F, typename G>
class Compose {
private:
    F f;
    G g;
    
public:
    Compose(F f_, G g_) : f(f_), g(g_) {}
    
    template<typename T>
    auto operator()(const T& x) const -> decltype(f(g(x))) {
        return f(g(x));
    }
};

// 辅助函数创建组合仿函数
template<typename F, typename G>
Compose<F, G> compose(F f, G g) {
    return Compose<F, G>(f, g);
}

// 使用示例
void testFunctionComposition() {
    vector<int> v = {1, 2, 3, 4, 5};
    
    // 组合：先平方，再加1
    auto square = [](int x) { return x * x; };
    auto addOne = [](int x) { return x + 1; };
    auto squareThenAddOne = compose(addOne, square);
    
    vector<int> result;
    transform(v.begin(), v.end(), back_inserter(result), squareThenAddOne);
    
    // 结果：{2, 5, 10, 17, 26}
    for (int x : result) {
        cout << x << " ";
    }
    cout << endl;
}
```

### 2. 条件仿函数
```cpp
#include <functional>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 条件仿函数：根据条件选择不同的操作
template<typename T>
class ConditionalFunctor {
private:
    T threshold;
    
public:
    ConditionalFunctor(T thresh) : threshold(thresh) {}
    
    T operator()(T value) const {
        if (value < threshold) {
            return value * 2;               // 小于阈值时翻倍
        } else {
            return value / 2;               // 大于等于阈值时减半
        }
    }
};

// 使用示例
void testConditionalFunctor() {
    vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    ConditionalFunctor<int> conditional(5);
    vector<int> result;
    transform(v.begin(), v.end(), back_inserter(result), conditional);
    
    // 结果：{2, 4, 6, 8, 2, 3, 3, 4, 4, 5}
    for (int x : result) {
        cout << x << " ";
    }
    cout << endl;
}
```

### 3. 状态保持仿函数
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 状态保持仿函数：记住之前的值
template<typename T>
class StatefulFunctor {
private:
    T previous;
    bool first;
    
public:
    StatefulFunctor() : previous(T()), first(true) {}
    
    T operator()(T current) {
        if (first) {
            first = false;
            previous = current;
            return current;
        }
        
        T result = current + previous;      // 当前值加上前一个值
        previous = current;
        return result;
    }
    
    void reset() {
        first = true;
        previous = T();
    }
};

// 使用示例
void testStatefulFunctor() {
    vector<int> v = {1, 2, 3, 4, 5};
    
    StatefulFunctor<int> stateful;
    vector<int> result;
    transform(v.begin(), v.end(), back_inserter(result), stateful);
    
    // 结果：{1, 3, 5, 7, 9} (每个元素加上前一个元素)
    for (int x : result) {
        cout << x << " ";
    }
    cout << endl;
}
```

## 💡 仿函数使用技巧

### 1. 选择合适的仿函数
```cpp
// 简单操作：使用标准仿函数
vector<int> v = {1, 2, 3, 4, 5};
sort(v.begin(), v.end(), greater<int>());

// 复杂操作：使用自定义仿函数
class ComplexComparator {
private:
    int factor;
    
public:
    ComplexComparator(int f) : factor(f) {}
    
    bool operator()(int a, int b) const {
        return (a % factor) < (b % factor);
    }
};

sort(v.begin(), v.end(), ComplexComparator(3));
```

### 2. 性能优化
```cpp
// 避免创建临时仿函数对象
class ExpensiveFunctor {
public:
    ExpensiveFunctor() {
        // 昂贵的初始化操作
    }
    
    int operator()(int x) const {
        // 复杂的计算
        return x * x + x + 1;
    }
};

// 重用仿函数对象
ExpensiveFunctor expensive;
vector<int> result;
transform(v.begin(), v.end(), back_inserter(result), expensive);
```

### 3. 调试和测试
```cpp
// 调试仿函数：记录调用信息
class DebugFunctor {
private:
    int callCount;
    
public:
    DebugFunctor() : callCount(0) {}
    
    int operator()(int x) {
        ++callCount;
        cout << "调用次数: " << callCount << ", 参数: " << x << endl;
        return x * 2;
    }
    
    int getCallCount() const { return callCount; }
};
```

## 🎯 仿函数应用场景

### 1. 算法定制
```cpp
// 自定义排序规则
class Person {
public:
    string name;
    int age;
    double salary;
    
    Person(string n, int a, double s) : name(n), age(a), salary(s) {}
};

class PersonComparator {
public:
    bool operator()(const Person& a, const Person& b) const {
        if (a.age != b.age) return a.age < b.age;
        if (a.salary != b.salary) return a.salary > b.salary;
        return a.name < b.name;
    }
};

vector<Person> people = {
    {"Alice", 25, 50000},
    {"Bob", 30, 60000},
    {"Charlie", 25, 55000}
};

sort(people.begin(), people.end(), PersonComparator());
```

### 2. 数据转换
```cpp
// 类型转换仿函数
class StringToInt {
public:
    int operator()(const string& s) const {
        return stoi(s);
    }
};

vector<string> strings = {"123", "456", "789"};
vector<int> numbers;
transform(strings.begin(), strings.end(), back_inserter(numbers), StringToInt());
```

### 3. 条件过滤
```cpp
// 条件过滤仿函数
class RangeFilter {
private:
    int min, max;
    
public:
    RangeFilter(int min_, int max_) : min(min_), max(max_) {}
    
    bool operator()(int x) const {
        return x >= min && x <= max;
    }
};

vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
vector<int> filtered;
copy_if(data.begin(), data.end(), back_inserter(filtered), RangeFilter(3, 7));
```

## ⚡ 性能考虑

### 1. 内联优化
- **简单仿函数**: 编译器容易内联
- **复杂仿函数**: 可能影响内联优化
- **模板仿函数**: 通常有更好的内联机会

### 2. 内存访问
- **状态仿函数**: 可能影响缓存性能
- **无状态仿函数**: 通常性能更好
- **大对象仿函数**: 注意内存布局

### 3. 函数调用开销
- **仿函数**: 通常比函数指针快
- **lambda表达式**: 现代编译器优化良好
- **std::function**: 有一定运行时开销

## 🚀 下一步
- 学习 [算法详解](./algorithms-guide.md)
- 了解 [容器详解](./containers-guide.md)
- 掌握 [迭代器详解](./iterators-guide.md)

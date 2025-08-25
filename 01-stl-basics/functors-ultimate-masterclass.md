# C++ STL 仿函数终极大师级教程

## 🎯 仿函数的核心哲学

仿函数（Functors）是STL中**最优雅的抽象**之一，它统一了**函数、函数对象、Lambda表达式**的概念，实现了**可调用对象的多态性**，是现代C++**函数式编程**的基石。

```
仿函数生态系统
├── 函数指针 (Function Pointers)
│   └── 传统C风格函数抽象
│
├── 函数对象 (Function Objects)
│   ├── 算术仿函数 (Arithmetic Functors)
│   │   ├── plus<T>、minus<T>、multiplies<T>
│   │   └── divides<T>、modulus<T>、negate<T>
│   ├── 比较仿函数 (Relational Functors) 
│   │   ├── equal_to<T>、not_equal_to<T>
│   │   ├── greater<T>、less<T>
│   │   └── greater_equal<T>、less_equal<T>
│   ├── 逻辑仿函数 (Logical Functors)
│   │   ├── logical_and<T>、logical_or<T>
│   │   └── logical_not<T>
│   └── 自定义仿函数 (Custom Functors)
│       ├── 单元仿函数 (Unary Functors)
│       ├── 二元仿函数 (Binary Functors)
│       └── 多态仿函数 (Polymorphic Functors)
│
├── Lambda表达式 (Lambda Expressions) - C++11
│   ├── 无捕获Lambda
│   ├── 值捕获Lambda
│   ├── 引用捕获Lambda
│   ├── 混合捕获Lambda
│   └── 泛型Lambda - C++14
│
├── 函数适配器 (Function Adapters)
│   ├── bind<F, Args...> - 参数绑定
│   ├── mem_fn<MemberFunc> - 成员函数适配
│   ├── not1、not2 - 逻辑否定
│   └── function<Signature> - 类型擦除
│
└── 现代函数工具 (Modern Function Tools) - C++17/20
    ├── invoke<F, Args...> - 统一调用
    ├── apply<F, Tuple> - 元组展开调用
    └── 概念约束 (Concepts) - C++20

设计原则:
✓ 统一调用接口 - operator()
✓ 类型安全 - 编译时检查
✓ 零开销抽象 - 内联优化
✓ 组合性 - 可以组合和适配
```

---

## 🏗️ 标准仿函数深度解析

### 🔢 算术仿函数的内部实现

```cpp
#include <functional>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>

class ArithmeticFunctorsMasterclass {
public:
    // 1. 标准算术仿函数的实现原理
    static void standardArithmeticFunctors() {
        std::cout << "=== 标准算术仿函数实现原理 ===" << std::endl;
        
        // 展示标准算术仿函数的简化实现
        std::cout << "1. 算术仿函数的内部结构:" << std::endl;
        
        // 自定义实现来理解原理
        template<typename T>
        struct MyPlus {
            constexpr T operator()(const T& lhs, const T& rhs) const {
                return lhs + rhs;
            }
            
            using first_argument_type = T;
            using second_argument_type = T;
            using result_type = T;
        };
        
        template<typename T>
        struct MyMinus {
            constexpr T operator()(const T& lhs, const T& rhs) const {
                return lhs - rhs;
            }
            
            using first_argument_type = T;
            using second_argument_type = T;
            using result_type = T;
        };
        
        template<typename T>
        struct MyMultiplies {
            constexpr T operator()(const T& lhs, const T& rhs) const {
                return lhs * rhs;
            }
        };
        
        template<typename T>
        struct MyNegate {
            constexpr T operator()(const T& arg) const {
                return -arg;
            }
            
            using argument_type = T;
            using result_type = T;
        };
        
        // 测试自定义仿函数
        MyPlus<int> my_plus;
        MyMinus<double> my_minus;
        MyMultiplies<float> my_multiplies;
        MyNegate<int> my_negate;
        
        std::cout << "my_plus(10, 5) = " << my_plus(10, 5) << std::endl;
        std::cout << "my_minus(10.5, 3.2) = " << my_minus(10.5, 3.2) << std::endl;
        std::cout << "my_multiplies(3.0f, 4.0f) = " << my_multiplies(3.0f, 4.0f) << std::endl;
        std::cout << "my_negate(42) = " << my_negate(42) << std::endl;
        
        // 与标准仿函数对比
        std::cout << "\n2. 标准仿函数使用:" << std::endl;
        
        std::plus<int> std_plus;
        std::minus<double> std_minus;
        std::multiplies<float> std_multiplies;
        std::negate<int> std_negate;
        
        std::cout << "std::plus<int>()(10, 5) = " << std_plus(10, 5) << std::endl;
        std::cout << "std::minus<double>()(10.5, 3.2) = " << std_minus(10.5, 3.2) << std::endl;
        std::cout << "std::multiplies<float>()(3.0f, 4.0f) = " << std_multiplies(3.0f, 4.0f) << std::endl;
        std::cout << "std::negate<int>()(42) = " << std_negate(42) << std::endl;
    }
    
    // 2. 算术仿函数的实际应用
    static void arithmeticFunctorApplications() {
        std::cout << "\n=== 算术仿函数实际应用 ===" << std::endl;
        
        std::vector<int> vec1 = {1, 2, 3, 4, 5};
        std::vector<int> vec2 = {10, 20, 30, 40, 50};
        std::vector<int> result(5);
        
        std::cout << "1. 向量运算:" << std::endl;
        
        // 向量加法
        std::transform(vec1.begin(), vec1.end(), vec2.begin(), result.begin(), std::plus<int>());
        std::cout << "vec1 + vec2: ";
        for(int x : result) std::cout << x << " ";
        std::cout << std::endl;
        
        // 向量减法  
        std::transform(vec1.begin(), vec1.end(), vec2.begin(), result.begin(), std::minus<int>());
        std::cout << "vec1 - vec2: ";
        for(int x : result) std::cout << x << " ";
        std::cout << std::endl;
        
        // 向量乘法
        std::transform(vec1.begin(), vec1.end(), vec2.begin(), result.begin(), std::multiplies<int>());
        std::cout << "vec1 * vec2: ";
        for(int x : result) std::cout << x << " ";
        std::cout << std::endl;
        
        // 向量取负
        std::transform(vec1.begin(), vec1.end(), result.begin(), std::negate<int>());
        std::cout << "-vec1: ";
        for(int x : result) std::cout << x << " ";
        std::cout << std::endl;
        
        std::cout << "\n2. 累积运算:" << std::endl;
        
        // 求和
        int sum = std::accumulate(vec1.begin(), vec1.end(), 0, std::plus<int>());
        std::cout << "sum(vec1) = " << sum << std::endl;
        
        // 求积
        int product = std::accumulate(vec1.begin(), vec1.end(), 1, std::multiplies<int>());
        std::cout << "product(vec1) = " << product << std::endl;
        
        // 求最大值
        int max_val = std::accumulate(vec1.begin(), vec1.end(), vec1[0], 
                                     [](int a, int b) { return std::max(a, b); });
        std::cout << "max(vec1) = " << max_val << std::endl;
        
        std::cout << "\n3. 函数式编程风格:" << std::endl;
        
        // 使用算术仿函数进行函数式变换
        auto square = [](int x) { return std::multiplies<int>()(x, x); };
        auto add_ten = [](int x) { return std::plus<int>()(x, 10); };
        
        std::vector<int> squares(5);
        std::transform(vec1.begin(), vec1.end(), squares.begin(), square);
        std::cout << "squares: ";
        for(int x : squares) std::cout << x << " ";
        std::cout << std::endl;
        
        std::vector<int> add_ten_result(5);
        std::transform(vec1.begin(), vec1.end(), add_ten_result.begin(), add_ten);
        std::cout << "add 10: ";
        for(int x : add_ten_result) std::cout << x << " ";
        std::cout << std::endl;
    }
    
    // 3. 高精度计算中的仿函数应用
    static void highPrecisionCalculations() {
        std::cout << "\n=== 高精度计算中的仿函数应用 ===" << std::endl;
        
        // 大数运算仿函数
        class BigInteger {
        private:
            std::vector<int> digits;  // 逆序存储数字
            bool negative;
            
        public:
            BigInteger(long long n = 0) : negative(n < 0) {
                n = std::abs(n);
                if(n == 0) {
                    digits.push_back(0);
                } else {
                    while(n > 0) {
                        digits.push_back(n % 10);
                        n /= 10;
                    }
                }
            }
            
            BigInteger operator+(const BigInteger& other) const {
                if(negative != other.negative) {
                    // 符号不同，转换为减法
                    BigInteger temp = other;
                    temp.negative = !temp.negative;
                    return *this - temp;
                }
                
                BigInteger result;
                result.negative = negative;
                result.digits.clear();
                
                int carry = 0;
                size_t max_size = std::max(digits.size(), other.digits.size());
                
                for(size_t i = 0; i < max_size || carry; ++i) {
                    int sum = carry;
                    if(i < digits.size()) sum += digits[i];
                    if(i < other.digits.size()) sum += other.digits[i];
                    
                    result.digits.push_back(sum % 10);
                    carry = sum / 10;
                }
                
                return result;
            }
            
            BigInteger operator*(const BigInteger& other) const {
                BigInteger result(0);
                result.negative = (negative != other.negative);
                result.digits.assign(digits.size() + other.digits.size(), 0);
                
                for(size_t i = 0; i < digits.size(); ++i) {
                    for(size_t j = 0; j < other.digits.size(); ++j) {
                        result.digits[i + j] += digits[i] * other.digits[j];
                    }
                }
                
                // 处理进位
                for(size_t i = 0; i < result.digits.size() - 1; ++i) {
                    result.digits[i + 1] += result.digits[i] / 10;
                    result.digits[i] %= 10;
                }
                
                // 移除前导零
                while(result.digits.size() > 1 && result.digits.back() == 0) {
                    result.digits.pop_back();
                }
                
                return result;
            }
            
            std::string toString() const {
                std::string result;
                if(negative && !(digits.size() == 1 && digits[0] == 0)) {
                    result += "-";
                }
                
                for(auto it = digits.rbegin(); it != digits.rend(); ++it) {
                    result += char('0' + *it);
                }
                
                return result;
            }
        };
        
        // 大数仿函数
        struct BigIntegerPlus {
            BigInteger operator()(const BigInteger& lhs, const BigInteger& rhs) const {
                return lhs + rhs;
            }
        };
        
        struct BigIntegerMultiplies {
            BigInteger operator()(const BigInteger& lhs, const BigInteger& rhs) const {
                return lhs * rhs;
            }
        };
        
        // 测试大数运算
        std::cout << "大数运算测试:" << std::endl;
        
        std::vector<BigInteger> big_numbers = {
            BigInteger(123456789),
            BigInteger(987654321),
            BigInteger(999999999),
            BigInteger(111111111)
        };
        
        std::cout << "大数列表: ";
        for(const auto& num : big_numbers) {
            std::cout << num.toString() << " ";
        }
        std::cout << std::endl;
        
        // 大数求和
        BigInteger sum = std::accumulate(big_numbers.begin(), big_numbers.end(), 
                                        BigInteger(0), BigIntegerPlus());
        std::cout << "大数求和: " << sum.toString() << std::endl;
        
        // 大数求积
        BigInteger product = std::accumulate(big_numbers.begin(), big_numbers.end(),
                                           BigInteger(1), BigIntegerMultiplies());
        std::cout << "大数求积: " << product.toString() << std::endl;
        
        // 斐波那契数列（大数版）
        std::cout << "\n斐波那契数列（前20项）:" << std::endl;
        std::vector<BigInteger> fibonacci = {BigInteger(0), BigInteger(1)};
        
        for(int i = 2; i < 20; ++i) {
            BigInteger next = BigIntegerPlus()(fibonacci[i-1], fibonacci[i-2]);
            fibonacci.push_back(next);
        }
        
        for(size_t i = 0; i < fibonacci.size(); ++i) {
            std::cout << "F(" << i << ") = " << fibonacci[i].toString() << std::endl;
        }
    }
};
```

### 📊 比较仿函数的威力

```cpp
class ComparativeFunctorsMasterclass {
public:
    // 1. 标准比较仿函数详解
    static void standardComparativeFunctors() {
        std::cout << "\n=== 标准比较仿函数详解 ===" << std::endl;
        
        std::vector<int> data = {5, 2, 8, 1, 9, 3, 7, 4, 6};
        
        std::cout << "原始数据: ";
        for(int x : data) std::cout << x << " ";
        std::cout << std::endl;
        
        // 1. 使用不同比较仿函数排序
        std::cout << "\n1. 不同排序方式:" << std::endl;
        
        // 升序排序
        auto ascending = data;
        std::sort(ascending.begin(), ascending.end(), std::less<int>());
        std::cout << "升序: ";
        for(int x : ascending) std::cout << x << " ";
        std::cout << std::endl;
        
        // 降序排序
        auto descending = data;
        std::sort(descending.begin(), descending.end(), std::greater<int>());
        std::cout << "降序: ";
        for(int x : descending) std::cout << x << " ";
        std::cout << std::endl;
        
        // 2. 查找操作
        std::cout << "\n2. 查找操作:" << std::endl;
        
        // 在有序数据中二分查找
        bool found_5 = std::binary_search(ascending.begin(), ascending.end(), 5, std::less<int>());
        std::cout << "二分查找5: " << (found_5 ? "找到" : "未找到") << std::endl;
        
        // 查找第一个大于等于5的元素
        auto it = std::lower_bound(ascending.begin(), ascending.end(), 5, std::less<int>());
        if(it != ascending.end()) {
            std::cout << "第一个 >= 5的元素: " << *it << std::endl;
        }
        
        // 3. 等式检测
        std::cout << "\n3. 等式检测:" << std::endl;
        
        std::vector<int> vec1 = {1, 2, 3, 4, 5};
        std::vector<int> vec2 = {1, 2, 3, 4, 5};
        std::vector<int> vec3 = {1, 2, 3, 4, 6};
        
        bool equal_12 = std::equal(vec1.begin(), vec1.end(), vec2.begin(), std::equal_to<int>());
        bool equal_13 = std::equal(vec1.begin(), vec1.end(), vec3.begin(), std::equal_to<int>());
        
        std::cout << "vec1 == vec2: " << (equal_12 ? "是" : "否") << std::endl;
        std::cout << "vec1 == vec3: " << (equal_13 ? "是" : "否") << std::endl;
        
        // 计数操作
        int count_greater_5 = std::count_if(data.begin(), data.end(), 
                                           [](int x) { return std::greater<int>()(x, 5); });
        std::cout << "大于5的元素个数: " << count_greater_5 << std::endl;
    }
    
    // 2. 自定义比较仿函数
    static void customComparativeFunctors() {
        std::cout << "\n=== 自定义比较仿函数 ===" << std::endl;
        
        // 复杂对象的比较
        struct Student {
            std::string name;
            int age;
            double grade;
            
            Student(const std::string& n, int a, double g) : name(n), age(a), grade(g) {}
        };
        
        std::vector<Student> students = {
            {"Alice", 20, 85.5},
            {"Bob", 19, 92.0},
            {"Charlie", 21, 78.5},
            {"David", 20, 88.0},
            {"Eve", 19, 95.5}
        };
        
        std::cout << "1. 多重排序条件:" << std::endl;
        
        // 按成绩排序（降序）
        struct GradeComparator {
            bool operator()(const Student& a, const Student& b) const {
                return a.grade > b.grade;  // 降序
            }
        };
        
        auto by_grade = students;
        std::sort(by_grade.begin(), by_grade.end(), GradeComparator());
        
        std::cout << "按成绩排序:" << std::endl;
        for(const auto& s : by_grade) {
            std::cout << "  " << s.name << ": " << s.grade << std::endl;
        }
        
        // 先按年龄，再按成绩排序
        struct AgeGradeComparator {
            bool operator()(const Student& a, const Student& b) const {
                if(a.age != b.age) {
                    return a.age < b.age;  // 年龄升序
                }
                return a.grade > b.grade;  // 成绩降序
            }
        };
        
        auto by_age_grade = students;
        std::sort(by_age_grade.begin(), by_age_grade.end(), AgeGradeComparator());
        
        std::cout << "\n按年龄后成绩排序:" << std::endl;
        for(const auto& s : by_age_grade) {
            std::cout << "  " << s.name << " (年龄:" << s.age << ", 成绩:" << s.grade << ")" << std::endl;
        }
        
        // 2. 函数式比较器
        std::cout << "\n2. 函数式比较器:" << std::endl;
        
        // Lambda比较器
        auto name_length_compare = [](const Student& a, const Student& b) {
            return a.name.length() < b.name.length();
        };
        
        auto by_name_length = students;
        std::sort(by_name_length.begin(), by_name_length.end(), name_length_compare);
        
        std::cout << "按姓名长度排序:" << std::endl;
        for(const auto& s : by_name_length) {
            std::cout << "  " << s.name << " (长度:" << s.name.length() << ")" << std::endl;
        }
        
        // 3. 模板化比较器
        std::cout << "\n3. 模板化比较器:" << std::endl;
        
        template<typename T, typename Extractor>
        class ExtractorComparator {
        private:
            Extractor extractor;
            bool ascending;
            
        public:
            ExtractorComparator(Extractor ext, bool asc = true) 
                : extractor(ext), ascending(asc) {}
            
            bool operator()(const T& a, const T& b) const {
                auto val_a = extractor(a);
                auto val_b = extractor(b);
                return ascending ? (val_a < val_b) : (val_a > val_b);
            }
        };
        
        // 使用模板化比较器
        auto age_extractor = [](const Student& s) { return s.age; };
        auto grade_extractor = [](const Student& s) { return s.grade; };
        
        auto by_age_desc = students;
        std::sort(by_age_desc.begin(), by_age_desc.end(), 
                 ExtractorComparator<Student, decltype(age_extractor)>(age_extractor, false));
        
        std::cout << "按年龄降序:" << std::endl;
        for(const auto& s : by_age_desc) {
            std::cout << "  " << s.name << " (" << s.age << "岁)" << std::endl;
        }
    }
    
    // 3. 高级比较技巧
    static void advancedComparisonTechniques() {
        std::cout << "\n=== 高级比较技巧 ===" << std::endl;
        
        // 1. 稳定排序 vs 不稳定排序
        std::cout << "1. 稳定排序演示:" << std::endl;
        
        struct Item {
            int value;
            char label;
            
            Item(int v, char l) : value(v), label(l) {}
        };
        
        std::vector<Item> items = {
            {3, 'A'}, {1, 'B'}, {3, 'C'}, {2, 'D'}, {1, 'E'}, {3, 'F'}
        };
        
        std::cout << "原始顺序: ";
        for(const auto& item : items) {
            std::cout << item.value << item.label << " ";
        }
        std::cout << std::endl;
        
        // 不稳定排序
        auto unstable_items = items;
        std::sort(unstable_items.begin(), unstable_items.end(),
                 [](const Item& a, const Item& b) { return a.value < b.value; });
        
        std::cout << "不稳定排序: ";
        for(const auto& item : unstable_items) {
            std::cout << item.value << item.label << " ";
        }
        std::cout << std::endl;
        
        // 稳定排序
        auto stable_items = items;
        std::stable_sort(stable_items.begin(), stable_items.end(),
                        [](const Item& a, const Item& b) { return a.value < b.value; });
        
        std::cout << "稳定排序: ";
        for(const auto& item : stable_items) {
            std::cout << item.value << item.label << " ";
        }
        std::cout << std::endl;
        
        // 2. 部分排序
        std::cout << "\n2. 部分排序技巧:" << std::endl;
        
        std::vector<int> large_data;
        for(int i = 1; i <= 100; ++i) {
            large_data.push_back(i);
        }
        
        // 随机打乱
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(large_data.begin(), large_data.end(), gen);
        
        std::cout << "随机数据前10个: ";
        for(int i = 0; i < 10; ++i) {
            std::cout << large_data[i] << " ";
        }
        std::cout << std::endl;
        
        // 找出最小的10个元素
        std::partial_sort(large_data.begin(), large_data.begin() + 10, large_data.end());
        
        std::cout << "最小的10个: ";
        for(int i = 0; i < 10; ++i) {
            std::cout << large_data[i] << " ";
        }
        std::cout << std::endl;
        
        // 3. nth_element - 找第n大的元素
        std::cout << "\n3. nth_element应用:" << std::endl;
        
        std::vector<int> test_data = {64, 34, 25, 12, 22, 11, 90, 88, 76, 50, 42};
        
        std::cout << "原始数据: ";
        for(int x : test_data) std::cout << x << " ";
        std::cout << std::endl;
        
        // 找中位数
        size_t median_pos = test_data.size() / 2;
        std::nth_element(test_data.begin(), test_data.begin() + median_pos, test_data.end());
        
        std::cout << "中位数: " << test_data[median_pos] << std::endl;
        std::cout << "nth_element后: ";
        for(int x : test_data) std::cout << x << " ";
        std::cout << std::endl;
        std::cout << "注意：中位数左边都<=中位数，右边都>=中位数" << std::endl;
    }
};
```

---

## 🚀 Lambda表达式与现代仿函数

### ⚡ Lambda表达式深度剖析

```cpp
class LambdaExpressionsMasterclass {
public:
    // 1. Lambda表达式的完整语法
    static void lambdaSyntaxComplete() {
        std::cout << "\n=== Lambda表达式完整语法 ===" << std::endl;
        
        std::cout << "1. Lambda基本语法:" << std::endl;
        std::cout << "[capture](parameters) -> return_type { body }" << std::endl;
        
        // 基本Lambda
        auto basic_lambda = []() { return 42; };
        std::cout << "basic_lambda() = " << basic_lambda() << std::endl;
        
        // 带参数的Lambda
        auto add_lambda = [](int a, int b) { return a + b; };
        std::cout << "add_lambda(3, 4) = " << add_lambda(3, 4) << std::endl;
        
        // 明确返回类型的Lambda
        auto divide_lambda = [](double a, double b) -> double {
            if(b == 0.0) return 0.0;
            return a / b;
        };
        std::cout << "divide_lambda(10.0, 3.0) = " << divide_lambda(10.0, 3.0) << std::endl;
        
        std::cout << "\n2. 捕获方式详解:" << std::endl;
        
        int x = 10, y = 20;
        std::string message = "Hello";
        
        // 值捕获
        auto value_capture = [x, y]() {
            return x + y;  // x和y是副本
        };
        std::cout << "值捕获: " << value_capture() << std::endl;
        
        // 引用捕获
        auto ref_capture = [&x, &y]() {
            x += 5;  // 修改原变量
            return x + y;
        };
        std::cout << "引用捕获前 x = " << x << std::endl;
        std::cout << "引用捕获结果: " << ref_capture() << std::endl;
        std::cout << "引用捕获后 x = " << x << std::endl;
        
        // 混合捕获
        auto mixed_capture = [x, &y, message](const std::string& suffix) {
            y += 10;  // 引用捕获，修改原变量
            return message + " " + suffix + ", x=" + std::to_string(x) + ", y=" + std::to_string(y);
        };
        std::cout << "混合捕获: " << mixed_capture("World") << std::endl;
        std::cout << "混合捕获后 y = " << y << std::endl;
        
        // 默认捕获
        auto default_value_capture = [=](int z) { return x + y + z; };  // 所有变量值捕获
        auto default_ref_capture = [&](int z) { return x + y + z; };    // 所有变量引用捕获
        
        std::cout << "默认值捕获: " << default_value_capture(5) << std::endl;
        std::cout << "默认引用捕获: " << default_ref_capture(5) << std::endl;
    }
    
    // 2. Lambda的高级特性
    static void lambdaAdvancedFeatures() {
        std::cout << "\n=== Lambda高级特性 ===" << std::endl;
        
        std::cout << "1. 可变Lambda (mutable):" << std::endl;
        
        int counter = 0;
        
        // 不可变Lambda（默认）
        auto immutable_lambda = [counter]() {
            // counter++;  // 错误：无法修改值捕获的变量
            return counter;
        };
        
        // 可变Lambda
        auto mutable_lambda = [counter](int increment) mutable {
            counter += increment;  // 可以修改值捕获的变量副本
            return counter;
        };
        
        std::cout << "原始counter: " << counter << std::endl;
        std::cout << "mutable_lambda(5): " << mutable_lambda(5) << std::endl;
        std::cout << "mutable_lambda(3): " << mutable_lambda(3) << std::endl;
        std::cout << "原始counter仍为: " << counter << std::endl;
        
        std::cout << "\n2. 泛型Lambda (C++14):" << std::endl;
        
        // 泛型Lambda使用auto参数
        auto generic_lambda = [](auto a, auto b) {
            return a + b;
        };
        
        std::cout << "generic_lambda(3, 4) = " << generic_lambda(3, 4) << std::endl;
        std::cout << "generic_lambda(3.5, 2.1) = " << generic_lambda(3.5, 2.1) << std::endl;
        std::cout << "generic_lambda(string, string) = " << 
                     generic_lambda(std::string("Hello"), std::string(" World")) << std::endl;
        
        // 更复杂的泛型Lambda
        auto print_container = [](const auto& container) {
            std::cout << "容器内容: ";
            for(const auto& element : container) {
                std::cout << element << " ";
            }
            std::cout << std::endl;
        };
        
        std::vector<int> vec = {1, 2, 3, 4, 5};
        std::list<std::string> lst = {"one", "two", "three"};
        
        print_container(vec);
        print_container(lst);
        
        std::cout << "\n3. 初始化捕获 (C++14):" << std::endl;
        
        // 移动捕获
        std::unique_ptr<int> ptr = std::make_unique<int>(42);
        
        auto move_capture = [p = std::move(ptr)]() {
            return *p;
        };
        
        std::cout << "移动捕获结果: " << move_capture() << std::endl;
        std::cout << "原指针是否有效: " << (ptr ? "是" : "否") << std::endl;
        
        // 计算捕获
        auto computed_capture = [factorial = [](int n) {
            int result = 1;
            for(int i = 2; i <= n; ++i) {
                result *= i;
            }
            return result;
        }(5)](int x) {
            return factorial + x;
        };
        
        std::cout << "计算捕获 (5! + 10): " << computed_capture(10) << std::endl;
    }
    
    // 3. Lambda的实际应用场景
    static void lambdaRealWorldApplications() {
        std::cout << "\n=== Lambda实际应用场景 ===" << std::endl;
        
        std::cout << "1. STL算法中的Lambda应用:" << std::endl;
        
        std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        
        // 过滤操作
        std::vector<int> evens;
        std::copy_if(numbers.begin(), numbers.end(), std::back_inserter(evens),
                    [](int n) { return n % 2 == 0; });
        
        std::cout << "偶数: ";
        for(int n : evens) std::cout << n << " ";
        std::cout << std::endl;
        
        // 变换操作
        std::vector<int> squares;
        std::transform(numbers.begin(), numbers.end(), std::back_inserter(squares),
                      [](int n) { return n * n; });
        
        std::cout << "平方: ";
        for(int n : squares) std::cout << n << " ";
        std::cout << std::endl;
        
        // 聚合操作
        int sum = std::accumulate(numbers.begin(), numbers.end(), 0,
                                 [](int acc, int n) { return acc + n; });
        std::cout << "求和: " << sum << std::endl;
        
        std::cout << "\n2. 事件处理和回调:" << std::endl;
        
        // 简单的事件系统
        class EventManager {
        private:
            std::vector<std::function<void(int)>> event_handlers;
            
        public:
            void subscribe(std::function<void(int)> handler) {
                event_handlers.push_back(handler);
            }
            
            void trigger_event(int value) {
                std::cout << "触发事件，值为: " << value << std::endl;
                for(auto& handler : event_handlers) {
                    handler(value);
                }
            }
        };
        
        EventManager event_manager;
        
        // 使用Lambda注册事件处理器
        event_manager.subscribe([](int value) {
            std::cout << "  处理器1: 收到值 " << value << std::endl;
        });
        
        event_manager.subscribe([](int value) {
            std::cout << "  处理器2: 值的平方是 " << (value * value) << std::endl;
        });
        
        std::string prefix = "Handler3";
        event_manager.subscribe([prefix](int value) {
            std::cout << "  " << prefix << ": 值为 " << value << 
                         (value % 2 == 0 ? " (偶数)" : " (奇数)") << std::endl;
        });
        
        event_manager.trigger_event(7);
        
        std::cout << "\n3. 函数式编程风格:" << std::endl;
        
        // 高阶函数：接受函数作为参数的函数
        auto apply_twice = [](auto func, auto value) {
            return func(func(value));
        };
        
        auto add_one = [](int x) { return x + 1; };
        auto multiply_by_three = [](int x) { return x * 3; };
        
        std::cout << "apply_twice(add_one, 5) = " << apply_twice(add_one, 5) << std::endl;
        std::cout << "apply_twice(multiply_by_three, 2) = " << apply_twice(multiply_by_three, 2) << std::endl;
        
        // 函数组合
        auto compose = [](auto f, auto g) {
            return [f, g](auto x) { return f(g(x)); };
        };
        
        auto add_one_then_multiply = compose(multiply_by_three, add_one);
        std::cout << "compose(multiply_by_three, add_one)(4) = " << add_one_then_multiply(4) << std::endl;
        
        // 柯里化
        auto curry_add = [](int a) {
            return [a](int b) {
                return a + b;
            };
        };
        
        auto add_10 = curry_add(10);
        std::cout << "curry_add(10)(5) = " << add_10(5) << std::endl;
    }
    
    // 4. Lambda性能考虑
    static void lambdaPerformanceConsiderations() {
        std::cout << "\n=== Lambda性能考虑 ===" << std::endl;
        
        const size_t N = 1000000;
        std::vector<int> data(N);
        std::iota(data.begin(), data.end(), 1);
        
        // 测试1: Lambda vs 函数指针 vs 仿函数
        std::cout << "性能测试 (对" << N << "个元素求和):" << std::endl;
        
        // 函数指针版本
        int (*func_ptr)(int, int) = [](int a, int b) { return a + b; };
        
        auto start = std::chrono::high_resolution_clock::now();
        int sum1 = std::accumulate(data.begin(), data.end(), 0, func_ptr);
        auto end = std::chrono::high_resolution_clock::now();
        auto func_ptr_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // Lambda版本
        start = std::chrono::high_resolution_clock::now();
        int sum2 = std::accumulate(data.begin(), data.end(), 0, 
                                  [](int a, int b) { return a + b; });
        end = std::chrono::high_resolution_clock::now();
        auto lambda_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // 仿函数版本
        start = std::chrono::high_resolution_clock::now();
        int sum3 = std::accumulate(data.begin(), data.end(), 0, std::plus<int>());
        end = std::chrono::high_resolution_clock::now();
        auto functor_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "函数指针: " << func_ptr_time.count() << "μs (sum=" << sum1 << ")" << std::endl;
        std::cout << "Lambda: " << lambda_time.count() << "μs (sum=" << sum2 << ")" << std::endl;
        std::cout << "仿函数: " << functor_time.count() << "μs (sum=" << sum3 << ")" << std::endl;
        
        // 测试2: 捕获开销
        std::cout << "\n捕获开销测试:" << std::endl;
        
        int external_value = 100;
        
        // 无捕获Lambda
        start = std::chrono::high_resolution_clock::now();
        int sum4 = std::accumulate(data.begin(), data.end(), 0,
                                  [](int a, int b) { return a + b; });
        end = std::chrono::high_resolution_clock::now();
        auto no_capture_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // 值捕获Lambda
        start = std::chrono::high_resolution_clock::now();
        int sum5 = std::accumulate(data.begin(), data.end(), 0,
                                  [external_value](int a, int b) { 
                                      return a + b + (external_value * 0); // 使用捕获的值
                                  });
        end = std::chrono::high_resolution_clock::now();
        auto value_capture_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // 引用捕获Lambda
        start = std::chrono::high_resolution_clock::now();
        int sum6 = std::accumulate(data.begin(), data.end(), 0,
                                  [&external_value](int a, int b) { 
                                      return a + b + (external_value * 0); // 使用捕获的引用
                                  });
        end = std::chrono::high_resolution_clock::now();
        auto ref_capture_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "无捕获: " << no_capture_time.count() << "μs" << std::endl;
        std::cout << "值捕获: " << value_capture_time.count() << "μs" << std::endl;
        std::cout << "引用捕获: " << ref_capture_time.count() << "μs" << std::endl;
        
        std::cout << "\n结论:" << std::endl;
        std::cout << "- 无捕获Lambda通常能被优化为函数指针" << std::endl;
        std::cout << "- 值捕获可能增加对象大小，但访问速度快" << std::endl;
        std::cout << "- 引用捕获节省空间，但可能增加间接访问开销" << std::endl;
        std::cout << "- 现代编译器通常能很好地优化Lambda表达式" << std::endl;
    }
};
```

现在让我更新TODO并创建最终的总结报告：

<function_calls>
<invoke name="todo_write">
<parameter name="merge">true

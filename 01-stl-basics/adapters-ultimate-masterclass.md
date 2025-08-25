# C++ STL 适配器终极大师级教程

## 🎯 适配器设计哲学

适配器是STL中**设计模式的典型体现**，它们不提供新功能，而是**改变现有接口**以适应特定需求。这体现了**适配器模式**和**组合优于继承**的设计理念。

```
STL适配器生态系统
├── 容器适配器 (Container Adapters)
│   ├── stack<T, Container>     ← LIFO接口适配
│   ├── queue<T, Container>     ← FIFO接口适配
│   └── priority_queue<T, Container, Compare> ← 优先级接口适配
│
├── 迭代器适配器 (Iterator Adapters)
│   ├── reverse_iterator<Iter>  ← 反向遍历适配
│   ├── insert_iterator<Container> ← 插入操作适配
│   ├── stream_iterator<T>      ← 流操作适配
│   └── move_iterator<Iter>     ← 移动语义适配
│
└── 函数适配器 (Function Adapters)
    ├── bind<F, Args...>        ← 参数绑定适配
    ├── mem_fn<R(T::*)(Args...)> ← 成员函数适配
    ├── not1, not2              ← 逻辑否定适配
    └── function<Signature>     ← 类型擦除适配

适配器设计原则:
✓ 接口转换而非功能扩展
✓ 组合现有组件
✓ 零开销抽象
✓ 类型安全的包装
```

---

## 🏗️ 容器适配器深度设计

### 📚 Stack Adapter - 栈适配器的艺术

```cpp
#include <stack>
#include <vector>
#include <deque>
#include <list>
#include <iostream>
#include <chrono>
#include <memory>

class StackAdapterMasterclass {
public:
    // 1. Stack的设计原理和实现
    static void stackDesignPrinciples() {
        std::cout << "=== Stack适配器设计原理 ===" << std::endl;
        
        // 展示stack的本质：接口限制
        std::cout << "1. Stack本质 - 接口限制:" << std::endl;
        
        std::deque<int> raw_deque = {1, 2, 3, 4, 5};
        std::stack<int> stack_adapter;
        
        // 原始deque的所有能力
        std::cout << "原始deque能力:" << std::endl;
        std::cout << "  可以随机访问: raw_deque[2] = " << raw_deque[2] << std::endl;
        std::cout << "  可以前端操作: push_front, pop_front" << std::endl;
        std::cout << "  可以后端操作: push_back, pop_back" << std::endl;
        std::cout << "  可以遍历: begin(), end()" << std::endl;
        
        // stack适配后的限制接口
        for(int x : raw_deque) {
            stack_adapter.push(x);
        }
        
        std::cout << "\nStack适配后的限制接口:" << std::endl;
        std::cout << "  只能访问顶部: top()" << std::endl;
        std::cout << "  只能压栈: push()" << std::endl;
        std::cout << "  只能出栈: pop()" << std::endl;
        std::cout << "  stack.top() = " << stack_adapter.top() << std::endl;
        
        // 演示适配器的接口限制价值
        std::cout << "\n2. 接口限制的价值:" << std::endl;
        std::cout << "- 防误用: 不能意外访问中间元素" << std::endl;
        std::cout << "- 语义清晰: 明确表达LIFO意图" << std::endl;
        std::cout << "- 设计安全: 编译时强制栈语义" << std::endl;
    }
    
    // 2. 自定义Stack适配器实现
    static void customStackImplementation() {
        std::cout << "\n=== 自定义Stack适配器实现 ===" << std::endl;
        
        // 完整的Stack适配器实现
        template<typename T, typename Container = std::deque<T>>
        class MyStack {
        public:
            // 类型别名
            using container_type = Container;
            using value_type = typename Container::value_type;
            using size_type = typename Container::size_type;
            using reference = typename Container::reference;
            using const_reference = typename Container::const_reference;
            
        private:
            Container c;  // 底层容器
            
        public:
            // 构造函数
            MyStack() = default;
            explicit MyStack(const Container& cont) : c(cont) {}
            explicit MyStack(Container&& cont) : c(std::move(cont)) {}
            
            template<typename Alloc>
            explicit MyStack(const Alloc& alloc) : c(alloc) {}
            
            template<typename Alloc>
            MyStack(const Container& cont, const Alloc& alloc) : c(cont, alloc) {}
            
            template<typename Alloc>
            MyStack(Container&& cont, const Alloc& alloc) : c(std::move(cont), alloc) {}
            
            // 核心操作
            bool empty() const { return c.empty(); }
            size_type size() const { return c.size(); }
            
            reference top() { return c.back(); }
            const_reference top() const { return c.back(); }
            
            void push(const value_type& value) { c.push_back(value); }
            void push(value_type&& value) { c.push_back(std::move(value)); }
            
            template<typename... Args>
            void emplace(Args&&... args) { c.emplace_back(std::forward<Args>(args)...); }
            
            void pop() { c.pop_back(); }
            
            void swap(MyStack& other) noexcept { 
                using std::swap;
                swap(c, other.c); 
            }
            
            // 获取底层容器（调试用）
            const Container& getContainer() const { return c; }
            
            // 比较操作
            friend bool operator==(const MyStack& lhs, const MyStack& rhs) {
                return lhs.c == rhs.c;
            }
            
            friend bool operator!=(const MyStack& lhs, const MyStack& rhs) {
                return !(lhs == rhs);
            }
            
            friend bool operator<(const MyStack& lhs, const MyStack& rhs) {
                return lhs.c < rhs.c;
            }
            
            friend bool operator<=(const MyStack& lhs, const MyStack& rhs) {
                return !(rhs < lhs);
            }
            
            friend bool operator>(const MyStack& lhs, const MyStack& rhs) {
                return rhs < lhs;
            }
            
            friend bool operator>=(const MyStack& lhs, const MyStack& rhs) {
                return !(lhs < rhs);
            }
        };
        
        // 测试自定义Stack
        std::cout << "自定义Stack测试:" << std::endl;
        
        MyStack<int> my_stack;
        for(int i = 1; i <= 5; ++i) {
            my_stack.push(i * 10);
        }
        
        std::cout << "栈内容 (top to bottom): ";
        MyStack<int> temp_stack = my_stack;  // 复制用于遍历
        while(!temp_stack.empty()) {
            std::cout << temp_stack.top() << " ";
            temp_stack.pop();
        }
        std::cout << std::endl;
        
        // 测试emplace
        my_stack.emplace(60);
        std::cout << "emplace(60)后栈顶: " << my_stack.top() << std::endl;
        
        // 测试不同底层容器
        std::cout << "\n不同底层容器的Stack:" << std::endl;
        
        MyStack<int, std::vector<int>> vector_stack;
        MyStack<int, std::list<int>> list_stack;
        
        for(int i = 1; i <= 3; ++i) {
            vector_stack.push(i);
            list_stack.push(i * 10);
        }
        
        std::cout << "vector_stack顶部: " << vector_stack.top() << std::endl;
        std::cout << "list_stack顶部: " << list_stack.top() << std::endl;
    }
    
    // 3. Stack的高级应用场景
    static void advancedStackApplications() {
        std::cout << "\n=== Stack高级应用场景 ===" << std::endl;
        
        // 应用1: 表达式求值器
        class ExpressionEvaluator {
        private:
            std::stack<double> operands;
            std::stack<char> operators;
            
            int precedence(char op) {
                switch(op) {
                    case '+': case '-': return 1;
                    case '*': case '/': return 2;
                    case '^': return 3;
                    default: return 0;
                }
            }
            
            bool isRightAssociative(char op) {
                return op == '^';
            }
            
            void processOperator() {
                if(operands.size() < 2 || operators.empty()) return;
                
                double b = operands.top(); operands.pop();
                double a = operands.top(); operands.pop();
                char op = operators.top(); operators.pop();
                
                double result;
                switch(op) {
                    case '+': result = a + b; break;
                    case '-': result = a - b; break;
                    case '*': result = a * b; break;
                    case '/': result = a / b; break;
                    case '^': result = std::pow(a, b); break;
                    default: result = 0;
                }
                
                operands.push(result);
            }
            
        public:
            double evaluate(const std::string& expression) {
                // 清空栈
                while(!operands.empty()) operands.pop();
                while(!operators.empty()) operators.pop();
                
                for(size_t i = 0; i < expression.length(); ++i) {
                    char c = expression[i];
                    
                    if(std::isspace(c)) continue;
                    
                    if(std::isdigit(c) || c == '.') {
                        // 解析数字
                        size_t j = i;
                        while(j < expression.length() && 
                              (std::isdigit(expression[j]) || expression[j] == '.')) {
                            ++j;
                        }
                        double num = std::stod(expression.substr(i, j - i));
                        operands.push(num);
                        i = j - 1;
                    } else if(c == '(') {
                        operators.push(c);
                    } else if(c == ')') {
                        while(!operators.empty() && operators.top() != '(') {
                            processOperator();
                        }
                        if(!operators.empty()) operators.pop(); // remove '('
                    } else if(c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
                        while(!operators.empty() && operators.top() != '(' &&
                              (precedence(operators.top()) > precedence(c) ||
                               (precedence(operators.top()) == precedence(c) && !isRightAssociative(c)))) {
                            processOperator();
                        }
                        operators.push(c);
                    }
                }
                
                while(!operators.empty()) {
                    processOperator();
                }
                
                return operands.empty() ? 0 : operands.top();
            }
        };
        
        std::cout << "表达式求值器测试:" << std::endl;
        ExpressionEvaluator evaluator;
        
        std::vector<std::string> expressions = {
            "3 + 4 * 2",
            "(3 + 4) * 2",
            "2 ^ 3 ^ 2",
            "10 - 2 * 3",
            "(10 - 2) * 3"
        };
        
        for(const auto& expr : expressions) {
            double result = evaluator.evaluate(expr);
            std::cout << expr << " = " << result << std::endl;
        }
        
        // 应用2: 调用栈跟踪器
        std::cout << "\n调用栈跟踪器:" << std::endl;
        
        class CallStackTracker {
        private:
            struct StackFrame {
                std::string function_name;
                std::string file_name;
                int line_number;
                std::chrono::steady_clock::time_point enter_time;
                
                StackFrame(const std::string& func, const std::string& file, int line)
                    : function_name(func), file_name(file), line_number(line),
                      enter_time(std::chrono::steady_clock::now()) {}
            };
            
            std::stack<StackFrame> call_stack;
            
        public:
            void enterFunction(const std::string& func, const std::string& file, int line) {
                call_stack.emplace(func, file, line);
                std::cout << std::string(call_stack.size() * 2, ' ') 
                          << "-> " << func << " (" << file << ":" << line << ")" << std::endl;
            }
            
            void exitFunction() {
                if(!call_stack.empty()) {
                    auto frame = call_stack.top();
                    call_stack.pop();
                    
                    auto duration = std::chrono::steady_clock::now() - frame.enter_time;
                    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration);
                    
                    std::cout << std::string((call_stack.size() + 1) * 2, ' ')
                              << "<- " << frame.function_name 
                              << " (执行时间: " << microseconds.count() << "μs)" << std::endl;
                }
            }
            
            void printCurrentStack() const {
                std::cout << "\n当前调用栈:" << std::endl;
                std::stack<StackFrame> temp = call_stack;
                std::vector<StackFrame> frames;
                
                while(!temp.empty()) {
                    frames.push_back(temp.top());
                    temp.pop();
                }
                
                for(auto it = frames.rbegin(); it != frames.rend(); ++it) {
                    std::cout << "  " << it->function_name 
                              << " (" << it->file_name << ":" << it->line_number << ")" << std::endl;
                }
            }
        };
        
        // 模拟函数调用
        CallStackTracker tracker;
        
        tracker.enterFunction("main", "main.cpp", 10);
        tracker.enterFunction("processData", "data.cpp", 25);
        tracker.enterFunction("validateInput", "validation.cpp", 40);
        tracker.printCurrentStack();
        tracker.exitFunction();
        tracker.enterFunction("transformData", "transform.cpp", 60);
        tracker.exitFunction();
        tracker.exitFunction();
        tracker.exitFunction();
    }
};
```

### 📋 Queue和Priority Queue适配器

```cpp
class QueueAdaptersMasterclass {
public:
    // 1. Queue适配器的设计特点
    static void queueAdapterDesign() {
        std::cout << "\n=== Queue适配器设计特点 ===" << std::endl;
        
        std::cout << "1. Queue vs Stack的接口差异:" << std::endl;
        
        // 对比展示
        std::stack<int> stk;
        std::queue<int> que;
        
        // 相同的操作
        for(int i = 1; i <= 5; ++i) {
            stk.push(i);
            que.push(i);
        }
        
        std::cout << "Stack (LIFO): ";
        while(!stk.empty()) {
            std::cout << stk.top() << " ";
            stk.pop();
        }
        std::cout << std::endl;
        
        std::cout << "Queue (FIFO): ";
        while(!que.empty()) {
            std::cout << que.front() << " ";
            que.pop();
        }
        std::cout << std::endl;
        
        // Queue的双端访问特性
        std::cout << "\n2. Queue的双端访问:" << std::endl;
        
        std::queue<std::string> string_queue;
        string_queue.push("first");
        string_queue.push("second");
        string_queue.push("third");
        
        std::cout << "front(): " << string_queue.front() << std::endl;
        std::cout << "back(): " << string_queue.back() << std::endl;
        
        // 修改操作
        string_queue.front() = "FIRST";
        string_queue.back() = "THIRD";
        
        std::cout << "修改后:" << std::endl;
        while(!string_queue.empty()) {
            std::cout << string_queue.front() << " ";
            string_queue.pop();
        }
        std::cout << std::endl;
    }
    
    // 2. Priority Queue的堆实现原理
    static void priorityQueueHeapPrinciples() {
        std::cout << "\n=== Priority Queue堆实现原理 ===" << std::endl;
        
        // 展示priority_queue的堆特性
        std::cout << "1. 默认最大堆行为:" << std::endl;
        
        std::priority_queue<int> max_heap;
        std::vector<int> data = {3, 1, 4, 1, 5, 9, 2, 6};
        
        std::cout << "插入顺序: ";
        for(int x : data) {
            max_heap.push(x);
            std::cout << x << " ";
        }
        std::cout << std::endl;
        
        std::cout << "最大堆输出: ";
        while(!max_heap.empty()) {
            std::cout << max_heap.top() << " ";
            max_heap.pop();
        }
        std::cout << std::endl;
        
        // 自定义比较器的最小堆
        std::cout << "\n2. 最小堆实现:" << std::endl;
        
        std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap;
        
        for(int x : data) {
            min_heap.push(x);
        }
        
        std::cout << "最小堆输出: ";
        while(!min_heap.empty()) {
            std::cout << min_heap.top() << " ";
            min_heap.pop();
        }
        std::cout << std::endl;
        
        // 复杂对象的优先队列
        std::cout << "\n3. 复杂对象优先队列:" << std::endl;
        
        struct Task {
            std::string name;
            int priority;
            std::chrono::steady_clock::time_point deadline;
            
            Task(const std::string& n, int p, int minutes_from_now = 60)
                : name(n), priority(p), 
                  deadline(std::chrono::steady_clock::now() + std::chrono::minutes(minutes_from_now)) {}
            
            // 优先级高的任务优先（数字越大优先级越高）
            bool operator<(const Task& other) const {
                if(priority != other.priority) {
                    return priority < other.priority;  // 优先级高的优先
                }
                return deadline > other.deadline;  // 截止时间早的优先
            }
        };
        
        std::priority_queue<Task> task_queue;
        
        task_queue.emplace("低优先级任务", 1, 120);
        task_queue.emplace("高优先级任务", 5, 90);
        task_queue.emplace("中优先级任务", 3, 60);
        task_queue.emplace("紧急任务", 5, 30);  // 同优先级，但更紧急
        
        std::cout << "任务执行顺序:" << std::endl;
        while(!task_queue.empty()) {
            const auto& task = task_queue.top();
            std::cout << "- " << task.name << " (优先级: " << task.priority << ")" << std::endl;
            task_queue.pop();
        }
    }
    
    // 3. 自定义Queue适配器实现
    static void customQueueImplementation() {
        std::cout << "\n=== 自定义Queue适配器实现 ===" << std::endl;
        
        // 线程安全的Queue适配器
        template<typename T, typename Container = std::deque<T>>
        class ThreadSafeQueue {
        private:
            Container c;
            mutable std::mutex mutex_;
            std::condition_variable condition_;
            
        public:
            using value_type = typename Container::value_type;
            using size_type = typename Container::size_type;
            
            ThreadSafeQueue() = default;
            
            // 禁用拷贝构造和赋值（简化实现）
            ThreadSafeQueue(const ThreadSafeQueue&) = delete;
            ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;
            
            void push(const T& item) {
                std::lock_guard<std::mutex> lock(mutex_);
                c.push_back(item);
                condition_.notify_one();
            }
            
            void push(T&& item) {
                std::lock_guard<std::mutex> lock(mutex_);
                c.push_back(std::move(item));
                condition_.notify_one();
            }
            
            template<typename... Args>
            void emplace(Args&&... args) {
                std::lock_guard<std::mutex> lock(mutex_);
                c.emplace_back(std::forward<Args>(args)...);
                condition_.notify_one();
            }
            
            // 阻塞式pop
            T pop() {
                std::unique_lock<std::mutex> lock(mutex_);
                condition_.wait(lock, [this] { return !c.empty(); });
                
                T result = std::move(c.front());
                c.pop_front();
                return result;
            }
            
            // 非阻塞式pop
            bool try_pop(T& item) {
                std::lock_guard<std::mutex> lock(mutex_);
                if(c.empty()) return false;
                
                item = std::move(c.front());
                c.pop_front();
                return true;
            }
            
            // 超时pop
            template<typename Rep, typename Period>
            bool pop_for(T& item, const std::chrono::duration<Rep, Period>& timeout) {
                std::unique_lock<std::mutex> lock(mutex_);
                if(condition_.wait_for(lock, timeout, [this] { return !c.empty(); })) {
                    item = std::move(c.front());
                    c.pop_front();
                    return true;
                }
                return false;
            }
            
            bool empty() const {
                std::lock_guard<std::mutex> lock(mutex_);
                return c.empty();
            }
            
            size_type size() const {
                std::lock_guard<std::mutex> lock(mutex_);
                return c.size();
            }
        };
        
        std::cout << "线程安全Queue演示:" << std::endl;
        
        ThreadSafeQueue<int> ts_queue;
        
        // 模拟生产者
        std::thread producer([&ts_queue]() {
            for(int i = 1; i <= 5; ++i) {
                ts_queue.push(i * 10);
                std::cout << "生产: " << i * 10 << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });
        
        // 模拟消费者
        std::thread consumer([&ts_queue]() {
            for(int i = 0; i < 5; ++i) {
                int item = ts_queue.pop();
                std::cout << "消费: " << item << std::endl;
            }
        });
        
        producer.join();
        consumer.join();
        
        std::cout << "生产者-消费者模式完成" << std::endl;
    }
};
```

---

## 🔗 迭代器适配器深度设计

### 🔄 Reverse Iterator - 反向迭代器的实现原理

```cpp
class ReverseIteratorMasterclass {
public:
    // 1. Reverse Iterator的实现原理
    static void reverseIteratorImplementation() {
        std::cout << "\n=== Reverse Iterator实现原理 ===" << std::endl;
        
        // 自定义Reverse Iterator实现
        template<typename Iterator>
        class MyReverseIterator {
        private:
            Iterator current;
            
        public:
            using iterator_type = Iterator;
            using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;
            using value_type = typename std::iterator_traits<Iterator>::value_type;
            using difference_type = typename std::iterator_traits<Iterator>::difference_type;
            using pointer = typename std::iterator_traits<Iterator>::pointer;
            using reference = typename std::iterator_traits<Iterator>::reference;
            
            // 构造函数
            MyReverseIterator() = default;
            explicit MyReverseIterator(Iterator it) : current(it) {}
            
            template<typename U>
            MyReverseIterator(const MyReverseIterator<U>& other) : current(other.base()) {}
            
            // 获取底层迭代器
            Iterator base() const { return current; }
            
            // 解引用操作 - 关键：返回前一个位置的值
            reference operator*() const {
                Iterator temp = current;
                return *--temp;
            }
            
            pointer operator->() const {
                return &(operator*());
            }
            
            // 移动操作 - 方向相反
            MyReverseIterator& operator++() {
                --current;
                return *this;
            }
            
            MyReverseIterator operator++(int) {
                MyReverseIterator temp = *this;
                --current;
                return temp;
            }
            
            MyReverseIterator& operator--() {
                ++current;
                return *this;
            }
            
            MyReverseIterator operator--(int) {
                MyReverseIterator temp = *this;
                ++current;
                return temp;
            }
            
            // 随机访问操作（如果底层迭代器支持）
            MyReverseIterator& operator+=(difference_type n) {
                current -= n;
                return *this;
            }
            
            MyReverseIterator operator+(difference_type n) const {
                return MyReverseIterator(current - n);
            }
            
            MyReverseIterator& operator-=(difference_type n) {
                current += n;
                return *this;
            }
            
            MyReverseIterator operator-(difference_type n) const {
                return MyReverseIterator(current + n);
            }
            
            reference operator[](difference_type n) const {
                return *(*this + n);
            }
            
            // 比较操作
            bool operator==(const MyReverseIterator& other) const {
                return current == other.current;
            }
            
            bool operator!=(const MyReverseIterator& other) const {
                return current != other.current;
            }
            
            bool operator<(const MyReverseIterator& other) const {
                return current > other.current;  // 注意方向相反
            }
        };
        
        // 测试自定义反向迭代器
        std::vector<int> vec = {1, 2, 3, 4, 5};
        
        std::cout << "原始顺序: ";
        for(auto it = vec.begin(); it != vec.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
        
        std::cout << "自定义反向: ";
        MyReverseIterator<std::vector<int>::iterator> rbegin(vec.end());
        MyReverseIterator<std::vector<int>::iterator> rend(vec.begin());
        
        for(auto it = rbegin; it != rend; ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
        
        // 演示base()的重要性
        std::cout << "\n演示base()方法:" << std::endl;
        auto rit = rbegin + 2;  // 指向第3个元素（从后往前数）
        auto normal_it = rit.base();  // 转换回正向迭代器
        
        std::cout << "反向迭代器指向: " << *rit << std::endl;
        std::cout << "对应的正向迭代器指向: " << *normal_it << std::endl;
        std::cout << "注意：base()指向的是反向迭代器的下一个位置！" << std::endl;
    }
    
    // 2. Reverse Iterator的高级应用
    static void reverseIteratorAdvancedApplications() {
        std::cout << "\n=== Reverse Iterator高级应用 ===" << std::endl;
        
        // 应用1: 反向搜索算法
        std::cout << "1. 反向搜索应用:" << std::endl;
        
        std::string text = "hello world programming";
        std::cout << "文本: " << text << std::endl;
        
        // 从后往前查找第一个'o'
        auto rit = std::find(text.rbegin(), text.rend(), 'o');
        if(rit != text.rend()) {
            // 转换为正向位置
            auto pos = rit.base() - 1;  // base()-1得到正确位置
            std::cout << "从后往前第一个'o'的位置: " << (pos - text.begin()) << std::endl;
            std::cout << "字符: " << *pos << std::endl;
        }
        
        // 应用2: 回文检测
        std::cout << "\n2. 回文检测:" << std::endl;
        
        auto isPalindrome = [](const std::string& s) {
            return std::equal(s.begin(), s.end(), s.rbegin());
        };
        
        std::vector<std::string> test_strings = {"racecar", "hello", "madam", "world", "level"};
        
        for(const auto& s : test_strings) {
            std::cout << "\"" << s << "\" 是回文: " << (isPalindrome(s) ? "是" : "否") << std::endl;
        }
        
        // 应用3: 反向复制和变换
        std::cout << "\n3. 反向变换应用:" << std::endl;
        
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        std::vector<int> reversed_squares;
        
        // 反向遍历并平方
        std::transform(numbers.rbegin(), numbers.rend(),
                      std::back_inserter(reversed_squares),
                      [](int x) { return x * x; });
        
        std::cout << "原数组: ";
        for(int x : numbers) std::cout << x << " ";
        std::cout << std::endl;
        
        std::cout << "反向平方: ";
        for(int x : reversed_squares) std::cout << x << " ";
        std::cout << std::endl;
    }
};
```

### 🔀 Insert Iterator - 插入迭代器的智慧

```cpp
class InsertIteratorMasterclass {
public:
    // 1. Insert Iterator的实现原理
    static void insertIteratorImplementation() {
        std::cout << "\n=== Insert Iterator实现原理 ===" << std::endl;
        
        // 自定义back_insert_iterator实现
        template<typename Container>
        class MyBackInsertIterator {
        private:
            Container* container;
            
        public:
            using iterator_category = std::output_iterator_tag;
            using value_type = void;
            using difference_type = void;
            using pointer = void;
            using reference = void;
            using container_type = Container;
            
            explicit MyBackInsertIterator(Container& c) : container(&c) {}
            
            // 关键：赋值操作转换为插入操作
            MyBackInsertIterator& operator=(const typename Container::value_type& value) {
                container->push_back(value);
                return *this;
            }
            
            MyBackInsertIterator& operator=(typename Container::value_type&& value) {
                container->push_back(std::move(value));
                return *this;
            }
            
            // 这些操作都返回自身（空操作）
            MyBackInsertIterator& operator*() { return *this; }
            MyBackInsertIterator& operator++() { return *this; }
            MyBackInsertIterator& operator++(int) { return *this; }
        };
        
        // 便利函数
        template<typename Container>
        MyBackInsertIterator<Container> my_back_inserter(Container& c) {
            return MyBackInsertIterator<Container>(c);
        }
        
        // 测试自定义插入迭代器
        std::vector<int> source = {1, 2, 3, 4, 5};
        std::vector<int> dest;
        
        std::cout << "使用自定义back_inserter:" << std::endl;
        std::copy(source.begin(), source.end(), my_back_inserter(dest));
        
        std::cout << "复制结果: ";
        for(int x : dest) std::cout << x << " ";
        std::cout << std::endl;
        
        // 自定义insert_iterator实现
        template<typename Container>
        class MyInsertIterator {
        private:
            Container* container;
            typename Container::iterator iter;
            
        public:
            using iterator_category = std::output_iterator_tag;
            using value_type = void;
            using difference_type = void;
            using pointer = void;
            using reference = void;
            using container_type = Container;
            
            MyInsertIterator(Container& c, typename Container::iterator i) 
                : container(&c), iter(i) {}
            
            MyInsertIterator& operator=(const typename Container::value_type& value) {
                iter = container->insert(iter, value);
                ++iter;  // 移动到插入元素之后
                return *this;
            }
            
            MyInsertIterator& operator=(typename Container::value_type&& value) {
                iter = container->insert(iter, std::move(value));
                ++iter;
                return *this;
            }
            
            MyInsertIterator& operator*() { return *this; }
            MyInsertIterator& operator++() { return *this; }
            MyInsertIterator& operator++(int) { return *this; }
        };
        
        template<typename Container>
        MyInsertIterator<Container> my_inserter(Container& c, typename Container::iterator i) {
            return MyInsertIterator<Container>(c, i);
        }
        
        // 测试insert_iterator
        std::vector<int> vec = {1, 5, 9};
        std::vector<int> to_insert = {2, 3, 4};
        
        std::cout << "\n插入前: ";
        for(int x : vec) std::cout << x << " ";
        std::cout << std::endl;
        
        auto insert_pos = vec.begin() + 1;
        std::copy(to_insert.begin(), to_insert.end(), my_inserter(vec, insert_pos));
        
        std::cout << "插入后: ";
        for(int x : vec) std::cout << x << " ";
        std::cout << std::endl;
    }
    
    // 2. Insert Iterator的实际应用
    static void insertIteratorApplications() {
        std::cout << "\n=== Insert Iterator实际应用 ===" << std::endl;
        
        // 应用1: 容器合并
        std::cout << "1. 容器合并:" << std::endl;
        
        std::vector<int> vec1 = {1, 3, 5};
        std::vector<int> vec2 = {2, 4, 6};
        std::vector<int> vec3 = {7, 8, 9};
        
        std::vector<int> merged;
        
        // 使用back_inserter合并多个容器
        std::copy(vec1.begin(), vec1.end(), std::back_inserter(merged));
        std::copy(vec2.begin(), vec2.end(), std::back_inserter(merged));
        std::copy(vec3.begin(), vec3.end(), std::back_inserter(merged));
        
        std::cout << "合并结果: ";
        for(int x : merged) std::cout << x << " ";
        std::cout << std::endl;
        
        // 应用2: 有序容器的插入
        std::cout << "\n2. 有序容器插入:" << std::endl;
        
        std::set<int> ordered_set = {1, 3, 5, 7, 9};
        std::vector<int> new_elements = {2, 4, 6, 8};
        
        std::cout << "原有序集合: ";
        for(int x : ordered_set) std::cout << x << " ";
        std::cout << std::endl;
        
        // 使用inserter插入到set
        std::copy(new_elements.begin(), new_elements.end(), 
                 std::inserter(ordered_set, ordered_set.end()));
        
        std::cout << "插入后: ";
        for(int x : ordered_set) std::cout << x << " ";
        std::cout << std::endl;
        
        // 应用3: 条件插入
        std::cout << "\n3. 条件插入:" << std::endl;
        
        std::vector<int> source_data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::vector<int> even_numbers, odd_numbers;
        
        // 分离奇偶数
        for(int x : source_data) {
            if(x % 2 == 0) {
                *std::back_inserter(even_numbers) = x;
            } else {
                *std::back_inserter(odd_numbers) = x;
            }
        }
        
        std::cout << "偶数: ";
        for(int x : even_numbers) std::cout << x << " ";
        std::cout << std::endl;
        
        std::cout << "奇数: ";
        for(int x : odd_numbers) std::cout << x << " ";
        std::cout << std::endl;
        
        // 更优雅的方式使用copy_if
        std::vector<int> even_copy, odd_copy;
        
        std::copy_if(source_data.begin(), source_data.end(),
                    std::back_inserter(even_copy),
                    [](int x) { return x % 2 == 0; });
        
        std::copy_if(source_data.begin(), source_data.end(),
                    std::back_inserter(odd_copy),
                    [](int x) { return x % 2 == 1; });
        
        std::cout << "\ncopy_if偶数: ";
        for(int x : even_copy) std::cout << x << " ";
        std::cout << std::endl;
        
        std::cout << "copy_if奇数: ";
        for(int x : odd_copy) std::cout << x << " ";
        std::cout << std::endl;
    }
};
```

现在让我更新TODO并继续创建其他适配器教程：

<function_calls>
<invoke name="todo_write">
<parameter name="merge">true

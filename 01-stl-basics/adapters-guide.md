# C++ 容器适配器详解

## 🎯 什么是容器适配器？

容器适配器是基于其他容器实现的特殊容器，它们提供特定的接口和行为：
- **stack**: 后进先出(LIFO)的栈
- **queue**: 先进先出(FIFO)的队列  
- **priority_queue**: 优先级队列，按优先级出队

## 🔧 基本特性

### Stack（栈）
- **LIFO原则**: 最后进入的元素最先出来
- **基于deque**: 默认使用deque作为底层容器
- **简单接口**: push, pop, top, empty, size

### Queue（队列）
- **FIFO原则**: 最先进入的元素最先出来
- **基于deque**: 默认使用deque作为底层容器
- **简单接口**: push, pop, front, back, empty, size

### Priority Queue（优先级队列）
- **优先级排序**: 按优先级出队，默认最大优先级先出
- **基于vector**: 默认使用vector作为底层容器
- **堆结构**: 内部维护最大堆或最小堆

## 📚 常用操作

### 1. Stack基本操作
```cpp
#include <stack>
using namespace std;

// 创建栈
stack<int> s1;
stack<int, vector<int>> s2;  // 指定底层容器

// 基本操作
s1.push(10);                 // 入栈
s1.push(20);
s1.push(30);

cout << "Top: " << s1.top() << endl;  // 查看栈顶：30
cout << "Size: " << s1.size() << endl;  // 大小：3

s1.pop();                    // 出栈
cout << "Top: " << s1.top() << endl;  // 新的栈顶：20

// 检查是否为空
if (!s1.empty()) {
    cout << "Stack is not empty" << endl;
}

// 遍历栈（注意：会清空栈）
while (!s1.empty()) {
    cout << s1.top() << " ";
    s1.pop();
}
```

### 2. Queue基本操作
```cpp
#include <queue>
using namespace std;

// 创建队列
queue<int> q1;
queue<int, list<int>> q2;    // 指定底层容器

// 基本操作
q1.push(10);                 // 入队
q1.push(20);
q1.push(30);

cout << "Front: " << q1.front() << endl;  // 队首：10
cout << "Back: " << q1.back() << endl;    // 队尾：30
cout << "Size: " << q1.size() << endl;    // 大小：3

q1.pop();                    // 出队
cout << "Front: " << q1.front() << endl;  // 新的队首：20

// 检查是否为空
if (!q1.empty()) {
    cout << "Queue is not empty" << endl;
}

// 遍历队列（注意：会清空队列）
while (!q1.empty()) {
    cout << q1.front() << " ";
    q1.pop();
}
```

### 3. Priority Queue基本操作
```cpp
#include <queue>
using namespace std;

// 创建优先级队列
priority_queue<int> pq1;     // 默认最大堆
priority_queue<int, vector<int>, greater<int>> pq2;  // 最小堆

// 基本操作
pq1.push(30);
pq1.push(10);
pq1.push(50);
pq1.push(20);

cout << "Top: " << pq1.top() << endl;  // 最大元素：50
cout << "Size: " << pq1.size() << endl;  // 大小：4

pq1.pop();                   // 出队最大元素
cout << "Top: " << pq1.top() << endl;  // 新的最大元素：30

// 自定义比较函数
struct Compare {
    bool operator()(int a, int b) const {
        return a > b;  // 最小堆
    }
};

priority_queue<int, vector<int>, Compare> pq3;

// 遍历优先级队列（注意：会清空队列）
while (!pq1.empty()) {
    cout << pq1.top() << " ";
    pq1.pop();
}
```

## ⚡ 性能分析

### 时间复杂度
- **Stack**: push/pop/top - O(1)
- **Queue**: push/pop/front/back - O(1)
- **Priority Queue**: push/pop - O(log n), top - O(1)

### 空间复杂度
- **存储**: O(n)
- **额外开销**: 容器适配器本身开销很小

## 💡 使用技巧

### 1. 选择底层容器
```cpp
// 对于频繁插入删除的栈
stack<int, list<int>> s;     // 使用list作为底层容器

// 对于需要随机访问的栈
stack<int, vector<int>> s;   // 使用vector作为底层容器

// 对于频繁插入删除的队列
queue<int, list<int>> q;     // 使用list作为底层容器
```

### 2. 自定义比较函数
```cpp
// 为自定义类型定义优先级
struct Task {
    int priority;
    string description;
    
    bool operator<(const Task& other) const {
        return priority < other.priority;  // 优先级高的先出队
    }
};

priority_queue<Task> taskQueue;
```

### 3. 实现特定功能
```cpp
// 使用两个栈实现队列
class MyQueue {
private:
    stack<int> input, output;
    
public:
    void push(int x) {
        input.push(x);
    }
    
    int pop() {
        if (output.empty()) {
            while (!input.empty()) {
                output.push(input.top());
                input.pop();
            }
        }
        int result = output.top();
        output.pop();
        return result;
    }
    
    int peek() {
        if (output.empty()) {
            while (!input.empty()) {
                output.push(input.top());
                input.pop();
            }
        }
        return output.top();
    }
    
    bool empty() {
        return input.empty() && output.empty();
    }
};
```

## 🎯 适用场景

### Stack适用场景
- **函数调用栈**: 递归、函数调用
- **括号匹配**: 检查括号是否匹配
- **深度优先搜索**: DFS算法
- **撤销操作**: 实现撤销功能

### Queue适用场景
- **广度优先搜索**: BFS算法
- **任务调度**: 先进先出的任务处理
- **缓冲区**: 数据流处理
- **打印队列**: 文档打印排队

### Priority Queue适用场景
- **任务调度**: 按优先级处理任务
- **事件处理**: 按时间顺序处理事件
- **算法实现**: Dijkstra、Prim等算法
- **Top K问题**: 找出前K个最大/最小元素

## 🔍 常见错误

### 1. 访问空容器
```cpp
stack<int> s;
int value = s.top();  // 错误！栈为空时访问top()

// 正确做法
if (!s.empty()) {
    int value = s.top();
}
```

### 2. 遍历后容器变空
```cpp
stack<int> s = {1, 2, 3, 4, 5};

// 错误：这样遍历会清空栈
while (!s.empty()) {
    cout << s.top() << " ";
    s.pop();
}

// 如果需要保留栈，应该先复制
stack<int> temp = s;
while (!temp.empty()) {
    cout << temp.top() << " ";
    temp.pop();
}
```

## 📝 实际应用示例

### 1. 括号匹配
```cpp
bool isValid(string s) {
    stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '{' || c == '[') {
            st.push(c);
        } else {
            if (st.empty()) return false;
            char top = st.top();
            if ((c == ')' && top == '(') ||
                (c == '}' && top == '{') ||
                (c == ']' && top == '[')) {
                st.pop();
            } else {
                return false;
            }
        }
    }
    return st.empty();
}
```

### 2. 滑动窗口最大值
```cpp
vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    vector<int> result;
    deque<int> dq;  // 使用deque实现单调队列
    
    for (int i = 0; i < nums.size(); ++i) {
        // 移除超出窗口的元素
        if (!dq.empty() && dq.front() == i - k) {
            dq.pop_front();
        }
        
        // 维护单调递减队列
        while (!dq.empty() && nums[dq.back()] < nums[i]) {
            dq.pop_back();
        }
        
        dq.push_back(i);
        
        if (i >= k - 1) {
            result.push_back(nums[dq.front()]);
        }
    }
    
    return result;
}
```

## 📝 练习题目
1. 实现一个支持getMin()的栈
2. 使用队列实现栈
3. 使用栈实现队列
4. 找出数组中第K大的元素

## 🚀 下一步
- 学习 [STL算法](./algorithms-guide.md)
- 开始 [数据结构实现](../02-data-structures/)
- 准备 [LeetCode刷题](../04-leetcode-guide/)

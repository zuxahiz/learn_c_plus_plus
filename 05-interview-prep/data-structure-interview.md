# 数据结构面试题详解

## 📋 目录

1. [数组与字符串](#数组与字符串)
2. [链表](#链表)
3. [栈与队列](#栈与队列)
4. [树](#树)
5. [图](#图)
6. [哈希表](#哈希表)
7. [堆](#堆)
8. [高级数据结构](#高级数据结构)

---

## 数组与字符串

### 1. 数组去重问题

**问题**: 如何在O(1)空间复杂度内去除有序数组中的重复元素？

**解答**:
```cpp
int removeDuplicates(vector<int>& nums) {
    if (nums.empty()) return 0;
    
    int slow = 0;
    for (int fast = 1; fast < nums.size(); fast++) {
        if (nums[fast] != nums[slow]) {
            nums[++slow] = nums[fast];
        }
    }
    return slow + 1;
}
```

**核心思想**: 双指针技巧，slow指针维护去重后的数组，fast指针遍历原数组。

**扩展问题**: 如果允许重复2次呢？
```cpp
int removeDuplicates(vector<int>& nums) {
    if (nums.size() <= 2) return nums.size();
    
    int slow = 2;
    for (int fast = 2; fast < nums.size(); fast++) {
        if (nums[fast] != nums[slow - 2]) {
            nums[slow++] = nums[fast];
        }
    }
    return slow;
}
```

### 2. 旋转数组

**问题**: 如何将数组向右旋转k个位置？

**解答**:
```cpp
void rotate(vector<int>& nums, int k) {
    int n = nums.size();
    k %= n;  // 处理k > n的情况
    
    // 方法1: 三次反转
    reverse(nums.begin(), nums.end());         // 全部反转
    reverse(nums.begin(), nums.begin() + k);  // 前k个反转
    reverse(nums.begin() + k, nums.end());    // 后n-k个反转
}
```

**时间复杂度**: O(n)，**空间复杂度**: O(1)

**其他方法**:
- 使用额外空间: O(n) 空间
- 环形替换: 适合数学推导，但实现复杂

### 3. 螺旋矩阵

**问题**: 按顺时针方向遍历二维矩阵

**解答**:
```cpp
vector<int> spiralOrder(vector<vector<int>>& matrix) {
    if (matrix.empty()) return {};
    
    int m = matrix.size(), n = matrix[0].size();
    vector<int> result;
    int top = 0, bottom = m - 1, left = 0, right = n - 1;
    
    while (top <= bottom && left <= right) {
        // 向右
        for (int j = left; j <= right; j++) {
            result.push_back(matrix[top][j]);
        }
        top++;
        
        // 向下
        for (int i = top; i <= bottom; i++) {
            result.push_back(matrix[i][right]);
        }
        right--;
        
        // 向左 (需要检查是否还有行)
        if (top <= bottom) {
            for (int j = right; j >= left; j--) {
                result.push_back(matrix[bottom][j]);
            }
            bottom--;
        }
        
        // 向上 (需要检查是否还有列)
        if (left <= right) {
            for (int i = bottom; i >= top; i--) {
                result.push_back(matrix[i][left]);
            }
            left++;
        }
    }
    
    return result;
}
```

---

## 链表

### 1. 链表反转

**问题**: 反转单链表

**解答**:
```cpp
ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;
    
    while (curr) {
        ListNode* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    
    return prev;
}
```

**递归解法**:
```cpp
ListNode* reverseList(ListNode* head) {
    if (!head || !head->next) return head;
    
    ListNode* newHead = reverseList(head->next);
    head->next->next = head;
    head->next = nullptr;
    
    return newHead;
}
```

### 2. 链表环检测

**问题**: 检测链表是否有环，如果有环，找到环的起始节点

**解答**:
```cpp
// 检测是否有环
bool hasCycle(ListNode *head) {
    ListNode* slow = head;
    ListNode* fast = head;
    
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    
    return false;
}

// 找到环的起始节点
ListNode *detectCycle(ListNode *head) {
    ListNode* slow = head;
    ListNode* fast = head;
    
    // 第一阶段：检测是否有环
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) break;
    }
    
    if (!fast || !fast->next) return nullptr;
    
    // 第二阶段：找到环的起始点
    slow = head;
    while (slow != fast) {
        slow = slow->next;
        fast = fast->next;
    }
    
    return slow;
}
```

**数学原理**: 
- 设链表头到环入口距离为a，环入口到相遇点距离为b，相遇点到环入口距离为c
- 相遇时：slow走了a+b，fast走了a+b+c+b = a+2b+c
- 由于fast速度是slow的2倍：2(a+b) = a+2b+c，得到a = c

### 3. 合并K个有序链表

**问题**: 合并K个升序链表

**解答** (分治法):
```cpp
ListNode* mergeKLists(vector<ListNode*>& lists) {
    if (lists.empty()) return nullptr;
    
    while (lists.size() > 1) {
        vector<ListNode*> merged;
        
        for (int i = 0; i < lists.size(); i += 2) {
            ListNode* l1 = lists[i];
            ListNode* l2 = (i + 1 < lists.size()) ? lists[i + 1] : nullptr;
            merged.push_back(mergeTwoLists(l1, l2));
        }
        
        lists = merged;
    }
    
    return lists[0];
}

ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    
    while (l1 && l2) {
        if (l1->val <= l2->val) {
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;
    }
    
    tail->next = l1 ? l1 : l2;
    return dummy.next;
}
```

**时间复杂度**: O(N log k)，其中N是所有节点总数，k是链表个数

---

## 栈与队列

### 1. 有效括号

**问题**: 判断括号字符串是否有效

**解答**:
```cpp
bool isValid(string s) {
    stack<char> st;
    unordered_map<char, char> mapping = {
        {')', '('}, {']', '['}, {'}', '{'}
    };
    
    for (char c : s) {
        if (mapping.count(c)) {  // 右括号
            if (st.empty() || st.top() != mapping[c]) {
                return false;
            }
            st.pop();
        } else {  // 左括号
            st.push(c);
        }
    }
    
    return st.empty();
}
```

### 2. 最小栈

**问题**: 设计一个栈，支持push、pop、top和getMin操作，所有操作的时间复杂度都是O(1)

**解答**:
```cpp
class MinStack {
private:
    stack<int> dataStack;
    stack<int> minStack;
    
public:
    void push(int val) {
        dataStack.push(val);
        if (minStack.empty() || val <= minStack.top()) {
            minStack.push(val);
        }
    }
    
    void pop() {
        if (dataStack.top() == minStack.top()) {
            minStack.pop();
        }
        dataStack.pop();
    }
    
    int top() {
        return dataStack.top();
    }
    
    int getMin() {
        return minStack.top();
    }
};
```

### 3. 用栈实现队列

**问题**: 用两个栈实现队列的基本操作

**解答**:
```cpp
class MyQueue {
private:
    stack<int> inStack, outStack;
    
    void transfer() {
        while (!inStack.empty()) {
            outStack.push(inStack.top());
            inStack.pop();
        }
    }
    
public:
    void push(int x) {
        inStack.push(x);
    }
    
    int pop() {
        if (outStack.empty()) {
            transfer();
        }
        int result = outStack.top();
        outStack.pop();
        return result;
    }
    
    int peek() {
        if (outStack.empty()) {
            transfer();
        }
        return outStack.top();
    }
    
    bool empty() {
        return inStack.empty() && outStack.empty();
    }
};
```

---

## 树

### 1. 二叉树的遍历

**问题**: 实现二叉树的前序、中序、后序遍历（迭代版本）

**前序遍历**:
```cpp
vector<int> preorderTraversal(TreeNode* root) {
    vector<int> result;
    stack<TreeNode*> st;
    
    while (root || !st.empty()) {
        while (root) {
            result.push_back(root->val);
            st.push(root);
            root = root->left;
        }
        root = st.top();
        st.pop();
        root = root->right;
    }
    
    return result;
}
```

**中序遍历**:
```cpp
vector<int> inorderTraversal(TreeNode* root) {
    vector<int> result;
    stack<TreeNode*> st;
    
    while (root || !st.empty()) {
        while (root) {
            st.push(root);
            root = root->left;
        }
        root = st.top();
        st.pop();
        result.push_back(root->val);
        root = root->right;
    }
    
    return result;
}
```

### 2. 二叉树的最大深度

**问题**: 计算二叉树的最大深度

**递归解法**:
```cpp
int maxDepth(TreeNode* root) {
    if (!root) return 0;
    return 1 + max(maxDepth(root->left), maxDepth(root->right));
}
```

**迭代解法**:
```cpp
int maxDepth(TreeNode* root) {
    if (!root) return 0;
    
    queue<TreeNode*> q;
    q.push(root);
    int depth = 0;
    
    while (!q.empty()) {
        int size = q.size();
        depth++;
        
        for (int i = 0; i < size; i++) {
            TreeNode* node = q.front();
            q.pop();
            
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }
    
    return depth;
}
```

### 3. 验证二叉搜索树

**问题**: 验证一棵二叉树是否为有效的二叉搜索树

**解答**:
```cpp
bool isValidBST(TreeNode* root) {
    return validate(root, LONG_MIN, LONG_MAX);
}

bool validate(TreeNode* root, long minVal, long maxVal) {
    if (!root) return true;
    
    if (root->val <= minVal || root->val >= maxVal) {
        return false;
    }
    
    return validate(root->left, minVal, root->val) && 
           validate(root->right, root->val, maxVal);
}
```

**中序遍历解法**:
```cpp
bool isValidBST(TreeNode* root) {
    TreeNode* prev = nullptr;
    return inorder(root, prev);
}

bool inorder(TreeNode* root, TreeNode*& prev) {
    if (!root) return true;
    
    if (!inorder(root->left, prev)) return false;
    
    if (prev && prev->val >= root->val) return false;
    prev = root;
    
    return inorder(root->right, prev);
}
```

---

## 图

### 1. 岛屿数量

**问题**: 在二维网格中计算岛屿的数量

**DFS解法**:
```cpp
int numIslands(vector<vector<char>>& grid) {
    if (grid.empty()) return 0;
    
    int m = grid.size(), n = grid[0].size();
    int count = 0;
    
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == '1') {
                count++;
                dfs(grid, i, j);
            }
        }
    }
    
    return count;
}

void dfs(vector<vector<char>>& grid, int i, int j) {
    if (i < 0 || i >= grid.size() || j < 0 || j >= grid[0].size() || 
        grid[i][j] == '0') {
        return;
    }
    
    grid[i][j] = '0';  // 标记为已访问
    
    dfs(grid, i + 1, j);
    dfs(grid, i - 1, j);
    dfs(grid, i, j + 1);
    dfs(grid, i, j - 1);
}
```

### 2. 课程表问题

**问题**: 判断是否可以完成所有课程（检测有向图中是否有环）

**拓扑排序解法**:
```cpp
bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
    vector<vector<int>> graph(numCourses);
    vector<int> indegree(numCourses, 0);
    
    // 构建图和入度数组
    for (auto& pre : prerequisites) {
        graph[pre[1]].push_back(pre[0]);
        indegree[pre[0]]++;
    }
    
    queue<int> q;
    for (int i = 0; i < numCourses; i++) {
        if (indegree[i] == 0) {
            q.push(i);
        }
    }
    
    int completed = 0;
    while (!q.empty()) {
        int course = q.front();
        q.pop();
        completed++;
        
        for (int next : graph[course]) {
            indegree[next]--;
            if (indegree[next] == 0) {
                q.push(next);
            }
        }
    }
    
    return completed == numCourses;
}
```

---

## 哈希表

### 1. 两数之和

**问题**: 在数组中找到和为目标值的两个数

**解答**:
```cpp
vector<int> twoSum(vector<int>& nums, int target) {
    unordered_map<int, int> map;
    
    for (int i = 0; i < nums.size(); i++) {
        int complement = target - nums[i];
        
        if (map.count(complement)) {
            return {map[complement], i};
        }
        
        map[nums[i]] = i;
    }
    
    return {};
}
```

### 2. LRU缓存

**问题**: 设计LRU缓存数据结构

**解答**:
```cpp
class LRUCache {
private:
    struct Node {
        int key, value;
        Node* prev;
        Node* next;
        Node(int k = 0, int v = 0) : key(k), value(v), prev(nullptr), next(nullptr) {}
    };
    
    int capacity;
    unordered_map<int, Node*> cache;
    Node* head;
    Node* tail;
    
    void addToHead(Node* node) {
        node->prev = head;
        node->next = head->next;
        head->next->prev = node;
        head->next = node;
    }
    
    void removeNode(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    
    void moveToHead(Node* node) {
        removeNode(node);
        addToHead(node);
    }
    
    Node* removeTail() {
        Node* last = tail->prev;
        removeNode(last);
        return last;
    }
    
public:
    LRUCache(int capacity) : capacity(capacity) {
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->prev = head;
    }
    
    int get(int key) {
        if (cache.count(key)) {
            Node* node = cache[key];
            moveToHead(node);
            return node->value;
        }
        return -1;
    }
    
    void put(int key, int value) {
        if (cache.count(key)) {
            Node* node = cache[key];
            node->value = value;
            moveToHead(node);
        } else {
            Node* newNode = new Node(key, value);
            
            if (cache.size() >= capacity) {
                Node* tail_node = removeTail();
                cache.erase(tail_node->key);
                delete tail_node;
            }
            
            cache[key] = newNode;
            addToHead(newNode);
        }
    }
};
```

---

## 堆

### 1. 数组中的第K个最大元素

**问题**: 找到数组中第K个最大的元素

**小顶堆解法**:
```cpp
int findKthLargest(vector<int>& nums, int k) {
    priority_queue<int, vector<int>, greater<int>> minHeap;
    
    for (int num : nums) {
        minHeap.push(num);
        if (minHeap.size() > k) {
            minHeap.pop();
        }
    }
    
    return minHeap.top();
}
```

**快速选择解法**:
```cpp
int findKthLargest(vector<int>& nums, int k) {
    return quickSelect(nums, 0, nums.size() - 1, nums.size() - k);
}

int quickSelect(vector<int>& nums, int left, int right, int k) {
    int pivot = partition(nums, left, right);
    
    if (pivot == k) {
        return nums[pivot];
    } else if (pivot < k) {
        return quickSelect(nums, pivot + 1, right, k);
    } else {
        return quickSelect(nums, left, pivot - 1, k);
    }
}

int partition(vector<int>& nums, int left, int right) {
    int pivot = nums[right];
    int i = left;
    
    for (int j = left; j < right; j++) {
        if (nums[j] <= pivot) {
            swap(nums[i++], nums[j]);
        }
    }
    
    swap(nums[i], nums[right]);
    return i;
}
```

---

## 高级数据结构

### 1. 前缀树（Trie）

**问题**: 实现前缀树

**解答**:
```cpp
class Trie {
private:
    struct TrieNode {
        bool isEnd;
        TrieNode* children[26];
        
        TrieNode() : isEnd(false) {
            for (int i = 0; i < 26; i++) {
                children[i] = nullptr;
            }
        }
    };
    
    TrieNode* root;
    
public:
    Trie() {
        root = new TrieNode();
    }
    
    void insert(string word) {
        TrieNode* node = root;
        for (char c : word) {
            int index = c - 'a';
            if (!node->children[index]) {
                node->children[index] = new TrieNode();
            }
            node = node->children[index];
        }
        node->isEnd = true;
    }
    
    bool search(string word) {
        TrieNode* node = searchPrefix(word);
        return node && node->isEnd;
    }
    
    bool startsWith(string prefix) {
        return searchPrefix(prefix) != nullptr;
    }
    
private:
    TrieNode* searchPrefix(string prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            int index = c - 'a';
            if (!node->children[index]) {
                return nullptr;
            }
            node = node->children[index];
        }
        return node;
    }
};
```

### 2. 并查集

**问题**: 实现并查集数据结构

**解答**:
```cpp
class UnionFind {
private:
    vector<int> parent;
    vector<int> rank;
    int count;
    
public:
    UnionFind(int n) : parent(n), rank(n, 0), count(n) {
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);  // 路径压缩
        }
        return parent[x];
    }
    
    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        
        if (rootX != rootY) {
            // 按秩合并
            if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            } else if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            } else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
            count--;
        }
    }
    
    bool connected(int x, int y) {
        return find(x) == find(y);
    }
    
    int getCount() {
        return count;
    }
};
```

## 💡 面试技巧

### 数据结构选择原则

1. **数组**: 需要随机访问、内存紧凑
2. **链表**: 频繁插入删除、大小动态变化
3. **栈**: LIFO操作、递归转迭代、表达式求值
4. **队列**: FIFO操作、BFS、任务调度
5. **哈希表**: 快速查找、计数、去重
6. **堆**: 动态维护最值、优先级队列
7. **树**: 层次关系、搜索、排序
8. **图**: 网络关系、路径问题

### 复杂度分析要点

- **时间复杂度**: 考虑最坏情况，分析循环嵌套
- **空间复杂度**: 考虑递归栈空间、额外数据结构
- **权衡**: 时间换空间 vs 空间换时间

### 编程注意事项

1. **边界条件**: 空输入、单元素、最大值
2. **溢出处理**: 整数溢出、数组越界
3. **内存管理**: 避免内存泄漏、野指针
4. **代码风格**: 清晰命名、适当注释

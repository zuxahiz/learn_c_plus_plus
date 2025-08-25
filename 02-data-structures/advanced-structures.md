# 高级数据结构详解

## 📋 目录
1. [堆数据结构](#堆数据结构)
2. [红黑树](#红黑树)
3. [AVL树（平衡二叉树）](#avl树平衡二叉树)
4. [B树与B+树](#b树与b树)

---

## 堆数据结构

### 🎯 堆的概念

堆是一种特殊的完全二叉树，满足堆性质：
- **最大堆**：父节点的值 ≥ 子节点的值
- **最小堆**：父节点的值 ≤ 子节点的值

### 🔧 堆的性质

1. **完全二叉树**：除了最后一层，其他层都被完全填满
2. **堆序性质**：满足最大堆或最小堆的性质
3. **数组表示**：通常用数组实现，节点i的子节点位于2i+1和2i+2

### 🏗️ 最大堆实现

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

class MaxHeap {
private:
    std::vector<int> heap;
    
    // 获取父节点索引
    int parent(int i) { return (i - 1) / 2; }
    
    // 获取左子节点索引
    int leftChild(int i) { return 2 * i + 1; }
    
    // 获取右子节点索引
    int rightChild(int i) { return 2 * i + 2; }
    
    // 上浮操作（用于插入）
    void heapifyUp(int index) {
        if (index > 0 && heap[parent(index)] < heap[index]) {
            std::swap(heap[index], heap[parent(index)]);
            heapifyUp(parent(index));
        }
    }
    
    // 下沉操作（用于删除）
    void heapifyDown(int index) {
        int largest = index;
        int left = leftChild(index);
        int right = rightChild(index);
        
        if (left < heap.size() && heap[left] > heap[largest]) {
            largest = left;
        }
        
        if (right < heap.size() && heap[right] > heap[largest]) {
            largest = right;
        }
        
        if (largest != index) {
            std::swap(heap[index], heap[largest]);
            heapifyDown(largest);
        }
    }
    
public:
    // 插入元素
    void insert(int value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }
    
    // 删除最大元素
    int extractMax() {
        if (heap.empty()) {
            throw std::runtime_error("Heap is empty");
        }
        
        int max = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        
        if (!heap.empty()) {
            heapifyDown(0);
        }
        
        return max;
    }
    
    // 获取最大元素
    int getMax() const {
        if (heap.empty()) {
            throw std::runtime_error("Heap is empty");
        }
        return heap[0];
    }
    
    // 检查是否为空
    bool empty() const {
        return heap.empty();
    }
    
    // 获取大小
    size_t size() const {
        return heap.size();
    }
    
    // 构建堆（Floyd建堆算法）
    void buildHeap(const std::vector<int>& arr) {
        heap = arr;
        for (int i = heap.size() / 2 - 1; i >= 0; --i) {
            heapifyDown(i);
        }
    }
    
    // 打印堆
    void printHeap() const {
        std::cout << "Max Heap: ";
        for (int val : heap) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
};
```

### 🏗️ 最小堆实现

```cpp
class MinHeap {
private:
    std::vector<int> heap;
    
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }
    
    void heapifyUp(int index) {
        if (index > 0 && heap[parent(index)] > heap[index]) {
            std::swap(heap[index], heap[parent(index)]);
            heapifyUp(parent(index));
        }
    }
    
    void heapifyDown(int index) {
        int smallest = index;
        int left = leftChild(index);
        int right = rightChild(index);
        
        if (left < heap.size() && heap[left] < heap[smallest]) {
            smallest = left;
        }
        
        if (right < heap.size() && heap[right] < heap[smallest]) {
            smallest = right;
        }
        
        if (smallest != index) {
            std::swap(heap[index], heap[smallest]);
            heapifyDown(smallest);
        }
    }
    
public:
    void insert(int value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }
    
    int extractMin() {
        if (heap.empty()) {
            throw std::runtime_error("Heap is empty");
        }
        
        int min = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        
        if (!heap.empty()) {
            heapifyDown(0);
        }
        
        return min;
    }
    
    int getMin() const {
        if (heap.empty()) {
            throw std::runtime_error("Heap is empty");
        }
        return heap[0];
    }
    
    bool empty() const { return heap.empty(); }
    size_t size() const { return heap.size(); }
    
    void printHeap() const {
        std::cout << "Min Heap: ";
        for (int val : heap) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
};
```

### 🎯 堆的应用

#### 优先级队列实现
```cpp
#include <queue>

// 使用STL的优先级队列
void demonstratePriorityQueue() {
    // 默认是最大堆
    std::priority_queue<int> maxPQ;
    maxPQ.push(10);
    maxPQ.push(30);
    maxPQ.push(20);
    
    std::cout << "Max PQ: ";
    while (!maxPQ.empty()) {
        std::cout << maxPQ.top() << " ";
        maxPQ.pop();
    }
    std::cout << std::endl;
    
    // 最小堆
    std::priority_queue<int, std::vector<int>, std::greater<int>> minPQ;
    minPQ.push(10);
    minPQ.push(30);
    minPQ.push(20);
    
    std::cout << "Min PQ: ";
    while (!minPQ.empty()) {
        std::cout << minPQ.top() << " ";
        minPQ.pop();
    }
    std::cout << std::endl;
}
```

#### 堆排序算法
```cpp
class HeapSort {
public:
    static void heapSort(std::vector<int>& arr) {
        // 建堆
        buildMaxHeap(arr);
        
        // 逐个提取最大元素
        for (int i = arr.size() - 1; i > 0; --i) {
            std::swap(arr[0], arr[i]);
            heapifyDown(arr, 0, i);
        }
    }
    
private:
    static void buildMaxHeap(std::vector<int>& arr) {
        for (int i = arr.size() / 2 - 1; i >= 0; --i) {
            heapifyDown(arr, i, arr.size());
        }
    }
    
    static void heapifyDown(std::vector<int>& arr, int index, int heapSize) {
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        
        if (left < heapSize && arr[left] > arr[largest]) {
            largest = left;
        }
        
        if (right < heapSize && arr[right] > arr[largest]) {
            largest = right;
        }
        
        if (largest != index) {
            std::swap(arr[index], arr[largest]);
            heapifyDown(arr, largest, heapSize);
        }
    }
};
```

---

## 红黑树

### 🎯 红黑树的概念

红黑树是一种自平衡的二叉搜索树，每个节点都有一个颜色属性（红色或黑色）。它通过颜色约束来保持树的近似平衡。

### 🔧 红黑树的性质

1. **节点颜色**：每个节点要么是红色，要么是黑色
2. **根节点**：根节点是黑色
3. **叶子节点**：所有叶子节点（NIL节点）都是黑色
4. **红色节点**：红色节点的子节点必须是黑色（不能有连续的红色节点）
5. **黑色高度**：从任一节点到其每个叶子的所有路径都包含相同数目的黑色节点

### 🏗️ 红黑树实现

```cpp
#include <iostream>

enum class Color { RED, BLACK };

template<typename T>
class RedBlackTree {
private:
    struct Node {
        T data;
        Color color;
        Node* left;
        Node* right;
        Node* parent;
        
        Node(T value) : data(value), color(Color::RED), 
                       left(nullptr), right(nullptr), parent(nullptr) {}
    };
    
    Node* root;
    Node* NIL;  // 哨兵节点
    
public:
    RedBlackTree() {
        NIL = new Node(T{});
        NIL->color = Color::BLACK;
        root = NIL;
    }
    
    ~RedBlackTree() {
        destroyTree(root);
        delete NIL;
    }
    
    void insert(T value) {
        Node* newNode = new Node(value);
        newNode->left = NIL;
        newNode->right = NIL;
        
        Node* parent = nullptr;
        Node* current = root;
        
        // 标准BST插入
        while (current != NIL) {
            parent = current;
            if (newNode->data < current->data) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        
        newNode->parent = parent;
        if (parent == nullptr) {
            root = newNode;
        } else if (newNode->data < parent->data) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }
        
        // 新节点为红色，需要修复
        insertFixup(newNode);
    }
    
    bool search(T value) const {
        return searchNode(root, value) != NIL;
    }
    
    void inorderTraversal() const {
        inorderHelper(root);
        std::cout << std::endl;
    }
    
    void printTree() const {
        printHelper(root, "", true);
    }
    
private:
    void insertFixup(Node* node) {
        while (node->parent && node->parent->color == Color::RED) {
            if (node->parent == node->parent->parent->left) {
                Node* uncle = node->parent->parent->right;
                
                if (uncle->color == Color::RED) {
                    // Case 1: 叔叔节点是红色
                    node->parent->color = Color::BLACK;
                    uncle->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->right) {
                        // Case 2: 叔叔是黑色，当前节点是右子节点
                        node = node->parent;
                        leftRotate(node);
                    }
                    // Case 3: 叔叔是黑色，当前节点是左子节点
                    node->parent->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    rightRotate(node->parent->parent);
                }
            } else {
                // 对称情况
                Node* uncle = node->parent->parent->left;
                
                if (uncle->color == Color::RED) {
                    node->parent->color = Color::BLACK;
                    uncle->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->left) {
                        node = node->parent;
                        rightRotate(node);
                    }
                    node->parent->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    leftRotate(node->parent->parent);
                }
            }
        }
        root->color = Color::BLACK;
    }
    
    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        
        if (y->left != NIL) {
            y->left->parent = x;
        }
        
        y->parent = x->parent;
        
        if (x->parent == nullptr) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        
        y->left = x;
        x->parent = y;
    }
    
    void rightRotate(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        
        if (x->right != NIL) {
            x->right->parent = y;
        }
        
        x->parent = y->parent;
        
        if (y->parent == nullptr) {
            root = x;
        } else if (y == y->parent->right) {
            y->parent->right = x;
        } else {
            y->parent->left = x;
        }
        
        x->right = y;
        y->parent = x;
    }
    
    Node* searchNode(Node* node, T value) const {
        if (node == NIL || value == node->data) {
            return node;
        }
        
        if (value < node->data) {
            return searchNode(node->left, value);
        } else {
            return searchNode(node->right, value);
        }
    }
    
    void inorderHelper(Node* node) const {
        if (node != NIL) {
            inorderHelper(node->left);
            std::cout << node->data << "(" << (node->color == Color::RED ? "R" : "B") << ") ";
            inorderHelper(node->right);
        }
    }
    
    void printHelper(Node* node, std::string indent, bool last) const {
        if (node != NIL) {
            std::cout << indent;
            if (last) {
                std::cout << "└──";
                indent += "   ";
            } else {
                std::cout << "├──";
                indent += "│  ";
            }
            
            std::cout << node->data << "(" << (node->color == Color::RED ? "R" : "B") << ")" << std::endl;
            
            printHelper(node->left, indent, false);
            printHelper(node->right, indent, true);
        }
    }
    
    void destroyTree(Node* node) {
        if (node != NIL) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
};
```

### 📊 红黑树的复杂度

| 操作 | 时间复杂度 | 说明 |
|------|------------|------|
| 搜索 | O(log n) | 树高度为O(log n) |
| 插入 | O(log n) | 插入+重新平衡 |
| 删除 | O(log n) | 删除+重新平衡 |
| 遍历 | O(n) | 访问所有节点 |

---

## AVL树（平衡二叉树）

### 🎯 AVL树的概念

AVL树是最早被发明的自平衡二叉搜索树。在AVL树中，任一节点对应的两棵子树的最大高度差为1，因此它也被称为高度平衡树。

### 🔧 AVL树的性质

1. **平衡因子**：每个节点的平衡因子 = 左子树高度 - 右子树高度
2. **平衡条件**：平衡因子只能是 -1、0、1
3. **自动平衡**：插入或删除后自动调整保持平衡
4. **高度限制**：树的高度始终保持在O(log n)

### 🏗️ AVL树实现

```cpp
#include <iostream>
#include <algorithm>

template<typename T>
class AVLTree {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        int height;
        
        Node(T value) : data(value), left(nullptr), right(nullptr), height(1) {}
    };
    
    Node* root;
    
public:
    AVLTree() : root(nullptr) {}
    
    ~AVLTree() {
        destroyTree(root);
    }
    
    void insert(T value) {
        root = insertNode(root, value);
    }
    
    void remove(T value) {
        root = removeNode(root, value);
    }
    
    bool search(T value) const {
        return searchNode(root, value) != nullptr;
    }
    
    void inorderTraversal() const {
        inorderHelper(root);
        std::cout << std::endl;
    }
    
    void printTree() const {
        printHelper(root, "", true);
    }
    
    int getHeight() const {
        return getNodeHeight(root);
    }
    
private:
    int getNodeHeight(Node* node) const {
        return node ? node->height : 0;
    }
    
    int getBalance(Node* node) const {
        return node ? getNodeHeight(node->left) - getNodeHeight(node->right) : 0;
    }
    
    void updateHeight(Node* node) {
        if (node) {
            node->height = 1 + std::max(getNodeHeight(node->left), getNodeHeight(node->right));
        }
    }
    
    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        
        // 执行旋转
        x->right = y;
        y->left = T2;
        
        // 更新高度
        updateHeight(y);
        updateHeight(x);
        
        return x;  // 新的根节点
    }
    
    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        
        // 执行旋转
        y->left = x;
        x->right = T2;
        
        // 更新高度
        updateHeight(x);
        updateHeight(y);
        
        return y;  // 新的根节点
    }
    
    Node* insertNode(Node* node, T value) {
        // 1. 标准BST插入
        if (!node) {
            return new Node(value);
        }
        
        if (value < node->data) {
            node->left = insertNode(node->left, value);
        } else if (value > node->data) {
            node->right = insertNode(node->right, value);
        } else {
            // 重复值，不插入
            return node;
        }
        
        // 2. 更新当前节点的高度
        updateHeight(node);
        
        // 3. 获取平衡因子
        int balance = getBalance(node);
        
        // 4. 如果不平衡，进行旋转
        // Left Left Case
        if (balance > 1 && value < node->left->data) {
            return rightRotate(node);
        }
        
        // Right Right Case
        if (balance < -1 && value > node->right->data) {
            return leftRotate(node);
        }
        
        // Left Right Case
        if (balance > 1 && value > node->left->data) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        
        // Right Left Case
        if (balance < -1 && value < node->right->data) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        
        return node;
    }
    
    Node* removeNode(Node* node, T value) {
        // 1. 标准BST删除
        if (!node) {
            return node;
        }
        
        if (value < node->data) {
            node->left = removeNode(node->left, value);
        } else if (value > node->data) {
            node->right = removeNode(node->right, value);
        } else {
            // 找到要删除的节点
            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;
                
                if (!temp) {
                    // 无子节点
                    temp = node;
                    node = nullptr;
                } else {
                    // 一个子节点
                    *node = *temp;
                }
                delete temp;
            } else {
                // 两个子节点：找到中序后继
                Node* temp = findMin(node->right);
                
                node->data = temp->data;
                node->right = removeNode(node->right, temp->data);
            }
        }
        
        if (!node) return node;
        
        // 2. 更新高度
        updateHeight(node);
        
        // 3. 获取平衡因子
        int balance = getBalance(node);
        
        // 4. 如果不平衡，进行旋转
        // Left Left Case
        if (balance > 1 && getBalance(node->left) >= 0) {
            return rightRotate(node);
        }
        
        // Left Right Case
        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        
        // Right Right Case
        if (balance < -1 && getBalance(node->right) <= 0) {
            return leftRotate(node);
        }
        
        // Right Left Case
        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        
        return node;
    }
    
    Node* findMin(Node* node) const {
        while (node->left) {
            node = node->left;
        }
        return node;
    }
    
    Node* searchNode(Node* node, T value) const {
        if (!node || node->data == value) {
            return node;
        }
        
        if (value < node->data) {
            return searchNode(node->left, value);
        } else {
            return searchNode(node->right, value);
        }
    }
    
    void inorderHelper(Node* node) const {
        if (node) {
            inorderHelper(node->left);
            std::cout << node->data << "(h:" << node->height << ",b:" << getBalance(node) << ") ";
            inorderHelper(node->right);
        }
    }
    
    void printHelper(Node* node, std::string indent, bool last) const {
        if (node) {
            std::cout << indent;
            if (last) {
                std::cout << "└──";
                indent += "   ";
            } else {
                std::cout << "├──";
                indent += "│  ";
            }
            
            std::cout << node->data << "(h:" << node->height << ",b:" << getBalance(node) << ")" << std::endl;
            
            printHelper(node->left, indent, false);
            printHelper(node->right, indent, true);
        }
    }
    
    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
};
```

### 📊 AVL树 vs 红黑树对比

| 特性 | AVL树 | 红黑树 |
|------|-------|--------|
| 平衡性 | 严格平衡 | 近似平衡 |
| 高度限制 | 1.44 log n | 2 log n |
| 插入性能 | 较慢（更多旋转） | 较快 |
| 查询性能 | 更快（更平衡） | 稍慢 |
| 内存开销 | 较少 | 较多（颜色位） |
| 使用场景 | 查询密集 | 插入删除密集 |

---

## B树与B+树

### 🎯 B树的概念

B树是一种自平衡的多路搜索树，主要用于数据库和文件系统。B树的特点是每个节点可以有多个键值和子节点。

### 🔧 B树的性质

1. **阶数**：B树有一个参数t（最小度数），满足：
   - 每个节点最多有2t-1个键
   - 每个内部节点最多有2t个子节点
   - 每个节点（除根节点）最少有t-1个键
   - 每个内部节点（除根节点）最少有t个子节点

2. **有序性**：节点内的键值有序排列

3. **平衡性**：所有叶子节点在同一层

### 🏗️ 简化的B树实现

```cpp
#include <vector>
#include <algorithm>

template<typename T>
class BTree {
private:
    struct Node {
        std::vector<T> keys;
        std::vector<Node*> children;
        bool isLeaf;
        
        Node(bool leaf) : isLeaf(leaf) {}
        
        ~Node() {
            for (Node* child : children) {
                delete child;
            }
        }
    };
    
    Node* root;
    int t;  // 最小度数
    
public:
    BTree(int degree) : root(nullptr), t(degree) {}
    
    ~BTree() {
        delete root;
    }
    
    void insert(T key) {
        if (!root) {
            root = new Node(true);
            root->keys.push_back(key);
            return;
        }
        
        if (root->keys.size() == 2 * t - 1) {
            Node* newRoot = new Node(false);
            newRoot->children.push_back(root);
            splitChild(newRoot, 0);
            root = newRoot;
        }
        
        insertNonFull(root, key);
    }
    
    bool search(T key) const {
        return searchNode(root, key);
    }
    
    void traverse() const {
        if (root) {
            traverseNode(root);
        }
        std::cout << std::endl;
    }
    
private:
    void insertNonFull(Node* node, T key) {
        int i = node->keys.size() - 1;
        
        if (node->isLeaf) {
            node->keys.push_back(T{});
            while (i >= 0 && node->keys[i] > key) {
                node->keys[i + 1] = node->keys[i];
                i--;
            }
            node->keys[i + 1] = key;
        } else {
            while (i >= 0 && node->keys[i] > key) {
                i--;
            }
            i++;
            
            if (node->children[i]->keys.size() == 2 * t - 1) {
                splitChild(node, i);
                if (node->keys[i] < key) {
                    i++;
                }
            }
            insertNonFull(node->children[i], key);
        }
    }
    
    void splitChild(Node* parent, int index) {
        Node* fullChild = parent->children[index];
        Node* newChild = new Node(fullChild->isLeaf);
        
        // 移动后半部分键到新节点
        for (int i = 0; i < t - 1; i++) {
            newChild->keys.push_back(fullChild->keys[i + t]);
        }
        
        // 如果不是叶子节点，移动子节点
        if (!fullChild->isLeaf) {
            for (int i = 0; i < t; i++) {
                newChild->children.push_back(fullChild->children[i + t]);
            }
            fullChild->children.resize(t);
        }
        
        // 调整原节点大小
        T middleKey = fullChild->keys[t - 1];
        fullChild->keys.resize(t - 1);
        
        // 在父节点中插入中间键
        parent->children.insert(parent->children.begin() + index + 1, newChild);
        parent->keys.insert(parent->keys.begin() + index, middleKey);
    }
    
    bool searchNode(Node* node, T key) const {
        if (!node) return false;
        
        int i = 0;
        while (i < node->keys.size() && key > node->keys[i]) {
            i++;
        }
        
        if (i < node->keys.size() && key == node->keys[i]) {
            return true;
        }
        
        if (node->isLeaf) {
            return false;
        }
        
        return searchNode(node->children[i], key);
    }
    
    void traverseNode(Node* node) const {
        int i;
        for (i = 0; i < node->keys.size(); i++) {
            if (!node->isLeaf) {
                traverseNode(node->children[i]);
            }
            std::cout << node->keys[i] << " ";
        }
        
        if (!node->isLeaf) {
            traverseNode(node->children[i]);
        }
    }
};
```

### 🎯 使用示例

```cpp
void demonstrateAdvancedStructures() {
    std::cout << "=== 高级数据结构演示 ===" << std::endl;
    
    // 最大堆演示
    std::cout << "\n--- 最大堆演示 ---" << std::endl;
    MaxHeap maxHeap;
    std::vector<int> data = {4, 10, 3, 5, 1};
    
    for (int val : data) {
        maxHeap.insert(val);
    }
    
    maxHeap.printHeap();
    
    while (!maxHeap.empty()) {
        std::cout << "Extract max: " << maxHeap.extractMax() << std::endl;
    }
    
    // 红黑树演示
    std::cout << "\n--- 红黑树演示 ---" << std::endl;
    RedBlackTree<int> rbTree;
    std::vector<int> rbData = {20, 10, 30, 5, 15, 25, 35};
    
    for (int val : rbData) {
        rbTree.insert(val);
    }
    
    std::cout << "中序遍历: ";
    rbTree.inorderTraversal();
    
    // AVL树演示
    std::cout << "\n--- AVL树演示 ---" << std::endl;
    AVLTree<int> avlTree;
    std::vector<int> avlData = {10, 20, 30, 40, 50, 25};
    
    for (int val : avlData) {
        avlTree.insert(val);
        std::cout << "插入 " << val << " 后的树高度: " << avlTree.getHeight() << std::endl;
    }
    
    std::cout << "中序遍历: ";
    avlTree.inorderTraversal();
    
    // B树演示
    std::cout << "\n--- B树演示 ---" << std::endl;
    BTree<int> btree(3);  // 最小度数为3
    std::vector<int> btreeData = {1, 3, 7, 10, 11, 13, 14, 15, 18, 16, 19, 24, 25, 26, 21, 4, 5, 20, 22, 2, 17, 12, 6};
    
    for (int val : btreeData) {
        btree.insert(val);
    }
    
    std::cout << "B树遍历: ";
    btree.traverse();
}
```

## 📊 性能对比总结

| 数据结构 | 平均查找 | 平均插入 | 平均删除 | 最坏查找 | 空间复杂度 |
|----------|----------|----------|----------|----------|------------|
| 堆 | O(n) | O(log n) | O(log n) | O(n) | O(n) |
| 红黑树 | O(log n) | O(log n) | O(log n) | O(log n) | O(n) |
| AVL树 | O(log n) | O(log n) | O(log n) | O(log n) | O(n) |
| B树 | O(log n) | O(log n) | O(log n) | O(log n) | O(n) |

这些高级数据结构在不同的应用场景中各有优势，选择合适的数据结构对程序性能至关重要！

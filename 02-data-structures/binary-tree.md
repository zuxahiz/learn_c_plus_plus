# C++ 二叉树实现详解

## 🎯 什么是二叉树？

二叉树是一种树形数据结构，其中每个节点最多有两个子节点，分别称为左子节点和右子节点。二叉树是许多高级数据结构的基础，如二叉搜索树、AVL树、红黑树等。

## 🔧 二叉树的基本概念

### 1. 节点结构
- **根节点**: 树的顶层节点
- **内部节点**: 有子节点的节点
- **叶子节点**: 没有子节点的节点
- **父节点**: 有子节点的节点
- **子节点**: 被父节点指向的节点

### 2. 树的属性
- **高度**: 从根到最远叶子节点的路径长度
- **深度**: 从根到某个节点的路径长度
- **度**: 节点的子节点数量
- **层**: 树的层级，根节点在第0层

## 📚 二叉树节点结构

### 基本节点结构
```cpp
#include <iostream>
#include <queue>
#include <stack>
using namespace std;

template<typename T>
struct TreeNode {
    T data;                     // 节点数据
    TreeNode<T>* left;          // 左子节点指针
    TreeNode<T>* right;         // 右子节点指针
    
    // 构造函数
    TreeNode(const T& value) : data(value), left(nullptr), right(nullptr) {}
    
    // 析构函数
    ~TreeNode() {
        // 注意：这里不删除子节点，由树类管理
    }
};
```

## 📚 二叉树类实现

### 基本二叉树类
```cpp
template<typename T>
class BinaryTree {
private:
    TreeNode<T>* root;          // 根节点指针
    size_t size_;               // 节点数量
    
public:
    // 构造函数
    BinaryTree() : root(nullptr), size_(0) {}
    
    // 拷贝构造函数
    BinaryTree(const BinaryTree& other) : root(nullptr), size_(0) {
        root = copyTree(other.root);
        size_ = other.size_;
    }
    
    // 赋值操作符
    BinaryTree& operator=(const BinaryTree& other) {
        if (this != &other) {
            clear();
            root = copyTree(other.root);
            size_ = other.size_;
        }
        return *this;
    }
    
    // 析构函数
    ~BinaryTree() {
        clear();
    }
    
    // 插入节点（按层次顺序）
    void insert(const T& value) {
        if (root == nullptr) {
            root = new TreeNode<T>(value);
            size_ = 1;
            return;
        }
        
        queue<TreeNode<T>*> q;
        q.push(root);
        
        while (!q.empty()) {
            TreeNode<T>* current = q.front();
            q.pop();
            
            if (current->left == nullptr) {
                current->left = new TreeNode<T>(value);
                break;
            } else {
                q.push(current->left);
            }
            
            if (current->right == nullptr) {
                current->right = new TreeNode<T>(value);
                break;
            } else {
                q.push(current->right);
            }
        }
        
        ++size_;
    }
    
    // 删除节点（按值删除）
    bool remove(const T& value) {
        if (root == nullptr) return false;
        
        // 如果根节点是要删除的节点
        if (root->data == value) {
            if (root->left == nullptr && root->right == nullptr) {
                delete root;
                root = nullptr;
                size_ = 0;
                return true;
            } else if (root->left == nullptr) {
                TreeNode<T>* temp = root;
                root = root->right;
                delete temp;
                --size_;
                return true;
            } else if (root->right == nullptr) {
                TreeNode<T>* temp = root;
                root = root->left;
                delete temp;
                --size_;
                return true;
            } else {
                // 找到右子树的最小节点
                TreeNode<T>* minNode = findMin(root->right);
                root->data = minNode->data;
                removeNode(root->right, minNode->data);
                return true;
            }
        }
        
        return removeNode(root, value);
    }
    
    // 查找节点
    TreeNode<T>* find(const T& value) const {
        return findNode(root, value);
    }
    
    // 检查是否为空
    bool empty() const {
        return root == nullptr;
    }
    
    // 获取大小
    size_t size() const {
        return size_;
    }
    
    // 获取高度
    size_t height() const {
        return getHeight(root);
    }
    
    // 清空树
    void clear() {
        clearTree(root);
        root = nullptr;
        size_ = 0;
    }
    
    // 打印树（层次遍历）
    void print() const {
        if (root == nullptr) {
            cout << "Empty tree" << endl;
            return;
        }
        
        queue<TreeNode<T>*> q;
        q.push(root);
        size_t level = 0;
        
        while (!q.empty()) {
            size_t levelSize = q.size();
            cout << "Level " << level << ": ";
            
            for (size_t i = 0; i < levelSize; ++i) {
                TreeNode<T>* current = q.front();
                q.pop();
                
                cout << current->data << " ";
                
                if (current->left) q.push(current->left);
                if (current->right) q.push(current->right);
            }
            
            cout << endl;
            ++level;
        }
    }
    
private:
    // 复制树
    TreeNode<T>* copyTree(TreeNode<T>* node) {
        if (node == nullptr) return nullptr;
        
        TreeNode<T>* newNode = new TreeNode<T>(node->data);
        newNode->left = copyTree(node->left);
        newNode->right = copyTree(node->right);
        
        return newNode;
    }
    
    // 删除节点（递归）
    bool removeNode(TreeNode<T>*& node, const T& value) {
        if (node == nullptr) return false;
        
        if (value < node->data) {
            return removeNode(node->left, value);
        } else if (value > node->data) {
            return removeNode(node->right, value);
        } else {
            // 找到要删除的节点
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                node = nullptr;
                --size_;
                return true;
            } else if (node->left == nullptr) {
                TreeNode<T>* temp = node;
                node = node->right;
                delete temp;
                --size_;
                return true;
            } else if (node->right == nullptr) {
                TreeNode<T>* temp = node;
                node = node->left;
                delete temp;
                --size_;
                return true;
            } else {
                // 有两个子节点，找到右子树的最小节点
                TreeNode<T>* minNode = findMin(node->right);
                node->data = minNode->data;
                removeNode(node->right, minNode->data);
                return true;
            }
        }
    }
    
    // 查找节点（递归）
    TreeNode<T>* findNode(TreeNode<T>* node, const T& value) const {
        if (node == nullptr || node->data == value) {
            return node;
        }
        
        TreeNode<T>* leftResult = findNode(node->left, value);
        if (leftResult) return leftResult;
        
        return findNode(node->right, value);
    }
    
    // 查找最小节点
    TreeNode<T>* findMin(TreeNode<T>* node) const {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }
    
    // 获取高度
    size_t getHeight(TreeNode<T>* node) const {
        if (node == nullptr) return 0;
        
        size_t leftHeight = getHeight(node->left);
        size_t rightHeight = getHeight(node->right);
        
        return max(leftHeight, rightHeight) + 1;
    }
    
    // 清空树
    void clearTree(TreeNode<T>* node) {
        if (node == nullptr) return;
        
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
};
```

## 📚 二叉搜索树 (BST) 实现

### BST类实现
```cpp
template<typename T>
class BinarySearchTree {
private:
    TreeNode<T>* root;
    size_t size_;
    
public:
    // 构造函数
    BinarySearchTree() : root(nullptr), size_(0) {}
    
    // 拷贝构造函数
    BinarySearchTree(const BinarySearchTree& other) : root(nullptr), size_(0) {
        root = copyTree(other.root);
        size_ = other.size_;
    }
    
    // 赋值操作符
    BinarySearchTree& operator=(const BinarySearchTree& other) {
        if (this != &other) {
            clear();
            root = copyTree(other.root);
            size_ = other.size_;
        }
        return *this;
    }
    
    // 析构函数
    ~BinarySearchTree() {
        clear();
    }
    
    // 插入节点
    void insert(const T& value) {
        root = insertNode(root, value);
        ++size_;
    }
    
    // 删除节点
    bool remove(const T& value) {
        if (find(value) == nullptr) return false;
        
        root = removeNode(root, value);
        --size_;
        return true;
    }
    
    // 查找节点
    TreeNode<T>* find(const T& value) const {
        return findNode(root, value);
    }
    
    // 查找最小值
    T findMin() const {
        if (root == nullptr) {
            throw runtime_error("Tree is empty");
        }
        return findMinNode(root)->data;
    }
    
    // 查找最大值
    T findMax() const {
        if (root == nullptr) {
            throw runtime_error("Tree is empty");
        }
        return findMaxNode(root)->data;
    }
    
    // 检查是否为空
    bool empty() const {
        return root == nullptr;
    }
    
    // 获取大小
    size_t size() const {
        return size_;
    }
    
    // 获取高度
    size_t height() const {
        return getHeight(root);
    }
    
    // 清空树
    void clear() {
        clearTree(root);
        root = nullptr;
        size_ = 0;
    }
    
    // 中序遍历（有序输出）
    void inorderTraversal() const {
        cout << "Inorder traversal: ";
        inorder(root);
        cout << endl;
    }
    
    // 前序遍历
    void preorderTraversal() const {
        cout << "Preorder traversal: ";
        preorder(root);
        cout << endl;
    }
    
    // 后序遍历
    void postorderTraversal() const {
        cout << "Postorder traversal: ";
        postorder(root);
        cout << endl;
    }
    
    // 层次遍历
    void levelOrderTraversal() const {
        if (root == nullptr) {
            cout << "Empty tree" << endl;
            return;
        }
        
        queue<TreeNode<T>*> q;
        q.push(root);
        
        cout << "Level order traversal: ";
        while (!q.empty()) {
            TreeNode<T>* current = q.front();
            q.pop();
            
            cout << current->data << " ";
            
            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
        cout << endl;
    }
    
private:
    // 插入节点（递归）
    TreeNode<T>* insertNode(TreeNode<T>* node, const T& value) {
        if (node == nullptr) {
            return new TreeNode<T>(value);
        }
        
        if (value < node->data) {
            node->left = insertNode(node->left, value);
        } else if (value > node->data) {
            node->right = insertNode(node->right, value);
        }
        // 如果值相等，不插入（BST不允许重复值）
        
        return node;
    }
    
    // 删除节点（递归）
    TreeNode<T>* removeNode(TreeNode<T>* node, const T& value) {
        if (node == nullptr) return nullptr;
        
        if (value < node->data) {
            node->left = removeNode(node->left, value);
        } else if (value > node->data) {
            node->right = removeNode(node->right, value);
        } else {
            // 找到要删除的节点
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            } else if (node->left == nullptr) {
                TreeNode<T>* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                TreeNode<T>* temp = node->left;
                delete node;
                return temp;
            } else {
                // 有两个子节点，找到右子树的最小节点
                TreeNode<T>* minNode = findMinNode(node->right);
                node->data = minNode->data;
                node->right = removeNode(node->right, minNode->data);
            }
        }
        
        return node;
    }
    
    // 查找节点（递归）
    TreeNode<T>* findNode(TreeNode<T>* node, const T& value) const {
        if (node == nullptr || node->data == value) {
            return node;
        }
        
        if (value < node->data) {
            return findNode(node->left, value);
        } else {
            return findNode(node->right, value);
        }
    }
    
    // 查找最小节点
    TreeNode<T>* findMinNode(TreeNode<T>* node) const {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }
    
    // 查找最大节点
    TreeNode<T>* findMaxNode(TreeNode<T>* node) const {
        while (node->right != nullptr) {
            node = node->right;
        }
        return node;
    }
    
    // 获取高度
    size_t getHeight(TreeNode<T>* node) const {
        if (node == nullptr) return 0;
        
        size_t leftHeight = getHeight(node->left);
        size_t rightHeight = getHeight(node->right);
        
        return max(leftHeight, rightHeight) + 1;
    }
    
    // 复制树
    TreeNode<T>* copyTree(TreeNode<T>* node) {
        if (node == nullptr) return nullptr;
        
        TreeNode<T>* newNode = new TreeNode<T>(node->data);
        newNode->left = copyTree(node->left);
        newNode->right = copyTree(node->right);
        
        return newNode;
    }
    
    // 清空树
    void clearTree(TreeNode<T>* node) {
        if (node == nullptr) return;
        
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
    
    // 中序遍历
    void inorder(TreeNode<T>* node) const {
        if (node == nullptr) return;
        
        inorder(node->left);
        cout << node->data << " ";
        inorder(node->right);
    }
    
    // 前序遍历
    void preorder(TreeNode<T>* node) const {
        if (node == nullptr) return;
        
        cout << node->data << " ";
        preorder(node->left);
        preorder(node->right);
    }
    
    // 后序遍历
    void postorder(TreeNode<T>* node) const {
        if (node == nullptr) return;
        
        postorder(node->left);
        postorder(node->right);
        cout << node->data << " ";
    }
};
```

## 📚 遍历算法详解

### 1. 深度优先搜索 (DFS)
```cpp
// 递归实现
void dfsRecursive(TreeNode<T>* node) {
    if (node == nullptr) return;
    
    // 前序遍历：根 -> 左 -> 右
    cout << node->data << " ";
    dfsRecursive(node->left);
    dfsRecursive(node->right);
}

// 迭代实现（使用栈）
void dfsIterative(TreeNode<T>* root) {
    if (root == nullptr) return;
    
    stack<TreeNode<T>*> s;
    s.push(root);
    
    while (!s.empty()) {
        TreeNode<T>* current = s.top();
        s.pop();
        
        cout << current->data << " ";
        
        // 先压入右子节点，再压入左子节点（栈的特性）
        if (current->right) s.push(current->right);
        if (current->left) s.push(current->left);
    }
}
```

### 2. 广度优先搜索 (BFS)
```cpp
void bfs(TreeNode<T>* root) {
    if (root == nullptr) return;
    
    queue<TreeNode<T>*> q;
    q.push(root);
    
    while (!q.empty()) {
        TreeNode<T>* current = q.front();
        q.pop();
        
        cout << current->data << " ";
        
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }
}
```

### 3. 中序遍历的迭代实现
```cpp
void inorderIterative(TreeNode<T>* root) {
    if (root == nullptr) return;
    
    stack<TreeNode<T>*> s;
    TreeNode<T>* current = root;
    
    while (current != nullptr || !s.empty()) {
        // 一直向左走到底
        while (current != nullptr) {
            s.push(current);
            current = current->left;
        }
        
        // 弹出栈顶元素并访问
        current = s.top();
        s.pop();
        cout << current->data << " ";
        
        // 转向右子树
        current = current->right;
    }
}
```

## 💡 高级功能实现

### 1. 检查是否为BST
```cpp
bool isBST(TreeNode<T>* root) const {
    return isBSTHelper(root, T(), T());
}

private:
bool isBSTHelper(TreeNode<T>* node, T min, T max) const {
    if (node == nullptr) return true;
    
    if (node->data <= min || node->data >= max) {
        return false;
    }
    
    return isBSTHelper(node->left, min, node->data) &&
           isBSTHelper(node->right, node->data, max);
}
```

### 2. 平衡检查
```cpp
bool isBalanced(TreeNode<T>* root) const {
    return getBalanceHeight(root) != -1;
}

private:
int getBalanceHeight(TreeNode<T>* node) const {
    if (node == nullptr) return 0;
    
    int leftHeight = getBalanceHeight(node->left);
    if (leftHeight == -1) return -1;
    
    int rightHeight = getBalanceHeight(node->right);
    if (rightHeight == -1) return -1;
    
    if (abs(leftHeight - rightHeight) > 1) return -1;
    
    return max(leftHeight, rightHeight) + 1;
}
```

### 3. 最近公共祖先 (LCA)
```cpp
TreeNode<T>* findLCA(TreeNode<T>* root, T value1, T value2) const {
    if (root == nullptr) return nullptr;
    
    if (root->data == value1 || root->data == value2) {
        return root;
    }
    
    TreeNode<T>* leftLCA = findLCA(root->left, value1, value2);
    TreeNode<T>* rightLCA = findLCA(root->right, value1, value2);
    
    if (leftLCA && rightLCA) {
        return root;
    }
    
    return leftLCA ? leftLCA : rightLCA;
}
```

## 🎯 实际应用示例

### 1. 表达式树求值
```cpp
// 假设节点存储操作符或操作数
double evaluateExpressionTree(TreeNode<string>* root) {
    if (root == nullptr) return 0;
    
    // 如果是叶子节点（操作数）
    if (root->left == nullptr && root->right == nullptr) {
        return stod(root->data);
    }
    
    // 递归计算左右子树
    double leftVal = evaluateExpressionTree(root->left);
    double rightVal = evaluateExpressionTree(root->right);
    
    // 根据操作符计算结果
    if (root->data == "+") return leftVal + rightVal;
    if (root->data == "-") return leftVal - rightVal;
    if (root->data == "*") return leftVal * rightVal;
    if (root->data == "/") return leftVal / rightVal;
    
    return 0;
}
```

### 2. 序列化和反序列化
```cpp
// 序列化（前序遍历）
string serialize(TreeNode<T>* root) {
    if (root == nullptr) return "#";
    
    return to_string(root->data) + "," + 
           serialize(root->left) + "," + 
           serialize(root->right);
}

// 反序列化
TreeNode<T>* deserialize(string& data) {
    if (data.empty()) return nullptr;
    
    size_t pos = data.find(',');
    string val = data.substr(0, pos);
    data = data.substr(pos + 1);
    
    if (val == "#") return nullptr;
    
    TreeNode<T>* root = new TreeNode<T>(stoi(val));
    root->left = deserialize(data);
    root->right = deserialize(data);
    
    return root;
}
```

## ⚡ 性能分析

### 时间复杂度
- **插入**: O(h) - h为树的高度
- **删除**: O(h)
- **查找**: O(h)
- **遍历**: O(n) - n为节点数量

### 空间复杂度
- **存储**: O(n)
- **递归调用栈**: O(h)
- **迭代遍历**: O(w) - w为树的最大宽度

### 优化建议
1. **平衡树**: 使用AVL树或红黑树保持平衡
2. **缓存友好**: 对于大数据集，考虑使用B树
3. **内存管理**: 使用对象池减少内存分配开销

## 🚀 下一步
- 学习 [堆实现](./heap.md)
- 了解 [哈希表实现](./hash-table.md)
- 掌握 [图实现](./graph.md)

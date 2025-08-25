# 搜索算法详解

## 🎯 搜索算法概述

搜索算法是计算机科学中用于在数据结构中查找特定元素的算法。根据数据的组织方式和搜索策略的不同，搜索算法可以分为多种类型。

### 搜索算法分类
1. **线性搜索**: 逐一检查每个元素
2. **二分搜索**: 基于有序数据的分治搜索
3. **哈希搜索**: 基于哈希表的直接定位
4. **树搜索**: 基于树结构的搜索
5. **图搜索**: 在图结构中的路径搜索

### 评价指标
- **时间复杂度**: 搜索操作的时间开销
- **空间复杂度**: 额外存储空间的需求
- **适用性**: 对数据结构和数据特征的要求

## 📚 1. 线性搜索算法

### 1.1 顺序搜索 (Sequential Search)

**算法思想**: 从数据结构的第一个元素开始，逐个比较直到找到目标元素或遍历完所有元素。

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <optional>
#include <chrono>

template<typename T>
class LinearSearch {
public:
    // 基础线性搜索
    static int basicLinearSearch(const std::vector<T>& arr, const T& target) {
        for (int i = 0; i < arr.size(); i++) {
            if (arr[i] == target) {
                return i;
            }
        }
        return -1; // 未找到
    }
    
    // 使用迭代器的线性搜索
    template<typename Iterator>
    static Iterator linearSearchIterator(Iterator begin, Iterator end, const T& target) {
        return std::find(begin, end, target);
    }
    
    // 带谓词的线性搜索
    template<typename Predicate>
    static int linearSearchWithPredicate(const std::vector<T>& arr, Predicate pred) {
        for (int i = 0; i < arr.size(); i++) {
            if (pred(arr[i])) {
                return i;
            }
        }
        return -1;
    }
    
    // 查找所有匹配元素
    static std::vector<int> findAllOccurrences(const std::vector<T>& arr, const T& target) {
        std::vector<int> indices;
        
        for (int i = 0; i < arr.size(); i++) {
            if (arr[i] == target) {
                indices.push_back(i);
            }
        }
        
        return indices;
    }
    
    // 哨兵搜索（减少比较次数）
    static int sentinelSearch(std::vector<T>& arr, const T& target) {
        if (arr.empty()) return -1;
        
        int n = arr.size();
        T last = arr[n - 1];
        arr[n - 1] = target; // 设置哨兵
        
        int i = 0;
        while (arr[i] != target) {
            i++;
        }
        
        arr[n - 1] = last; // 恢复原值
        
        if (i < n - 1 || arr[n - 1] == target) {
            return i;
        }
        
        return -1;
    }
    
    // 递归线性搜索
    static int recursiveLinearSearch(const std::vector<T>& arr, const T& target, int index = 0) {
        if (index >= arr.size()) {
            return -1;
        }
        
        if (arr[index] == target) {
            return index;
        }
        
        return recursiveLinearSearch(arr, target, index + 1);
    }
    
    // 双向搜索
    static int bidirectionalSearch(const std::vector<T>& arr, const T& target) {
        int left = 0;
        int right = arr.size() - 1;
        
        while (left <= right) {
            if (arr[left] == target) {
                return left;
            }
            if (arr[right] == target) {
                return right;
            }
            
            left++;
            right--;
        }
        
        return -1;
    }
    
    // 统计元素出现次数
    static int countOccurrences(const std::vector<T>& arr, const T& target) {
        int count = 0;
        for (const T& element : arr) {
            if (element == target) {
                count++;
            }
        }
        return count;
    }
};
```

**复杂度分析**:
- 时间复杂度: O(n)
- 空间复杂度: O(1)
- 适用场景: 无序数据、小数据集

### 1.2 跳跃搜索 (Jump Search)

**算法思想**: 通过固定步长跳跃来减少比较次数，然后在确定的区间内进行线性搜索。

```cpp
template<typename T>
class JumpSearch {
public:
    // 基础跳跃搜索
    static int jumpSearch(const std::vector<T>& arr, const T& target) {
        int n = arr.size();
        int step = std::sqrt(n);
        int prev = 0;
        
        // 跳跃查找
        while (arr[std::min(step, n) - 1] < target) {
            prev = step;
            step += std::sqrt(n);
            if (prev >= n) {
                return -1;
            }
        }
        
        // 线性搜索
        while (arr[prev] < target) {
            prev++;
            if (prev == std::min(step, n)) {
                return -1;
            }
        }
        
        if (arr[prev] == target) {
            return prev;
        }
        
        return -1;
    }
    
    // 自适应步长跳跃搜索
    static int adaptiveJumpSearch(const std::vector<T>& arr, const T& target) {
        int n = arr.size();
        int step = 1;
        int prev = 0;
        
        // 指数增长查找区间
        while (step < n && arr[step - 1] < target) {
            prev = step;
            step *= 2;
        }
        
        // 在确定区间内二分搜索
        int end = std::min(step, n);
        return binarySearchInRange(arr, target, prev, end - 1);
    }
    
    // 改进的跳跃搜索（使用不同步长策略）
    static int improvedJumpSearch(const std::vector<T>& arr, const T& target) {
        int n = arr.size();
        
        // 使用黄金比例作为步长因子
        double phi = (1 + std::sqrt(5)) / 2;
        int step = static_cast<int>(n / phi);
        int prev = 0;
        
        while (arr[std::min(step, n) - 1] < target) {
            prev = step;
            step = static_cast<int>(step * phi);
            if (prev >= n) {
                return -1;
            }
        }
        
        // 在区间内进行优化的线性搜索
        return optimizedLinearSearch(arr, target, prev, std::min(step, n));
    }
    
private:
    static int binarySearchInRange(const std::vector<T>& arr, const T& target, int left, int right) {
        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            if (arr[mid] == target) {
                return mid;
            } else if (arr[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return -1;
    }
    
    static int optimizedLinearSearch(const std::vector<T>& arr, const T& target, int start, int end) {
        for (int i = start; i < end; i++) {
            if (arr[i] == target) {
                return i;
            }
            if (arr[i] > target) {
                break; // 提前终止
            }
        }
        return -1;
    }
};
```

**复杂度分析**:
- 时间复杂度: O(√n)
- 空间复杂度: O(1)
- 适用场景: 有序数组，介于线性和二分搜索之间

## 📚 2. 二分搜索算法

### 2.1 标准二分搜索 (Binary Search)

**算法思想**: 在有序数组中，每次比较中间元素，根据比较结果缩小搜索范围。

```cpp
template<typename T>
class BinarySearch {
public:
    // 基础二分搜索（递归版本）
    static int recursiveBinarySearch(const std::vector<T>& arr, const T& target, int left, int right) {
        if (left > right) {
            return -1;
        }
        
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            return recursiveBinarySearch(arr, target, mid + 1, right);
        } else {
            return recursiveBinarySearch(arr, target, left, mid - 1);
        }
    }
    
    // 二分搜索（迭代版本）
    static int iterativeBinarySearch(const std::vector<T>& arr, const T& target) {
        int left = 0;
        int right = arr.size() - 1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            if (arr[mid] == target) {
                return mid;
            } else if (arr[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return -1;
    }
    
    // 查找第一个出现位置
    static int findFirst(const std::vector<T>& arr, const T& target) {
        int left = 0;
        int right = arr.size() - 1;
        int result = -1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            if (arr[mid] == target) {
                result = mid;
                right = mid - 1; // 继续在左半部分搜索
            } else if (arr[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return result;
    }
    
    // 查找最后一个出现位置
    static int findLast(const std::vector<T>& arr, const T& target) {
        int left = 0;
        int right = arr.size() - 1;
        int result = -1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            if (arr[mid] == target) {
                result = mid;
                left = mid + 1; // 继续在右半部分搜索
            } else if (arr[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return result;
    }
    
    // 查找插入位置（lower_bound）
    static int lowerBound(const std::vector<T>& arr, const T& target) {
        int left = 0;
        int right = arr.size();
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            
            if (arr[mid] < target) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        
        return left;
    }
    
    // 查找上界（upper_bound）
    static int upperBound(const std::vector<T>& arr, const T& target) {
        int left = 0;
        int right = arr.size();
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            
            if (arr[mid] <= target) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        
        return left;
    }
    
    // 统计目标元素出现次数
    static int countOccurrences(const std::vector<T>& arr, const T& target) {
        int first = findFirst(arr, target);
        if (first == -1) {
            return 0;
        }
        
        int last = findLast(arr, target);
        return last - first + 1;
    }
    
    // 二分搜索变种：找到峰值元素
    static int findPeakElement(const std::vector<T>& arr) {
        int left = 0;
        int right = arr.size() - 1;
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            
            if (arr[mid] < arr[mid + 1]) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        
        return left;
    }
    
    // 旋转数组中的搜索
    static int searchInRotatedArray(const std::vector<T>& arr, const T& target) {
        int left = 0;
        int right = arr.size() - 1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            if (arr[mid] == target) {
                return mid;
            }
            
            // 判断哪一半是有序的
            if (arr[left] <= arr[mid]) {
                // 左半部分有序
                if (target >= arr[left] && target < arr[mid]) {
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            } else {
                // 右半部分有序
                if (target > arr[mid] && target <= arr[right]) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
        }
        
        return -1;
    }
};
```

### 2.2 插值搜索 (Interpolation Search)

**算法思想**: 根据目标值与数组边界值的比例来估算目标位置，适用于均匀分布的有序数组。

```cpp
template<typename T>
class InterpolationSearch {
public:
    // 基础插值搜索
    static int interpolationSearch(const std::vector<T>& arr, const T& target) {
        int left = 0;
        int right = arr.size() - 1;
        
        while (left <= right && target >= arr[left] && target <= arr[right]) {
            // 避免除零
            if (arr[left] == arr[right]) {
                return (arr[left] == target) ? left : -1;
            }
            
            // 插值公式
            int pos = left + ((double)(target - arr[left]) / (arr[right] - arr[left])) * (right - left);
            
            if (arr[pos] == target) {
                return pos;
            } else if (arr[pos] < target) {
                left = pos + 1;
            } else {
                right = pos - 1;
            }
        }
        
        return -1;
    }
    
    // 改进的插值搜索（混合二分搜索）
    static int hybridInterpolationSearch(const std::vector<T>& arr, const T& target) {
        int left = 0;
        int right = arr.size() - 1;
        int iterations = 0;
        const int MAX_INTERPOLATION_ITERATIONS = 10;
        
        while (left <= right && target >= arr[left] && target <= arr[right]) {
            if (arr[left] == arr[right]) {
                return (arr[left] == target) ? left : -1;
            }
            
            int pos;
            
            // 前几次使用插值搜索
            if (iterations < MAX_INTERPOLATION_ITERATIONS) {
                pos = left + ((double)(target - arr[left]) / (arr[right] - arr[left])) * (right - left);
                iterations++;
            } else {
                // 之后使用二分搜索
                pos = left + (right - left) / 2;
            }
            
            if (arr[pos] == target) {
                return pos;
            } else if (arr[pos] < target) {
                left = pos + 1;
            } else {
                right = pos - 1;
            }
        }
        
        return -1;
    }
    
    // 递归插值搜索
    static int recursiveInterpolationSearch(const std::vector<T>& arr, const T& target, int left, int right) {
        if (left > right || target < arr[left] || target > arr[right]) {
            return -1;
        }
        
        if (arr[left] == arr[right]) {
            return (arr[left] == target) ? left : -1;
        }
        
        int pos = left + ((double)(target - arr[left]) / (arr[right] - arr[left])) * (right - left);
        
        if (arr[pos] == target) {
            return pos;
        } else if (arr[pos] < target) {
            return recursiveInterpolationSearch(arr, target, pos + 1, right);
        } else {
            return recursiveInterpolationSearch(arr, target, left, pos - 1);
        }
    }
};
```

**复杂度分析**:
- 时间复杂度: O(log log n) 均匀分布时，O(n) 最坏情况
- 空间复杂度: O(1)
- 适用场景: 均匀分布的大型有序数组

### 2.3 指数搜索 (Exponential Search)

**算法思想**: 先用指数增长找到搜索范围，然后在该范围内进行二分搜索。

```cpp
template<typename T>
class ExponentialSearch {
public:
    // 基础指数搜索
    static int exponentialSearch(const std::vector<T>& arr, const T& target) {
        if (arr.empty()) return -1;
        
        // 如果目标在第一个位置
        if (arr[0] == target) {
            return 0;
        }
        
        // 找到搜索范围
        int bound = 1;
        while (bound < arr.size() && arr[bound] < target) {
            bound *= 2;
        }
        
        // 在找到的范围内进行二分搜索
        int left = bound / 2;
        int right = std::min(bound, static_cast<int>(arr.size()) - 1);
        
        return binarySearch(arr, target, left, right);
    }
    
    // 无界数组的指数搜索
    static int unboundedExponentialSearch(const std::vector<T>& arr, const T& target) {
        if (arr.empty()) return -1;
        
        if (arr[0] == target) {
            return 0;
        }
        
        int bound = 1;
        
        // 找到上界，处理可能的数组边界
        try {
            while (bound < arr.size() && arr[bound] < target) {
                bound *= 2;
            }
        } catch (...) {
            // 如果访问越界，使用当前bound作为上界
        }
        
        int left = bound / 2;
        int right = std::min(bound, static_cast<int>(arr.size()) - 1);
        
        return binarySearch(arr, target, left, right);
    }
    
    // 双端指数搜索
    static int bidirectionalExponentialSearch(const std::vector<T>& arr, const T& target) {
        int n = arr.size();
        if (n == 0) return -1;
        
        int mid = n / 2;
        
        if (arr[mid] == target) {
            return mid;
        }
        
        if (target < arr[mid]) {
            // 在左半部分使用指数搜索
            return exponentialSearchInRange(arr, target, 0, mid - 1);
        } else {
            // 在右半部分使用指数搜索
            return exponentialSearchInRange(arr, target, mid + 1, n - 1);
        }
    }
    
private:
    static int binarySearch(const std::vector<T>& arr, const T& target, int left, int right) {
        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            if (arr[mid] == target) {
                return mid;
            } else if (arr[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return -1;
    }
    
    static int exponentialSearchInRange(const std::vector<T>& arr, const T& target, int start, int end) {
        if (start > end) return -1;
        
        int bound = 1;
        while (start + bound <= end && arr[start + bound] < target) {
            bound *= 2;
        }
        
        int left = start + bound / 2;
        int right = std::min(start + bound, end);
        
        return binarySearch(arr, target, left, right);
    }
};
```

**复杂度分析**:
- 时间复杂度: O(log n)
- 空间复杂度: O(1)
- 适用场景: 无界或大型有序数组

## 📚 3. 树搜索算法

### 3.1 二叉搜索树搜索

**算法思想**: 利用二叉搜索树的性质，左子树小于根节点，右子树大于根节点。

```cpp
template<typename T>
struct TreeNode {
    T data;
    TreeNode* left;
    TreeNode* right;
    
    TreeNode(const T& value) : data(value), left(nullptr), right(nullptr) {}
};

template<typename T>
class BinarySearchTree {
private:
    TreeNode<T>* root;
    
public:
    BinarySearchTree() : root(nullptr) {}
    
    // 搜索操作（递归）
    bool searchRecursive(const T& target) {
        return searchRecursiveHelper(root, target);
    }
    
    // 搜索操作（迭代）
    bool searchIterative(const T& target) {
        TreeNode<T>* current = root;
        
        while (current != nullptr) {
            if (current->data == target) {
                return true;
            } else if (target < current->data) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        
        return false;
    }
    
    // 查找最小值
    TreeNode<T>* findMin() {
        if (root == nullptr) return nullptr;
        
        TreeNode<T>* current = root;
        while (current->left != nullptr) {
            current = current->left;
        }
        
        return current;
    }
    
    // 查找最大值
    TreeNode<T>* findMax() {
        if (root == nullptr) return nullptr;
        
        TreeNode<T>* current = root;
        while (current->right != nullptr) {
            current = current->right;
        }
        
        return current;
    }
    
    // 查找前驱节点
    TreeNode<T>* findPredecessor(const T& target) {
        TreeNode<T>* current = root;
        TreeNode<T>* predecessor = nullptr;
        
        while (current != nullptr) {
            if (target > current->data) {
                predecessor = current;
                current = current->right;
            } else {
                current = current->left;
            }
        }
        
        return predecessor;
    }
    
    // 查找后继节点
    TreeNode<T>* findSuccessor(const T& target) {
        TreeNode<T>* current = root;
        TreeNode<T>* successor = nullptr;
        
        while (current != nullptr) {
            if (target < current->data) {
                successor = current;
                current = current->left;
            } else {
                current = current->right;
            }
        }
        
        return successor;
    }
    
    // 范围搜索
    std::vector<T> rangeSearch(const T& min, const T& max) {
        std::vector<T> result;
        rangeSearchHelper(root, min, max, result);
        return result;
    }
    
    // K最近邻搜索
    std::vector<T> kNearestNeighbors(const T& target, int k) {
        std::priority_queue<std::pair<T, T>, std::vector<std::pair<T, T>>, 
                           std::function<bool(const std::pair<T, T>&, const std::pair<T, T>&)>>
                           pq([](const std::pair<T, T>& a, const std::pair<T, T>& b) {
                               return std::abs(a.second - a.first) < std::abs(b.second - b.first);
                           });
        
        kNearestHelper(root, target, k, pq);
        
        std::vector<T> result;
        while (!pq.empty()) {
            result.push_back(pq.top().second);
            pq.pop();
        }
        
        std::reverse(result.begin(), result.end());
        return result;
    }
    
    // 插入操作
    void insert(const T& value) {
        root = insertHelper(root, value);
    }
    
private:
    bool searchRecursiveHelper(TreeNode<T>* node, const T& target) {
        if (node == nullptr) {
            return false;
        }
        
        if (node->data == target) {
            return true;
        } else if (target < node->data) {
            return searchRecursiveHelper(node->left, target);
        } else {
            return searchRecursiveHelper(node->right, target);
        }
    }
    
    void rangeSearchHelper(TreeNode<T>* node, const T& min, const T& max, std::vector<T>& result) {
        if (node == nullptr) return;
        
        if (node->data >= min && node->data <= max) {
            result.push_back(node->data);
        }
        
        if (node->data > min) {
            rangeSearchHelper(node->left, min, max, result);
        }
        
        if (node->data < max) {
            rangeSearchHelper(node->right, min, max, result);
        }
    }
    
    void kNearestHelper(TreeNode<T>* node, const T& target, int k,
                       std::priority_queue<std::pair<T, T>, std::vector<std::pair<T, T>>,
                                          std::function<bool(const std::pair<T, T>&, const std::pair<T, T>&)>>& pq) {
        if (node == nullptr) return;
        
        T distance = std::abs(node->data - target);
        
        if (pq.size() < k) {
            pq.push({target, node->data});
        } else if (distance < std::abs(pq.top().second - pq.top().first)) {
            pq.pop();
            pq.push({target, node->data});
        }
        
        // 决定搜索哪个子树
        if (target < node->data) {
            kNearestHelper(node->left, target, k, pq);
            if (pq.size() < k || distance < std::abs(pq.top().second - pq.top().first)) {
                kNearestHelper(node->right, target, k, pq);
            }
        } else {
            kNearestHelper(node->right, target, k, pq);
            if (pq.size() < k || distance < std::abs(pq.top().second - pq.top().first)) {
                kNearestHelper(node->left, target, k, pq);
            }
        }
    }
    
    TreeNode<T>* insertHelper(TreeNode<T>* node, const T& value) {
        if (node == nullptr) {
            return new TreeNode<T>(value);
        }
        
        if (value < node->data) {
            node->left = insertHelper(node->left, value);
        } else if (value > node->data) {
            node->right = insertHelper(node->right, value);
        }
        
        return node;
    }
};
```

### 3.2 平衡搜索树

```cpp
// AVL树的简化搜索实现
template<typename T>
class AVLTree {
private:
    struct AVLNode {
        T data;
        AVLNode* left;
        AVLNode* right;
        int height;
        
        AVLNode(const T& value) : data(value), left(nullptr), right(nullptr), height(1) {}
    };
    
    AVLNode* root;
    
    int getHeight(AVLNode* node) {
        return node ? node->height : 0;
    }
    
    int getBalance(AVLNode* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }
    
public:
    AVLTree() : root(nullptr) {}
    
    // AVL树搜索（与BST相同，但保证平衡）
    bool search(const T& target) {
        AVLNode* current = root;
        
        while (current != nullptr) {
            if (current->data == target) {
                return true;
            } else if (target < current->data) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        
        return false;
    }
    
    // 获取树的高度
    int getTreeHeight() {
        return getHeight(root);
    }
    
    // 验证搜索效率
    int searchDepth(const T& target) {
        AVLNode* current = root;
        int depth = 0;
        
        while (current != nullptr) {
            depth++;
            if (current->data == target) {
                return depth;
            } else if (target < current->data) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        
        return -1; // 未找到
    }
};
```

## 📚 4. 哈希搜索算法

### 4.1 哈希表搜索

**算法思想**: 通过哈希函数直接计算元素位置，实现常数时间搜索。

```cpp
template<typename Key, typename Value>
class HashTable {
private:
    struct HashNode {
        Key key;
        Value value;
        HashNode* next;
        
        HashNode(const Key& k, const Value& v) : key(k), value(v), next(nullptr) {}
    };
    
    std::vector<HashNode*> table;
    size_t tableSize;
    size_t numElements;
    
    size_t hashFunction(const Key& key) const {
        return std::hash<Key>{}(key) % tableSize;
    }
    
public:
    HashTable(size_t size = 101) : tableSize(size), numElements(0) {
        table.resize(tableSize, nullptr);
    }
    
    // 搜索操作
    bool search(const Key& key, Value& value) {
        size_t index = hashFunction(key);
        HashNode* current = table[index];
        
        while (current != nullptr) {
            if (current->key == key) {
                value = current->value;
                return true;
            }
            current = current->next;
        }
        
        return false;
    }
    
    // 快速存在性检查
    bool contains(const Key& key) {
        size_t index = hashFunction(key);
        HashNode* current = table[index];
        
        while (current != nullptr) {
            if (current->key == key) {
                return true;
            }
            current = current->next;
        }
        
        return false;
    }
    
    // 插入操作
    void insert(const Key& key, const Value& value) {
        size_t index = hashFunction(key);
        HashNode* current = table[index];
        
        // 检查是否已存在
        while (current != nullptr) {
            if (current->key == key) {
                current->value = value; // 更新值
                return;
            }
            current = current->next;
        }
        
        // 插入新节点
        HashNode* newNode = new HashNode(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        numElements++;
    }
    
    // 获取所有匹配的键
    std::vector<Key> searchByValue(const Value& value) {
        std::vector<Key> result;
        
        for (size_t i = 0; i < tableSize; i++) {
            HashNode* current = table[i];
            while (current != nullptr) {
                if (current->value == value) {
                    result.push_back(current->key);
                }
                current = current->next;
            }
        }
        
        return result;
    }
    
    // 前缀搜索（对字符串键）
    std::vector<std::pair<Key, Value>> prefixSearch(const std::string& prefix) {
        std::vector<std::pair<Key, Value>> result;
        
        for (size_t i = 0; i < tableSize; i++) {
            HashNode* current = table[i];
            while (current != nullptr) {
                std::string keyStr = std::to_string(current->key);
                if (keyStr.substr(0, prefix.length()) == prefix) {
                    result.push_back({current->key, current->value});
                }
                current = current->next;
            }
        }
        
        return result;
    }
    
    // 性能统计
    void printStats() {
        size_t maxChainLength = 0;
        size_t nonEmptyBuckets = 0;
        size_t totalChainLength = 0;
        
        for (size_t i = 0; i < tableSize; i++) {
            size_t chainLength = 0;
            HashNode* current = table[i];
            
            while (current != nullptr) {
                chainLength++;
                current = current->next;
            }
            
            if (chainLength > 0) {
                nonEmptyBuckets++;
                totalChainLength += chainLength;
                maxChainLength = std::max(maxChainLength, chainLength);
            }
        }
        
        std::cout << "哈希表统计信息:" << std::endl;
        std::cout << "  表大小: " << tableSize << std::endl;
        std::cout << "  元素数量: " << numElements << std::endl;
        std::cout << "  负载因子: " << static_cast<double>(numElements) / tableSize << std::endl;
        std::cout << "  非空桶数: " << nonEmptyBuckets << std::endl;
        std::cout << "  最大链长: " << maxChainLength << std::endl;
        std::cout << "  平均链长: " << (nonEmptyBuckets > 0 ? static_cast<double>(totalChainLength) / nonEmptyBuckets : 0) << std::endl;
    }
};
```

### 4.2 布隆过滤器搜索

```cpp
class BloomFilter {
private:
    std::vector<bool> bitArray;
    size_t size;
    size_t numHashFunctions;
    size_t numElements;
    
    std::vector<std::function<size_t(const std::string&)>> hashFunctions;
    
    void initializeHashFunctions() {
        // 实现多个独立的哈希函数
        hashFunctions.push_back([this](const std::string& str) {
            size_t hash = 5381;
            for (char c : str) {
                hash = ((hash << 5) + hash) + c;
            }
            return hash % size;
        });
        
        hashFunctions.push_back([this](const std::string& str) {
            size_t hash = 0;
            for (char c : str) {
                hash = c + (hash << 6) + (hash << 16) - hash;
            }
            return hash % size;
        });
        
        hashFunctions.push_back([this](const std::string& str) {
            const size_t FNV_PRIME = 1099511628211ULL;
            const size_t FNV_OFFSET_BASIS = 14695981039346656037ULL;
            
            size_t hash = FNV_OFFSET_BASIS;
            for (char c : str) {
                hash ^= c;
                hash *= FNV_PRIME;
            }
            return hash % size;
        });
    }
    
public:
    BloomFilter(size_t bitArraySize, size_t numHashes) 
        : size(bitArraySize), numHashFunctions(numHashes), numElements(0) {
        bitArray.resize(size, false);
        initializeHashFunctions();
    }
    
    // 添加元素
    void add(const std::string& item) {
        for (size_t i = 0; i < numHashFunctions && i < hashFunctions.size(); i++) {
            size_t index = hashFunctions[i](item);
            bitArray[index] = true;
        }
        numElements++;
    }
    
    // 搜索元素（可能存在）
    bool mightContain(const std::string& item) const {
        for (size_t i = 0; i < numHashFunctions && i < hashFunctions.size(); i++) {
            size_t index = hashFunctions[i](item);
            if (!bitArray[index]) {
                return false; // 肯定不存在
            }
        }
        return true; // 可能存在
    }
    
    // 计算误判率
    double getFalsePositiveRate() const {
        if (numElements == 0) return 0.0;
        
        double bitsPerElement = static_cast<double>(size) / numElements;
        double exponent = -numHashFunctions / bitsPerElement;
        return std::pow(1.0 - std::exp(exponent), numHashFunctions);
    }
    
    // 估算包含的元素数量
    size_t estimateElementCount() const {
        size_t setBits = 0;
        for (bool bit : bitArray) {
            if (bit) setBits++;
        }
        
        if (setBits == 0) return 0;
        if (setBits == size) return SIZE_MAX; // 饱和
        
        double ratio = static_cast<double>(setBits) / size;
        return static_cast<size_t>(-size * std::log(1 - ratio) / numHashFunctions);
    }
};
```

## 📚 5. 特殊搜索算法

### 5.1 子串搜索算法

#### KMP算法 (Knuth-Morris-Pratt)

```cpp
class StringSearch {
public:
    // KMP算法的前缀函数
    static std::vector<int> computeLPS(const std::string& pattern) {
        int m = pattern.length();
        std::vector<int> lps(m, 0);
        int len = 0;
        int i = 1;
        
        while (i < m) {
            if (pattern[i] == pattern[len]) {
                len++;
                lps[i] = len;
                i++;
            } else {
                if (len != 0) {
                    len = lps[len - 1];
                } else {
                    lps[i] = 0;
                    i++;
                }
            }
        }
        
        return lps;
    }
    
    // KMP搜索
    static std::vector<int> KMPSearch(const std::string& text, const std::string& pattern) {
        std::vector<int> result;
        int n = text.length();
        int m = pattern.length();
        
        if (m == 0) return result;
        
        std::vector<int> lps = computeLPS(pattern);
        
        int i = 0; // text的索引
        int j = 0; // pattern的索引
        
        while (i < n) {
            if (pattern[j] == text[i]) {
                i++;
                j++;
            }
            
            if (j == m) {
                result.push_back(i - j);
                j = lps[j - 1];
            } else if (i < n && pattern[j] != text[i]) {
                if (j != 0) {
                    j = lps[j - 1];
                } else {
                    i++;
                }
            }
        }
        
        return result;
    }
    
    // Boyer-Moore算法（简化版）
    static std::vector<int> boyerMooreSearch(const std::string& text, const std::string& pattern) {
        std::vector<int> result;
        int n = text.length();
        int m = pattern.length();
        
        if (m == 0) return result;
        
        // 坏字符表
        std::unordered_map<char, int> badChar;
        for (int i = 0; i < m; i++) {
            badChar[pattern[i]] = i;
        }
        
        int shift = 0;
        while (shift <= n - m) {
            int j = m - 1;
            
            while (j >= 0 && pattern[j] == text[shift + j]) {
                j--;
            }
            
            if (j < 0) {
                result.push_back(shift);
                shift += (shift + m < n) ? m - badChar[text[shift + m]] : 1;
            } else {
                shift += std::max(1, j - badChar[text[shift + j]]);
            }
        }
        
        return result;
    }
    
    // Rabin-Karp算法
    static std::vector<int> rabinKarpSearch(const std::string& text, const std::string& pattern) {
        std::vector<int> result;
        int n = text.length();
        int m = pattern.length();
        
        if (m == 0) return result;
        
        const int BASE = 256;
        const int MOD = 101;
        
        int patternHash = 0;
        int textHash = 0;
        int h = 1;
        
        // 计算h = pow(BASE, m-1) % MOD
        for (int i = 0; i < m - 1; i++) {
            h = (h * BASE) % MOD;
        }
        
        // 计算模式串和文本第一个窗口的哈希值
        for (int i = 0; i < m; i++) {
            patternHash = (BASE * patternHash + pattern[i]) % MOD;
            textHash = (BASE * textHash + text[i]) % MOD;
        }
        
        // 滑动窗口
        for (int i = 0; i <= n - m; i++) {
            if (patternHash == textHash) {
                // 哈希值相等，验证字符串
                bool match = true;
                for (int j = 0; j < m; j++) {
                    if (text[i + j] != pattern[j]) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    result.push_back(i);
                }
            }
            
            // 计算下一个窗口的哈希值
            if (i < n - m) {
                textHash = (BASE * (textHash - text[i] * h) + text[i + m]) % MOD;
                if (textHash < 0) {
                    textHash += MOD;
                }
            }
        }
        
        return result;
    }
};
```

### 5.2 多维搜索算法

#### K-D树搜索

```cpp
template<typename T>
class KDTree {
private:
    struct KDNode {
        std::vector<T> point;
        KDNode* left;
        KDNode* right;
        
        KDNode(const std::vector<T>& p) : point(p), left(nullptr), right(nullptr) {}
    };
    
    KDNode* root;
    int dimensions;
    
    KDNode* buildTree(std::vector<std::vector<T>>& points, int depth) {
        if (points.empty()) return nullptr;
        
        int axis = depth % dimensions;
        
        // 按当前维度排序
        std::sort(points.begin(), points.end(), 
                 [axis](const std::vector<T>& a, const std::vector<T>& b) {
                     return a[axis] < b[axis];
                 });
        
        int median = points.size() / 2;
        KDNode* node = new KDNode(points[median]);
        
        std::vector<std::vector<T>> leftPoints(points.begin(), points.begin() + median);
        std::vector<std::vector<T>> rightPoints(points.begin() + median + 1, points.end());
        
        node->left = buildTree(leftPoints, depth + 1);
        node->right = buildTree(rightPoints, depth + 1);
        
        return node;
    }
    
    double distance(const std::vector<T>& a, const std::vector<T>& b) {
        double sum = 0;
        for (int i = 0; i < dimensions; i++) {
            double diff = a[i] - b[i];
            sum += diff * diff;
        }
        return std::sqrt(sum);
    }
    
    void nearestNeighborHelper(KDNode* node, const std::vector<T>& target, 
                              int depth, KDNode*& best, double& bestDist) {
        if (node == nullptr) return;
        
        double dist = distance(node->point, target);
        if (dist < bestDist) {
            bestDist = dist;
            best = node;
        }
        
        int axis = depth % dimensions;
        KDNode* goodSide = nullptr;
        KDNode* badSide = nullptr;
        
        if (target[axis] < node->point[axis]) {
            goodSide = node->left;
            badSide = node->right;
        } else {
            goodSide = node->right;
            badSide = node->left;
        }
        
        nearestNeighborHelper(goodSide, target, depth + 1, best, bestDist);
        
        // 检查是否需要搜索另一侧
        if (std::abs(target[axis] - node->point[axis]) < bestDist) {
            nearestNeighborHelper(badSide, target, depth + 1, best, bestDist);
        }
    }
    
    void rangeSearchHelper(KDNode* node, const std::vector<T>& minBounds, 
                          const std::vector<T>& maxBounds, 
                          std::vector<std::vector<T>>& result) {
        if (node == nullptr) return;
        
        // 检查当前点是否在范围内
        bool inRange = true;
        for (int i = 0; i < dimensions; i++) {
            if (node->point[i] < minBounds[i] || node->point[i] > maxBounds[i]) {
                inRange = false;
                break;
            }
        }
        
        if (inRange) {
            result.push_back(node->point);
        }
        
        // 递归搜索子树
        rangeSearchHelper(node->left, minBounds, maxBounds, result);
        rangeSearchHelper(node->right, minBounds, maxBounds, result);
    }
    
public:
    KDTree(std::vector<std::vector<T>>& points, int dims) 
        : dimensions(dims) {
        root = buildTree(points, 0);
    }
    
    // 最近邻搜索
    std::vector<T> nearestNeighbor(const std::vector<T>& target) {
        if (root == nullptr) return {};
        
        KDNode* best = nullptr;
        double bestDist = std::numeric_limits<double>::max();
        
        nearestNeighborHelper(root, target, 0, best, bestDist);
        
        return best ? best->point : std::vector<T>();
    }
    
    // 范围搜索
    std::vector<std::vector<T>> rangeSearch(const std::vector<T>& minBounds, 
                                           const std::vector<T>& maxBounds) {
        std::vector<std::vector<T>> result;
        rangeSearchHelper(root, minBounds, maxBounds, result);
        return result;
    }
    
    // K最近邻搜索
    std::vector<std::vector<T>> kNearestNeighbors(const std::vector<T>& target, int k) {
        std::priority_queue<std::pair<double, std::vector<T>>> pq;
        
        kNearestHelper(root, target, 0, k, pq);
        
        std::vector<std::vector<T>> result;
        while (!pq.empty()) {
            result.push_back(pq.top().second);
            pq.pop();
        }
        
        std::reverse(result.begin(), result.end());
        return result;
    }
    
private:
    void kNearestHelper(KDNode* node, const std::vector<T>& target, int depth, int k,
                       std::priority_queue<std::pair<double, std::vector<T>>>& pq) {
        if (node == nullptr) return;
        
        double dist = distance(node->point, target);
        
        if (pq.size() < k) {
            pq.push({dist, node->point});
        } else if (dist < pq.top().first) {
            pq.pop();
            pq.push({dist, node->point});
        }
        
        int axis = depth % dimensions;
        KDNode* goodSide = nullptr;
        KDNode* badSide = nullptr;
        
        if (target[axis] < node->point[axis]) {
            goodSide = node->left;
            badSide = node->right;
        } else {
            goodSide = node->right;
            badSide = node->left;
        }
        
        kNearestHelper(goodSide, target, depth + 1, k, pq);
        
        if (pq.size() < k || std::abs(target[axis] - node->point[axis]) < pq.top().first) {
            kNearestHelper(badSide, target, depth + 1, k, pq);
        }
    }
};
```

## 📊 搜索算法性能对比

### 时间复杂度对比表

| 算法 | 最好时间 | 平均时间 | 最坏时间 | 空间复杂度 | 数据要求 |
|------|----------|----------|----------|------------|----------|
| 线性搜索 | O(1) | O(n) | O(n) | O(1) | 无 |
| 二分搜索 | O(1) | O(log n) | O(log n) | O(1) | 有序 |
| 插值搜索 | O(1) | O(log log n) | O(n) | O(1) | 有序+均匀分布 |
| 跳跃搜索 | O(1) | O(√n) | O(√n) | O(1) | 有序 |
| 指数搜索 | O(1) | O(log n) | O(log n) | O(1) | 有序 |
| BST搜索 | O(log n) | O(log n) | O(n) | O(n) | BST |
| 哈希搜索 | O(1) | O(1) | O(n) | O(n) | 哈希表 |

### 选择指南

1. **无序数据**: 线性搜索、哈希搜索
2. **有序数据**: 二分搜索、插值搜索、跳跃搜索
3. **大数据集**: 哈希搜索、平衡树搜索
4. **多维数据**: K-D树、R树
5. **字符串搜索**: KMP、Boyer-Moore、Rabin-Karp
6. **近似搜索**: 布隆过滤器、LSH

搜索算法的选择需要根据数据特征、性能要求和空间限制来综合考虑！🎯

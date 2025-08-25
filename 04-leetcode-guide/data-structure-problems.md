# LeetCode 数据结构题详解 - 深度进阶版

## 🎯 数据结构题概述

数据结构题是LeetCode中的核心部分，考察对各种数据结构的理解和应用能力。掌握数据结构题不仅能提高算法思维，还能在实际工程中解决复杂问题。

## 📚 线性结构问题

### 1. 数组和链表操作

#### 1.1 两数之和 (Two Sum)
**题目**: 给定一个整数数组 `nums` 和一个整数目标值 `target`，请你在该数组中找出和为目标值 `target` 的那两个整数，并返回它们的数组下标。

**解题思路**:
1. **暴力解法**: 双重循环，时间复杂度O(n²)
2. **哈希表优化**: 一次遍历，用哈希表存储已访问的数字
3. **排序+双指针**: 先排序，再用双指针查找

**最优解法 - 哈希表**:
```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> map;
        
        for (int i = 0; i < nums.size(); ++i) {
            int complement = target - nums[i];
            
            // 检查补数是否在哈希表中
            if (map.count(complement)) {
                return {map[complement], i};
            }
            
            // 将当前数字和索引加入哈希表
            map[nums[i]] = i;
        }
        
        return {}; // 无解
    }
};
```

**进阶思考**:
- 如果数组有序，可以使用双指针法，时间复杂度O(n)
- 如果要求返回所有解，需要处理重复元素
- 如果数组很大，可以考虑分治或并行处理

#### 1.2 盛最多水的容器 (Container With Most Water)
**题目**: 给定一个长度为 `n` 的整数数组 `height`，有 `n` 条垂线，第 `i` 条线的两个端点是 `(i, 0)` 和 `(i, height[i])`。找出其中的两条线，使得它们与 `x` 轴共同构成的容器可以容纳最多的水。

**解题思路**:
1. **暴力解法**: 枚举所有可能的线对，时间复杂度O(n²)
2. **双指针优化**: 从两端向中间移动，每次移动高度较小的指针

**双指针解法**:
```cpp
class Solution {
public:
    int maxArea(vector<int>& height) {
        int left = 0, right = height.size() - 1;
        int maxArea = 0;
        
        while (left < right) {
            // 计算当前容器的面积
            int width = right - left;
            int h = min(height[left], height[right]);
            int area = width * h;
            
            maxArea = max(maxArea, area);
            
            // 移动高度较小的指针
            if (height[left] < height[right]) {
                left++;
            } else {
                right--;
            }
        }
        
        return maxArea;
    }
};
```

**为什么双指针法正确**:
- 容器的面积由宽度和高度决定
- 当移动高度较大的指针时，宽度减少，高度不会增加，面积必然减少
- 只有移动高度较小的指针，才有可能找到更大的面积

#### 1.3 三数之和 (3Sum)
**题目**: 给你一个包含 `n` 个整数的数组 `nums`，判断 `nums` 中是否存在三个元素 `a`，`b`，`c`，使得 `a + b + c = 0`？请你找出所有和为 `0` 且不重复的三元组。

**解题思路**:
1. **排序预处理**: 先排序数组，便于去重和双指针操作
2. **固定第一个数**: 遍历数组，固定第一个数
3. **双指针查找**: 在剩余部分使用双指针查找和为 `-first` 的两个数
4. **去重处理**: 跳过重复元素，避免重复结果

**完整解法**:
```cpp
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> result;
        int n = nums.size();
        
        if (n < 3) return result;
        
        // 排序数组
        sort(nums.begin(), nums.end());
        
        for (int i = 0; i < n - 2; ++i) {
            // 跳过重复的第一个数
            if (i > 0 && nums[i] == nums[i-1]) continue;
            
            // 如果第一个数大于0，后面不可能有解
            if (nums[i] > 0) break;
            
            int left = i + 1, right = n - 1;
            
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                
                if (sum == 0) {
                    result.push_back({nums[i], nums[left], nums[right]});
                    
                    // 跳过重复的left和right
                    while (left < right && nums[left] == nums[left+1]) left++;
                    while (left < right && nums[right] == nums[right-1]) right--;
                    
                    left++;
                    right--;
                } else if (sum < 0) {
                    left++;
                } else {
                    right--;
                }
            }
        }
        
        return result;
    }
};
```

**时间复杂度分析**:
- 排序: O(n log n)
- 外层循环: O(n)
- 内层双指针: O(n)
- 总体: O(n²)

**空间复杂度**: O(1) (不考虑输出数组)

### 2. 滑动窗口问题

#### 2.1 无重复字符的最长子串 (Longest Substring Without Repeating Characters)
**题目**: 给定一个字符串 `s`，请你找出其中不含有重复字符的最长子串的长度。

**解题思路**:
1. **滑动窗口**: 维护一个不包含重复字符的窗口
2. **哈希表记录**: 记录每个字符最后出现的位置
3. **窗口收缩**: 遇到重复字符时，收缩窗口左边界

**滑动窗口解法**:
```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_map<char, int> charMap;
        int left = 0, maxLength = 0;
        
        for (int right = 0; right < s.length(); ++right) {
            char currentChar = s[right];
            
            // 如果字符已存在，更新左边界
            if (charMap.count(currentChar)) {
                left = max(left, charMap[currentChar] + 1);
            }
            
            // 更新字符的最后出现位置
            charMap[currentChar] = right;
            
            // 更新最大长度
            maxLength = max(maxLength, right - left + 1);
        }
        
        return maxLength;
    }
};
```

**优化版本 - 使用数组**:
```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        vector<int> charIndex(128, -1); // ASCII字符
        int left = 0, maxLength = 0;
        
        for (int right = 0; right < s.length(); ++right) {
            char currentChar = s[right];
            
            // 如果字符已存在且在当前窗口内
            if (charIndex[currentChar] >= left) {
                left = charIndex[currentChar] + 1;
            }
            
            charIndex[currentChar] = right;
            maxLength = max(maxLength, right - left + 1);
        }
        
        return maxLength;
    }
};
```

#### 2.2 滑动窗口最大值 (Sliding Window Maximum)
**题目**: 给你一个整数数组 `nums`，有一个大小为 `k` 的滑动窗口从数组的最左侧移动到数组的最右侧。你只可以看到在滑动窗口内的 `k` 个数字。滑动窗口每次只向右移动一位。返回滑动窗口中的最大值。

**解题思路**:
1. **单调队列**: 维护一个单调递减的双端队列
2. **队列存储索引**: 队列中存储的是数组索引，便于判断是否超出窗口
3. **维护单调性**: 每次添加新元素时，移除队列中所有小于新元素的元素

**单调队列解法**:
```cpp
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        vector<int> result;
        deque<int> dq; // 存储索引
        
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
            
            // 当窗口大小达到k时，记录结果
            if (i >= k - 1) {
                result.push_back(nums[dq.front()]);
            }
        }
        
        return result;
    }
};
```

**为什么单调队列有效**:
- 队列中的元素按索引顺序排列
- 队列中的元素值单调递减
- 队首元素总是当前窗口的最大值
- 时间复杂度: O(n)，每个元素最多入队出队一次

### 3. 前缀和和差分数组

#### 3.1 和为K的子数组 (Subarray Sum Equals K)
**题目**: 给你一个整数数组 `nums` 和一个整数 `k`，请你统计并返回该数组中和为 `k` 的连续子数组的个数。

**解题思路**:
1. **前缀和**: 计算数组的前缀和
2. **哈希表**: 记录每个前缀和出现的次数
3. **查找补数**: 对于每个位置，查找前缀和为 `sum - k` 的位置数量

**前缀和+哈希表解法**:
```cpp
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        unordered_map<int, int> prefixSumCount;
        prefixSumCount[0] = 1; // 前缀和为0的出现次数为1
        
        int sum = 0, count = 0;
        
        for (int num : nums) {
            sum += num;
            
            // 查找前缀和为 sum - k 的位置数量
            if (prefixSumCount.count(sum - k)) {
                count += prefixSumCount[sum - k];
            }
            
            // 更新当前前缀和的出现次数
            prefixSumCount[sum]++;
        }
        
        return count;
    }
};
```

**为什么需要 `prefixSumCount[0] = 1`**:
- 考虑子数组从开头开始的情况
- 如果 `sum == k`，那么 `sum - k == 0`
- 需要统计前缀和为0的位置数量

#### 3.2 区间加法 (Range Addition)
**题目**: 假设你有一个长度为 `n` 的数组，初始情况下所有的数字均为 `0`，你将会被给出 `k` 个更新的操作。其中，每个操作会被表示为一个三元组：`[startIndex, endIndex, inc]`，你需要将子数组 `A[startIndex ... endIndex]`（包括 `startIndex` 和 `endIndex`）中的数值增加 `inc`。请你返回 `k` 次操作后的数组。

**解题思路**:
1. **差分数组**: 在区间开始位置加增量，在区间结束位置后减增量
2. **前缀和恢复**: 通过前缀和恢复原数组

**差分数组解法**:
```cpp
class Solution {
public:
    vector<int> getModifiedArray(int length, vector<vector<int>>& updates) {
        vector<int> diff(length + 1, 0);
        
        // 应用差分操作
        for (const auto& update : updates) {
            int start = update[0];
            int end = update[1];
            int inc = update[2];
            
            diff[start] += inc;
            diff[end + 1] -= inc;
        }
        
        // 通过前缀和恢复原数组
        vector<int> result(length);
        result[0] = diff[0];
        
        for (int i = 1; i < length; ++i) {
            result[i] = result[i-1] + diff[i];
        }
        
        return result;
    }
};
```

**差分数组的优势**:
- 区间更新操作的时间复杂度为O(1)
- 最终恢复数组的时间复杂度为O(n)
- 总体时间复杂度为O(n + k)，其中k为操作数量

## 📚 树形结构问题

### 1. 二叉树遍历

#### 1.1 二叉树的层序遍历 (Binary Tree Level Order Traversal)
**题目**: 给你一个二叉树，请你返回其按层序遍历得到的节点值。（即逐层地，从左到右访问所有节点）。

**解题思路**:
1. **BFS**: 使用队列进行广度优先搜索
2. **层次记录**: 记录每层的节点数量
3. **结果收集**: 将每层的节点值收集到结果中

**BFS解法**:
```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> result;
        
        if (!root) return result;
        
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            int levelSize = q.size();
            vector<int> currentLevel;
            
            // 处理当前层的所有节点
            for (int i = 0; i < levelSize; ++i) {
                TreeNode* node = q.front();
                q.pop();
                
                currentLevel.push_back(node->val);
                
                // 添加下一层的节点
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            
            result.push_back(currentLevel);
        }
        
        return result;
    }
};
```

**递归解法**:
```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> result;
        dfs(root, 0, result);
        return result;
    }
    
private:
    void dfs(TreeNode* node, int level, vector<vector<int>>& result) {
        if (!node) return;
        
        // 如果当前层还没有创建，创建新层
        if (level >= result.size()) {
            result.push_back({});
        }
        
        // 将当前节点加入对应层
        result[level].push_back(node->val);
        
        // 递归处理左右子树
        dfs(node->left, level + 1, result);
        dfs(node->right, level + 1, result);
    }
};
```

#### 1.2 锯齿形层序遍历 (Binary Tree Zigzag Level Order Traversal)
**题目**: 给定一个二叉树，返回其节点值的锯齿形层序遍历。（即先从左往右，再从右往左进行下一层遍历，以此类推，层与层之间交替进行）。

**解题思路**:
1. **BFS基础**: 仍然使用队列进行层次遍历
2. **方向控制**: 使用标志位控制每层的遍历方向
3. **结果调整**: 根据方向决定是否反转当前层的结果

**BFS+方向控制解法**:
```cpp
class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> result;
        
        if (!root) return result;
        
        queue<TreeNode*> q;
        q.push(root);
        bool leftToRight = true; // 控制遍历方向
        
        while (!q.empty()) {
            int levelSize = q.size();
            vector<int> currentLevel;
            
            for (int i = 0; i < levelSize; ++i) {
                TreeNode* node = q.front();
                q.pop();
                
                currentLevel.push_back(node->val);
                
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            
            // 根据方向决定是否反转
            if (!leftToRight) {
                reverse(currentLevel.begin(), currentLevel.end());
            }
            
            result.push_back(currentLevel);
            leftToRight = !leftToRight; // 切换方向
        }
        
        return result;
    }
};
```

**优化版本 - 双端队列**:
```cpp
class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> result;
        
        if (!root) return result;
        
        queue<TreeNode*> q;
        q.push(root);
        bool leftToRight = true;
        
        while (!q.empty()) {
            int levelSize = q.size();
            deque<int> currentLevel;
            
            for (int i = 0; i < levelSize; ++i) {
                TreeNode* node = q.front();
                q.pop();
                
                // 根据方向决定插入位置
                if (leftToRight) {
                    currentLevel.push_back(node->val);
                } else {
                    currentLevel.push_front(node->val);
                }
                
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            
            result.push_back(vector<int>(currentLevel.begin(), currentLevel.end()));
            leftToRight = !leftToRight;
        }
        
        return result;
    }
};
```

### 2. 二叉树性质问题

#### 2.1 对称二叉树 (Symmetric Tree)
**题目**: 给定一个二叉树，检查它是否是镜像对称的。

**解题思路**:
1. **递归比较**: 比较左右子树的镜像对称性
2. **对称条件**: 左子树的左子节点等于右子树的右子节点，左子树的右子节点等于右子树的左子节点

**递归解法**:
```cpp
class Solution {
public:
    bool isSymmetric(TreeNode* root) {
        if (!root) return true;
        return isMirror(root->left, root->right);
    }
    
private:
    bool isMirror(TreeNode* left, TreeNode* right) {
        // 如果两个节点都为空，对称
        if (!left && !right) return true;
        
        // 如果只有一个节点为空，不对称
        if (!left || !right) return false;
        
        // 比较节点值和递归比较子树
        return (left->val == right->val) &&
               isMirror(left->left, right->right) &&
               isMirror(left->right, right->left);
    }
};
```

**迭代解法 - 使用队列**:
```cpp
class Solution {
public:
    bool isSymmetric(TreeNode* root) {
        if (!root) return true;
        
        queue<TreeNode*> q;
        q.push(root->left);
        q.push(root->right);
        
        while (!q.empty()) {
            TreeNode* left = q.front(); q.pop();
            TreeNode* right = q.front(); q.pop();
            
            if (!left && !right) continue;
            if (!left || !right) return false;
            if (left->val != right->val) return false;
            
            // 按对称顺序入队
            q.push(left->left);
            q.push(right->right);
            q.push(left->right);
            q.push(right->left);
        }
        
        return true;
    }
};
```

#### 2.2 平衡二叉树 (Balanced Binary Tree)
**题目**: 给定一个二叉树，判断它是否是高度平衡的二叉树。一棵高度平衡二叉树定义为：一个二叉树每个节点的左右两个子树的高度差的绝对值不超过1。

**解题思路**:
1. **自底向上**: 从叶子节点开始计算高度
2. **平衡检查**: 在计算高度的同时检查平衡性
3. **提前返回**: 如果发现不平衡，立即返回-1

**自底向上解法**:
```cpp
class Solution {
public:
    bool isBalanced(TreeNode* root) {
        return getHeight(root) != -1;
    }
    
private:
    int getHeight(TreeNode* node) {
        if (!node) return 0;
        
        // 递归计算左右子树高度
        int leftHeight = getHeight(node->left);
        if (leftHeight == -1) return -1; // 左子树不平衡
        
        int rightHeight = getHeight(node->right);
        if (rightHeight == -1) return -1; // 右子树不平衡
        
        // 检查当前节点是否平衡
        if (abs(leftHeight - rightHeight) > 1) return -1;
        
        // 返回当前节点的高度
        return max(leftHeight, rightHeight) + 1;
    }
};
```

**自顶向下解法**:
```cpp
class Solution {
public:
    bool isBalanced(TreeNode* root) {
        if (!root) return true;
        
        // 检查当前节点是否平衡
        if (abs(getHeight(root->left) - getHeight(root->right)) > 1) {
            return false;
        }
        
        // 递归检查左右子树
        return isBalanced(root->left) && isBalanced(root->right);
    }
    
private:
    int getHeight(TreeNode* node) {
        if (!node) return 0;
        return max(getHeight(node->left), getHeight(node->right)) + 1;
    }
};
```

**两种解法的比较**:
- **自底向上**: 时间复杂度O(n)，每个节点只访问一次
- **自顶向下**: 时间复杂度O(n log n)，存在重复计算

## 💡 解题技巧总结

### 1. 数据结构选择原则
- **数组问题**: 考虑双指针、滑动窗口、前缀和
- **链表问题**: 考虑快慢指针、反转、合并
- **树问题**: 考虑递归、BFS、DFS、层次遍历
- **图问题**: 考虑BFS、DFS、拓扑排序、并查集

### 2. 常见优化技巧
- **空间换时间**: 使用哈希表、前缀和等预处理
- **时间换空间**: 使用原地算法、滚动数组等
- **数据结构优化**: 使用单调栈、单调队列等特殊结构
- **算法优化**: 使用分治、贪心、动态规划等思想

### 3. 边界条件处理
- **空输入**: 检查空数组、空字符串、空指针
- **单元素**: 处理只有一个元素的情况
- **重复元素**: 考虑去重、计数等处理
- **溢出**: 注意整数溢出、数组越界等

## 🚀 下一步
- 学习 [动态规划题详解](./dynamic-programming.md)
- 了解 [图论算法题详解](./graph-algorithms.md)
- 掌握 [面试高频题](./interview-high-frequency.md)

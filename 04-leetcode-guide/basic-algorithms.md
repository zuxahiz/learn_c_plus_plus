# LeetCode 基础算法题详解

## 🎯 基础算法概述

基础算法是算法学习的基础，包括数组操作、字符串处理、双指针技巧、滑动窗口、前缀和等。掌握这些基础算法是解决更复杂问题的前提。

## 📚 数组和字符串基础

### 1. 两数之和 (Two Sum)
**题目**: 给定一个整数数组 `nums` 和一个整数目标值 `target`，请你在该数组中找出和为目标值 `target` 的那两个整数，并返回它们的数组下标。

**解法1: 暴力解法**
```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        int n = nums.size();
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (nums[i] + nums[j] == target) {
                    return {i, j};
                }
            }
        }
        return {};
    }
};
```
**时间复杂度**: O(n²) **空间复杂度**: O(1)

**解法2: 哈希表优化**
```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> map;
        for (int i = 0; i < nums.size(); ++i) {
            int complement = target - nums[i];
            if (map.count(complement)) {
                return {map[complement], i};
            }
            map[nums[i]] = i;
        }
        return {};
    }
};
```
**时间复杂度**: O(n) **空间复杂度**: O(n)

**解题思路**:
- 使用哈希表存储已遍历的数字及其索引
- 对于每个数字，检查 `target - nums[i]` 是否在哈希表中
- 如果找到，返回两个索引；否则将当前数字加入哈希表

### 2. 盛最多水的容器 (Container With Most Water)
**题目**: 给定一个长度为 `n` 的整数数组 `height`，有 `n` 条垂线，第 `i` 条线的两个端点是 `(i, 0)` 和 `(i, height[i])`。找出其中的两条线，使得它们与 `x` 轴共同构成的容器可以容纳最多的水。

**解法: 双指针**
```cpp
class Solution {
public:
    int maxArea(vector<int>& height) {
        int left = 0, right = height.size() - 1;
        int maxArea = 0;
        
        while (left < right) {
            int width = right - left;
            int h = min(height[left], height[right]);
            maxArea = max(maxArea, width * h);
            
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
**时间复杂度**: O(n) **空间复杂度**: O(1)

**解题思路**:
- 使用双指针从两端向中间移动
- 容器的面积 = 宽度 × 高度，高度取两端的较小值
- 移动高度较小的指针，因为移动较高的指针不会增加面积

### 3. 三数之和 (3Sum)
**题目**: 给你一个包含 `n` 个整数的数组 `nums`，判断 `nums` 中是否存在三个元素 `a`，`b`，`c`，使得 `a + b + c = 0`？请你找出所有和为 `0` 且不重复的三元组。

**解法: 排序 + 双指针**
```cpp
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> result;
        int n = nums.size();
        
        if (n < 3) return result;
        
        sort(nums.begin(), nums.end());
        
        for (int i = 0; i < n - 2; ++i) {
            // 跳过重复元素
            if (i > 0 && nums[i] == nums[i-1]) continue;
            
            int left = i + 1, right = n - 1;
            
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                
                if (sum == 0) {
                    result.push_back({nums[i], nums[left], nums[right]});
                    
                    // 跳过重复元素
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
**时间复杂度**: O(n²) **空间复杂度**: O(1)

**解题思路**:
- 先排序数组，便于去重和双指针操作
- 固定第一个数，使用双指针寻找剩余两个数
- 注意去重处理，避免重复结果

## 📚 双指针技巧

### 1. 无重复字符的最长子串 (Longest Substring Without Repeating Characters)
**题目**: 给定一个字符串 `s`，请你找出其中不含有重复字符的最长子串的长度。

**解法: 滑动窗口 + 哈希表**
```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_map<char, int> map;
        int left = 0, maxLength = 0;
        
        for (int right = 0; right < s.length(); ++right) {
            if (map.count(s[right])) {
                left = max(left, map[s[right]] + 1);
            }
            map[s[right]] = right;
            maxLength = max(maxLength, right - left + 1);
        }
        
        return maxLength;
    }
};
```
**时间复杂度**: O(n) **空间复杂度**: O(min(m, n))

**解题思路**:
- 使用滑动窗口维护无重复字符的子串
- 哈希表记录每个字符最后出现的位置
- 当遇到重复字符时，移动左指针到重复字符之后

### 2. 最长回文子串 (Longest Palindromic Substring)
**题目**: 给你一个字符串 `s`，找到 `s` 中最长的回文子串。

**解法1: 中心扩展法**
```cpp
class Solution {
public:
    string longestPalindrome(string s) {
        int start = 0, maxLength = 1;
        int n = s.length();
        
        for (int i = 0; i < n; ++i) {
            // 奇数长度回文串
            int len1 = expandAroundCenter(s, i, i);
            // 偶数长度回文串
            int len2 = expandAroundCenter(s, i, i + 1);
            
            int len = max(len1, len2);
            if (len > maxLength) {
                maxLength = len;
                start = i - (len - 1) / 2;
            }
        }
        
        return s.substr(start, maxLength);
    }
    
private:
    int expandAroundCenter(const string& s, int left, int right) {
        while (left >= 0 && right < s.length() && s[left] == s[right]) {
            left--;
            right++;
        }
        return right - left - 1;
    }
};
```
**时间复杂度**: O(n²) **空间复杂度**: O(1)

**解法2: 动态规划**
```cpp
class Solution {
public:
    string longestPalindrome(string s) {
        int n = s.length();
        if (n < 2) return s;
        
        vector<vector<bool>> dp(n, vector<bool>(n, false));
        
        // 单个字符都是回文
        for (int i = 0; i < n; ++i) {
            dp[i][i] = true;
        }
        
        int start = 0, maxLength = 1;
        
        // 检查长度为2的子串
        for (int i = 0; i < n - 1; ++i) {
            if (s[i] == s[i + 1]) {
                dp[i][i + 1] = true;
                start = i;
                maxLength = 2;
            }
        }
        
        // 检查长度大于2的子串
        for (int len = 3; len <= n; ++len) {
            for (int i = 0; i <= n - len; ++i) {
                int j = i + len - 1;
                if (s[i] == s[j] && dp[i + 1][j - 1]) {
                    dp[i][j] = true;
                    if (len > maxLength) {
                        start = i;
                        maxLength = len;
                    }
                }
            }
        }
        
        return s.substr(start, maxLength);
    }
};
```
**时间复杂度**: O(n²) **空间复杂度**: O(n²)

## 📚 滑动窗口技巧

### 1. 滑动窗口最大值 (Sliding Window Maximum)
**题目**: 给你一个整数数组 `nums`，有一个大小为 `k` 的滑动窗口从数组的最左侧移动到数组的最右侧。你只可以看到在滑动窗口内的 `k` 个数字。滑动窗口每次只向右移动一位。返回滑动窗口中的最大值。

**解法: 单调队列**
```cpp
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        vector<int> result;
        deque<int> dq;
        
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
};
```
**时间复杂度**: O(n) **空间复杂度**: O(k)

**解题思路**:
- 使用双端队列维护单调递减序列
- 队列中存储的是索引，便于判断是否超出窗口
- 每次添加新元素时，移除队列中所有小于新元素的元素

### 2. 最小覆盖子串 (Minimum Window Substring)
**题目**: 给你一个字符串 `s`、一个字符串 `t`。返回 `s` 中涵盖 `t` 所有字符的最小子串。如果 `s` 中不存在涵盖 `t` 所有字符的子串，则返回空字符串。

**解法: 滑动窗口 + 哈希表**
```cpp
class Solution {
public:
    string minWindow(string s, string t) {
        vector<int> need(128, 0);
        vector<int> window(128, 0);
        
        // 统计t中每个字符的出现次数
        for (char c : t) {
            need[c]++;
        }
        
        int left = 0, right = 0;
        int start = 0, minLen = INT_MAX;
        int valid = 0;
        int needSize = 0;
        
        // 统计需要覆盖的字符种类数
        for (int i = 0; i < 128; ++i) {
            if (need[i] > 0) needSize++;
        }
        
        while (right < s.length()) {
            char c = s[right];
            right++;
            
            if (need[c] > 0) {
                window[c]++;
                if (window[c] == need[c]) {
                    valid++;
                }
            }
            
            // 尝试收缩窗口
            while (valid == needSize) {
                if (right - left < minLen) {
                    start = left;
                    minLen = right - left;
                }
                
                char d = s[left];
                left++;
                
                if (need[d] > 0) {
                    if (window[d] == need[d]) {
                        valid--;
                    }
                    window[d]--;
                }
            }
        }
        
        return minLen == INT_MAX ? "" : s.substr(start, minLen);
    }
};
```
**时间复杂度**: O(n) **空间复杂度**: O(1)

**解题思路**:
- 使用滑动窗口维护包含t中所有字符的子串
- 使用两个哈希表分别记录需要的字符和当前窗口的字符
- 当窗口包含所有需要的字符时，尝试收缩窗口找到最小值

## 📚 前缀和技巧

### 1. 和为K的子数组 (Subarray Sum Equals K)
**题目**: 给你一个整数数组 `nums` 和一个整数 `k`，请你统计并返回该数组中和为 `k` 的连续子数组的个数。

**解法: 前缀和 + 哈希表**
```cpp
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        unordered_map<int, int> map;
        map[0] = 1;  // 前缀和为0的出现次数为1
        
        int sum = 0, count = 0;
        
        for (int num : nums) {
            sum += num;
            
            // 如果存在前缀和为 sum - k，说明找到了和为k的子数组
            if (map.count(sum - k)) {
                count += map[sum - k];
            }
            
            map[sum]++;
        }
        
        return count;
    }
};
```
**时间复杂度**: O(n) **空间复杂度**: O(n)

**解题思路**:
- 使用前缀和计算任意子数组的和
- 对于每个位置，检查是否存在前缀和为 `sum - k` 的位置
- 哈希表记录每个前缀和出现的次数

### 2. 连续子数组的最大和 (Maximum Subarray)
**题目**: 给你一个整数数组 `nums`，请你找出一个具有最大和的连续子数组（子数组最少包含一个元素），返回其最大和。

**解法1: 动态规划**
```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int n = nums.size();
        vector<int> dp(n);
        dp[0] = nums[0];
        int maxSum = dp[0];
        
        for (int i = 1; i < n; ++i) {
            dp[i] = max(nums[i], dp[i-1] + nums[i]);
            maxSum = max(maxSum, dp[i]);
        }
        
        return maxSum;
    }
};
```

**解法2: 贪心算法**
```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int currentSum = nums[0];
        int maxSum = nums[0];
        
        for (int i = 1; i < nums.size(); ++i) {
            currentSum = max(nums[i], currentSum + nums[i]);
            maxSum = max(maxSum, currentSum);
        }
        
        return maxSum;
    }
};
```
**时间复杂度**: O(n) **空间复杂度**: O(1)

**解题思路**:
- 对于每个位置，选择加入前面的子数组或重新开始
- 维护当前子数组的和和全局最大和
- 当当前和为负数时，重新开始计算

## 📚 差分数组技巧

### 1. 区间加法 (Range Addition)
**题目**: 假设你有一个长度为 `n` 的数组，初始情况下所有的数字均为 `0`，你将会被给出 `k` 个更新的操作。其中，每个操作会被表示为一个三元组：`[startIndex, endIndex, inc]`，你需要将子数组 `A[startIndex ... endIndex]`（包括 `startIndex` 和 `endIndex`）中的数值增加 `inc`。请你返回 `k` 次操作后的数组。

**解法: 差分数组**
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
        
        // 恢复原数组
        vector<int> result(length);
        result[0] = diff[0];
        for (int i = 1; i < length; ++i) {
            result[i] = result[i-1] + diff[i];
        }
        
        return result;
    }
};
```
**时间复杂度**: O(n + k) **空间复杂度**: O(n)

**解题思路**:
- 使用差分数组记录区间变化
- 在区间开始位置加增量，在区间结束位置后减增量
- 通过前缀和恢复原数组

## 💡 解题技巧总结

### 1. 双指针技巧
- **对撞指针**: 从两端向中间移动，适用于有序数组
- **快慢指针**: 一个快一个慢，适用于链表环检测
- **滑动窗口**: 维护一个可变大小的窗口

### 2. 哈希表应用
- **快速查找**: O(1)时间复杂度的查找
- **去重**: 避免重复计算
- **计数**: 统计元素出现次数

### 3. 排序预处理
- **双指针**: 排序后使用双指针技巧
- **二分查找**: 在有序数组中快速查找
- **去重**: 排序后容易去除重复元素

### 4. 空间换时间
- **前缀和**: 预处理数组，快速计算区间和
- **差分数组**: 快速处理区间更新操作
- **哈希表**: 用空间换取查找时间

## 🎯 练习建议

### 1. 循序渐进
- 从简单题目开始，建立信心
- 逐步增加难度，提高解题能力
- 多总结解题思路和技巧

### 2. 分类练习
- 按算法类型分组练习
- 掌握每种算法的适用场景
- 学会选择最优解法

### 3. 代码规范
- 变量命名清晰，有意义的函数名
- 添加必要的注释说明
- 处理边界条件和异常情况

## 🚀 下一步
- 学习 [数据结构题](./data-structure-problems.md)
- 了解 [动态规划题](./dynamic-programming.md)
- 掌握 [图论算法题](./graph-algorithms.md)

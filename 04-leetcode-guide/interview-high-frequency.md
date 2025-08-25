# LeetCode 面试高频题详解

## 🎯 面试高频题概述

这份指南收录了各大互联网公司面试中最常出现的LeetCode题目，按照出现频率和重要程度进行分类。掌握这些题目能够显著提高面试通过率。

### 题目分布
- **超高频题 (20题)**: 面试必考，出现频率>80%
- **高频题 (50题)**: 经常出现，出现频率50-80%
- **中频题 (80题)**: 偶尔出现，出现频率20-50%
- **低频题 (100题)**: 很少出现，但值得了解

## 🔥 超高频题 (Top 20)

### 1. 两数之和 (LeetCode 1) ⭐⭐⭐⭐⭐

**面试频率**: 95% | **公司**: 所有公司 | **难度**: Easy

```cpp
class Solution {
public:
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
};
```

**面试考点**:
- 哈希表的应用
- 时间复杂度优化：O(n²) → O(n)
- 空间换时间的思想

**变体题目**:
- 三数之和 (15)
- 四数之和 (18)
- 两数之和II (167)

### 2. 反转链表 (LeetCode 206) ⭐⭐⭐⭐⭐

**面试频率**: 92% | **公司**: 所有公司 | **难度**: Easy

```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* current = head;
        
        while (current != nullptr) {
            ListNode* next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        
        return prev;
    }
    
    // 递归版本
    ListNode* reverseListRecursive(ListNode* head) {
        if (head == nullptr || head->next == nullptr) {
            return head;
        }
        
        ListNode* newHead = reverseListRecursive(head->next);
        head->next->next = head;
        head->next = nullptr;
        
        return newHead;
    }
};
```

**面试考点**:
- 链表操作基础
- 迭代vs递归实现
- 指针操作技巧

**变体题目**:
- 反转链表II (92)
- K个一组翻转链表 (25)

### 3. 有效的括号 (LeetCode 20) ⭐⭐⭐⭐⭐

**面试频率**: 88% | **公司**: 微软、亚马逊、谷歌 | **难度**: Easy

```cpp
class Solution {
public:
    bool isValid(string s) {
        stack<char> st;
        unordered_map<char, char> pairs = {{')', '('}, {'}', '{'}, {']', '['}};
        
        for (char c : s) {
            if (pairs.count(c)) {
                if (st.empty() || st.top() != pairs[c]) {
                    return false;
                }
                st.pop();
            } else {
                st.push(c);
            }
        }
        
        return st.empty();
    }
};
```

**面试考点**:
- 栈的经典应用
- 配对问题的解决思路
- 边界条件处理

### 4. 合并两个有序链表 (LeetCode 21) ⭐⭐⭐⭐⭐

**面试频率**: 85% | **公司**: 所有公司 | **难度**: Easy

```cpp
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        ListNode dummy(0);
        ListNode* current = &dummy;
        
        while (list1 && list2) {
            if (list1->val <= list2->val) {
                current->next = list1;
                list1 = list1->next;
            } else {
                current->next = list2;
                list2 = list2->next;
            }
            current = current->next;
        }
        
        current->next = list1 ? list1 : list2;
        return dummy.next;
    }
    
    // 递归版本
    ListNode* mergeTwoListsRecursive(ListNode* list1, ListNode* list2) {
        if (!list1) return list2;
        if (!list2) return list1;
        
        if (list1->val <= list2->val) {
            list1->next = mergeTwoListsRecursive(list1->next, list2);
            return list1;
        } else {
            list2->next = mergeTwoListsRecursive(list1, list2->next);
            return list2;
        }
    }
};
```

**面试考点**:
- 归并思想
- 链表操作
- 虚拟头节点技巧

### 5. 爬楼梯 (LeetCode 70) ⭐⭐⭐⭐⭐

**面试频率**: 82% | **公司**: 字节跳动、阿里巴巴 | **难度**: Easy

```cpp
class Solution {
public:
    int climbStairs(int n) {
        if (n <= 2) return n;
        
        int prev2 = 1, prev1 = 2;
        for (int i = 3; i <= n; i++) {
            int current = prev1 + prev2;
            prev2 = prev1;
            prev1 = current;
        }
        
        return prev1;
    }
    
    // 矩阵快速幂版本 O(log n)
    int climbStairsFast(int n) {
        if (n <= 2) return n;
        
        vector<vector<long long>> base = {{1, 1}, {1, 0}};
        vector<vector<long long>> result = matrixPower(base, n);
        
        return result[0][0];
    }
    
private:
    vector<vector<long long>> matrixMultiply(vector<vector<long long>>& A, vector<vector<long long>>& B) {
        return {{A[0][0]*B[0][0] + A[0][1]*B[1][0], A[0][0]*B[0][1] + A[0][1]*B[1][1]},
                {A[1][0]*B[0][0] + A[1][1]*B[1][0], A[1][0]*B[0][1] + A[1][1]*B[1][1]}};
    }
    
    vector<vector<long long>> matrixPower(vector<vector<long long>>& matrix, int n) {
        vector<vector<long long>> result = {{1, 0}, {0, 1}}; // 单位矩阵
        
        while (n > 0) {
            if (n & 1) {
                result = matrixMultiply(result, matrix);
            }
            matrix = matrixMultiply(matrix, matrix);
            n >>= 1;
        }
        
        return result;
    }
};
```

**面试考点**:
- 动态规划入门
- 斐波那契数列
- 空间优化技巧

### 6. 最大子数组和 (LeetCode 53) ⭐⭐⭐⭐⭐

**面试频率**: 80% | **公司**: 亚马逊、微软 | **难度**: Medium

```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int maxSoFar = nums[0];
        int maxEndingHere = nums[0];
        
        for (int i = 1; i < nums.size(); i++) {
            maxEndingHere = max(nums[i], maxEndingHere + nums[i]);
            maxSoFar = max(maxSoFar, maxEndingHere);
        }
        
        return maxSoFar;
    }
    
    // 分治法版本
    int maxSubArrayDivideConquer(vector<int>& nums) {
        return helper(nums, 0, nums.size() - 1);
    }
    
private:
    int helper(vector<int>& nums, int left, int right) {
        if (left == right) return nums[left];
        
        int mid = left + (right - left) / 2;
        int leftSum = helper(nums, left, mid);
        int rightSum = helper(nums, mid + 1, right);
        int crossSum = crossSum(nums, left, mid, right);
        
        return max({leftSum, rightSum, crossSum});
    }
    
    int crossSum(vector<int>& nums, int left, int mid, int right) {
        int leftSum = INT_MIN;
        int sum = 0;
        for (int i = mid; i >= left; i--) {
            sum += nums[i];
            leftSum = max(leftSum, sum);
        }
        
        int rightSum = INT_MIN;
        sum = 0;
        for (int i = mid + 1; i <= right; i++) {
            sum += nums[i];
            rightSum = max(rightSum, sum);
        }
        
        return leftSum + rightSum;
    }
};
```

**面试考点**:
- Kadane算法
- 动态规划思想
- 分治法思想

### 7. 买卖股票的最佳时机 (LeetCode 121) ⭐⭐⭐⭐⭐

**面试频率**: 78% | **公司**: 字节跳动、腾讯 | **难度**: Easy

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int minPrice = INT_MAX;
        int maxProfit = 0;
        
        for (int price : prices) {
            if (price < minPrice) {
                minPrice = price;
            } else if (price - minPrice > maxProfit) {
                maxProfit = price - minPrice;
            }
        }
        
        return maxProfit;
    }
    
    // 动态规划版本
    int maxProfitDP(vector<int>& prices) {
        int n = prices.size();
        if (n <= 1) return 0;
        
        // dp[i][0] 表示第i天不持有股票的最大利润
        // dp[i][1] 表示第i天持有股票的最大利润
        int hold = -prices[0];    // 持有股票
        int noHold = 0;           // 不持有股票
        
        for (int i = 1; i < n; i++) {
            int newNoHold = max(noHold, hold + prices[i]);
            int newHold = max(hold, -prices[i]);
            
            noHold = newNoHold;
            hold = newHold;
        }
        
        return noHold;
    }
};
```

**面试考点**:
- 贪心算法思想
- 动态规划状态设计
- 状态压缩优化

### 8. 环形链表 (LeetCode 141) ⭐⭐⭐⭐⭐

**面试频率**: 75% | **公司**: 所有公司 | **难度**: Easy

```cpp
class Solution {
public:
    bool hasCycle(ListNode *head) {
        if (!head || !head->next) return false;
        
        ListNode* slow = head;
        ListNode* fast = head->next;
        
        while (fast && fast->next) {
            if (slow == fast) return true;
            slow = slow->next;
            fast = fast->next->next;
        }
        
        return false;
    }
    
    // 哈希表版本
    bool hasCycleHashSet(ListNode *head) {
        unordered_set<ListNode*> visited;
        
        while (head) {
            if (visited.count(head)) return true;
            visited.insert(head);
            head = head->next;
        }
        
        return false;
    }
};
```

**面试考点**:
- 快慢指针技巧
- Floyd判圈算法
- 链表环检测

### 9. 三数之和 (LeetCode 15) ⭐⭐⭐⭐⭐

**面试频率**: 72% | **公司**: 字节跳动、亚马逊 | **难度**: Medium

```cpp
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> result;
        sort(nums.begin(), nums.end());
        
        for (int i = 0; i < nums.size() - 2; i++) {
            if (i > 0 && nums[i] == nums[i-1]) continue; // 跳过重复
            
            int left = i + 1;
            int right = nums.size() - 1;
            
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                
                if (sum == 0) {
                    result.push_back({nums[i], nums[left], nums[right]});
                    
                    // 跳过重复
                    while (left < right && nums[left] == nums[left + 1]) left++;
                    while (left < right && nums[right] == nums[right - 1]) right--;
                    
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

**面试考点**:
- 双指针技巧
- 去重处理
- 排序的应用

### 10. 合并K个升序链表 (LeetCode 23) ⭐⭐⭐⭐⭐

**面试频率**: 70% | **公司**: 谷歌、微软 | **难度**: Hard

```cpp
class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.empty()) return nullptr;
        
        while (lists.size() > 1) {
            vector<ListNode*> mergedLists;
            
            for (int i = 0; i < lists.size(); i += 2) {
                ListNode* l1 = lists[i];
                ListNode* l2 = (i + 1 < lists.size()) ? lists[i + 1] : nullptr;
                mergedLists.push_back(mergeTwoLists(l1, l2));
            }
            
            lists = mergedLists;
        }
        
        return lists[0];
    }
    
    // 优先队列版本
    ListNode* mergeKListsPQ(vector<ListNode*>& lists) {
        auto cmp = [](ListNode* a, ListNode* b) { return a->val > b->val; };
        priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> pq(cmp);
        
        for (ListNode* list : lists) {
            if (list) pq.push(list);
        }
        
        ListNode dummy(0);
        ListNode* current = &dummy;
        
        while (!pq.empty()) {
            ListNode* node = pq.top();
            pq.pop();
            
            current->next = node;
            current = current->next;
            
            if (node->next) {
                pq.push(node->next);
            }
        }
        
        return dummy.next;
    }
    
private:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* current = &dummy;
        
        while (l1 && l2) {
            if (l1->val <= l2->val) {
                current->next = l1;
                l1 = l1->next;
            } else {
                current->next = l2;
                l2 = l2->next;
            }
            current = current->next;
        }
        
        current->next = l1 ? l1 : l2;
        return dummy.next;
    }
};
```

**面试考点**:
- 分治思想
- 优先队列应用
- 链表合并技巧

## 🔥 高频题 (Top 21-70)

### 数组和字符串

#### 11. 盛最多水的容器 (LeetCode 11) ⭐⭐⭐⭐

```cpp
class Solution {
public:
    int maxArea(vector<int>& height) {
        int left = 0, right = height.size() - 1;
        int maxArea = 0;
        
        while (left < right) {
            int area = min(height[left], height[right]) * (right - left);
            maxArea = max(maxArea, area);
            
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

#### 12. 最长公共前缀 (LeetCode 14) ⭐⭐⭐⭐

```cpp
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) return "";
        
        string prefix = strs[0];
        
        for (int i = 1; i < strs.size(); i++) {
            while (strs[i].find(prefix) != 0) {
                prefix = prefix.substr(0, prefix.length() - 1);
                if (prefix.empty()) return "";
            }
        }
        
        return prefix;
    }
    
    // 垂直扫描版本
    string longestCommonPrefixVertical(vector<string>& strs) {
        if (strs.empty()) return "";
        
        for (int i = 0; i < strs[0].length(); i++) {
            char c = strs[0][i];
            for (int j = 1; j < strs.size(); j++) {
                if (i >= strs[j].length() || strs[j][i] != c) {
                    return strs[0].substr(0, i);
                }
            }
        }
        
        return strs[0];
    }
};
```

#### 13. 字母异位词分组 (LeetCode 49) ⭐⭐⭐⭐

```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> groups;
        
        for (string& str : strs) {
            string key = str;
            sort(key.begin(), key.end());
            groups[key].push_back(str);
        }
        
        vector<vector<string>> result;
        for (auto& [key, group] : groups) {
            result.push_back(group);
        }
        
        return result;
    }
    
    // 计数法版本
    vector<vector<string>> groupAnagramsCount(vector<string>& strs) {
        unordered_map<string, vector<string>> groups;
        
        for (string& str : strs) {
            string key = getKey(str);
            groups[key].push_back(str);
        }
        
        vector<vector<string>> result;
        for (auto& [key, group] : groups) {
            result.push_back(group);
        }
        
        return result;
    }
    
private:
    string getKey(const string& str) {
        vector<int> count(26, 0);
        for (char c : str) {
            count[c - 'a']++;
        }
        
        string key = "";
        for (int i = 0; i < 26; i++) {
            key += to_string(count[i]) + "#";
        }
        
        return key;
    }
};
```

### 链表

#### 14. 环形链表 II (LeetCode 142) ⭐⭐⭐⭐

```cpp
class Solution {
public:
    ListNode *detectCycle(ListNode *head) {
        if (!head || !head->next) return nullptr;
        
        // 第一阶段：检测是否有环
        ListNode* slow = head;
        ListNode* fast = head;
        
        do {
            slow = slow->next;
            fast = fast->next->next;
        } while (fast && fast->next && slow != fast);
        
        if (!fast || !fast->next) return nullptr;
        
        // 第二阶段：找到环的起点
        slow = head;
        while (slow != fast) {
            slow = slow->next;
            fast = fast->next;
        }
        
        return slow;
    }
};
```

#### 15. 相交链表 (LeetCode 160) ⭐⭐⭐⭐

```cpp
class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        if (!headA || !headB) return nullptr;
        
        ListNode* pA = headA;
        ListNode* pB = headB;
        
        while (pA != pB) {
            pA = pA ? pA->next : headB;
            pB = pB ? pB->next : headA;
        }
        
        return pA;
    }
    
    // 长度差法
    ListNode *getIntersectionNodeLength(ListNode *headA, ListNode *headB) {
        int lenA = getLength(headA);
        int lenB = getLength(headB);
        
        // 让长的链表先走差值步
        while (lenA > lenB) {
            headA = headA->next;
            lenA--;
        }
        while (lenB > lenA) {
            headB = headB->next;
            lenB--;
        }
        
        // 同时移动直到相遇
        while (headA && headB) {
            if (headA == headB) return headA;
            headA = headA->next;
            headB = headB->next;
        }
        
        return nullptr;
    }
    
private:
    int getLength(ListNode* head) {
        int length = 0;
        while (head) {
            length++;
            head = head->next;
        }
        return length;
    }
};
```

### 二叉树

#### 16. 二叉树的最大深度 (LeetCode 104) ⭐⭐⭐⭐

```cpp
class Solution {
public:
    int maxDepth(TreeNode* root) {
        if (!root) return 0;
        return 1 + max(maxDepth(root->left), maxDepth(root->right));
    }
    
    // 迭代版本
    int maxDepthIterative(TreeNode* root) {
        if (!root) return 0;
        
        queue<pair<TreeNode*, int>> q;
        q.push({root, 1});
        int maxDepth = 0;
        
        while (!q.empty()) {
            auto [node, depth] = q.front();
            q.pop();
            
            maxDepth = max(maxDepth, depth);
            
            if (node->left) q.push({node->left, depth + 1});
            if (node->right) q.push({node->right, depth + 1});
        }
        
        return maxDepth;
    }
};
```

#### 17. 对称二叉树 (LeetCode 101) ⭐⭐⭐⭐

```cpp
class Solution {
public:
    bool isSymmetric(TreeNode* root) {
        if (!root) return true;
        return isSymmetricHelper(root->left, root->right);
    }
    
private:
    bool isSymmetricHelper(TreeNode* left, TreeNode* right) {
        if (!left && !right) return true;
        if (!left || !right) return false;
        
        return left->val == right->val &&
               isSymmetricHelper(left->left, right->right) &&
               isSymmetricHelper(left->right, right->left);
    }
    
    // 迭代版本
    bool isSymmetricIterative(TreeNode* root) {
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
            
            q.push(left->left);
            q.push(right->right);
            q.push(left->right);
            q.push(right->left);
        }
        
        return true;
    }
};
```

## 🎯 公司特色高频题

### 字节跳动高频题

#### 18. 最长无重复字符的子串 (LeetCode 3) ⭐⭐⭐⭐⭐

```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_map<char, int> charIndex;
        int maxLength = 0;
        int start = 0;
        
        for (int end = 0; end < s.length(); end++) {
            if (charIndex.count(s[end]) && charIndex[s[end]] >= start) {
                start = charIndex[s[end]] + 1;
            }
            charIndex[s[end]] = end;
            maxLength = max(maxLength, end - start + 1);
        }
        
        return maxLength;
    }
    
    // 滑动窗口版本
    int lengthOfLongestSubstringSlidingWindow(string s) {
        vector<int> charSet(128, -1);
        int maxLength = 0;
        int start = 0;
        
        for (int end = 0; end < s.length(); end++) {
            if (charSet[s[end]] >= start) {
                start = charSet[s[end]] + 1;
            }
            charSet[s[end]] = end;
            maxLength = max(maxLength, end - start + 1);
        }
        
        return maxLength;
    }
};
```

#### 19. 岛屿数量 (LeetCode 200) ⭐⭐⭐⭐

```cpp
class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;
        
        int rows = grid.size();
        int cols = grid[0].size();
        int count = 0;
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] == '1') {
                    dfs(grid, i, j);
                    count++;
                }
            }
        }
        
        return count;
    }
    
private:
    void dfs(vector<vector<char>>& grid, int i, int j) {
        int rows = grid.size();
        int cols = grid[0].size();
        
        if (i < 0 || i >= rows || j < 0 || j >= cols || grid[i][j] == '0') {
            return;
        }
        
        grid[i][j] = '0';
        
        dfs(grid, i + 1, j);
        dfs(grid, i - 1, j);
        dfs(grid, i, j + 1);
        dfs(grid, i, j - 1);
    }
    
    // BFS版本
    int numIslandsBFS(vector<vector<char>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;
        
        int rows = grid.size();
        int cols = grid[0].size();
        int count = 0;
        
        vector<pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] == '1') {
                    grid[i][j] = '0';
                    queue<pair<int, int>> q;
                    q.push({i, j});
                    
                    while (!q.empty()) {
                        auto [x, y] = q.front();
                        q.pop();
                        
                        for (auto [dx, dy] : directions) {
                            int nx = x + dx;
                            int ny = y + dy;
                            
                            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && grid[nx][ny] == '1') {
                                grid[nx][ny] = '0';
                                q.push({nx, ny});
                            }
                        }
                    }
                    
                    count++;
                }
            }
        }
        
        return count;
    }
};
```

### 阿里巴巴高频题

#### 20. 排序链表 (LeetCode 148) ⭐⭐⭐⭐

```cpp
class Solution {
public:
    ListNode* sortList(ListNode* head) {
        if (!head || !head->next) return head;
        
        // 找到中点并分割
        ListNode* mid = getMid(head);
        ListNode* left = head;
        ListNode* right = mid->next;
        mid->next = nullptr;
        
        // 递归排序
        left = sortList(left);
        right = sortList(right);
        
        // 合并
        return merge(left, right);
    }
    
private:
    ListNode* getMid(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;
        ListNode* prev = nullptr;
        
        while (fast && fast->next) {
            prev = slow;
            slow = slow->next;
            fast = fast->next->next;
        }
        
        return prev;
    }
    
    ListNode* merge(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* current = &dummy;
        
        while (l1 && l2) {
            if (l1->val <= l2->val) {
                current->next = l1;
                l1 = l1->next;
            } else {
                current->next = l2;
                l2 = l2->next;
            }
            current = current->next;
        }
        
        current->next = l1 ? l1 : l2;
        return dummy.next;
    }
    
    // 自底向上归并排序（O(1)空间）
    ListNode* sortListBottomUp(ListNode* head) {
        if (!head || !head->next) return head;
        
        int length = getLength(head);
        ListNode dummy(0);
        dummy.next = head;
        
        for (int size = 1; size < length; size *= 2) {
            ListNode* prev = &dummy;
            ListNode* current = dummy.next;
            
            while (current) {
                ListNode* left = current;
                ListNode* right = split(left, size);
                current = split(right, size);
                
                prev = merge(prev, left, right);
            }
        }
        
        return dummy.next;
    }
    
    int getLength(ListNode* head) {
        int length = 0;
        while (head) {
            length++;
            head = head->next;
        }
        return length;
    }
    
    ListNode* split(ListNode* head, int size) {
        while (head && --size > 0) {
            head = head->next;
        }
        
        if (!head) return nullptr;
        
        ListNode* next = head->next;
        head->next = nullptr;
        return next;
    }
    
    ListNode* merge(ListNode* prev, ListNode* l1, ListNode* l2) {
        while (l1 && l2) {
            if (l1->val <= l2->val) {
                prev->next = l1;
                l1 = l1->next;
            } else {
                prev->next = l2;
                l2 = l2->next;
            }
            prev = prev->next;
        }
        
        prev->next = l1 ? l1 : l2;
        while (prev->next) {
            prev = prev->next;
        }
        
        return prev;
    }
};
```

## 📊 面试准备策略

### 1. 按难度分层练习

#### Easy题目策略 (30%时间)
- **目标**: 5分钟内完成
- **重点**: 语法熟练度、基本思路
- **练习量**: 每天3-5题

#### Medium题目策略 (50%时间)
- **目标**: 15-20分钟内完成
- **重点**: 算法思想、代码优化
- **练习量**: 每天2-3题

#### Hard题目策略 (20%时间)
- **目标**: 30-40分钟内完成
- **重点**: 复杂算法、边界处理
- **练习量**: 每周3-5题

### 2. 按公司准备

#### 大厂通用题目
- 数组、链表、二叉树基础操作
- DFS、BFS、动态规划
- 双指针、滑动窗口

#### 字节跳动偏好
- 字符串处理
- 图论算法
- 系统设计相关

#### 阿里巴巴偏好
- 排序算法
- 海量数据处理
- 分布式系统

#### 腾讯偏好
- 动态规划
- 树形结构
- 游戏相关算法

### 3. 时间安排

#### 面试前1个月
- **周1-2**: 超高频题 (20题)
- **周3-4**: 高频题 (50题)

#### 面试前1周
- **复习**: 所有做过的题目
- **模拟**: 每天1-2轮模拟面试
- **总结**: 整理解题模板

### 4. 面试技巧

#### 解题步骤
1. **理解题意**: 询问clarification questions
2. **分析复杂度**: 说出时间和空间复杂度
3. **设计算法**: 先说思路，再写代码
4. **测试验证**: 用例子验证算法正确性
5. **优化讨论**: 讨论可能的优化方案

#### 沟通技巧
- **思路清晰**: 边写边解释
- **处理压力**: 遇到困难时保持冷静
- **时间管理**: 合理分配时间
- **代码质量**: 注意变量命名和代码结构

## 🔥 最后冲刺清单

### 必须掌握的算法
- [ ] 双指针（快慢指针、左右指针）
- [ ] 滑动窗口
- [ ] BFS/DFS
- [ ] 动态规划（线性DP、背包问题）
- [ ] 二分查找
- [ ] 排序算法
- [ ] 链表操作
- [ ] 树的遍历

### 必须掌握的数据结构
- [ ] 数组和字符串
- [ ] 链表（单链表、双链表）
- [ ] 栈和队列
- [ ] 哈希表
- [ ] 二叉树
- [ ] 堆（优先队列）
- [ ] 并查集

### 面试前检查
- [ ] 代码模板准备完毕
- [ ] 常见题型解法熟练
- [ ] 时间复杂度分析清楚
- [ ] 边界条件处理完善
- [ ] 代码风格统一

## 🚀 下一步
- 查看 [动态规划题详解](./dynamic-programming.md)
- 学习 [图论算法题详解](./graph-algorithms.md)
- 回顾 [数据结构题详解](./data-structure-problems.md)
- 练习 [基础算法题详解](./basic-algorithms.md)

---

**温馨提示**: 面试成功的关键不仅在于解题能力，更在于清晰的思路表达和良好的沟通能力。祝你面试顺利！🎉

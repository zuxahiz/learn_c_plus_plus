# LeetCode 动态规划题详解

## 🎯 动态规划概述

动态规划(Dynamic Programming)是算法设计中的重要思想，通过把原问题分解为相对简单的子问题的方式求解复杂问题。它适用于有最优子结构性质并且子问题重叠的问题。

### 核心思想
1. **状态定义**: 确定dp数组的含义
2. **状态转移方程**: 找出状态间的递推关系
3. **初始化**: 确定边界条件
4. **遍历顺序**: 确定计算顺序
5. **返回结果**: 确定最终答案

## 📚 1. 线性DP (Linear DP)

### 1.1 爬楼梯问题 (LeetCode 70)

**问题描述**: 假设你正在爬楼梯。需要 n 阶你才能到达楼顶。每次你可以爬 1 或 2 个台阶。你有多少种不同的方法可以爬到楼顶呢？

**解题思路**:
- 状态定义：`dp[i]` 表示爬到第i阶楼梯的方法数
- 状态转移：`dp[i] = dp[i-1] + dp[i-2]`
- 初始化：`dp[0] = 1, dp[1] = 1`

```cpp
class Solution {
public:
    int climbStairs(int n) {
        if (n <= 1) return 1;
        
        vector<int> dp(n + 1);
        dp[0] = 1;
        dp[1] = 1;
        
        for (int i = 2; i <= n; i++) {
            dp[i] = dp[i-1] + dp[i-2];
        }
        
        return dp[n];
    }
    
    // 优化空间复杂度版本
    int climbStairsOptimized(int n) {
        if (n <= 1) return 1;
        
        int prev2 = 1, prev1 = 1;
        int current;
        
        for (int i = 2; i <= n; i++) {
            current = prev1 + prev2;
            prev2 = prev1;
            prev1 = current;
        }
        
        return current;
    }
};
```

**复杂度分析**:
- 时间复杂度：O(n)
- 空间复杂度：O(n) -> O(1)

### 1.2 打家劫舍系列

#### 打家劫舍 I (LeetCode 198)

**问题描述**: 你是一个专业的小偷，计划偷窃沿街的房屋。每间房内都藏有一定的现金，影响你偷窃的唯一制约因素就是相邻的房屋装有相互连通的防盗系统。

```cpp
class Solution {
public:
    int rob(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        if (n == 1) return nums[0];
        
        vector<int> dp(n);
        dp[0] = nums[0];
        dp[1] = max(nums[0], nums[1]);
        
        for (int i = 2; i < n; i++) {
            dp[i] = max(dp[i-1], dp[i-2] + nums[i]);
        }
        
        return dp[n-1];
    }
    
    // 空间优化版本
    int robOptimized(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        if (n == 1) return nums[0];
        
        int prev2 = nums[0];
        int prev1 = max(nums[0], nums[1]);
        
        for (int i = 2; i < n; i++) {
            int current = max(prev1, prev2 + nums[i]);
            prev2 = prev1;
            prev1 = current;
        }
        
        return prev1;
    }
};
```

#### 打家劫舍 II (LeetCode 213)

**问题描述**: 所有的房屋都围成一圈，这意味着第一个房屋和最后一个房屋是紧挨着的。

```cpp
class Solution {
public:
    int rob(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        if (n == 1) return nums[0];
        if (n == 2) return max(nums[0], nums[1]);
        
        // 情况1：抢第一间房，不抢最后一间房
        int case1 = robLinear(nums, 0, n-2);
        
        // 情况2：不抢第一间房，可能抢最后一间房
        int case2 = robLinear(nums, 1, n-1);
        
        return max(case1, case2);
    }
    
private:
    int robLinear(vector<int>& nums, int start, int end) {
        int prev2 = 0, prev1 = 0;
        
        for (int i = start; i <= end; i++) {
            int current = max(prev1, prev2 + nums[i]);
            prev2 = prev1;
            prev1 = current;
        }
        
        return prev1;
    }
};
```

#### 打家劫舍 III (LeetCode 337)

**问题描述**: 小偷又发现了一个新的可行窃的地区。这个地区只有一个入口，我们称之为"根"。 除了"根"之外，每栋房子有且只有一个"父"房子与之相连。

```cpp
class Solution {
public:
    struct TreeNode {
        int val;
        TreeNode *left;
        TreeNode *right;
        TreeNode() : val(0), left(nullptr), right(nullptr) {}
        TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
        TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
    };
    
    int rob(TreeNode* root) {
        auto result = robHelper(root);
        return max(result.first, result.second);
    }
    
private:
    // 返回值：{不抢当前节点的最大收益, 抢当前节点的最大收益}
    pair<int, int> robHelper(TreeNode* node) {
        if (node == nullptr) {
            return {0, 0};
        }
        
        auto left = robHelper(node->left);
        auto right = robHelper(node->right);
        
        // 不抢当前节点：左右子树都可以自由选择抢或不抢
        int notRob = max(left.first, left.second) + max(right.first, right.second);
        
        // 抢当前节点：左右子树都不能抢根节点
        int rob = node->val + left.first + right.first;
        
        return {notRob, rob};
    }
};
```

### 1.3 买卖股票系列

#### 买卖股票的最佳时机 (LeetCode 121)

**问题描述**: 给定一个数组 prices ，它的第 i 个元素 prices[i] 表示一支给定股票第 i 天的价格。你只能选择某一天买入这只股票，并选择在未来的某一天卖出该股票。

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        if (n <= 1) return 0;
        
        int minPrice = prices[0];
        int maxProfit = 0;
        
        for (int i = 1; i < n; i++) {
            maxProfit = max(maxProfit, prices[i] - minPrice);
            minPrice = min(minPrice, prices[i]);
        }
        
        return maxProfit;
    }
    
    // DP版本
    int maxProfitDP(vector<int>& prices) {
        int n = prices.size();
        if (n <= 1) return 0;
        
        // dp[i][0] 表示第i天不持有股票的最大利润
        // dp[i][1] 表示第i天持有股票的最大利润
        vector<vector<int>> dp(n, vector<int>(2, 0));
        
        dp[0][0] = 0;           // 第0天不持有股票
        dp[0][1] = -prices[0];  // 第0天持有股票
        
        for (int i = 1; i < n; i++) {
            dp[i][0] = max(dp[i-1][0], dp[i-1][1] + prices[i]);  // 不持有：昨天就不持有 或 昨天持有今天卖出
            dp[i][1] = max(dp[i-1][1], -prices[i]);             // 持有：昨天就持有 或 今天买入
        }
        
        return dp[n-1][0];
    }
};
```

#### 买卖股票的最佳时机 II (LeetCode 122)

**问题描述**: 可以多次买卖股票。

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        if (n <= 1) return 0;
        
        // 贪心解法：只要今天价格比昨天高就卖出
        int profit = 0;
        for (int i = 1; i < n; i++) {
            if (prices[i] > prices[i-1]) {
                profit += prices[i] - prices[i-1];
            }
        }
        return profit;
    }
    
    // DP解法
    int maxProfitDP(vector<int>& prices) {
        int n = prices.size();
        if (n <= 1) return 0;
        
        vector<vector<int>> dp(n, vector<int>(2, 0));
        dp[0][0] = 0;
        dp[0][1] = -prices[0];
        
        for (int i = 1; i < n; i++) {
            dp[i][0] = max(dp[i-1][0], dp[i-1][1] + prices[i]);
            dp[i][1] = max(dp[i-1][1], dp[i-1][0] - prices[i]);  // 可以多次买卖
        }
        
        return dp[n-1][0];
    }
};
```

#### 买卖股票的最佳时机 III (LeetCode 123)

**问题描述**: 最多可以完成两笔交易。

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        if (n <= 1) return 0;
        
        // buy1: 第一次买入后的最大利润
        // sell1: 第一次卖出后的最大利润
        // buy2: 第二次买入后的最大利润
        // sell2: 第二次卖出后的最大利润
        int buy1 = -prices[0], sell1 = 0;
        int buy2 = -prices[0], sell2 = 0;
        
        for (int i = 1; i < n; i++) {
            buy1 = max(buy1, -prices[i]);
            sell1 = max(sell1, buy1 + prices[i]);
            buy2 = max(buy2, sell1 - prices[i]);
            sell2 = max(sell2, buy2 + prices[i]);
        }
        
        return sell2;
    }
    
    // 通用DP解法
    int maxProfitGeneral(vector<int>& prices) {
        int n = prices.size();
        int k = 2; // 最多2次交易
        
        if (n <= 1) return 0;
        
        // dp[i][j][0] 表示第i天，最多j次交易，当前不持有股票的最大利润
        // dp[i][j][1] 表示第i天，最多j次交易，当前持有股票的最大利润
        vector<vector<vector<int>>> dp(n, vector<vector<int>>(k+1, vector<int>(2, 0)));
        
        // 初始化
        for (int j = 0; j <= k; j++) {
            dp[0][j][0] = 0;
            dp[0][j][1] = -prices[0];
        }
        
        for (int i = 1; i < n; i++) {
            for (int j = 1; j <= k; j++) {
                dp[i][j][0] = max(dp[i-1][j][0], dp[i-1][j][1] + prices[i]);
                dp[i][j][1] = max(dp[i-1][j][1], dp[i-1][j-1][0] - prices[i]);
            }
        }
        
        return dp[n-1][k][0];
    }
};
```

## 📚 2. 区间DP (Interval DP)

### 2.1 戳气球 (LeetCode 312)

**问题描述**: 有 n 个气球，编号为0 到 n-1，每个气球上都标有一个数字，这些数字存在数组 nums 中。现在要求你戳破所有的气球。

```cpp
class Solution {
public:
    int maxCoins(vector<int>& nums) {
        int n = nums.size();
        // 在首尾添加虚拟气球
        vector<int> points(n + 2);
        points[0] = points[n + 1] = 1;
        for (int i = 1; i <= n; i++) {
            points[i] = nums[i - 1];
        }
        
        // dp[i][j] 表示戳破开区间 (i,j) 内所有气球能获得的最大硬币数
        vector<vector<int>> dp(n + 2, vector<int>(n + 2, 0));
        
        // 区间长度从小到大
        for (int len = 3; len <= n + 2; len++) {
            for (int i = 0; i <= n + 2 - len; i++) {
                int j = i + len - 1;
                // 枚举最后戳破的气球
                for (int k = i + 1; k < j; k++) {
                    dp[i][j] = max(dp[i][j], 
                                   dp[i][k] + dp[k][j] + points[i] * points[k] * points[j]);
                }
            }
        }
        
        return dp[0][n + 1];
    }
};
```

**解题关键**:
1. 将问题转化为"最后戳破"的问题
2. 添加虚拟边界简化边界处理
3. 区间DP：先计算小区间，再计算大区间

### 2.2 石子游戏 (LeetCode 877)

**问题描述**: 亚历克斯和李用几堆石子在做游戏。偶数堆石子排成一行，每堆都有正整数颗石子 piles[i]。

```cpp
class Solution {
public:
    bool stoneGame(vector<int>& piles) {
        int n = piles.size();
        // dp[i][j] 表示当剩下的石子堆为下标 i 到下标 j 时，当前玩家与另一个玩家的石子数量之差的最大值
        vector<vector<int>> dp(n, vector<int>(n, 0));
        
        // 初始化：只有一堆石子时，当前玩家直接拿走
        for (int i = 0; i < n; i++) {
            dp[i][i] = piles[i];
        }
        
        // 区间长度从2开始
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1;
                // 当前玩家选择拿走左边或右边的石子堆
                dp[i][j] = max(piles[i] - dp[i+1][j], piles[j] - dp[i][j-1]);
            }
        }
        
        return dp[0][n-1] > 0;
    }
    
    // 简化版本：由于是偶数堆，先手必胜
    bool stoneGameSimple(vector<int>& piles) {
        return true;  // 数学证明：先手可以选择所有奇数位置或所有偶数位置
    }
};
```

### 2.3 合并石头的最低成本 (LeetCode 1000)

**问题描述**: 有 N 堆石头排成一排，第 i 堆中有 stones[i] 块石头。每次移动（move）需要将连续的 K 堆石头合并为一堆。

```cpp
class Solution {
public:
    int mergeStones(vector<int>& stones, int k) {
        int n = stones.size();
        if ((n - 1) % (k - 1) != 0) return -1;
        
        // 前缀和数组
        vector<int> prefixSum(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefixSum[i + 1] = prefixSum[i] + stones[i];
        }
        
        // dp[i][j][t] 表示将区间[i,j]合并成t堆的最小成本
        vector<vector<vector<int>>> dp(n, vector<vector<int>>(n, vector<int>(k + 1, INT_MAX)));
        
        // 初始化：每个位置合并成1堆的成本为0
        for (int i = 0; i < n; i++) {
            dp[i][i][1] = 0;
        }
        
        // 枚举区间长度
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1;
                
                // 合并成t堆 (t > 1)
                for (int t = 2; t <= k; t++) {
                    for (int mid = i; mid < j; mid += k - 1) {
                        if (dp[i][mid][1] != INT_MAX && dp[mid + 1][j][t - 1] != INT_MAX) {
                            dp[i][j][t] = min(dp[i][j][t], dp[i][mid][1] + dp[mid + 1][j][t - 1]);
                        }
                    }
                }
                
                // 合并成1堆
                if (dp[i][j][k] != INT_MAX) {
                    dp[i][j][1] = dp[i][j][k] + prefixSum[j + 1] - prefixSum[i];
                }
            }
        }
        
        return dp[0][n - 1][1];
    }
};
```

## 📚 3. 背包问题

### 3.1 0-1背包问题

**问题模板**: 给定n个物品，每个物品有重量和价值，背包容量为W，求背包能装下的最大价值。

```cpp
class Knapsack01 {
public:
    int knapsack(vector<int>& weights, vector<int>& values, int W) {
        int n = weights.size();
        // dp[i][w] 表示前i个物品，背包容量为w时的最大价值
        vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));
        
        for (int i = 1; i <= n; i++) {
            for (int w = 1; w <= W; w++) {
                // 不选第i个物品
                dp[i][w] = dp[i-1][w];
                
                // 选第i个物品（如果能放下）
                if (w >= weights[i-1]) {
                    dp[i][w] = max(dp[i][w], dp[i-1][w-weights[i-1]] + values[i-1]);
                }
            }
        }
        
        return dp[n][W];
    }
    
    // 空间优化版本
    int knapsackOptimized(vector<int>& weights, vector<int>& values, int W) {
        int n = weights.size();
        vector<int> dp(W + 1, 0);
        
        for (int i = 0; i < n; i++) {
            // 逆序遍历，避免重复使用
            for (int w = W; w >= weights[i]; w--) {
                dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
            }
        }
        
        return dp[W];
    }
};
```

### 3.2 完全背包问题

**问题模板**: 每个物品可以无限次使用。

```cpp
class KnapsackComplete {
public:
    int knapsack(vector<int>& weights, vector<int>& values, int W) {
        int n = weights.size();
        vector<int> dp(W + 1, 0);
        
        for (int i = 0; i < n; i++) {
            // 正序遍历，允许重复使用
            for (int w = weights[i]; w <= W; w++) {
                dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
            }
        }
        
        return dp[W];
    }
};
```

### 3.3 多重背包问题

**问题模板**: 每个物品有数量限制。

```cpp
class KnapsackMultiple {
public:
    int knapsack(vector<int>& weights, vector<int>& values, vector<int>& counts, int W) {
        int n = weights.size();
        vector<int> dp(W + 1, 0);
        
        for (int i = 0; i < n; i++) {
            // 二进制优化
            vector<int> newWeights, newValues;
            int count = counts[i];
            int k = 1;
            
            while (count > 0) {
                int take = min(k, count);
                newWeights.push_back(take * weights[i]);
                newValues.push_back(take * values[i]);
                count -= take;
                k *= 2;
            }
            
            // 转化为0-1背包
            for (int j = 0; j < newWeights.size(); j++) {
                for (int w = W; w >= newWeights[j]; w--) {
                    dp[w] = max(dp[w], dp[w - newWeights[j]] + newValues[j]);
                }
            }
        }
        
        return dp[W];
    }
};
```

### 3.4 分割等和子集 (LeetCode 416)

```cpp
class Solution {
public:
    bool canPartition(vector<int>& nums) {
        int sum = accumulate(nums.begin(), nums.end(), 0);
        if (sum % 2 == 1) return false;
        
        int target = sum / 2;
        vector<bool> dp(target + 1, false);
        dp[0] = true;
        
        for (int num : nums) {
            for (int j = target; j >= num; j--) {
                dp[j] = dp[j] || dp[j - num];
            }
        }
        
        return dp[target];
    }
};
```

## 📚 4. 树形DP

### 4.1 二叉树中的最大路径和 (LeetCode 124)

```cpp
class Solution {
public:
    int maxPathSum(TreeNode* root) {
        int maxSum = INT_MIN;
        helper(root, maxSum);
        return maxSum;
    }
    
private:
    int helper(TreeNode* node, int& maxSum) {
        if (node == nullptr) return 0;
        
        // 计算左右子树的最大贡献值（负数时取0）
        int leftGain = max(helper(node->left, maxSum), 0);
        int rightGain = max(helper(node->right, maxSum), 0);
        
        // 计算经过当前节点的最大路径和
        int currentMaxPath = node->val + leftGain + rightGain;
        
        // 更新全局最大值
        maxSum = max(maxSum, currentMaxPath);
        
        // 返回当前节点能为其父节点提供的最大贡献值
        return node->val + max(leftGain, rightGain);
    }
};
```

### 4.2 监控二叉树 (LeetCode 968)

```cpp
class Solution {
public:
    int minCameraCover(TreeNode* root) {
        int cameras = 0;
        int result = dfs(root, cameras);
        
        // 如果根节点未被覆盖，需要放置摄像头
        if (result == 0) cameras++;
        
        return cameras;
    }
    
private:
    // 返回值：0-未被覆盖，1-已覆盖但无摄像头，2-有摄像头
    int dfs(TreeNode* node, int& cameras) {
        if (node == nullptr) return 1; // 空节点视为已覆盖
        
        int left = dfs(node->left, cameras);
        int right = dfs(node->right, cameras);
        
        // 如果子节点有未被覆盖的，当前节点必须放摄像头
        if (left == 0 || right == 0) {
            cameras++;
            return 2;
        }
        
        // 如果子节点有摄像头，当前节点被覆盖
        if (left == 2 || right == 2) {
            return 1;
        }
        
        // 子节点都被覆盖但没有摄像头，当前节点未被覆盖
        return 0;
    }
};
```

## 📚 5. 状态压缩DP

### 5.1 旅行商问题 (TSP)

```cpp
class TSP {
public:
    int tsp(vector<vector<int>>& dist) {
        int n = dist.size();
        // dp[mask][i] 表示访问了mask集合中的城市，当前在城市i的最小距离
        vector<vector<int>> dp(1 << n, vector<int>(n, INT_MAX));
        
        // 从城市0开始
        dp[1][0] = 0;
        
        for (int mask = 1; mask < (1 << n); mask++) {
            for (int u = 0; u < n; u++) {
                if (!(mask & (1 << u)) || dp[mask][u] == INT_MAX) continue;
                
                for (int v = 0; v < n; v++) {
                    if (mask & (1 << v)) continue;
                    
                    int newMask = mask | (1 << v);
                    dp[newMask][v] = min(dp[newMask][v], dp[mask][u] + dist[u][v]);
                }
            }
        }
        
        int result = INT_MAX;
        for (int i = 1; i < n; i++) {
            if (dp[(1 << n) - 1][i] != INT_MAX) {
                result = min(result, dp[(1 << n) - 1][i] + dist[i][0]);
            }
        }
        
        return result;
    }
};
```

### 5.2 最短超级串 (LeetCode 943)

```cpp
class Solution {
public:
    string shortestSuperstring(vector<string>& words) {
        int n = words.size();
        
        // 计算重叠长度
        vector<vector<int>> overlap(n, vector<int>(n, 0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    overlap[i][j] = getOverlap(words[i], words[j]);
                }
            }
        }
        
        // dp[mask][i] 表示访问了mask集合中的字符串，最后一个是i的最小长度
        vector<vector<int>> dp(1 << n, vector<int>(n, INT_MAX));
        vector<vector<int>> parent(1 << n, vector<int>(n, -1));
        
        // 初始化
        for (int i = 0; i < n; i++) {
            dp[1 << i][i] = words[i].length();
        }
        
        for (int mask = 1; mask < (1 << n); mask++) {
            for (int i = 0; i < n; i++) {
                if (!(mask & (1 << i)) || dp[mask][i] == INT_MAX) continue;
                
                for (int j = 0; j < n; j++) {
                    if (mask & (1 << j)) continue;
                    
                    int newMask = mask | (1 << j);
                    int newLen = dp[mask][i] + words[j].length() - overlap[i][j];
                    
                    if (newLen < dp[newMask][j]) {
                        dp[newMask][j] = newLen;
                        parent[newMask][j] = i;
                    }
                }
            }
        }
        
        // 找到最优解
        int finalMask = (1 << n) - 1;
        int last = 0;
        for (int i = 1; i < n; i++) {
            if (dp[finalMask][i] < dp[finalMask][last]) {
                last = i;
            }
        }
        
        // 重构答案
        vector<int> path;
        int mask = finalMask;
        while (last != -1) {
            path.push_back(last);
            int prev = parent[mask][last];
            mask ^= (1 << last);
            last = prev;
        }
        
        reverse(path.begin(), path.end());
        
        string result = words[path[0]];
        for (int i = 1; i < path.size(); i++) {
            int prev = path[i-1];
            int curr = path[i];
            result += words[curr].substr(overlap[prev][curr]);
        }
        
        return result;
    }
    
private:
    int getOverlap(const string& a, const string& b) {
        int maxOverlap = 0;
        int minLen = min(a.length(), b.length());
        
        for (int i = 1; i <= minLen; i++) {
            if (a.substr(a.length() - i) == b.substr(0, i)) {
                maxOverlap = i;
            }
        }
        
        return maxOverlap;
    }
};
```

## 💡 动态规划解题技巧

### 1. 状态定义技巧
- **明确状态含义**: 状态应该包含解决问题所需的最少信息
- **降维思考**: 考虑是否可以优化空间复杂度
- **边界处理**: 仔细处理边界条件和初始状态

### 2. 状态转移技巧
- **最优子结构**: 确保子问题的最优解能构成原问题的最优解
- **无后效性**: 当前状态只依赖于之前的状态，不依赖于未来
- **重叠子问题**: 确保存在重复计算的子问题

### 3. 优化技巧
- **滚动数组**: 当只需要前几个状态时，可以使用滚动数组优化空间
- **状态压缩**: 当状态数量较少时，可以用位运算表示状态
- **记忆化搜索**: 递归+记忆化，代码更直观

### 4. 常见陷阱
- **初始化错误**: 边界条件设置不当
- **遍历顺序错误**: 特别是在空间优化时
- **状态定义不完整**: 缺少必要的维度信息

## 🔥 经典题目推荐

### 入门级 (Easy)
1. 爬楼梯 (70)
2. 最大子序和 (53)
3. 打家劫舍 (198)
4. 买卖股票的最佳时机 (121)

### 进阶级 (Medium)
1. 最长递增子序列 (300)
2. 不同路径 (62)
3. 最长公共子序列 (1143)
4. 分割等和子集 (416)
5. 完全平方数 (279)

### 挑战级 (Hard)
1. 编辑距离 (72)
2. 最大矩形 (85)
3. 戳气球 (312)
4. 最短超级串 (943)
5. 合并石头的最低成本 (1000)

## 🚀 下一步
- 练习 [图论算法题详解](./graph-algorithms.md)
- 学习 [面试高频题](./interview-high-frequency.md)
- 回顾 [数据结构题详解](./data-structure-problems.md)
- 查看 [基础算法题详解](./basic-algorithms.md)

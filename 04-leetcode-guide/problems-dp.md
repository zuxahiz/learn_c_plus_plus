# 🧮 动态规划问题完整列表

## 📈 统计信息
- **总题目数**: ~500题
- **Easy**: ~80题
- **Medium**: ~300题  
- **Hard**: ~120题
- **高频面试题**: ~100题

## 🎯 核心知识点
- 线性DP (一维状态)
- 区间DP (区间状态)
- 背包DP (容量状态)
- 树形DP (树状态)
- 状态压缩DP (位状态)
- 数位DP (数字状态)
- 概率DP (概率状态)

---

## 📚 分类题目列表

### 🔰 线性DP (Easy-Medium 150题)

#### 基础一维DP
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 70 | Climbing Stairs | Easy | ⭐⭐⭐ | 基础DP |
| 198 | House Robber | Easy | ⭐⭐⭐ | 线性DP |
| 213 | House Robber II | Medium | ⭐⭐ | 环形DP |
| 337 | House Robber III | Medium | ⭐⭐ | 树形DP |
| 53 | Maximum Subarray | Easy | ⭐⭐⭐ | Kadane算法 |
| 121 | Best Time to Buy and Sell Stock | Easy | ⭐⭐⭐ | 状态机DP |
| 122 | Best Time to Buy and Sell Stock II | Easy | ⭐⭐ | 贪心/DP |
| 123 | Best Time to Buy and Sell Stock III | Hard | ⭐⭐ | 状态机DP |
| 188 | Best Time to Buy and Sell Stock IV | Hard | ⭐⭐ | 状态机DP |
| 309 | Best Time to Buy and Sell Stock with Cooldown | Medium | ⭐⭐ | 状态机DP |
| 714 | Best Time to Buy and Sell Stock with Fee | Medium | ⭐⭐ | 状态机DP |
| 746 | Min Cost Climbing Stairs | Easy | ⭐⭐ | 基础DP |
| 392 | Is Subsequence | Easy | ⭐⭐ | 双指针/DP |
| 413 | Arithmetic Slices | Medium | ⭐ | 线性DP |
| 446 | Arithmetic Slices II | Hard | ⭐ | 线性DP |

#### 最长子序列类
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 300 | Longest Increasing Subsequence | Medium | ⭐⭐⭐ | LIS |
| 354 | Russian Doll Envelopes | Hard | ⭐⭐ | LIS应用 |
| 368 | Largest Divisible Subset | Medium | ⭐ | LIS变种 |
| 376 | Wiggle Subsequence | Medium | ⭐⭐ | 状态DP |
| 646 | Maximum Length of Pair Chain | Medium | ⭐ | LIS变种 |
| 673 | Number of Longest Increasing Subsequence | Medium | ⭐⭐ | LIS计数 |
| 1048 | Longest String Chain | Medium | ⭐ | LIS应用 |
| 1143 | Longest Common Subsequence | Medium | ⭐⭐⭐ | LCS |
| 1035 | Uncrossed Lines | Medium | ⭐ | LCS变种 |
| 516 | Longest Palindromic Subsequence | Medium | ⭐⭐ | 回文DP |
| 712 | Minimum ASCII Delete Sum | Medium | ⭐ | LCS变种 |
| 583 | Delete Operation for Two Strings | Medium | ⭐ | LCS应用 |
| 718 | Maximum Length of Repeated Subarray | Medium | ⭐ | 连续LCS |
| 1092 | Shortest Common Supersequence | Hard | ⭐ | LCS应用 |
| 1312 | Minimum Insertion Steps to Make a String Palindrome | Hard | ⭐ | 回文DP |

#### 路径问题类
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 62 | Unique Paths | Medium | ⭐⭐⭐ | 路径DP |
| 63 | Unique Paths II | Medium | ⭐⭐ | 路径DP |
| 64 | Minimum Path Sum | Medium | ⭐⭐⭐ | 路径DP |
| 120 | Triangle | Medium | ⭐⭐ | 路径DP |
| 174 | Dungeon Game | Hard | ⭐⭐ | 逆向DP |
| 931 | Minimum Falling Path Sum | Medium | ⭐⭐ | 路径DP |
| 1289 | Minimum Falling Path Sum II | Hard | ⭐ | 路径DP |
| 1594 | Maximum Non Negative Product in a Matrix | Medium | ⭐ | 路径DP |
| 576 | Out of Boundary Paths | Medium | ⭐ | 路径计数 |
| 688 | Knight Probability in Chessboard | Medium | ⭐ | 概率DP |
| 935 | Knight Dialer | Medium | ⭐ | 路径计数 |
| 1301 | Number of Paths with Max Score | Hard | ⭐ | 路径DP |
| 1463 | Cherry Pickup II | Hard | ⭐ | 多维路径DP |
| 741 | Cherry Pickup | Hard | ⭐⭐ | 三维路径DP |
| 1937 | Maximum Number of Points with Cost | Medium | ⭐ | 路径DP |

### 🔶 背包DP (Medium-Hard 100题)

#### 01背包类
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 416 | Partition Equal Subset Sum | Medium | ⭐⭐⭐ | 01背包 |
| 494 | Target Sum | Medium | ⭐⭐⭐ | 01背包变种 |
| 1049 | Last Stone Weight II | Medium | ⭐⭐ | 01背包 |
| 698 | Partition to K Equal Sum Subsets | Medium | ⭐⭐ | 回溯+DP |
| 473 | Matchsticks to Square | Medium | ⭐ | 回溯+DP |
| 805 | Split Array With Same Average | Hard | ⭐ | 01背包 |
| 956 | Tallest Billboard | Hard | ⭐ | 背包变种 |
| 1049 | Last Stone Weight II | Medium | ⭐⭐ | 01背包 |
| 1774 | Closest Dessert Cost | Medium | ⭐ | 01背包 |
| 1856 | Maximum Subarray Min-Product | Medium | ⭐ | 单调栈+前缀和 |

#### 完全背包类
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 322 | Coin Change | Medium | ⭐⭐⭐ | 完全背包 |
| 518 | Coin Change 2 | Medium | ⭐⭐⭐ | 完全背包计数 |
| 139 | Word Break | Medium | ⭐⭐⭐ | 完全背包变种 |
| 140 | Word Break II | Hard | ⭐⭐ | 完全背包+回溯 |
| 279 | Perfect Squares | Medium | ⭐⭐ | 完全背包 |
| 377 | Combination Sum IV | Medium | ⭐⭐ | 完全背包排列 |
| 343 | Integer Break | Medium | ⭐⭐ | 数学DP |
| 1449 | Form Largest Integer With Digits That Add up to Target | Hard | ⭐ | 完全背包 |
| 983 | Minimum Cost For Tickets | Medium | ⭐⭐ | DP选择 |
| 1155 | Number of Dice Rolls With Target Sum | Medium | ⭐⭐ | 完全背包 |

#### 多重背包类
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 474 | Ones and Zeroes | Medium | ⭐⭐ | 二维背包 |
| 879 | Profitable Schemes | Hard | ⭐ | 二维背包 |
| 1641 | Count Sorted Vowel Strings | Medium | ⭐ | 组合DP |
| 1048 | Longest String Chain | Medium | ⭐ | 链式DP |
| 1473 | Paint House III | Hard | ⭐ | 多维状态DP |
| 1547 | Minimum Cost to Cut a Stick | Hard | ⭐ | 区间DP |
| 1531 | String Compression II | Hard | ⭐ | 复杂状态DP |
| 1690 | Stone Game VII | Medium | ⭐ | 博弈DP |
| 1770 | Maximum Score from Performing Multiplication Operations | Medium | ⭐ | 区间选择DP |
| 1866 | Number of Ways to Rearrange Sticks With K Sticks Visible | Hard | ⭐ | 排列DP |

### 🔴 区间DP (Hard 80题)

#### 基础区间DP
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 5 | Longest Palindromic Substring | Medium | ⭐⭐⭐ | 中心扩展/DP |
| 647 | Palindromic Substrings | Medium | ⭐⭐ | 回文DP |
| 516 | Longest Palindromic Subsequence | Medium | ⭐⭐ | 回文DP |
| 132 | Palindrome Partitioning II | Hard | ⭐⭐ | 区间DP |
| 131 | Palindrome Partitioning | Medium | ⭐⭐ | 回溯+DP |
| 1312 | Minimum Insertion Steps to Make a String Palindrome | Hard | ⭐ | 回文DP |
| 1246 | Palindrome Removal | Hard | ⭐ | 区间DP |
| 1278 | Palindrome Partitioning III | Hard | ⭐ | 区间DP |
| 730 | Count Different Palindromic Subsequences | Hard | ⭐ | 回文DP |
| 1147 | Longest Chunked Palindrome Decomposition | Hard | ⭐ | 贪心+DP |

#### 经典区间DP
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 312 | Burst Balloons | Hard | ⭐⭐⭐ | 区间DP |
| 87 | Scramble String | Hard | ⭐ | 区间DP |
| 96 | Unique Binary Search Trees | Medium | ⭐⭐ | 卡特兰数 |
| 95 | Unique Binary Search Trees II | Medium | ⭐⭐ | 区间DP+构造 |
| 375 | Guess Number Higher or Lower II | Medium | ⭐ | 极小极大DP |
| 546 | Remove Boxes | Hard | ⭐ | 区间DP |
| 664 | Strange Printer | Hard | ⭐ | 区间DP |
| 877 | Stone Game | Medium | ⭐⭐ | 博弈DP |
| 1000 | Minimum Cost to Merge Stones | Hard | ⭐ | 区间DP |
| 1039 | Minimum Score Triangulation of Polygon | Medium | ⭐ | 区间DP |

#### 博弈DP类
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 486 | Predict the Winner | Medium | ⭐ | 博弈DP |
| 877 | Stone Game | Medium | ⭐⭐ | 博弈DP |
| 1140 | Stone Game II | Medium | ⭐ | 博弈DP |
| 1406 | Stone Game III | Hard | ⭐ | 博弈DP |
| 1510 | Stone Game IV | Hard | ⭐ | 博弈DP |
| 1563 | Stone Game V | Hard | ⭐ | 博弈DP |
| 1686 | Stone Game VI | Medium | ⭐ | 贪心+博弈 |
| 1690 | Stone Game VII | Medium | ⭐ | 博弈DP |
| 1872 | Stone Game VIII | Hard | ⭐ | 博弈DP |
| 1927 | Sum Game | Medium | ⭐ | 博弈理论 |

### 🌟 树形DP (Medium-Hard 60题)

#### 基础树形DP
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 337 | House Robber III | Medium | ⭐⭐ | 树形DP |
| 543 | Diameter of Binary Tree | Easy | ⭐⭐⭐ | 树形DP |
| 124 | Binary Tree Maximum Path Sum | Hard | ⭐⭐⭐ | 树形DP |
| 968 | Binary Tree Cameras | Hard | ⭐⭐ | 树形DP |
| 979 | Distribute Coins in Binary Tree | Medium | ⭐⭐ | 树形DP |
| 1617 | Count Subtrees With Max Distance Between Cities | Hard | ⭐ | 树形DP |
| 834 | Sum of Distances in Tree | Hard | ⭐ | 换根DP |
| 1026 | Maximum Difference Between Node and Ancestor | Medium | ⭐ | 树形DP |
| 1372 | Longest ZigZag Path in a Binary Tree | Medium | ⭐ | 树形DP |
| 1339 | Maximum Product of Splitted Binary Tree | Medium | ⭐ | 树形DP |

#### 换根DP类
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 310 | Minimum Height Trees | Medium | ⭐⭐ | 换根DP |
| 834 | Sum of Distances in Tree | Hard | ⭐ | 换根DP |
| 1245 | Tree Diameter | Medium | ⭐ | 换根DP |
| 1519 | Number of Nodes in the Sub-Tree With the Same Label | Medium | ⭐ | 换根DP |
| 1522 | Diameter of N-Ary Tree | Medium | ⭐ | 树形DP |
| 1534 | Count Good Triplets | Easy | ⭐ | 暴力 |
| 1617 | Count Subtrees With Max Distance Between Cities | Hard | ⭐ | 换根DP |
| 2049 | Count Nodes With the Highest Score | Medium | ⭐ | 换根DP |
| 2050 | Parallel Courses III | Hard | ⭐ | 拓扑排序+DP |
| 2246 | Longest Path With Different Adjacent Characters | Hard | ⭐ | 换根DP |

### 💫 状态压缩DP (Hard 50题)

#### 子集状态DP
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 464 | Can I Win | Medium | ⭐ | 状态压缩DP |
| 473 | Matchsticks to Square | Medium | ⭐ | 状态压缩DP |
| 526 | Beautiful Arrangement | Medium | ⭐ | 状态压缩DP |
| 691 | Stickers to Spell Word | Hard | ⭐ | 状态压缩DP |
| 698 | Partition to K Equal Sum Subsets | Medium | ⭐⭐ | 状态压缩DP |
| 847 | Shortest Path Visiting All Nodes | Hard | ⭐⭐ | 状态压缩BFS |
| 943 | Find the Shortest Superstring | Hard | ⭐ | 状态压缩DP |
| 996 | Number of Squareful Arrays | Hard | ⭐ | 状态压缩DP |
| 1125 | Smallest Sufficient Team | Hard | ⭐ | 状态压缩DP |
| 1434 | Number of Ways to Wear Different Hats | Hard | ⭐ | 状态压缩DP |

#### TSP类问题
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 847 | Shortest Path Visiting All Nodes | Hard | ⭐⭐ | TSP变种 |
| 943 | Find the Shortest Superstring | Hard | ⭐ | TSP应用 |
| 980 | Unique Paths III | Hard | ⭐ | 状态压缩DFS |
| 1239 | Maximum Length of a Concatenated String | Medium | ⭐ | 状态压缩DP |
| 1349 | Maximum Students Taking Exam | Hard | ⭐ | 状态压缩DP |
| 1411 | Number of Ways to Paint N × 3 Grid | Hard | ⭐ | 状态压缩DP |
| 1494 | Parallel Courses II | Hard | ⭐ | 状态压缩DP |
| 1595 | Minimum Cost to Connect Two Groups of Points | Hard | ⭐ | 状态压缩DP |
| 1655 | Distribute Repeating Integers | Hard | ⭐ | 状态压缩DP |
| 1681 | Minimum Incompatibility | Hard | ⭐ | 状态压缩DP |

## 🎯 重点题目推荐

### 🔥 面试必备 (Top 30)
| 难度 | 题目 | 重要性 | 考察频率 |
|------|------|---------|----------|
| Easy | 70. Climbing Stairs | ⭐⭐⭐⭐⭐ | 极高 |
| Easy | 198. House Robber | ⭐⭐⭐⭐⭐ | 极高 |
| Medium | 300. Longest Increasing Subsequence | ⭐⭐⭐⭐⭐ | 极高 |
| Medium | 322. Coin Change | ⭐⭐⭐⭐⭐ | 极高 |
| Medium | 416. Partition Equal Subset Sum | ⭐⭐⭐⭐ | 很高 |
| Medium | 1143. Longest Common Subsequence | ⭐⭐⭐⭐ | 很高 |
| Hard | 72. Edit Distance | ⭐⭐⭐⭐⭐ | 极高 |
| Hard | 312. Burst Balloons | ⭐⭐⭐⭐ | 很高 |
| Hard | 124. Binary Tree Maximum Path Sum | ⭐⭐⭐⭐ | 很高 |
| Medium | 62. Unique Paths | ⭐⭐⭐⭐ | 很高 |

### 🏢 公司偏好题目

#### 🟦 Facebook/Meta
- 70. Climbing Stairs
- 198. House Robber
- 322. Coin Change
- 1143. Longest Common Subsequence
- 72. Edit Distance

#### 🟨 Google
- 300. Longest Increasing Subsequence
- 312. Burst Balloons
- 124. Binary Tree Maximum Path Sum
- 847. Shortest Path Visiting All Nodes
- 943. Find the Shortest Superstring

#### 🟧 Amazon
- 322. Coin Change
- 198. House Robber
- 62. Unique Paths
- 139. Word Break
- 121. Best Time to Buy and Sell Stock

#### ⬛ Apple
- 70. Climbing Stairs
- 300. Longest Increasing Subsequence
- 416. Partition Equal Subset Sum
- 518. Coin Change 2
- 1143. Longest Common Subsequence

#### 🟩 Microsoft
- 198. House Robber
- 322. Coin Change
- 72. Edit Distance
- 300. Longest Increasing Subsequence
- 62. Unique Paths

## 📝 学习建议

### 🎯 刷题顺序
1. **基础阶段**: 线性DP (70, 198, 53, 121)
2. **进阶阶段**: 背包DP (322, 416, 518)
3. **提高阶段**: LIS/LCS (300, 1143, 72)
4. **高级阶段**: 区间DP和状态压缩DP

### 💡 核心思路
1. **状态定义**: 明确dp[i]代表什么
2. **状态转移**: 找到递推关系
3. **初始化**: 确定边界条件
4. **优化空间**: 滚动数组优化

### ⚡ DP分类记忆
- **线性DP**: 状态按顺序排列
- **区间DP**: 状态表示区间
- **背包DP**: 状态表示容量
- **树形DP**: 状态在树上传递
- **状态压缩DP**: 用位运算压缩状态

### 🎨 经典DP模板

#### 线性DP模板
```cpp
// 一维线性DP
vector<int> dp(n + 1);
dp[0] = 初始值;
for (int i = 1; i <= n; i++) {
    dp[i] = 状态转移方程;
}
```

#### 背包DP模板
```cpp
// 01背包
vector<int> dp(capacity + 1, 0);
for (int i = 0; i < n; i++) {
    for (int j = capacity; j >= weight[i]; j--) {
        dp[j] = max(dp[j], dp[j - weight[i]] + value[i]);
    }
}

// 完全背包
for (int i = 0; i < n; i++) {
    for (int j = weight[i]; j <= capacity; j++) {
        dp[j] = max(dp[j], dp[j - weight[i]] + value[i]);
    }
}
```

#### 区间DP模板
```cpp
// 区间DP
vector<vector<int>> dp(n, vector<int>(n, 0));
for (int len = 2; len <= n; len++) {
    for (int i = 0; i <= n - len; i++) {
        int j = i + len - 1;
        for (int k = i; k < j; k++) {
            dp[i][j] = min(dp[i][j], dp[i][k] + dp[k+1][j] + cost);
        }
    }
}
```

**动态规划是解决最优化问题的核心方法！** 🚀

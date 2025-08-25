# 算法面试题详解

## 📋 目录

1. [排序算法](#排序算法)
2. [搜索算法](#搜索算法)
3. [动态规划](#动态规划)
4. [贪心算法](#贪心算法)
5. [回溯算法](#回溯算法)
6. [分治算法](#分治算法)
7. [图算法](#图算法)
8. [字符串算法](#字符串算法)

---

## 排序算法

### 1. 快速排序的实现和优化

**问题**: 实现快速排序，并说明时间复杂度

**标准实现**:
```cpp
void quickSort(vector<int>& nums, int left, int right) {
    if (left >= right) return;
    
    int pivot = partition(nums, left, right);
    quickSort(nums, left, pivot - 1);
    quickSort(nums, pivot + 1, right);
}

int partition(vector<int>& nums, int left, int right) {
    int pivot = nums[right];  // 选择最右边元素作为基准
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

**优化版本**:
```cpp
void quickSortOptimized(vector<int>& nums, int left, int right) {
    while (left < right) {
        // 小数组使用插入排序
        if (right - left + 1 < 10) {
            insertionSort(nums, left, right);
            return;
        }
        
        // 三数取中法选择基准
        int mid = left + (right - left) / 2;
        if (nums[mid] < nums[left]) swap(nums[left], nums[mid]);
        if (nums[right] < nums[left]) swap(nums[left], nums[right]);
        if (nums[right] < nums[mid]) swap(nums[mid], nums[right]);
        swap(nums[mid], nums[right]);
        
        int pivot = partition(nums, left, right);
        
        // 尾递归优化：对较短的部分递归
        if (pivot - left < right - pivot) {
            quickSortOptimized(nums, left, pivot - 1);
            left = pivot + 1;
        } else {
            quickSortOptimized(nums, pivot + 1, right);
            right = pivot - 1;
        }
    }
}
```

**复杂度分析**:
- **最好/平均时间复杂度**: O(n log n)
- **最坏时间复杂度**: O(n²) - 已排序或逆序
- **空间复杂度**: O(log n) - 递归栈

### 2. 归并排序

**问题**: 实现归并排序，分析其稳定性

**解答**:
```cpp
void mergeSort(vector<int>& nums, int left, int right) {
    if (left >= right) return;
    
    int mid = left + (right - left) / 2;
    mergeSort(nums, left, mid);
    mergeSort(nums, mid + 1, right);
    merge(nums, left, mid, right);
}

void merge(vector<int>& nums, int left, int mid, int right) {
    vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    
    while (i <= mid && j <= right) {
        if (nums[i] <= nums[j]) {  // <= 保证稳定性
            temp[k++] = nums[i++];
        } else {
            temp[k++] = nums[j++];
        }
    }
    
    while (i <= mid) temp[k++] = nums[i++];
    while (j <= right) temp[k++] = nums[j++];
    
    for (int i = 0; i < k; i++) {
        nums[left + i] = temp[i];
    }
}
```

**特点**:
- **稳定排序**: 相等元素相对位置不变
- **时间复杂度**: 恒定O(n log n)
- **空间复杂度**: O(n) - 需要额外数组

### 3. 堆排序

**问题**: 实现堆排序

**解答**:
```cpp
void heapSort(vector<int>& nums) {
    int n = nums.size();
    
    // 构建最大堆
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(nums, n, i);
    }
    
    // 逐个提取元素
    for (int i = n - 1; i > 0; i--) {
        swap(nums[0], nums[i]);
        heapify(nums, i, 0);
    }
}

void heapify(vector<int>& nums, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < n && nums[left] > nums[largest]) {
        largest = left;
    }
    
    if (right < n && nums[right] > nums[largest]) {
        largest = right;
    }
    
    if (largest != i) {
        swap(nums[i], nums[largest]);
        heapify(nums, n, largest);
    }
}
```

---

## 搜索算法

### 1. 二分查找及其变种

**基础二分查找**:
```cpp
int binarySearch(vector<int>& nums, int target) {
    int left = 0, right = nums.size() - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (nums[mid] == target) {
            return mid;
        } else if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return -1;
}
```

**查找第一个出现位置**:
```cpp
int findFirst(vector<int>& nums, int target) {
    int left = 0, right = nums.size() - 1;
    int result = -1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (nums[mid] == target) {
            result = mid;
            right = mid - 1;  // 继续向左查找
        } else if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return result;
}
```

**查找最后一个出现位置**:
```cpp
int findLast(vector<int>& nums, int target) {
    int left = 0, right = nums.size() - 1;
    int result = -1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (nums[mid] == target) {
            result = mid;
            left = mid + 1;  // 继续向右查找
        } else if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return result;
}
```

### 2. 搜索旋转排序数组

**问题**: 在旋转排序数组中搜索目标值

**解答**:
```cpp
int search(vector<int>& nums, int target) {
    int left = 0, right = nums.size() - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (nums[mid] == target) return mid;
        
        // 判断哪边是有序的
        if (nums[left] <= nums[mid]) {  // 左半部分有序
            if (nums[left] <= target && target < nums[mid]) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        } else {  // 右半部分有序
            if (nums[mid] < target && target <= nums[right]) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
    }
    
    return -1;
}
```

---

## 动态规划

### 1. 最长公共子序列 (LCS)

**问题**: 求两个字符串的最长公共子序列长度

**解答**:
```cpp
int longestCommonSubsequence(string text1, string text2) {
    int m = text1.length(), n = text2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text1[i - 1] == text2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    
    return dp[m][n];
}
```

**空间优化版本**:
```cpp
int longestCommonSubsequence(string text1, string text2) {
    int m = text1.length(), n = text2.length();
    vector<int> prev(n + 1, 0), curr(n + 1, 0);
    
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text1[i - 1] == text2[j - 1]) {
                curr[j] = prev[j - 1] + 1;
            } else {
                curr[j] = max(prev[j], curr[j - 1]);
            }
        }
        prev = curr;
    }
    
    return curr[n];
}
```

### 2. 背包问题

**0-1背包**:
```cpp
int knapsack(int capacity, vector<int>& weights, vector<int>& values) {
    int n = weights.size();
    vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, 0));
    
    for (int i = 1; i <= n; i++) {
        for (int w = 1; w <= capacity; w++) {
            if (weights[i - 1] <= w) {
                dp[i][w] = max(dp[i - 1][w], 
                              dp[i - 1][w - weights[i - 1]] + values[i - 1]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }
    
    return dp[n][capacity];
}
```

**空间优化（一维数组）**:
```cpp
int knapsack(int capacity, vector<int>& weights, vector<int>& values) {
    vector<int> dp(capacity + 1, 0);
    
    for (int i = 0; i < weights.size(); i++) {
        for (int w = capacity; w >= weights[i]; w--) {
            dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
        }
    }
    
    return dp[capacity];
}
```

### 3. 编辑距离

**问题**: 计算将字符串s1转换为s2的最小编辑距离

**解答**:
```cpp
int minDistance(string word1, string word2) {
    int m = word1.length(), n = word2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1));
    
    // 初始化边界
    for (int i = 0; i <= m; i++) dp[i][0] = i;
    for (int j = 0; j <= n; j++) dp[0][j] = j;
    
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (word1[i - 1] == word2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + min({
                    dp[i - 1][j],     // 删除
                    dp[i][j - 1],     // 插入
                    dp[i - 1][j - 1]  // 替换
                });
            }
        }
    }
    
    return dp[m][n];
}
```

---

## 贪心算法

### 1. 活动选择问题

**问题**: 给定一组活动的开始和结束时间，选择最多的不重叠活动

**解答**:
```cpp
int activitySelection(vector<vector<int>>& intervals) {
    if (intervals.empty()) return 0;
    
    // 按结束时间排序
    sort(intervals.begin(), intervals.end(), 
         [](const vector<int>& a, const vector<int>& b) {
             return a[1] < b[1];
         });
    
    int count = 1;
    int lastEnd = intervals[0][1];
    
    for (int i = 1; i < intervals.size(); i++) {
        if (intervals[i][0] >= lastEnd) {
            count++;
            lastEnd = intervals[i][1];
        }
    }
    
    return count;
}
```

### 2. 分发糖果

**问题**: 给孩子分糖果，相邻孩子评分高的要拿更多糖果，求最少糖果数

**解答**:
```cpp
int candy(vector<int>& ratings) {
    int n = ratings.size();
    vector<int> candies(n, 1);
    
    // 从左到右扫描
    for (int i = 1; i < n; i++) {
        if (ratings[i] > ratings[i - 1]) {
            candies[i] = candies[i - 1] + 1;
        }
    }
    
    // 从右到左扫描
    for (int i = n - 2; i >= 0; i--) {
        if (ratings[i] > ratings[i + 1]) {
            candies[i] = max(candies[i], candies[i + 1] + 1);
        }
    }
    
    return accumulate(candies.begin(), candies.end(), 0);
}
```

### 3. 跳跃游戏

**问题**: 判断是否能跳到数组的最后一个位置

**解答**:
```cpp
bool canJump(vector<int>& nums) {
    int maxReach = 0;
    
    for (int i = 0; i < nums.size(); i++) {
        if (i > maxReach) return false;
        maxReach = max(maxReach, i + nums[i]);
        if (maxReach >= nums.size() - 1) return true;
    }
    
    return true;
}

// 跳跃游戏II：求最少跳跃次数
int jump(vector<int>& nums) {
    int jumps = 0, currentEnd = 0, farthest = 0;
    
    for (int i = 0; i < nums.size() - 1; i++) {
        farthest = max(farthest, i + nums[i]);
        
        if (i == currentEnd) {
            jumps++;
            currentEnd = farthest;
        }
    }
    
    return jumps;
}
```

---

## 回溯算法

### 1. N皇后问题

**问题**: 在N×N棋盘上放置N个皇后，使它们不能相互攻击

**解答**:
```cpp
vector<vector<string>> solveNQueens(int n) {
    vector<vector<string>> result;
    vector<string> board(n, string(n, '.'));
    backtrack(board, 0, result);
    return result;
}

void backtrack(vector<string>& board, int row, vector<vector<string>>& result) {
    if (row == board.size()) {
        result.push_back(board);
        return;
    }
    
    for (int col = 0; col < board.size(); col++) {
        if (isValid(board, row, col)) {
            board[row][col] = 'Q';
            backtrack(board, row + 1, result);
            board[row][col] = '.';
        }
    }
}

bool isValid(vector<string>& board, int row, int col) {
    int n = board.size();
    
    // 检查列
    for (int i = 0; i < row; i++) {
        if (board[i][col] == 'Q') return false;
    }
    
    // 检查左上对角线
    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j] == 'Q') return false;
    }
    
    // 检查右上对角线
    for (int i = row - 1, j = col + 1; i >= 0 && j < n; i--, j++) {
        if (board[i][j] == 'Q') return false;
    }
    
    return true;
}
```

### 2. 全排列

**问题**: 生成数组的所有排列

**解答**:
```cpp
vector<vector<int>> permute(vector<int>& nums) {
    vector<vector<int>> result;
    vector<int> current;
    vector<bool> used(nums.size(), false);
    backtrack(nums, current, used, result);
    return result;
}

void backtrack(vector<int>& nums, vector<int>& current, 
               vector<bool>& used, vector<vector<int>>& result) {
    if (current.size() == nums.size()) {
        result.push_back(current);
        return;
    }
    
    for (int i = 0; i < nums.size(); i++) {
        if (!used[i]) {
            current.push_back(nums[i]);
            used[i] = true;
            backtrack(nums, current, used, result);
            current.pop_back();
            used[i] = false;
        }
    }
}
```

### 3. 单词搜索

**问题**: 在二维字符网格中搜索单词

**解答**:
```cpp
bool exist(vector<vector<char>>& board, string word) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            if (dfs(board, word, i, j, 0)) {
                return true;
            }
        }
    }
    return false;
}

bool dfs(vector<vector<char>>& board, string& word, int i, int j, int index) {
    if (index == word.length()) return true;
    
    if (i < 0 || i >= board.size() || j < 0 || j >= board[0].size() || 
        board[i][j] != word[index]) {
        return false;
    }
    
    char temp = board[i][j];
    board[i][j] = '#';  // 标记已访问
    
    bool found = dfs(board, word, i + 1, j, index + 1) ||
                 dfs(board, word, i - 1, j, index + 1) ||
                 dfs(board, word, i, j + 1, index + 1) ||
                 dfs(board, word, i, j - 1, index + 1);
    
    board[i][j] = temp;  // 恢复
    return found;
}
```

---

## 分治算法

### 1. 归并排序中的逆序对

**问题**: 计算数组中的逆序对数量

**解答**:
```cpp
int mergeSort(vector<int>& nums, int left, int right) {
    if (left >= right) return 0;
    
    int mid = left + (right - left) / 2;
    int count = mergeSort(nums, left, mid) + mergeSort(nums, mid + 1, right);
    
    return count + merge(nums, left, mid, right);
}

int merge(vector<int>& nums, int left, int mid, int right) {
    vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0, count = 0;
    
    while (i <= mid && j <= right) {
        if (nums[i] <= nums[j]) {
            temp[k++] = nums[i++];
        } else {
            temp[k++] = nums[j++];
            count += (mid - i + 1);  // 逆序对数量
        }
    }
    
    while (i <= mid) temp[k++] = nums[i++];
    while (j <= right) temp[k++] = nums[j++];
    
    for (int i = 0; i < k; i++) {
        nums[left + i] = temp[i];
    }
    
    return count;
}
```

### 2. 最大子数组和

**问题**: 找到数组中和最大的连续子数组

**分治解法**:
```cpp
int maxSubArray(vector<int>& nums) {
    return divideConquer(nums, 0, nums.size() - 1);
}

int divideConquer(vector<int>& nums, int left, int right) {
    if (left == right) return nums[left];
    
    int mid = left + (right - left) / 2;
    int leftMax = divideConquer(nums, left, mid);
    int rightMax = divideConquer(nums, mid + 1, right);
    int crossMax = maxCrossingSum(nums, left, mid, right);
    
    return max({leftMax, rightMax, crossMax});
}

int maxCrossingSum(vector<int>& nums, int left, int mid, int right) {
    int leftSum = INT_MIN, sum = 0;
    for (int i = mid; i >= left; i--) {
        sum += nums[i];
        leftSum = max(leftSum, sum);
    }
    
    int rightSum = INT_MIN; sum = 0;
    for (int i = mid + 1; i <= right; i++) {
        sum += nums[i];
        rightSum = max(rightSum, sum);
    }
    
    return leftSum + rightSum;
}
```

---

## 图算法

### 1. 最短路径算法

**Dijkstra算法**:
```cpp
vector<int> dijkstra(vector<vector<pair<int, int>>>& graph, int start) {
    int n = graph.size();
    vector<int> dist(n, INT_MAX);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    dist[start] = 0;
    pq.push({0, start});
    
    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();
        
        if (d > dist[u]) continue;
        
        for (auto& edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;
            
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
    
    return dist;
}
```

**Floyd-Warshall算法**:
```cpp
vector<vector<int>> floydWarshall(vector<vector<int>>& graph) {
    int n = graph.size();
    vector<vector<int>> dist = graph;
    
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
    
    return dist;
}
```

### 2. 最小生成树

**Kruskal算法**:
```cpp
struct Edge {
    int u, v, weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class UnionFind {
    vector<int> parent, rank;
public:
    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    
    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false;
        
        if (rank[px] < rank[py]) swap(px, py);
        parent[py] = px;
        if (rank[px] == rank[py]) rank[px]++;
        return true;
    }
};

int kruskal(int n, vector<Edge>& edges) {
    sort(edges.begin(), edges.end());
    UnionFind uf(n);
    int mstWeight = 0, edgeCount = 0;
    
    for (Edge& e : edges) {
        if (uf.unite(e.u, e.v)) {
            mstWeight += e.weight;
            if (++edgeCount == n - 1) break;
        }
    }
    
    return mstWeight;
}
```

---

## 字符串算法

### 1. KMP算法

**问题**: 在文本中查找模式串

**解答**:
```cpp
vector<int> buildLPS(string pattern) {
    int m = pattern.length();
    vector<int> lps(m, 0);
    int len = 0, i = 1;
    
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            lps[i++] = ++len;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i++] = 0;
            }
        }
    }
    
    return lps;
}

int KMP(string text, string pattern) {
    int n = text.length(), m = pattern.length();
    vector<int> lps = buildLPS(pattern);
    
    int i = 0, j = 0;
    while (i < n) {
        if (text[i] == pattern[j]) {
            i++; j++;
        }
        
        if (j == m) {
            return i - j;  // 找到匹配
        } else if (i < n && text[i] != pattern[j]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    
    return -1;  // 未找到
}
```

### 2. 最长回文子串

**Manacher算法**:
```cpp
string longestPalindrome(string s) {
    string processed = "#";
    for (char c : s) {
        processed += c;
        processed += "#";
    }
    
    int n = processed.length();
    vector<int> P(n, 0);
    int center = 0, right = 0;
    int maxLen = 0, centerIndex = 0;
    
    for (int i = 0; i < n; i++) {
        int mirror = 2 * center - i;
        
        if (i < right) {
            P[i] = min(right - i, P[mirror]);
        }
        
        // 尝试扩展回文
        while (i + P[i] + 1 < n && i - P[i] - 1 >= 0 &&
               processed[i + P[i] + 1] == processed[i - P[i] - 1]) {
            P[i]++;
        }
        
        // 更新center和right
        if (i + P[i] > right) {
            center = i;
            right = i + P[i];
        }
        
        // 更新最长回文
        if (P[i] > maxLen) {
            maxLen = P[i];
            centerIndex = i;
        }
    }
    
    int start = (centerIndex - maxLen) / 2;
    return s.substr(start, maxLen);
}
```

## 💡 面试技巧总结

### 算法选择指南

1. **排序**: 根据数据特点选择合适算法
   - 小数据量: 插入排序
   - 一般情况: 快速排序
   - 稳定性要求: 归并排序
   - 空间限制: 堆排序

2. **搜索**: 
   - 有序数据: 二分查找
   - 无序数据: 哈希表
   - 范围查询: 线段树/树状数组

3. **动态规划**:
   - 最优子结构 + 重叠子问题
   - 状态定义要清晰
   - 考虑空间优化

4. **贪心算法**:
   - 局部最优导致全局最优
   - 需要证明贪心选择性质

### 复杂度分析要点

- **递归算法**: 主定理或递推关系
- **分治算法**: T(n) = aT(n/b) + f(n)
- **动态规划**: 状态数 × 转移时间
- **图算法**: 通常O(V+E)或O(V²)

### 编程实践建议

1. **代码结构**: 模块化，函数职责单一
2. **边界处理**: 空输入、单元素、溢出
3. **测试用例**: 覆盖各种情况
4. **时间管理**: 先实现基本版本，再优化

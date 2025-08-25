/**
 * @file algorithms-demo.cpp
 * @brief 常用算法实现演示
 * @author C++ Learning Guide
 * @date 2024
 * 
 * 这个文件演示了各种常用算法的实现，包括：
 * - 排序算法
 * - 搜索算法
 * - 动态规划
 * - 图算法
 * - 字符串算法
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <algorithm>
#include <climits>
#include <chrono>

using namespace std;

// ================== 排序算法 ==================

class SortingAlgorithms {
public:
    // 快速排序
    static void quickSort(vector<int>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }
    
    // 归并排序
    static void mergeSort(vector<int>& arr, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    }
    
    // 堆排序
    static void heapSort(vector<int>& arr) {
        int n = arr.size();
        
        // 构建最大堆
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(arr, n, i);
        }
        
        // 逐个提取元素
        for (int i = n - 1; i > 0; i--) {
            swap(arr[0], arr[i]);
            heapify(arr, i, 0);
        }
    }
    
    // 冒泡排序（用于教学目的）
    static void bubbleSort(vector<int>& arr) {
        int n = arr.size();
        for (int i = 0; i < n - 1; i++) {
            bool swapped = false;
            for (int j = 0; j < n - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                    swapped = true;
                }
            }
            if (!swapped) break;  // 优化：如果没有交换，数组已经有序
        }
    }

private:
    static int partition(vector<int>& arr, int low, int high) {
        int pivot = arr[high];
        int i = low - 1;
        
        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        return i + 1;
    }
    
    static void merge(vector<int>& arr, int left, int mid, int right) {
        vector<int> temp(right - left + 1);
        int i = left, j = mid + 1, k = 0;
        
        while (i <= mid && j <= right) {
            if (arr[i] <= arr[j]) {
                temp[k++] = arr[i++];
            } else {
                temp[k++] = arr[j++];
            }
        }
        
        while (i <= mid) temp[k++] = arr[i++];
        while (j <= right) temp[k++] = arr[j++];
        
        for (int i = 0; i < k; i++) {
            arr[left + i] = temp[i];
        }
    }
    
    static void heapify(vector<int>& arr, int n, int i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        
        if (left < n && arr[left] > arr[largest]) {
            largest = left;
        }
        
        if (right < n && arr[right] > arr[largest]) {
            largest = right;
        }
        
        if (largest != i) {
            swap(arr[i], arr[largest]);
            heapify(arr, n, largest);
        }
    }
};

// ================== 搜索算法 ==================

class SearchAlgorithms {
public:
    // 二分查找
    static int binarySearch(const vector<int>& arr, int target) {
        int left = 0, right = arr.size() - 1;
        
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
        
        return -1;  // 未找到
    }
    
    // 在旋转排序数组中搜索
    static int searchInRotatedArray(const vector<int>& nums, int target) {
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
    
    // 寻找峰值
    static int findPeakElement(const vector<int>& nums) {
        int left = 0, right = nums.size() - 1;
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            
            if (nums[mid] > nums[mid + 1]) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        
        return left;
    }
};

// ================== 动态规划 ==================

class DynamicProgramming {
public:
    // 斐波那契数列
    static long long fibonacci(int n) {
        if (n <= 1) return n;
        
        vector<long long> dp(n + 1);
        dp[0] = 0;
        dp[1] = 1;
        
        for (int i = 2; i <= n; i++) {
            dp[i] = dp[i - 1] + dp[i - 2];
        }
        
        return dp[n];
    }
    
    // 最长公共子序列
    static int longestCommonSubsequence(const string& text1, const string& text2) {
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
    
    // 0-1背包问题
    static int knapsack(int capacity, const vector<int>& weights, const vector<int>& values) {
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
    
    // 编辑距离
    static int editDistance(const string& word1, const string& word2) {
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
    
    // 最大子数组和 (Kadane算法)
    static int maxSubarraySum(const vector<int>& nums) {
        int maxSum = nums[0];
        int currentSum = nums[0];
        
        for (int i = 1; i < nums.size(); i++) {
            currentSum = max(nums[i], currentSum + nums[i]);
            maxSum = max(maxSum, currentSum);
        }
        
        return maxSum;
    }
};

// ================== 图算法 ==================

class GraphAlgorithms {
private:
    struct Edge {
        int to, weight;
        Edge(int t, int w) : to(t), weight(w) {}
    };
    
public:
    // DFS遍历
    static void dfs(const vector<vector<int>>& graph, int start, vector<bool>& visited) {
        visited[start] = true;
        cout << start << " ";
        
        for (int neighbor : graph[start]) {
            if (!visited[neighbor]) {
                dfs(graph, neighbor, visited);
            }
        }
    }
    
    // BFS遍历
    static void bfs(const vector<vector<int>>& graph, int start) {
        vector<bool> visited(graph.size(), false);
        queue<int> q;
        
        visited[start] = true;
        q.push(start);
        
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            cout << node << " ";
            
            for (int neighbor : graph[node]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
    }
    
    // Dijkstra最短路径算法
    static vector<int> dijkstra(const vector<vector<Edge>>& graph, int start) {
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
            
            for (const Edge& edge : graph[u]) {
                int v = edge.to;
                int weight = edge.weight;
                
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }
        
        return dist;
    }
    
    // 拓扑排序
    static vector<int> topologicalSort(const vector<vector<int>>& graph) {
        int n = graph.size();
        vector<int> indegree(n, 0);
        
        // 计算入度
        for (int i = 0; i < n; i++) {
            for (int neighbor : graph[i]) {
                indegree[neighbor]++;
            }
        }
        
        queue<int> q;
        for (int i = 0; i < n; i++) {
            if (indegree[i] == 0) {
                q.push(i);
            }
        }
        
        vector<int> result;
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            result.push_back(node);
            
            for (int neighbor : graph[node]) {
                indegree[neighbor]--;
                if (indegree[neighbor] == 0) {
                    q.push(neighbor);
                }
            }
        }
        
        return result.size() == n ? result : vector<int>();  // 如果有环则返回空
    }
    
    // 检测有向图中的环
    static bool hasCycle(const vector<vector<int>>& graph) {
        int n = graph.size();
        vector<int> color(n, 0);  // 0: 白色, 1: 灰色, 2: 黑色
        
        function<bool(int)> dfsVisit = [&](int u) -> bool {
            color[u] = 1;  // 标记为灰色
            
            for (int v : graph[u]) {
                if (color[v] == 1) {  // 发现后向边，有环
                    return true;
                }
                if (color[v] == 0 && dfsVisit(v)) {
                    return true;
                }
            }
            
            color[u] = 2;  // 标记为黑色
            return false;
        };
        
        for (int i = 0; i < n; i++) {
            if (color[i] == 0 && dfsVisit(i)) {
                return true;
            }
        }
        
        return false;
    }
};

// ================== 字符串算法 ==================

class StringAlgorithms {
public:
    // KMP算法
    static int KMP(const string& text, const string& pattern) {
        vector<int> lps = computeLPS(pattern);
        int n = text.length(), m = pattern.length();
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
    
    // 最长回文子串 (Manacher算法)
    static string longestPalindrome(const string& s) {
        if (s.empty()) return "";
        
        // 预处理字符串
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
    
    // 字符串匹配的所有位置
    static vector<int> findAllOccurrences(const string& text, const string& pattern) {
        vector<int> result;
        vector<int> lps = computeLPS(pattern);
        int n = text.length(), m = pattern.length();
        int i = 0, j = 0;
        
        while (i < n) {
            if (text[i] == pattern[j]) {
                i++; j++;
            }
            
            if (j == m) {
                result.push_back(i - j);
                j = lps[j - 1];
            } else if (i < n && text[i] != pattern[j]) {
                if (j != 0) {
                    j = lps[j - 1];
                } else {
                    i++;
                }
            }
        }
        
        return result;
    }

private:
    static vector<int> computeLPS(const string& pattern) {
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
};

// ================== 测试和演示 ==================

void testSortingAlgorithms() {
    cout << "=== Sorting Algorithms Test ===" << endl;
    
    vector<int> arr = {64, 34, 25, 12, 22, 11, 90};
    cout << "Original array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;
    
    // 测试快速排序
    vector<int> arr1 = arr;
    SortingAlgorithms::quickSort(arr1, 0, arr1.size() - 1);
    cout << "Quick sort: ";
    for (int x : arr1) cout << x << " ";
    cout << endl;
    
    // 测试归并排序
    vector<int> arr2 = arr;
    SortingAlgorithms::mergeSort(arr2, 0, arr2.size() - 1);
    cout << "Merge sort: ";
    for (int x : arr2) cout << x << " ";
    cout << endl;
    
    // 测试堆排序
    vector<int> arr3 = arr;
    SortingAlgorithms::heapSort(arr3);
    cout << "Heap sort: ";
    for (int x : arr3) cout << x << " ";
    cout << endl;
}

void testSearchAlgorithms() {
    cout << "\n=== Search Algorithms Test ===" << endl;
    
    vector<int> sorted_arr = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    cout << "Sorted array: ";
    for (int x : sorted_arr) cout << x << " ";
    cout << endl;
    
    int target = 7;
    int result = SearchAlgorithms::binarySearch(sorted_arr, target);
    cout << "Binary search for " << target << ": index " << result << endl;
    
    vector<int> rotated = {4, 5, 6, 7, 0, 1, 2};
    cout << "Rotated array: ";
    for (int x : rotated) cout << x << " ";
    cout << endl;
    
    target = 0;
    result = SearchAlgorithms::searchInRotatedArray(rotated, target);
    cout << "Search " << target << " in rotated array: index " << result << endl;
}

void testDynamicProgramming() {
    cout << "\n=== Dynamic Programming Test ===" << endl;
    
    // 测试斐波那契
    int n = 10;
    cout << "Fibonacci(" << n << ") = " << DynamicProgramming::fibonacci(n) << endl;
    
    // 测试最长公共子序列
    string text1 = "abcde", text2 = "ace";
    cout << "LCS of '" << text1 << "' and '" << text2 << "': " 
         << DynamicProgramming::longestCommonSubsequence(text1, text2) << endl;
    
    // 测试背包问题
    vector<int> weights = {1, 3, 4, 5};
    vector<int> values = {1, 4, 5, 7};
    int capacity = 7;
    cout << "Knapsack (capacity=" << capacity << "): " 
         << DynamicProgramming::knapsack(capacity, weights, values) << endl;
    
    // 测试最大子数组和
    vector<int> nums = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    cout << "Max subarray sum: " << DynamicProgramming::maxSubarraySum(nums) << endl;
}

void testGraphAlgorithms() {
    cout << "\n=== Graph Algorithms Test ===" << endl;
    
    // 构建图 (邻接表表示)
    vector<vector<int>> graph = {
        {1, 2},     // 0 -> 1, 2
        {3, 4},     // 1 -> 3, 4
        {5},        // 2 -> 5
        {},         // 3 -> (none)
        {5},        // 4 -> 5
        {}          // 5 -> (none)
    };
    
    cout << "Graph adjacency list:" << endl;
    for (int i = 0; i < graph.size(); i++) {
        cout << i << " -> ";
        for (int neighbor : graph[i]) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
    
    cout << "DFS from node 0: ";
    vector<bool> visited(graph.size(), false);
    GraphAlgorithms::dfs(graph, 0, visited);
    cout << endl;
    
    cout << "BFS from node 0: ";
    GraphAlgorithms::bfs(graph, 0);
    cout << endl;
    
    // 测试拓扑排序
    vector<int> topo_result = GraphAlgorithms::topologicalSort(graph);
    cout << "Topological sort: ";
    for (int node : topo_result) {
        cout << node << " ";
    }
    cout << endl;
}

void testStringAlgorithms() {
    cout << "\n=== String Algorithms Test ===" << endl;
    
    string text = "ABABDABACDABABCABCABCABCABC";
    string pattern = "ABABCABCAB";
    
    cout << "Text: " << text << endl;
    cout << "Pattern: " << pattern << endl;
    
    int pos = StringAlgorithms::KMP(text, pattern);
    cout << "KMP search result: " << (pos != -1 ? "found at position " + to_string(pos) : "not found") << endl;
    
    vector<int> all_pos = StringAlgorithms::findAllOccurrences(text, "ABC");
    cout << "All occurrences of 'ABC': ";
    for (int p : all_pos) {
        cout << p << " ";
    }
    cout << endl;
    
    string s = "babad";
    cout << "Longest palindrome in '" << s << "': " << StringAlgorithms::longestPalindrome(s) << endl;
}

void performanceComparison() {
    cout << "\n=== Performance Comparison ===" << endl;
    
    const int size = 10000;
    vector<int> data(size);
    for (int i = 0; i < size; i++) {
        data[i] = rand() % 10000;
    }
    
    // 测试不同排序算法的性能
    vector<string> sort_names = {"Quick Sort", "Merge Sort", "Heap Sort", "Bubble Sort"};
    vector<function<void(vector<int>&)>> sort_funcs = {
        [](vector<int>& arr) { SortingAlgorithms::quickSort(arr, 0, arr.size() - 1); },
        [](vector<int>& arr) { SortingAlgorithms::mergeSort(arr, 0, arr.size() - 1); },
        [](vector<int>& arr) { SortingAlgorithms::heapSort(arr); },
        [](vector<int>& arr) { SortingAlgorithms::bubbleSort(arr); }
    };
    
    for (int i = 0; i < sort_names.size(); i++) {
        vector<int> test_data = data;
        
        auto start = chrono::high_resolution_clock::now();
        sort_funcs[i](test_data);
        auto end = chrono::high_resolution_clock::now();
        
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << sort_names[i] << " (" << size << " elements): " 
             << duration.count() << " microseconds" << endl;
        
        // 验证结果是否正确
        bool is_sorted = is_sorted(test_data.begin(), test_data.end());
        cout << "  Result: " << (is_sorted ? "Correct" : "Incorrect") << endl;
    }
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    
    try {
        testSortingAlgorithms();
        testSearchAlgorithms();
        testDynamicProgramming();
        testGraphAlgorithms();
        testStringAlgorithms();
        performanceComparison();
        
        cout << "\n=== All Tests Completed Successfully ===" << endl;
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}

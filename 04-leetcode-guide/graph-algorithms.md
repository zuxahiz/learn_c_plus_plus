# LeetCode 图论算法题详解

## 🎯 图论算法概述

图论是计算机科学中的重要分支，在实际应用中有着广泛的使用场景。LeetCode中的图论题目覆盖了从基础的图遍历到高级的网络流算法。

### 核心概念
1. **图的表示**: 邻接矩阵、邻接表、边列表
2. **图的遍历**: DFS、BFS
3. **连通性**: 连通分量、强连通分量
4. **最短路径**: Dijkstra、Bellman-Ford、Floyd-Warshall
5. **最小生成树**: Prim、Kruskal
6. **拓扑排序**: Kahn算法、DFS

## 📚 1. 图的表示和基础遍历

### 1.1 图的表示方式

```cpp
// 图的基本表示类
class Graph {
public:
    int vertices;
    
    // 邻接表表示
    vector<vector<int>> adjList;
    
    // 邻接矩阵表示
    vector<vector<int>> adjMatrix;
    
    // 边列表表示（带权重）
    vector<tuple<int, int, int>> edges; // {from, to, weight}
    
    Graph(int v) : vertices(v) {
        adjList.resize(v);
        adjMatrix.resize(v, vector<int>(v, 0));
    }
    
    // 添加边（无向图）
    void addEdgeUndirected(int u, int v) {
        adjList[u].push_back(v);
        adjList[v].push_back(u);
        adjMatrix[u][v] = adjMatrix[v][u] = 1;
    }
    
    // 添加边（有向图）
    void addEdgeDirected(int u, int v, int weight = 1) {
        adjList[u].push_back(v);
        adjMatrix[u][v] = weight;
        edges.push_back({u, v, weight});
    }
    
    // 打印图
    void printGraph() {
        cout << "邻接表表示:" << endl;
        for (int i = 0; i < vertices; i++) {
            cout << i << ": ";
            for (int neighbor : adjList[i]) {
                cout << neighbor << " ";
            }
            cout << endl;
        }
    }
};
```

### 1.2 深度优先搜索 (DFS)

#### 基本DFS实现

```cpp
class DFS {
public:
    // 递归DFS
    void dfsRecursive(vector<vector<int>>& graph, int node, vector<bool>& visited) {
        visited[node] = true;
        cout << node << " ";
        
        for (int neighbor : graph[node]) {
            if (!visited[neighbor]) {
                dfsRecursive(graph, neighbor, visited);
            }
        }
    }
    
    // 迭代DFS
    void dfsIterative(vector<vector<int>>& graph, int start) {
        int n = graph.size();
        vector<bool> visited(n, false);
        stack<int> st;
        
        st.push(start);
        
        while (!st.empty()) {
            int node = st.top();
            st.pop();
            
            if (!visited[node]) {
                visited[node] = true;
                cout << node << " ";
                
                // 逆序添加邻居，保证访问顺序
                for (int i = graph[node].size() - 1; i >= 0; i--) {
                    if (!visited[graph[node][i]]) {
                        st.push(graph[node][i]);
                    }
                }
            }
        }
    }
    
    // 检查路径是否存在
    bool hasPath(vector<vector<int>>& graph, int start, int end) {
        if (start == end) return true;
        
        int n = graph.size();
        vector<bool> visited(n, false);
        
        function<bool(int)> dfs = [&](int node) -> bool {
            if (node == end) return true;
            
            visited[node] = true;
            for (int neighbor : graph[node]) {
                if (!visited[neighbor] && dfs(neighbor)) {
                    return true;
                }
            }
            return false;
        };
        
        return dfs(start);
    }
};
```

#### 岛屿数量 (LeetCode 200)

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
        
        grid[i][j] = '0'; // 标记为已访问
        
        // 四个方向
        dfs(grid, i + 1, j);
        dfs(grid, i - 1, j);
        dfs(grid, i, j + 1);
        dfs(grid, i, j - 1);
    }
};
```

#### 被围绕的区域 (LeetCode 130)

```cpp
class Solution {
public:
    void solve(vector<vector<char>>& board) {
        if (board.empty() || board[0].empty()) return;
        
        int rows = board.size();
        int cols = board[0].size();
        
        // 从边界的'O'开始DFS，标记为不会被翻转的'O'
        for (int i = 0; i < rows; i++) {
            if (board[i][0] == 'O') dfs(board, i, 0);
            if (board[i][cols - 1] == 'O') dfs(board, i, cols - 1);
        }
        
        for (int j = 0; j < cols; j++) {
            if (board[0][j] == 'O') dfs(board, 0, j);
            if (board[rows - 1][j] == 'O') dfs(board, rows - 1, j);
        }
        
        // 遍历整个board，恢复标记
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (board[i][j] == 'O') {
                    board[i][j] = 'X'; // 被围绕的'O'变成'X'
                } else if (board[i][j] == '#') {
                    board[i][j] = 'O'; // 不被围绕的'O'恢复
                }
            }
        }
    }
    
private:
    void dfs(vector<vector<char>>& board, int i, int j) {
        int rows = board.size();
        int cols = board[0].size();
        
        if (i < 0 || i >= rows || j < 0 || j >= cols || board[i][j] != 'O') {
            return;
        }
        
        board[i][j] = '#'; // 标记为不会被翻转
        
        dfs(board, i + 1, j);
        dfs(board, i - 1, j);
        dfs(board, i, j + 1);
        dfs(board, i, j - 1);
    }
};
```

### 1.3 广度优先搜索 (BFS)

#### 基本BFS实现

```cpp
class BFS {
public:
    // 基本BFS遍历
    void bfs(vector<vector<int>>& graph, int start) {
        int n = graph.size();
        vector<bool> visited(n, false);
        queue<int> q;
        
        q.push(start);
        visited[start] = true;
        
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
    
    // 计算最短距离
    vector<int> shortestPath(vector<vector<int>>& graph, int start) {
        int n = graph.size();
        vector<int> distance(n, -1);
        queue<int> q;
        
        q.push(start);
        distance[start] = 0;
        
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            
            for (int neighbor : graph[node]) {
                if (distance[neighbor] == -1) {
                    distance[neighbor] = distance[node] + 1;
                    q.push(neighbor);
                }
            }
        }
        
        return distance;
    }
    
    // 层次遍历
    vector<vector<int>> levelOrder(vector<vector<int>>& graph, int start) {
        vector<vector<int>> result;
        if (graph.empty()) return result;
        
        queue<int> q;
        vector<bool> visited(graph.size(), false);
        
        q.push(start);
        visited[start] = true;
        
        while (!q.empty()) {
            int size = q.size();
            vector<int> level;
            
            for (int i = 0; i < size; i++) {
                int node = q.front();
                q.pop();
                level.push_back(node);
                
                for (int neighbor : graph[node]) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
            
            result.push_back(level);
        }
        
        return result;
    }
};
```

#### 单词接龙 (LeetCode 127)

```cpp
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> wordSet(wordList.begin(), wordList.end());
        if (wordSet.find(endWord) == wordSet.end()) return 0;
        
        queue<string> q;
        q.push(beginWord);
        int level = 1;
        
        while (!q.empty()) {
            int size = q.size();
            
            for (int i = 0; i < size; i++) {
                string word = q.front();
                q.pop();
                
                if (word == endWord) return level;
                
                // 尝试改变每个位置的字符
                for (int j = 0; j < word.length(); j++) {
                    char originalChar = word[j];
                    
                    for (char c = 'a'; c <= 'z'; c++) {
                        if (c == originalChar) continue;
                        
                        word[j] = c;
                        if (wordSet.find(word) != wordSet.end()) {
                            q.push(word);
                            wordSet.erase(word); // 避免重复访问
                        }
                    }
                    
                    word[j] = originalChar; // 恢复
                }
            }
            
            level++;
        }
        
        return 0;
    }
};
```

#### 腐烂的橘子 (LeetCode 994)

```cpp
class Solution {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        int rows = grid.size();
        int cols = grid[0].size();
        
        queue<pair<int, int>> q;
        int freshOranges = 0;
        
        // 找到所有腐烂的橘子作为起点
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] == 2) {
                    q.push({i, j});
                } else if (grid[i][j] == 1) {
                    freshOranges++;
                }
            }
        }
        
        if (freshOranges == 0) return 0;
        
        int minutes = 0;
        vector<vector<int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        
        while (!q.empty()) {
            int size = q.size();
            bool hasNewRotten = false;
            
            for (int i = 0; i < size; i++) {
                auto [x, y] = q.front();
                q.pop();
                
                for (auto& dir : directions) {
                    int nx = x + dir[0];
                    int ny = y + dir[1];
                    
                    if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && grid[nx][ny] == 1) {
                        grid[nx][ny] = 2;
                        q.push({nx, ny});
                        freshOranges--;
                        hasNewRotten = true;
                    }
                }
            }
            
            if (hasNewRotten) minutes++;
        }
        
        return freshOranges == 0 ? minutes : -1;
    }
};
```

## 📚 2. 连通性问题

### 2.1 并查集 (Union-Find)

```cpp
class UnionFind {
private:
    vector<int> parent;
    vector<int> rank;
    int components;
    
public:
    UnionFind(int n) : parent(n), rank(n, 0), components(n) {
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // 路径压缩
        }
        return parent[x];
    }
    
    bool unionSets(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        
        if (rootX == rootY) return false;
        
        // 按秩合并
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
        
        components--;
        return true;
    }
    
    bool connected(int x, int y) {
        return find(x) == find(y);
    }
    
    int getComponents() {
        return components;
    }
};
```

#### 朋友圈 (LeetCode 547)

```cpp
class Solution {
public:
    int findCircleNum(vector<vector<int>>& isConnected) {
        int n = isConnected.size();
        UnionFind uf(n);
        
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (isConnected[i][j] == 1) {
                    uf.unionSets(i, j);
                }
            }
        }
        
        return uf.getComponents();
    }
};
```

#### 冗余连接 (LeetCode 684)

```cpp
class Solution {
public:
    vector<int> findRedundantConnection(vector<vector<int>>& edges) {
        int n = edges.size();
        UnionFind uf(n + 1);
        
        for (auto& edge : edges) {
            if (!uf.unionSets(edge[0], edge[1])) {
                return edge; // 这条边造成了环
            }
        }
        
        return {};
    }
};
```

#### 账户合并 (LeetCode 721)

```cpp
class Solution {
public:
    vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
        UnionFind uf(accounts.size());
        unordered_map<string, int> emailToId; // 邮箱到账户ID的映射
        
        // 建立邮箱到账户的映射，并合并相同邮箱的账户
        for (int i = 0; i < accounts.size(); i++) {
            for (int j = 1; j < accounts[i].size(); j++) {
                string email = accounts[i][j];
                if (emailToId.find(email) != emailToId.end()) {
                    uf.unionSets(i, emailToId[email]);
                } else {
                    emailToId[email] = i;
                }
            }
        }
        
        // 将邮箱按根账户分组
        unordered_map<int, vector<string>> groups;
        for (auto& [email, id] : emailToId) {
            groups[uf.find(id)].push_back(email);
        }
        
        // 构建结果
        vector<vector<string>> result;
        for (auto& [rootId, emails] : groups) {
            sort(emails.begin(), emails.end());
            vector<string> account = {accounts[rootId][0]};
            account.insert(account.end(), emails.begin(), emails.end());
            result.push_back(account);
        }
        
        return result;
    }
};
```

### 2.2 强连通分量

#### Kosaraju算法

```cpp
class KosarajuSCC {
private:
    vector<vector<int>> graph;
    vector<vector<int>> reverseGraph;
    vector<bool> visited;
    stack<int> finishOrder;
    
    void dfs1(int u) {
        visited[u] = true;
        for (int v : graph[u]) {
            if (!visited[v]) {
                dfs1(v);
            }
        }
        finishOrder.push(u);
    }
    
    void dfs2(int u, vector<int>& component) {
        visited[u] = true;
        component.push_back(u);
        for (int v : reverseGraph[u]) {
            if (!visited[v]) {
                dfs2(v, component);
            }
        }
    }
    
public:
    vector<vector<int>> findSCC(vector<vector<int>>& g) {
        int n = g.size();
        graph = g;
        reverseGraph.resize(n);
        visited.resize(n, false);
        
        // 构建反向图
        for (int u = 0; u < n; u++) {
            for (int v : graph[u]) {
                reverseGraph[v].push_back(u);
            }
        }
        
        // 第一次DFS，获取完成顺序
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                dfs1(i);
            }
        }
        
        // 重置visited数组
        fill(visited.begin(), visited.end(), false);
        
        // 第二次DFS，在反向图上按完成顺序的逆序进行
        vector<vector<int>> sccs;
        while (!finishOrder.empty()) {
            int u = finishOrder.top();
            finishOrder.pop();
            
            if (!visited[u]) {
                vector<int> component;
                dfs2(u, component);
                sccs.push_back(component);
            }
        }
        
        return sccs;
    }
};
```

## 📚 3. 拓扑排序

### 3.1 Kahn算法 (基于入度)

```cpp
class TopologicalSort {
public:
    vector<int> kahnsAlgorithm(int numCourses, vector<vector<int>>& prerequisites) {
        vector<vector<int>> graph(numCourses);
        vector<int> indegree(numCourses, 0);
        
        // 构建图和计算入度
        for (auto& prereq : prerequisites) {
            graph[prereq[1]].push_back(prereq[0]);
            indegree[prereq[0]]++;
        }
        
        // 将入度为0的节点加入队列
        queue<int> q;
        for (int i = 0; i < numCourses; i++) {
            if (indegree[i] == 0) {
                q.push(i);
            }
        }
        
        vector<int> result;
        while (!q.empty()) {
            int course = q.front();
            q.pop();
            result.push_back(course);
            
            // 遍历该课程的后续课程
            for (int nextCourse : graph[course]) {
                indegree[nextCourse]--;
                if (indegree[nextCourse] == 0) {
                    q.push(nextCourse);
                }
            }
        }
        
        // 检查是否有环
        return result.size() == numCourses ? result : vector<int>();
    }
};
```

#### 课程表 (LeetCode 207)

```cpp
class Solution {
public:
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        vector<vector<int>> graph(numCourses);
        vector<int> indegree(numCourses, 0);
        
        for (auto& prereq : prerequisites) {
            graph[prereq[1]].push_back(prereq[0]);
            indegree[prereq[0]]++;
        }
        
        queue<int> q;
        for (int i = 0; i < numCourses; i++) {
            if (indegree[i] == 0) {
                q.push(i);
            }
        }
        
        int completed = 0;
        while (!q.empty()) {
            int course = q.front();
            q.pop();
            completed++;
            
            for (int nextCourse : graph[course]) {
                indegree[nextCourse]--;
                if (indegree[nextCourse] == 0) {
                    q.push(nextCourse);
                }
            }
        }
        
        return completed == numCourses;
    }
};
```

#### 课程表 II (LeetCode 210)

```cpp
class Solution {
public:
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        vector<vector<int>> graph(numCourses);
        vector<int> indegree(numCourses, 0);
        
        for (auto& prereq : prerequisites) {
            graph[prereq[1]].push_back(prereq[0]);
            indegree[prereq[0]]++;
        }
        
        queue<int> q;
        for (int i = 0; i < numCourses; i++) {
            if (indegree[i] == 0) {
                q.push(i);
            }
        }
        
        vector<int> result;
        while (!q.empty()) {
            int course = q.front();
            q.pop();
            result.push_back(course);
            
            for (int nextCourse : graph[course]) {
                indegree[nextCourse]--;
                if (indegree[nextCourse] == 0) {
                    q.push(nextCourse);
                }
            }
        }
        
        return result.size() == numCourses ? result : vector<int>();
    }
};
```

### 3.2 基于DFS的拓扑排序

```cpp
class TopologicalSortDFS {
private:
    vector<vector<int>> graph;
    vector<int> color; // 0: 白色(未访问), 1: 灰色(正在访问), 2: 黑色(已完成)
    vector<int> result;
    bool hasCycle;
    
    void dfs(int u) {
        color[u] = 1; // 标记为正在访问
        
        for (int v : graph[u]) {
            if (color[v] == 1) {
                hasCycle = true; // 发现后向边，存在环
                return;
            }
            if (color[v] == 0) {
                dfs(v);
            }
        }
        
        color[u] = 2; // 标记为已完成
        result.push_back(u);
    }
    
public:
    vector<int> topologicalSort(int n, vector<vector<int>>& edges) {
        graph.resize(n);
        color.resize(n, 0);
        hasCycle = false;
        
        // 构建图
        for (auto& edge : edges) {
            graph[edge[0]].push_back(edge[1]);
        }
        
        // 对每个未访问的节点进行DFS
        for (int i = 0; i < n; i++) {
            if (color[i] == 0) {
                dfs(i);
                if (hasCycle) return {};
            }
        }
        
        reverse(result.begin(), result.end());
        return result;
    }
};
```

## 📚 4. 最短路径算法

### 4.1 Dijkstra算法

```cpp
class Dijkstra {
public:
    vector<int> shortestPath(vector<vector<pair<int, int>>>& graph, int start) {
        int n = graph.size();
        vector<int> dist(n, INT_MAX);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        
        dist[start] = 0;
        pq.push({0, start});
        
        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            
            if (d > dist[u]) continue;
            
            for (auto [v, weight] : graph[u]) {
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }
        
        return dist;
    }
    
    // 带路径记录的版本
    pair<vector<int>, vector<int>> shortestPathWithPath(vector<vector<pair<int, int>>>& graph, int start) {
        int n = graph.size();
        vector<int> dist(n, INT_MAX);
        vector<int> parent(n, -1);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        
        dist[start] = 0;
        pq.push({0, start});
        
        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            
            if (d > dist[u]) continue;
            
            for (auto [v, weight] : graph[u]) {
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }
        
        return {dist, parent};
    }
    
    // 重构路径
    vector<int> reconstructPath(vector<int>& parent, int start, int end) {
        vector<int> path;
        for (int v = end; v != -1; v = parent[v]) {
            path.push_back(v);
        }
        reverse(path.begin(), path.end());
        return path[0] == start ? path : vector<int>();
    }
};
```

#### 网络延迟时间 (LeetCode 743)

```cpp
class Solution {
public:
    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        vector<vector<pair<int, int>>> graph(n + 1);
        
        // 构建图
        for (auto& time : times) {
            graph[time[0]].push_back({time[1], time[2]});
        }
        
        // Dijkstra算法
        vector<int> dist(n + 1, INT_MAX);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        
        dist[k] = 0;
        pq.push({0, k});
        
        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            
            if (d > dist[u]) continue;
            
            for (auto [v, weight] : graph[u]) {
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }
        
        int maxTime = 0;
        for (int i = 1; i <= n; i++) {
            if (dist[i] == INT_MAX) return -1;
            maxTime = max(maxTime, dist[i]);
        }
        
        return maxTime;
    }
};
```

#### 最便宜的航班 (LeetCode 787)

```cpp
class Solution {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
        vector<vector<pair<int, int>>> graph(n);
        
        for (auto& flight : flights) {
            graph[flight[0]].push_back({flight[1], flight[2]});
        }
        
        // {cost, {node, stops}}
        priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>> pq;
        vector<vector<int>> dist(n, vector<int>(k + 2, INT_MAX));
        
        pq.push({0, src, 0});
        dist[src][0] = 0;
        
        while (!pq.empty()) {
            auto [cost, node, stops] = pq.top();
            pq.pop();
            
            if (node == dst) return cost;
            if (stops > k) continue;
            if (cost > dist[node][stops]) continue;
            
            for (auto [nextNode, price] : graph[node]) {
                int newCost = cost + price;
                int newStops = stops + 1;
                
                if (newCost < dist[nextNode][newStops]) {
                    dist[nextNode][newStops] = newCost;
                    pq.push({newCost, nextNode, newStops});
                }
            }
        }
        
        return -1;
    }
};
```

### 4.2 Bellman-Ford算法

```cpp
class BellmanFord {
public:
    pair<vector<int>, bool> shortestPath(int n, vector<tuple<int, int, int>>& edges, int start) {
        vector<int> dist(n, INT_MAX);
        dist[start] = 0;
        
        // 松弛操作进行n-1次
        for (int i = 0; i < n - 1; i++) {
            bool updated = false;
            for (auto [u, v, weight] : edges) {
                if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    updated = true;
                }
            }
            if (!updated) break; // 提前终止优化
        }
        
        // 检查负环
        bool hasNegativeCycle = false;
        for (auto [u, v, weight] : edges) {
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                hasNegativeCycle = true;
                break;
            }
        }
        
        return {dist, hasNegativeCycle};
    }
};
```

### 4.3 Floyd-Warshall算法

```cpp
class FloydWarshall {
public:
    vector<vector<int>> allPairsShortestPath(vector<vector<int>>& graph) {
        int n = graph.size();
        vector<vector<int>> dist = graph;
        
        // 初始化
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) {
                    dist[i][j] = 0;
                } else if (dist[i][j] == 0) {
                    dist[i][j] = INT_MAX;
                }
            }
        }
        
        // Floyd-Warshall核心算法
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
};
```

## 📚 5. 最小生成树

### 5.1 Prim算法

```cpp
class Prim {
public:
    int minimumSpanningTree(int n, vector<tuple<int, int, int>>& edges) {
        vector<vector<pair<int, int>>> graph(n);
        
        // 构建图
        for (auto [u, v, weight] : edges) {
            graph[u].push_back({v, weight});
            graph[v].push_back({u, weight});
        }
        
        vector<bool> inMST(n, false);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        
        // 从节点0开始
        inMST[0] = true;
        for (auto [v, weight] : graph[0]) {
            pq.push({weight, v});
        }
        
        int mstWeight = 0;
        int edgesAdded = 0;
        
        while (!pq.empty() && edgesAdded < n - 1) {
            auto [weight, v] = pq.top();
            pq.pop();
            
            if (inMST[v]) continue;
            
            inMST[v] = true;
            mstWeight += weight;
            edgesAdded++;
            
            for (auto [nextV, nextWeight] : graph[v]) {
                if (!inMST[nextV]) {
                    pq.push({nextWeight, nextV});
                }
            }
        }
        
        return edgesAdded == n - 1 ? mstWeight : -1;
    }
};
```

### 5.2 Kruskal算法

```cpp
class Kruskal {
public:
    int minimumSpanningTree(int n, vector<tuple<int, int, int>>& edges) {
        // 按权重排序
        sort(edges.begin(), edges.end(), [](const auto& a, const auto& b) {
            return get<2>(a) < get<2>(b);
        });
        
        UnionFind uf(n);
        int mstWeight = 0;
        int edgesAdded = 0;
        
        for (auto [u, v, weight] : edges) {
            if (uf.unionSets(u, v)) {
                mstWeight += weight;
                edgesAdded++;
                if (edgesAdded == n - 1) break;
            }
        }
        
        return edgesAdded == n - 1 ? mstWeight : -1;
    }
};
```

#### 连接所有点的最小费用 (LeetCode 1584)

```cpp
class Solution {
public:
    int minCostConnectPoints(vector<vector<int>>& points) {
        int n = points.size();
        vector<tuple<int, int, int>> edges;
        
        // 生成所有边
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                int dist = abs(points[i][0] - points[j][0]) + abs(points[i][1] - points[j][1]);
                edges.push_back({i, j, dist});
            }
        }
        
        // Kruskal算法
        sort(edges.begin(), edges.end(), [](const auto& a, const auto& b) {
            return get<2>(a) < get<2>(b);
        });
        
        UnionFind uf(n);
        int totalCost = 0;
        int edgesAdded = 0;
        
        for (auto [u, v, cost] : edges) {
            if (uf.unionSets(u, v)) {
                totalCost += cost;
                edgesAdded++;
                if (edgesAdded == n - 1) break;
            }
        }
        
        return totalCost;
    }
};
```

## 📚 6. 特殊图算法

### 6.1 二分图检测

```cpp
class BipartiteGraph {
public:
    bool isBipartite(vector<vector<int>>& graph) {
        int n = graph.size();
        vector<int> color(n, -1);
        
        for (int i = 0; i < n; i++) {
            if (color[i] == -1) {
                if (!dfs(graph, i, 0, color)) {
                    return false;
                }
            }
        }
        
        return true;
    }
    
private:
    bool dfs(vector<vector<int>>& graph, int node, int c, vector<int>& color) {
        color[node] = c;
        
        for (int neighbor : graph[node]) {
            if (color[neighbor] == c) {
                return false; // 相邻节点同色
            }
            if (color[neighbor] == -1 && !dfs(graph, neighbor, 1 - c, color)) {
                return false;
            }
        }
        
        return true;
    }
    
    // BFS版本
    bool isBipartiteBFS(vector<vector<int>>& graph) {
        int n = graph.size();
        vector<int> color(n, -1);
        
        for (int i = 0; i < n; i++) {
            if (color[i] == -1) {
                queue<int> q;
                q.push(i);
                color[i] = 0;
                
                while (!q.empty()) {
                    int node = q.front();
                    q.pop();
                    
                    for (int neighbor : graph[node]) {
                        if (color[neighbor] == color[node]) {
                            return false;
                        }
                        if (color[neighbor] == -1) {
                            color[neighbor] = 1 - color[node];
                            q.push(neighbor);
                        }
                    }
                }
            }
        }
        
        return true;
    }
};
```

### 6.2 关键路径和关键边

#### 关键连接 (LeetCode 1192)

```cpp
class Solution {
public:
    vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections) {
        vector<vector<int>> graph(n);
        for (auto& conn : connections) {
            graph[conn[0]].push_back(conn[1]);
            graph[conn[1]].push_back(conn[0]);
        }
        
        vector<int> disc(n, -1); // 发现时间
        vector<int> low(n, -1);  // 能回到的最早节点
        vector<bool> visited(n, false);
        vector<vector<int>> bridges;
        int timer = 0;
        
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                tarjan(i, -1, graph, disc, low, visited, bridges, timer);
            }
        }
        
        return bridges;
    }
    
private:
    void tarjan(int u, int parent, vector<vector<int>>& graph, 
                vector<int>& disc, vector<int>& low, vector<bool>& visited,
                vector<vector<int>>& bridges, int& timer) {
        visited[u] = true;
        disc[u] = low[u] = timer++;
        
        for (int v : graph[u]) {
            if (v == parent) continue;
            
            if (!visited[v]) {
                tarjan(v, u, graph, disc, low, visited, bridges, timer);
                low[u] = min(low[u], low[v]);
                
                // 如果low[v] > disc[u]，则(u,v)是桥
                if (low[v] > disc[u]) {
                    bridges.push_back({u, v});
                }
            } else {
                low[u] = min(low[u], disc[v]);
            }
        }
    }
};
```

## 💡 图论算法解题技巧

### 1. 算法选择指南
- **DFS**: 路径问题、连通性、拓扑排序
- **BFS**: 最短路径（无权图）、层次遍历
- **并查集**: 动态连通性、最小生成树
- **Dijkstra**: 单源最短路径（非负权重）
- **Bellman-Ford**: 单源最短路径（可能有负权重）
- **Floyd-Warshall**: 所有点对最短路径

### 2. 复杂度分析
- **DFS/BFS**: O(V + E)
- **Dijkstra**: O((V + E) log V)
- **Bellman-Ford**: O(VE)
- **Floyd-Warshall**: O(V³)
- **并查集**: O(α(n))均摊时间

### 3. 常见优化技巧
- **双向BFS**: 减少搜索空间
- **A*算法**: 启发式搜索
- **路径压缩**: 并查集优化
- **二进制优化**: 多重背包等问题

### 4. 注意事项
- **图的表示**: 根据稠密程度选择邻接表或邻接矩阵
- **边界条件**: 空图、单点图、自环、重边
- **数据范围**: 选择合适的数据类型和算法

## 🔥 经典题目推荐

### 基础遍历 (Easy-Medium)
1. 岛屿数量 (200)
2. 克隆图 (133)
3. 课程表 (207)
4. 腐烂的橘子 (994)

### 连通性问题 (Medium)
1. 朋友圈 (547)
2. 冗余连接 (684)
3. 账户合并 (721)
4. 最长连续序列 (128)

### 最短路径 (Medium-Hard)
1. 网络延迟时间 (743)
2. 最便宜的航班 (787)
3. 单词接龙 (127)
4. 到达目的地的方案数 (1976)

### 高级算法 (Hard)
1. 关键连接 (1192)
2. 最小生成树 (1584)
3. 外星人字典 (269)
4. 强连通分量相关问题

## 🚀 下一步
- 练习 [面试高频题](./interview-high-frequency.md)
- 学习 [动态规划题详解](./dynamic-programming.md)
- 回顾 [数据结构题详解](./data-structure-problems.md)
- 查看 [基础算法题详解](./basic-algorithms.md)

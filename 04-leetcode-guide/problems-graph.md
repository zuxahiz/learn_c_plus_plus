# 🕸️ 图论问题完整列表

## 📈 统计信息
- **总题目数**: ~300题
- **Easy**: ~40题
- **Medium**: ~160题  
- **Hard**: ~100题
- **高频面试题**: ~80题

## 🎯 核心知识点
- 图的表示 (邻接表、邻接矩阵)
- 图的遍历 (DFS、BFS)
- 连通性问题 (连通分量、强连通分量)
- 最短路径 (Dijkstra、Floyd、Bellman-Ford)
- 最小生成树 (Prim、Kruskal)
- 拓扑排序
- 网络流问题
- 并查集应用

---

## 📚 分类题目列表

### 🔰 图基础遍历 (Easy-Medium 80题)

#### DFS/BFS基础类
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 200 | Number of Islands | Medium | ⭐⭐⭐ | DFS/BFS |
| 695 | Max Area of Island | Medium | ⭐⭐⭐ | DFS |
| 463 | Island Perimeter | Easy | ⭐⭐ | DFS |
| 733 | Flood Fill | Easy | ⭐⭐⭐ | DFS/BFS |
| 130 | Surrounded Regions | Medium | ⭐⭐ | DFS |
| 417 | Pacific Atlantic Water Flow | Medium | ⭐⭐ | DFS |
| 1020 | Number of Enclaves | Medium | ⭐ | DFS |
| 1254 | Number of Closed Islands | Medium | ⭐ | DFS |
| 1034 | Coloring A Border | Medium | ⭐ | DFS |
| 1559 | Detect Cycles in 2D Grid | Medium | ⭐ | DFS |
| 79 | Word Search | Medium | ⭐⭐⭐ | DFS回溯 |
| 212 | Word Search II | Hard | ⭐⭐⭐ | DFS+Trie |
| 694 | Number of Distinct Islands | Medium | ⭐⭐ | DFS |
| 711 | Number of Distinct Islands II | Hard | ⭐ | DFS+变换 |
| 1391 | Check if There is a Valid Path in a Grid | Medium | ⭐ | DFS |

#### BFS层次遍历类
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 542 | 01 Matrix | Medium | ⭐⭐ | BFS |
| 994 | Rotting Oranges | Medium | ⭐⭐⭐ | BFS |
| 1162 | As Far from Land as Possible | Medium | ⭐⭐ | BFS |
| 286 | Walls and Gates | Medium | ⭐⭐ | BFS |
| 317 | Shortest Distance from All Buildings | Hard | ⭐ | BFS |
| 934 | Shortest Bridge | Medium | ⭐⭐ | BFS |
| 1091 | Shortest Path in Binary Matrix | Medium | ⭐⭐ | BFS |
| 1293 | Shortest Path in a Grid with Obstacles Elimination | Hard | ⭐ | BFS |
| 1168 | Optimize Water Distribution in a Village | Hard | ⭐ | MST |
| 407 | Trapping Rain Water II | Hard | ⭐⭐ | BFS+堆 |
| 909 | Snakes and Ladders | Medium | ⭐ | BFS |
| 1129 | Shortest Path with Alternating Colors | Medium | ⭐ | BFS |
| 1263 | Minimum Moves to Move a Box to Their Target Location | Hard | ⭐ | BFS |
| 1345 | Jump Game IV | Hard | ⭐ | BFS |
| 847 | Shortest Path Visiting All Nodes | Hard | ⭐⭐ | BFS+状态压缩 |

#### 并查集基础类
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 990 | Satisfiability of Equality Equations | Medium | ⭐⭐ | 并查集 |
| 721 | Accounts Merge | Medium | ⭐⭐⭐ | 并查集 |
| 684 | Redundant Connection | Medium | ⭐⭐ | 并查集 |
| 685 | Redundant Connection II | Hard | ⭐⭐ | 并查集 |
| 547 | Number of Provinces | Medium | ⭐⭐⭐ | 并查集/DFS |
| 1319 | Number of Operations to Make Network Connected | Medium | ⭐⭐ | 并查集 |
| 1202 | Smallest String With Swaps | Medium | ⭐⭐ | 并查集 |
| 1488 | Avoid Flood in The City | Medium | ⭐ | 并查集 |
| 1584 | Min Cost to Connect All Points | Medium | ⭐⭐ | MST |
| 1697 | Checking Existence of Edge Length Limited Paths | Hard | ⭐ | 并查集 |
| 803 | Bricks Falling When Hit | Hard | ⭐ | 逆向并查集 |
| 1258 | Synonymous Sentences | Medium | ⭐ | 并查集 |
| 952 | Largest Component Size by Common Factor | Hard | ⭐ | 并查集 |
| 1632 | Rank Transform of a Matrix | Hard | ⭐ | 并查集 |
| 1998 | GCD Sort of an Array | Hard | ⭐ | 并查集 |

### 🔶 图的路径问题 (Medium-Hard 100题)

#### 最短路径类
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 743 | Network Delay Time | Medium | ⭐⭐⭐ | Dijkstra |
| 787 | Cheapest Flights Within K Stops | Medium | ⭐⭐⭐ | Bellman-Ford |
| 1334 | Find the City With the Smallest Number of Neighbors | Medium | ⭐⭐ | Floyd |
| 1368 | Minimum Cost to Make at Least One Valid Path | Hard | ⭐ | Dijkstra |
| 1631 | Path With Minimum Effort | Medium | ⭐⭐ | Dijkstra |
| 1514 | Path with Maximum Probability | Medium | ⭐ | Dijkstra |
| 778 | Swim in Rising Water | Hard | ⭐ | Dijkstra |
| 1976 | Number of Ways to Arrive at Destination | Medium | ⭐ | Dijkstra |
| 1786 | Number of Restricted Paths From First to Last Node | Medium | ⭐ | Dijkstra |
| 882 | Reachable Nodes In Subdivided Graph | Hard | ⭐ | Dijkstra |
| 1928 | Minimum Cost to Reach Destination in Time | Hard | ⭐ | Dijkstra |
| 1499 | Max Value of Equation | Hard | ⭐ | 单调队列 |
| 1102 | Path With Maximum Minimum Value | Medium | ⭐ | 二分+BFS |
| 1548 | The Most Similar Path in a Graph | Hard | ⭐ | DP |
| 1263 | Minimum Moves to Move a Box | Hard | ⭐ | BFS |

#### 特殊路径问题类
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 1277 | Count Square Submatrices with All Ones | Medium | ⭐ | DP |
| 1219 | Path with Maximum Gold | Medium | ⭐⭐ | DFS |
| 1066 | Campus Bikes II | Medium | ⭐ | 状态压缩DP |
| 1349 | Maximum Students Taking Exam | Hard | ⭐ | 状态压缩DP |
| 1601 | Maximum Number of Achievable Transfer Requests | Hard | ⭐ | 回溯 |
| 1655 | Distribute Repeating Integers | Hard | ⭐ | 状态压缩DP |
| 1681 | Minimum Incompatibility | Hard | ⭐ | 状态压缩DP |
| 1723 | Find Minimum Time to Finish All Jobs | Hard | ⭐ | 回溯 |
| 1755 | Closest Subsequence Sum | Hard | ⭐ | 双向搜索 |
| 1766 | Tree of Coprimes | Hard | ⭐ | DFS |
| 1724 | Checking Existence of Edge Length Limited Paths II | Hard | ⭐ | 动态连通性 |
| 1857 | Largest Color Value in a Directed Graph | Hard | ⭐ | 拓扑排序 |
| 1928 | Minimum Cost to Reach Destination in Time | Hard | ⭐ | Dijkstra |
| 1976 | Number of Ways to Arrive at Destination | Medium | ⭐ | Dijkstra |
| 2045 | Second Minimum Time to Reach Destination | Hard | ⭐ | BFS |

#### 环检测类
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 207 | Course Schedule | Medium | ⭐⭐⭐ | 拓扑排序 |
| 210 | Course Schedule II | Medium | ⭐⭐⭐ | 拓扑排序 |
| 1136 | Parallel Courses | Medium | ⭐ | 拓扑排序 |
| 1494 | Parallel Courses II | Hard | ⭐ | 状态压缩DP |
| 2050 | Parallel Courses III | Hard | ⭐ | 拓扑排序 |
| 802 | Find Eventual Safe States | Medium | ⭐⭐ | DFS |
| 1591 | Strange Printer II | Hard | ⭐ | 拓扑排序 |
| 444 | Sequence Reconstruction | Medium | ⭐ | 拓扑排序 |
| 269 | Alien Dictionary | Hard | ⭐⭐ | 拓扑排序 |
| 310 | Minimum Height Trees | Medium | ⭐⭐ | 拓扑排序 |
| 1203 | Sort Items by Groups Respecting Dependencies | Hard | ⭐ | 拓扑排序 |
| 631 | Design Excel Sum Formula | Hard | ⭐ | 拓扑排序 |
| 1462 | Course Schedule IV | Medium | ⭐ | Floyd/DFS |
| 1557 | Minimum Number of Vertices to Reach All Nodes | Medium | ⭐ | 入度 |
| 1976 | Number of Ways to Arrive at Destination | Medium | ⭐ | 拓扑排序 |

### 🔴 图的高级算法 (Hard 120题)

#### 网络流类
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 1617 | Count Subtrees With Max Distance Between Cities | Hard | ⭐ | 状态枚举 |
| 1595 | Minimum Cost to Connect Two Groups of Points | Hard | ⭐ | 状态压缩DP |
| 1879 | Minimum XOR Sum of Two Arrays | Hard | ⭐ | 状态压缩DP |
| 1125 | Smallest Sufficient Team | Hard | ⭐ | 状态压缩DP |
| 1349 | Maximum Students Taking Exam | Hard | ⭐ | 二分图匹配 |
| 1066 | Campus Bikes II | Medium | ⭐ | 最小权匹配 |
| 1820 | Maximum Number of Accepted Invitations | Medium | ⭐ | 二分图匹配 |
| 1947 | Maximum Compatibility Score Sum | Medium | ⭐ | 状态压缩DP |
| 2123 | Minimum Operations to Remove Adjacent Ones in Matrix | Hard | ⭐ | 二分图匹配 |
| 1681 | Minimum Incompatibility | Hard | ⭐ | 状态压缩DP |

#### 强连通分量类
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 1192 | Critical Connections in a Network | Hard | ⭐⭐ | Tarjan |
| 1489 | Find Critical and Pseudo-Critical Edges in MST | Hard | ⭐ | MST |
| 1568 | Minimum Number of Days to Disconnect Island | Hard | ⭐ | 割点 |
| 1761 | Minimum Degree of a Connected Trio in a Graph | Hard | ⭐ | 图论 |
| 924 | Minimize Malware Spread | Hard | ⭐ | 并查集 |
| 928 | Minimize Malware Spread II | Hard | ⭐ | 并查集 |
| 1376 | Time Needed to Inform All Employees | Medium | ⭐ | DFS |
| 1377 | Frog Position After T Seconds | Hard | ⭐ | DFS |
| 1466 | Reorder Routes to Make All Paths Lead to the City Zero | Medium | ⭐ | DFS |
| 1575 | Count All Possible Routes | Hard | ⭐ | DP |

#### 最小生成树类
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 1584 | Min Cost to Connect All Points | Medium | ⭐⭐ | MST |
| 1135 | Connecting Cities With Minimum Cost | Medium | ⭐⭐ | MST |
| 1168 | Optimize Water Distribution in a Village | Hard | ⭐ | MST |
| 1489 | Find Critical and Pseudo-Critical Edges | Hard | ⭐ | MST |
| 1724 | Checking Existence of Edge Length Limited Paths II | Hard | ⭐ | 动态MST |
| 1562 | Find Latest Group of Size M | Medium | ⭐ | 并查集 |
| 1627 | Graph Connectivity With Threshold | Hard | ⭐ | 并查集 |
| 1998 | GCD Sort of an Array | Hard | ⭐ | MST |
| 2077 | Paths in Maze That Lead to Same Room | Medium | ⭐ | 图遍历 |
| 2092 | Find All People With Secret | Hard | ⭐ | 并查集 |

#### 特殊图算法类
| 题号 | 题目 | 难度 | 频率 | 标签 |
|------|------|------|------|------|
| 332 | Reconstruct Itinerary | Medium | ⭐⭐ | 欧拉路径 |
| 753 | Cracking the Safe | Hard | ⭐ | 欧拉路径 |
| 1494 | Parallel Courses II | Hard | ⭐ | 状态压缩 |
| 1210 | Minimum Moves to Reach Target with Rotations | Hard | ⭐ | BFS |
| 1298 | Maximum Candies You Can Get from Boxes | Hard | ⭐ | BFS |
| 1345 | Jump Game IV | Hard | ⭐ | BFS |
| 1377 | Frog Position After T Seconds | Hard | ⭐ | DFS |
| 1568 | Minimum Number of Days to Disconnect Island | Hard | ⭐ | 割点 |
| 1579 | Remove Max Number of Edges to Keep Graph Fully Traversable | Hard | ⭐ | 并查集 |
| 1659 | Maximize Grid Happiness | Hard | ⭐ | 状态压缩DP |
| 1724 | Checking Existence of Edge Length Limited Paths II | Hard | ⭐ | 动态连通性 |
| 1928 | Minimum Cost to Reach Destination in Time | Hard | ⭐ | Dijkstra |
| 1970 | Last Day Where You Can Still Cross | Hard | ⭐ | 二分+并查集 |
| 2076 | Process Restricted Friend Requests | Hard | ⭐ | 并查集 |
| 2101 | Detonate the Maximum Bombs | Medium | ⭐ | DFS |

## 🎯 重点题目推荐

### 🔥 面试必备 (Top 20)
| 难度 | 题目 | 重要性 | 考察频率 |
|------|------|---------|----------|
| Medium | 200. Number of Islands | ⭐⭐⭐⭐⭐ | 极高 |
| Medium | 207. Course Schedule | ⭐⭐⭐⭐⭐ | 极高 |
| Medium | 994. Rotting Oranges | ⭐⭐⭐⭐ | 很高 |
| Easy | 733. Flood Fill | ⭐⭐⭐⭐ | 很高 |
| Medium | 721. Accounts Merge | ⭐⭐⭐⭐ | 很高 |
| Medium | 547. Number of Provinces | ⭐⭐⭐⭐ | 很高 |
| Medium | 743. Network Delay Time | ⭐⭐⭐⭐ | 很高 |
| Hard | 1192. Critical Connections | ⭐⭐⭐⭐ | 很高 |
| Medium | 684. Redundant Connection | ⭐⭐⭐ | 高 |
| Medium | 695. Max Area of Island | ⭐⭐⭐ | 高 |

### 🏢 公司偏好题目

#### 🟦 Facebook/Meta
- 200. Number of Islands
- 721. Accounts Merge
- 994. Rotting Oranges
- 1091. Shortest Path in Binary Matrix
- 1192. Critical Connections

#### 🟨 Google
- 207. Course Schedule
- 743. Network Delay Time
- 787. Cheapest Flights Within K Stops
- 1584. Min Cost to Connect All Points
- 847. Shortest Path Visiting All Nodes

#### 🟧 Amazon
- 200. Number of Islands
- 994. Rotting Oranges
- 547. Number of Provinces
- 1162. As Far from Land as Possible
- 1319. Number of Operations to Make Network Connected

#### ⬛ Apple
- 733. Flood Fill
- 200. Number of Islands
- 207. Course Schedule
- 684. Redundant Connection
- 542. 01 Matrix

#### 🟩 Microsoft
- 200. Number of Islands
- 207. Course Schedule
- 721. Accounts Merge
- 743. Network Delay Time
- 547. Number of Provinces

## 📝 学习建议

### 🎯 刷题顺序
1. **基础阶段**: 图的遍历 (200, 733, 695, 463)
2. **进阶阶段**: 拓扑排序和并查集 (207, 547, 684, 721)
3. **提高阶段**: 最短路径 (743, 787, 1631)
4. **高级阶段**: 复杂图算法 (1192, 332, 1584)

### 💡 核心算法
1. **DFS/BFS**: 图的基础遍历算法
2. **并查集**: 处理连通性问题
3. **拓扑排序**: 有向无环图的排序
4. **最短路径**: Dijkstra、Floyd、Bellman-Ford
5. **最小生成树**: Prim、Kruskal算法
6. **强连通分量**: Tarjan、Kosaraju算法

### ⚡ 复杂度分析
- **DFS/BFS**: O(V + E) - V顶点数，E边数
- **Dijkstra**: O((V + E)logV) - 使用优先队列
- **Floyd**: O(V³) - 全源最短路径
- **并查集**: O(α(n)) - 近似常数时间
- **Kruskal**: O(ElogE) - 主要是排序的复杂度

### 🎨 图算法模板

#### DFS模板
```cpp
void dfs(vector<vector<int>>& graph, int node, vector<bool>& visited) {
    visited[node] = true;
    
    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            dfs(graph, neighbor, visited);
        }
    }
}
```

#### BFS模板
```cpp
int bfs(vector<vector<int>>& graph, int start, int target) {
    queue<int> q;
    vector<bool> visited(graph.size(), false);
    
    q.push(start);
    visited[start] = true;
    int level = 0;
    
    while (!q.empty()) {
        int size = q.size();
        for (int i = 0; i < size; i++) {
            int node = q.front();
            q.pop();
            
            if (node == target) return level;
            
            for (int neighbor : graph[node]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
        level++;
    }
    
    return -1;
}
```

#### 并查集模板
```cpp
class UnionFind {
public:
    vector<int> parent, rank;
    
    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false;
        
        if (rank[px] < rank[py]) {
            parent[px] = py;
        } else if (rank[px] > rank[py]) {
            parent[py] = px;
        } else {
            parent[py] = px;
            rank[px]++;
        }
        return true;
    }
};
```

#### Dijkstra模板
```cpp
vector<int> dijkstra(vector<vector<pair<int, int>>>& graph, int start) {
    int n = graph.size();
    vector<int> dist(n, INT_MAX);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    dist[start] = 0;
    pq.push({0, start});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        if (d > dist[u]) continue;
        
        for (auto [v, w] : graph[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    
    return dist;
}
```

**图论是算法中最重要的分支之一，掌握图算法对解决复杂问题至关重要！** 🕸️🚀

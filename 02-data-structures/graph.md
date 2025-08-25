# 图数据结构实现详解

## 🎯 图的基本概念

图(Graph)是由顶点(Vertex)和边(Edge)组成的数据结构，用于表示对象之间的关系。图在计算机科学中有着广泛的应用，如社交网络、路径规划、依赖关系等。

### 图的基本术语
- **顶点(Vertex/Node)**: 图中的基本单元
- **边(Edge)**: 连接两个顶点的线
- **有向图**: 边有方向的图
- **无向图**: 边无方向的图
- **权重图**: 边带有权重的图
- **度数**: 与顶点相连的边数

### 图的分类
1. **按方向**: 有向图、无向图
2. **按权重**: 有权图、无权图
3. **按连通性**: 连通图、非连通图
4. **按环**: 有环图、无环图(DAG)

## 📚 图的表示方法

### 1. 邻接矩阵实现

```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <limits>
#include <functional>

template<typename T>
class AdjacencyMatrix {
private:
    std::vector<std::vector<int>> matrix;
    std::vector<T> vertices;
    std::unordered_map<T, int> vertexIndex;
    bool isDirected;
    
public:
    // 构造函数
    explicit AdjacencyMatrix(bool directed = false) : isDirected(directed) {}
    
    // 添加顶点
    void addVertex(const T& vertex) {
        if (vertexIndex.find(vertex) != vertexIndex.end()) {
            return; // 顶点已存在
        }
        
        int index = vertices.size();
        vertices.push_back(vertex);
        vertexIndex[vertex] = index;
        
        // 扩展矩阵
        for (auto& row : matrix) {
            row.push_back(0);
        }
        matrix.push_back(std::vector<int>(vertices.size(), 0));
    }
    
    // 添加边
    void addEdge(const T& from, const T& to, int weight = 1) {
        addVertex(from);
        addVertex(to);
        
        int fromIndex = vertexIndex[from];
        int toIndex = vertexIndex[to];
        
        matrix[fromIndex][toIndex] = weight;
        
        if (!isDirected) {
            matrix[toIndex][fromIndex] = weight;
        }
    }
    
    // 删除边
    void removeEdge(const T& from, const T& to) {
        if (vertexIndex.find(from) == vertexIndex.end() || 
            vertexIndex.find(to) == vertexIndex.end()) {
            return;
        }
        
        int fromIndex = vertexIndex[from];
        int toIndex = vertexIndex[to];
        
        matrix[fromIndex][toIndex] = 0;
        
        if (!isDirected) {
            matrix[toIndex][fromIndex] = 0;
        }
    }
    
    // 检查边是否存在
    bool hasEdge(const T& from, const T& to) const {
        if (vertexIndex.find(from) == vertexIndex.end() || 
            vertexIndex.find(to) == vertexIndex.end()) {
            return false;
        }
        
        int fromIndex = vertexIndex.at(from);
        int toIndex = vertexIndex.at(to);
        
        return matrix[fromIndex][toIndex] != 0;
    }
    
    // 获取边的权重
    int getWeight(const T& from, const T& to) const {
        if (!hasEdge(from, to)) {
            return 0;
        }
        
        int fromIndex = vertexIndex.at(from);
        int toIndex = vertexIndex.at(to);
        
        return matrix[fromIndex][toIndex];
    }
    
    // 获取顶点的邻居
    std::vector<T> getNeighbors(const T& vertex) const {
        std::vector<T> neighbors;
        
        if (vertexIndex.find(vertex) == vertexIndex.end()) {
            return neighbors;
        }
        
        int index = vertexIndex.at(vertex);
        
        for (int i = 0; i < vertices.size(); ++i) {
            if (matrix[index][i] != 0) {
                neighbors.push_back(vertices[i]);
            }
        }
        
        return neighbors;
    }
    
    // 获取顶点数量
    size_t getVertexCount() const {
        return vertices.size();
    }
    
    // 获取边数量
    size_t getEdgeCount() const {
        size_t count = 0;
        
        for (int i = 0; i < matrix.size(); ++i) {
            for (int j = 0; j < matrix[i].size(); ++j) {
                if (matrix[i][j] != 0) {
                    count++;
                }
            }
        }
        
        return isDirected ? count : count / 2;
    }
    
    // 打印图
    void printGraph() const {
        std::cout << "邻接矩阵表示:" << std::endl;
        
        // 打印顶点标题
        std::cout << "     ";
        for (const auto& vertex : vertices) {
            std::cout << vertex << "  ";
        }
        std::cout << std::endl;
        
        // 打印矩阵
        for (int i = 0; i < matrix.size(); ++i) {
            std::cout << vertices[i] << " -> ";
            for (int j = 0; j < matrix[i].size(); ++j) {
                std::cout << matrix[i][j] << "  ";
            }
            std::cout << std::endl;
        }
    }
    
    // 获取所有顶点
    const std::vector<T>& getVertices() const {
        return vertices;
    }
    
    // 获取矩阵
    const std::vector<std::vector<int>>& getMatrix() const {
        return matrix;
    }
};
```

### 2. 邻接表实现

```cpp
template<typename T>
class AdjacencyList {
private:
    struct Edge {
        T destination;
        int weight;
        
        Edge(const T& dest, int w = 1) : destination(dest), weight(w) {}
        
        bool operator==(const Edge& other) const {
            return destination == other.destination;
        }
    };
    
    std::unordered_map<T, std::vector<Edge>> adjList;
    bool isDirected;
    
public:
    // 构造函数
    explicit AdjacencyList(bool directed = false) : isDirected(directed) {}
    
    // 添加顶点
    void addVertex(const T& vertex) {
        if (adjList.find(vertex) == adjList.end()) {
            adjList[vertex] = std::vector<Edge>();
        }
    }
    
    // 添加边
    void addEdge(const T& from, const T& to, int weight = 1) {
        addVertex(from);
        addVertex(to);
        
        // 检查边是否已存在
        auto& edges = adjList[from];
        auto it = std::find_if(edges.begin(), edges.end(),
            [&to](const Edge& edge) { return edge.destination == to; });
        
        if (it != edges.end()) {
            it->weight = weight; // 更新权重
        } else {
            edges.emplace_back(to, weight); // 添加新边
        }
        
        // 无向图需要添加反向边
        if (!isDirected) {
            auto& reverseEdges = adjList[to];
            auto reverseIt = std::find_if(reverseEdges.begin(), reverseEdges.end(),
                [&from](const Edge& edge) { return edge.destination == from; });
            
            if (reverseIt != reverseEdges.end()) {
                reverseIt->weight = weight;
            } else {
                reverseEdges.emplace_back(from, weight);
            }
        }
    }
    
    // 删除边
    void removeEdge(const T& from, const T& to) {
        if (adjList.find(from) != adjList.end()) {
            auto& edges = adjList[from];
            edges.erase(std::remove_if(edges.begin(), edges.end(),
                [&to](const Edge& edge) { return edge.destination == to; }),
                edges.end());
        }
        
        if (!isDirected && adjList.find(to) != adjList.end()) {
            auto& edges = adjList[to];
            edges.erase(std::remove_if(edges.begin(), edges.end(),
                [&from](const Edge& edge) { return edge.destination == from; }),
                edges.end());
        }
    }
    
    // 删除顶点
    void removeVertex(const T& vertex) {
        // 删除所有指向该顶点的边
        for (auto& [v, edges] : adjList) {
            edges.erase(std::remove_if(edges.begin(), edges.end(),
                [&vertex](const Edge& edge) { return edge.destination == vertex; }),
                edges.end());
        }
        
        // 删除该顶点的邻接表
        adjList.erase(vertex);
    }
    
    // 检查边是否存在
    bool hasEdge(const T& from, const T& to) const {
        auto it = adjList.find(from);
        if (it == adjList.end()) {
            return false;
        }
        
        const auto& edges = it->second;
        return std::find_if(edges.begin(), edges.end(),
            [&to](const Edge& edge) { return edge.destination == to; }) != edges.end();
    }
    
    // 获取边的权重
    int getWeight(const T& from, const T& to) const {
        auto it = adjList.find(from);
        if (it == adjList.end()) {
            return 0;
        }
        
        const auto& edges = it->second;
        auto edgeIt = std::find_if(edges.begin(), edges.end(),
            [&to](const Edge& edge) { return edge.destination == to; });
        
        return edgeIt != edges.end() ? edgeIt->weight : 0;
    }
    
    // 获取顶点的邻居
    std::vector<T> getNeighbors(const T& vertex) const {
        std::vector<T> neighbors;
        
        auto it = adjList.find(vertex);
        if (it != adjList.end()) {
            for (const auto& edge : it->second) {
                neighbors.push_back(edge.destination);
            }
        }
        
        return neighbors;
    }
    
    // 获取顶点的邻居及权重
    std::vector<std::pair<T, int>> getNeighborsWithWeights(const T& vertex) const {
        std::vector<std::pair<T, int>> neighbors;
        
        auto it = adjList.find(vertex);
        if (it != adjList.end()) {
            for (const auto& edge : it->second) {
                neighbors.emplace_back(edge.destination, edge.weight);
            }
        }
        
        return neighbors;
    }
    
    // 获取顶点数量
    size_t getVertexCount() const {
        return adjList.size();
    }
    
    // 获取边数量
    size_t getEdgeCount() const {
        size_t count = 0;
        
        for (const auto& [vertex, edges] : adjList) {
            count += edges.size();
        }
        
        return isDirected ? count : count / 2;
    }
    
    // 获取顶点的出度
    size_t getOutDegree(const T& vertex) const {
        auto it = adjList.find(vertex);
        return it != adjList.end() ? it->second.size() : 0;
    }
    
    // 获取顶点的入度
    size_t getInDegree(const T& vertex) const {
        size_t inDegree = 0;
        
        for (const auto& [v, edges] : adjList) {
            for (const auto& edge : edges) {
                if (edge.destination == vertex) {
                    inDegree++;
                }
            }
        }
        
        return inDegree;
    }
    
    // 打印图
    void printGraph() const {
        std::cout << "邻接表表示:" << std::endl;
        
        for (const auto& [vertex, edges] : adjList) {
            std::cout << vertex << " -> ";
            for (const auto& edge : edges) {
                std::cout << "(" << edge.destination << "," << edge.weight << ") ";
            }
            std::cout << std::endl;
        }
    }
    
    // 获取所有顶点
    std::vector<T> getVertices() const {
        std::vector<T> vertices;
        for (const auto& [vertex, edges] : adjList) {
            vertices.push_back(vertex);
        }
        return vertices;
    }
    
    // 获取邻接表
    const std::unordered_map<T, std::vector<Edge>>& getAdjList() const {
        return adjList;
    }
};
```

### 3. 边列表实现

```cpp
template<typename T>
class EdgeList {
private:
    struct Edge {
        T from;
        T to;
        int weight;
        
        Edge(const T& f, const T& t, int w = 1) : from(f), to(t), weight(w) {}
        
        bool operator==(const Edge& other) const {
            return from == other.from && to == other.to;
        }
        
        bool operator<(const Edge& other) const {
            if (weight != other.weight) {
                return weight < other.weight;
            }
            if (from != other.from) {
                return from < other.from;
            }
            return to < other.to;
        }
    };
    
    std::vector<Edge> edges;
    std::unordered_set<T> vertices;
    bool isDirected;
    
public:
    // 构造函数
    explicit EdgeList(bool directed = false) : isDirected(directed) {}
    
    // 添加顶点
    void addVertex(const T& vertex) {
        vertices.insert(vertex);
    }
    
    // 添加边
    void addEdge(const T& from, const T& to, int weight = 1) {
        vertices.insert(from);
        vertices.insert(to);
        
        // 检查边是否已存在
        auto it = std::find_if(edges.begin(), edges.end(),
            [&from, &to](const Edge& edge) {
                return edge.from == from && edge.to == to;
            });
        
        if (it != edges.end()) {
            it->weight = weight; // 更新权重
        } else {
            edges.emplace_back(from, to, weight);
        }
        
        // 无向图需要添加反向边
        if (!isDirected) {
            auto reverseIt = std::find_if(edges.begin(), edges.end(),
                [&from, &to](const Edge& edge) {
                    return edge.from == to && edge.to == from;
                });
            
            if (reverseIt != edges.end()) {
                reverseIt->weight = weight;
            } else {
                edges.emplace_back(to, from, weight);
            }
        }
    }
    
    // 删除边
    void removeEdge(const T& from, const T& to) {
        edges.erase(std::remove_if(edges.begin(), edges.end(),
            [&from, &to](const Edge& edge) {
                return edge.from == from && edge.to == to;
            }), edges.end());
        
        if (!isDirected) {
            edges.erase(std::remove_if(edges.begin(), edges.end(),
                [&from, &to](const Edge& edge) {
                    return edge.from == to && edge.to == from;
                }), edges.end());
        }
    }
    
    // 检查边是否存在
    bool hasEdge(const T& from, const T& to) const {
        return std::find_if(edges.begin(), edges.end(),
            [&from, &to](const Edge& edge) {
                return edge.from == from && edge.to == to;
            }) != edges.end();
    }
    
    // 获取边的权重
    int getWeight(const T& from, const T& to) const {
        auto it = std::find_if(edges.begin(), edges.end(),
            [&from, &to](const Edge& edge) {
                return edge.from == from && edge.to == to;
            });
        
        return it != edges.end() ? it->weight : 0;
    }
    
    // 获取顶点的邻居
    std::vector<T> getNeighbors(const T& vertex) const {
        std::vector<T> neighbors;
        
        for (const auto& edge : edges) {
            if (edge.from == vertex) {
                neighbors.push_back(edge.to);
            }
        }
        
        return neighbors;
    }
    
    // 获取所有边
    std::vector<std::tuple<T, T, int>> getAllEdges() const {
        std::vector<std::tuple<T, T, int>> result;
        
        for (const auto& edge : edges) {
            result.emplace_back(edge.from, edge.to, edge.weight);
        }
        
        return result;
    }
    
    // 按权重排序边
    void sortEdgesByWeight() {
        std::sort(edges.begin(), edges.end());
    }
    
    // 获取顶点数量
    size_t getVertexCount() const {
        return vertices.size();
    }
    
    // 获取边数量
    size_t getEdgeCount() const {
        return isDirected ? edges.size() : edges.size() / 2;
    }
    
    // 打印图
    void printGraph() const {
        std::cout << "边列表表示:" << std::endl;
        
        for (const auto& edge : edges) {
            std::cout << edge.from << " -> " << edge.to 
                     << " (权重: " << edge.weight << ")" << std::endl;
        }
    }
    
    // 获取所有顶点
    std::vector<T> getVertices() const {
        return std::vector<T>(vertices.begin(), vertices.end());
    }
};
```

## 📚 图的遍历算法

### 深度优先搜索 (DFS)

```cpp
template<typename T>
class GraphTraversal {
public:
    // DFS递归实现
    static void dfsRecursive(const AdjacencyList<T>& graph, const T& start,
                           std::unordered_set<T>& visited,
                           std::function<void(const T&)> process = nullptr) {
        visited.insert(start);
        
        if (process) {
            process(start);
        } else {
            std::cout << start << " ";
        }
        
        for (const T& neighbor : graph.getNeighbors(start)) {
            if (visited.find(neighbor) == visited.end()) {
                dfsRecursive(graph, neighbor, visited, process);
            }
        }
    }
    
    // DFS迭代实现
    static void dfsIterative(const AdjacencyList<T>& graph, const T& start,
                           std::function<void(const T&)> process = nullptr) {
        std::unordered_set<T> visited;
        std::stack<T> stack;
        
        stack.push(start);
        
        while (!stack.empty()) {
            T current = stack.top();
            stack.pop();
            
            if (visited.find(current) == visited.end()) {
                visited.insert(current);
                
                if (process) {
                    process(current);
                } else {
                    std::cout << current << " ";
                }
                
                // 将邻居加入栈（逆序以保持遍历顺序）
                auto neighbors = graph.getNeighbors(current);
                for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) {
                    if (visited.find(*it) == visited.end()) {
                        stack.push(*it);
                    }
                }
            }
        }
    }
    
    // BFS实现
    static void bfs(const AdjacencyList<T>& graph, const T& start,
                   std::function<void(const T&)> process = nullptr) {
        std::unordered_set<T> visited;
        std::queue<T> queue;
        
        visited.insert(start);
        queue.push(start);
        
        while (!queue.empty()) {
            T current = queue.front();
            queue.pop();
            
            if (process) {
                process(current);
            } else {
                std::cout << current << " ";
            }
            
            for (const T& neighbor : graph.getNeighbors(start)) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    queue.push(neighbor);
                }
            }
        }
    }
    
    // 检查路径是否存在
    static bool hasPath(const AdjacencyList<T>& graph, const T& start, const T& end) {
        if (start == end) return true;
        
        std::unordered_set<T> visited;
        return hasPathHelper(graph, start, end, visited);
    }
    
private:
    static bool hasPathHelper(const AdjacencyList<T>& graph, const T& current, 
                            const T& end, std::unordered_set<T>& visited) {
        if (current == end) return true;
        
        visited.insert(current);
        
        for (const T& neighbor : graph.getNeighbors(current)) {
            if (visited.find(neighbor) == visited.end()) {
                if (hasPathHelper(graph, neighbor, end, visited)) {
                    return true;
                }
            }
        }
        
        return false;
    }
};
```

## 📚 图的算法实现

### 1. 拓扑排序

```cpp
template<typename T>
class TopologicalSort {
public:
    // Kahn算法（基于入度）
    static std::vector<T> kahnAlgorithm(const AdjacencyList<T>& graph) {
        std::vector<T> result;
        std::unordered_map<T, size_t> inDegree;
        std::queue<T> zeroInDegreeQueue;
        
        // 计算所有顶点的入度
        for (const T& vertex : graph.getVertices()) {
            inDegree[vertex] = graph.getInDegree(vertex);
            if (inDegree[vertex] == 0) {
                zeroInDegreeQueue.push(vertex);
            }
        }
        
        while (!zeroInDegreeQueue.empty()) {
            T current = zeroInDegreeQueue.front();
            zeroInDegreeQueue.pop();
            result.push_back(current);
            
            // 减少邻居的入度
            for (const T& neighbor : graph.getNeighbors(current)) {
                inDegree[neighbor]--;
                if (inDegree[neighbor] == 0) {
                    zeroInDegreeQueue.push(neighbor);
                }
            }
        }
        
        // 检查是否有环
        if (result.size() != graph.getVertexCount()) {
            return {}; // 存在环，无法拓扑排序
        }
        
        return result;
    }
    
    // DFS算法
    static std::vector<T> dfsAlgorithm(const AdjacencyList<T>& graph) {
        std::vector<T> result;
        std::unordered_set<T> visited;
        std::unordered_set<T> recursionStack;
        
        for (const T& vertex : graph.getVertices()) {
            if (visited.find(vertex) == visited.end()) {
                if (!dfsHelper(graph, vertex, visited, recursionStack, result)) {
                    return {}; // 存在环
                }
            }
        }
        
        std::reverse(result.begin(), result.end());
        return result;
    }
    
private:
    static bool dfsHelper(const AdjacencyList<T>& graph, const T& vertex,
                         std::unordered_set<T>& visited,
                         std::unordered_set<T>& recursionStack,
                         std::vector<T>& result) {
        visited.insert(vertex);
        recursionStack.insert(vertex);
        
        for (const T& neighbor : graph.getNeighbors(vertex)) {
            if (recursionStack.find(neighbor) != recursionStack.end()) {
                return false; // 发现环
            }
            
            if (visited.find(neighbor) == visited.end()) {
                if (!dfsHelper(graph, neighbor, visited, recursionStack, result)) {
                    return false;
                }
            }
        }
        
        recursionStack.erase(vertex);
        result.push_back(vertex);
        return true;
    }
};
```

### 2. 最短路径算法

```cpp
template<typename T>
class ShortestPath {
public:
    // Dijkstra算法
    static std::unordered_map<T, int> dijkstra(const AdjacencyList<T>& graph, const T& start) {
        std::unordered_map<T, int> distances;
        std::unordered_set<T> visited;
        
        // 使用优先队列（最小堆）
        auto compare = [&distances](const T& a, const T& b) {
            return distances[a] > distances[b];
        };
        std::priority_queue<T, std::vector<T>, decltype(compare)> pq(compare);
        
        // 初始化距离
        for (const T& vertex : graph.getVertices()) {
            distances[vertex] = std::numeric_limits<int>::max();
        }
        distances[start] = 0;
        pq.push(start);
        
        while (!pq.empty()) {
            T current = pq.top();
            pq.pop();
            
            if (visited.find(current) != visited.end()) {
                continue;
            }
            
            visited.insert(current);
            
            for (const auto& [neighbor, weight] : graph.getNeighborsWithWeights(current)) {
                int newDistance = distances[current] + weight;
                
                if (newDistance < distances[neighbor]) {
                    distances[neighbor] = newDistance;
                    pq.push(neighbor);
                }
            }
        }
        
        return distances;
    }
    
    // Bellman-Ford算法（可处理负权重）
    static std::pair<std::unordered_map<T, int>, bool> bellmanFord(
        const EdgeList<T>& graph, const T& start) {
        
        std::unordered_map<T, int> distances;
        
        // 初始化距离
        for (const T& vertex : graph.getVertices()) {
            distances[vertex] = std::numeric_limits<int>::max();
        }
        distances[start] = 0;
        
        size_t vertexCount = graph.getVertexCount();
        auto edges = graph.getAllEdges();
        
        // 松弛边V-1次
        for (size_t i = 0; i < vertexCount - 1; ++i) {
            for (const auto& [from, to, weight] : edges) {
                if (distances[from] != std::numeric_limits<int>::max()) {
                    if (distances[from] + weight < distances[to]) {
                        distances[to] = distances[from] + weight;
                    }
                }
            }
        }
        
        // 检查负权重环
        for (const auto& [from, to, weight] : edges) {
            if (distances[from] != std::numeric_limits<int>::max()) {
                if (distances[from] + weight < distances[to]) {
                    return {distances, false}; // 存在负权重环
                }
            }
        }
        
        return {distances, true};
    }
    
    // Floyd-Warshall算法（所有点对最短路径）
    static std::unordered_map<T, std::unordered_map<T, int>> floydWarshall(
        const AdjacencyMatrix<T>& graph) {
        
        const auto& vertices = graph.getVertices();
        const auto& matrix = graph.getMatrix();
        
        std::unordered_map<T, std::unordered_map<T, int>> distances;
        
        // 初始化距离矩阵
        for (size_t i = 0; i < vertices.size(); ++i) {
            for (size_t j = 0; j < vertices.size(); ++j) {
                if (i == j) {
                    distances[vertices[i]][vertices[j]] = 0;
                } else if (matrix[i][j] != 0) {
                    distances[vertices[i]][vertices[j]] = matrix[i][j];
                } else {
                    distances[vertices[i]][vertices[j]] = std::numeric_limits<int>::max();
                }
            }
        }
        
        // Floyd-Warshall核心算法
        for (const T& k : vertices) {
            for (const T& i : vertices) {
                for (const T& j : vertices) {
                    if (distances[i][k] != std::numeric_limits<int>::max() &&
                        distances[k][j] != std::numeric_limits<int>::max()) {
                        
                        int newDistance = distances[i][k] + distances[k][j];
                        if (newDistance < distances[i][j]) {
                            distances[i][j] = newDistance;
                        }
                    }
                }
            }
        }
        
        return distances;
    }
};
```

### 3. 最小生成树算法

```cpp
template<typename T>
class MinimumSpanningTree {
public:
    // Kruskal算法
    static std::vector<std::tuple<T, T, int>> kruskal(const EdgeList<T>& graph) {
        std::vector<std::tuple<T, T, int>> mst;
        auto edges = graph.getAllEdges();
        
        // 按权重排序边
        std::sort(edges.begin(), edges.end(),
            [](const auto& a, const auto& b) {
                return std::get<2>(a) < std::get<2>(b);
            });
        
        // 并查集
        UnionFind<T> uf;
        for (const T& vertex : graph.getVertices()) {
            uf.makeSet(vertex);
        }
        
        for (const auto& [from, to, weight] : edges) {
            if (uf.find(from) != uf.find(to)) {
                uf.unionSets(from, to);
                mst.emplace_back(from, to, weight);
            }
        }
        
        return mst;
    }
    
    // Prim算法
    static std::vector<std::tuple<T, T, int>> prim(const AdjacencyList<T>& graph, const T& start) {
        std::vector<std::tuple<T, T, int>> mst;
        std::unordered_set<T> visited;
        
        // 优先队列：{权重, {from, to}}
        auto compare = [](const std::pair<int, std::pair<T, T>>& a,
                         const std::pair<int, std::pair<T, T>>& b) {
            return a.first > b.first;
        };
        
        std::priority_queue<std::pair<int, std::pair<T, T>>,
                           std::vector<std::pair<int, std::pair<T, T>>>,
                           decltype(compare)> pq(compare);
        
        visited.insert(start);
        
        // 添加起始顶点的所有边
        for (const auto& [neighbor, weight] : graph.getNeighborsWithWeights(start)) {
            pq.push({weight, {start, neighbor}});
        }
        
        while (!pq.empty() && mst.size() < graph.getVertexCount() - 1) {
            auto [weight, edge] = pq.top();
            pq.pop();
            
            auto [from, to] = edge;
            
            if (visited.find(to) != visited.end()) {
                continue; // 已访问过
            }
            
            visited.insert(to);
            mst.emplace_back(from, to, weight);
            
            // 添加新顶点的所有边
            for (const auto& [neighbor, edgeWeight] : graph.getNeighborsWithWeights(to)) {
                if (visited.find(neighbor) == visited.end()) {
                    pq.push({edgeWeight, {to, neighbor}});
                }
            }
        }
        
        return mst;
    }
    
private:
    // 简单的并查集实现
    template<typename U>
    class UnionFind {
    private:
        std::unordered_map<U, U> parent;
        std::unordered_map<U, int> rank;
        
    public:
        void makeSet(const U& item) {
            parent[item] = item;
            rank[item] = 0;
        }
        
        U find(const U& item) {
            if (parent[item] != item) {
                parent[item] = find(parent[item]); // 路径压缩
            }
            return parent[item];
        }
        
        void unionSets(const U& a, const U& b) {
            U rootA = find(a);
            U rootB = find(b);
            
            if (rootA != rootB) {
                // 按秩合并
                if (rank[rootA] < rank[rootB]) {
                    parent[rootA] = rootB;
                } else if (rank[rootA] > rank[rootB]) {
                    parent[rootB] = rootA;
                } else {
                    parent[rootB] = rootA;
                    rank[rootA]++;
                }
            }
        }
    };
};
```

## 📚 图的高级算法

### 1. 强连通分量（Tarjan算法）

```cpp
template<typename T>
class StronglyConnectedComponents {
private:
    std::unordered_map<T, int> discovery;
    std::unordered_map<T, int> low;
    std::unordered_set<T> onStack;
    std::stack<T> stack;
    std::vector<std::vector<T>> sccs;
    int time;
    
    void tarjanDFS(const AdjacencyList<T>& graph, const T& vertex) {
        discovery[vertex] = low[vertex] = ++time;
        stack.push(vertex);
        onStack.insert(vertex);
        
        for (const T& neighbor : graph.getNeighbors(vertex)) {
            if (discovery.find(neighbor) == discovery.end()) {
                // 未访问过的邻居
                tarjanDFS(graph, neighbor);
                low[vertex] = std::min(low[vertex], low[neighbor]);
            } else if (onStack.find(neighbor) != onStack.end()) {
                // 在栈中的邻居（后向边）
                low[vertex] = std::min(low[vertex], discovery[neighbor]);
            }
        }
        
        // 如果vertex是SCC的根
        if (low[vertex] == discovery[vertex]) {
            std::vector<T> scc;
            T current;
            
            do {
                current = stack.top();
                stack.pop();
                onStack.erase(current);
                scc.push_back(current);
            } while (current != vertex);
            
            sccs.push_back(scc);
        }
    }
    
public:
    std::vector<std::vector<T>> findSCCs(const AdjacencyList<T>& graph) {
        // 重置状态
        discovery.clear();
        low.clear();
        onStack.clear();
        while (!stack.empty()) stack.pop();
        sccs.clear();
        time = 0;
        
        for (const T& vertex : graph.getVertices()) {
            if (discovery.find(vertex) == discovery.end()) {
                tarjanDFS(graph, vertex);
            }
        }
        
        return sccs;
    }
};
```

### 2. 关键路径（拓扑排序+最长路径）

```cpp
template<typename T>
class CriticalPath {
public:
    struct Activity {
        T from;
        T to;
        int duration;
        int earliestStart;
        int latestStart;
        bool isCritical;
        
        Activity(const T& f, const T& t, int d) 
            : from(f), to(t), duration(d), earliestStart(0), latestStart(0), isCritical(false) {}
    };
    
    static std::vector<Activity> findCriticalPath(const AdjacencyList<T>& graph) {
        std::vector<T> topoOrder = TopologicalSort<T>::kahnAlgorithm(graph);
        if (topoOrder.empty()) {
            return {}; // 图中有环，无法计算关键路径
        }
        
        std::unordered_map<T, int> earliestFinish;
        std::unordered_map<T, int> latestFinish;
        std::vector<Activity> activities;
        
        // 初始化
        for (const T& vertex : graph.getVertices()) {
            earliestFinish[vertex] = 0;
        }
        
        // 计算最早完成时间（前向计算）
        for (const T& vertex : topoOrder) {
            for (const auto& [neighbor, duration] : graph.getNeighborsWithWeights(vertex)) {
                earliestFinish[neighbor] = std::max(earliestFinish[neighbor],
                                                  earliestFinish[vertex] + duration);
                activities.emplace_back(vertex, neighbor, duration);
            }
        }
        
        // 项目总时间
        int projectTime = *std::max_element(earliestFinish.begin(), earliestFinish.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; }).first;
        
        // 初始化最晚完成时间
        for (const T& vertex : graph.getVertices()) {
            latestFinish[vertex] = projectTime;
        }
        
        // 计算最晚完成时间（后向计算）
        for (auto it = topoOrder.rbegin(); it != topoOrder.rend(); ++it) {
            const T& vertex = *it;
            for (const auto& [neighbor, duration] : graph.getNeighborsWithWeights(vertex)) {
                latestFinish[vertex] = std::min(latestFinish[vertex],
                                              latestFinish[neighbor] - duration);
            }
        }
        
        // 计算活动的最早开始时间和最晚开始时间
        for (auto& activity : activities) {
            activity.earliestStart = earliestFinish[activity.from];
            activity.latestStart = latestFinish[activity.to] - activity.duration;
            activity.isCritical = (activity.earliestStart == activity.latestStart);
        }
        
        return activities;
    }
};
```

## 🧪 图的应用示例

### 1. 社交网络分析

```cpp
template<typename T>
class SocialNetwork {
private:
    AdjacencyList<T> network;
    
public:
    SocialNetwork() : network(false) {} // 无向图
    
    void addUser(const T& user) {
        network.addVertex(user);
    }
    
    void addFriendship(const T& user1, const T& user2) {
        network.addEdge(user1, user2);
    }
    
    void removeFriendship(const T& user1, const T& user2) {
        network.removeEdge(user1, user2);
    }
    
    // 查找共同朋友
    std::vector<T> findMutualFriends(const T& user1, const T& user2) {
        std::vector<T> mutualFriends;
        auto friends1 = network.getNeighbors(user1);
        auto friends2 = network.getNeighbors(user2);
        
        std::sort(friends1.begin(), friends1.end());
        std::sort(friends2.begin(), friends2.end());
        
        std::set_intersection(friends1.begin(), friends1.end(),
                            friends2.begin(), friends2.end(),
                            std::back_inserter(mutualFriends));
        
        return mutualFriends;
    }
    
    // 推荐朋友（朋友的朋友）
    std::vector<T> recommendFriends(const T& user, int maxRecommendations = 5) {
        std::unordered_map<T, int> candidates;
        auto friends = network.getNeighbors(user);
        std::unordered_set<T> userFriends(friends.begin(), friends.end());
        
        for (const T& friend_ : friends) {
            for (const T& friendOfFriend : network.getNeighbors(friend_)) {
                if (friendOfFriend != user && 
                    userFriends.find(friendOfFriend) == userFriends.end()) {
                    candidates[friendOfFriend]++;
                }
            }
        }
        
        // 按推荐分数排序
        std::vector<std::pair<T, int>> sortedCandidates(candidates.begin(), candidates.end());
        std::sort(sortedCandidates.begin(), sortedCandidates.end(),
                 [](const auto& a, const auto& b) { return a.second > b.second; });
        
        std::vector<T> recommendations;
        int count = 0;
        for (const auto& [candidate, score] : sortedCandidates) {
            if (count >= maxRecommendations) break;
            recommendations.push_back(candidate);
            count++;
        }
        
        return recommendations;
    }
    
    // 计算两个用户之间的最短路径长度
    int getShortestPathLength(const T& user1, const T& user2) {
        if (user1 == user2) return 0;
        
        std::unordered_map<T, int> distances;
        std::queue<T> queue;
        
        distances[user1] = 0;
        queue.push(user1);
        
        while (!queue.empty()) {
            T current = queue.front();
            queue.pop();
            
            for (const T& neighbor : network.getNeighbors(current)) {
                if (distances.find(neighbor) == distances.end()) {
                    distances[neighbor] = distances[current] + 1;
                    
                    if (neighbor == user2) {
                        return distances[neighbor];
                    }
                    
                    queue.push(neighbor);
                }
            }
        }
        
        return -1; // 不连通
    }
    
    // 分析网络统计信息
    void analyzeNetwork() {
        auto vertices = network.getVertices();
        size_t vertexCount = vertices.size();
        size_t edgeCount = network.getEdgeCount();
        
        std::cout << "社交网络统计信息:" << std::endl;
        std::cout << "  用户数量: " << vertexCount << std::endl;
        std::cout << "  好友关系数: " << edgeCount << std::endl;
        std::cout << "  平均好友数: " << (vertexCount > 0 ? 2.0 * edgeCount / vertexCount : 0) << std::endl;
        
        // 计算度数分布
        std::unordered_map<size_t, size_t> degreeDistribution;
        for (const T& user : vertices) {
            size_t degree = network.getNeighbors(user).size();
            degreeDistribution[degree]++;
        }
        
        std::cout << "  度数分布:" << std::endl;
        for (const auto& [degree, count] : degreeDistribution) {
            std::cout << "    度数 " << degree << ": " << count << " 个用户" << std::endl;
        }
    }
};
```

### 2. 路径规划系统

```cpp
template<typename T>
class PathPlanning {
private:
    AdjacencyList<T> roadNetwork;
    
public:
    PathPlanning() : roadNetwork(true) {} // 有向图
    
    void addLocation(const T& location) {
        roadNetwork.addVertex(location);
    }
    
    void addRoad(const T& from, const T& to, int distance) {
        roadNetwork.addEdge(from, to, distance);
    }
    
    // 查找最短路径
    std::pair<std::vector<T>, int> findShortestPath(const T& start, const T& destination) {
        std::unordered_map<T, int> distances;
        std::unordered_map<T, T> previous;
        
        // 使用Dijkstra算法
        auto compare = [&distances](const T& a, const T& b) {
            return distances[a] > distances[b];
        };
        std::priority_queue<T, std::vector<T>, decltype(compare)> pq(compare);
        
        // 初始化
        for (const T& location : roadNetwork.getVertices()) {
            distances[location] = std::numeric_limits<int>::max();
        }
        distances[start] = 0;
        pq.push(start);
        
        while (!pq.empty()) {
            T current = pq.top();
            pq.pop();
            
            if (current == destination) {
                break;
            }
            
            for (const auto& [neighbor, distance] : roadNetwork.getNeighborsWithWeights(current)) {
                int newDistance = distances[current] + distance;
                
                if (newDistance < distances[neighbor]) {
                    distances[neighbor] = newDistance;
                    previous[neighbor] = current;
                    pq.push(neighbor);
                }
            }
        }
        
        // 重构路径
        std::vector<T> path;
        int totalDistance = distances[destination];
        
        if (totalDistance != std::numeric_limits<int>::max()) {
            T current = destination;
            while (previous.find(current) != previous.end()) {
                path.push_back(current);
                current = previous[current];
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
        }
        
        return {path, totalDistance == std::numeric_limits<int>::max() ? -1 : totalDistance};
    }
    
    // 查找K条最短路径
    std::vector<std::pair<std::vector<T>, int>> findKShortestPaths(
        const T& start, const T& destination, int k) {
        
        std::vector<std::pair<std::vector<T>, int>> paths;
        
        // 简化版本：使用Yen's算法的基本思想
        auto [shortestPath, shortestDistance] = findShortestPath(start, destination);
        
        if (shortestDistance == -1) {
            return paths; // 无路径
        }
        
        paths.emplace_back(shortestPath, shortestDistance);
        
        // 这里可以实现更复杂的K最短路径算法
        // 为了简化，我们只返回一条路径
        
        return paths;
    }
    
    // 计算行程时间（考虑交通状况）
    int calculateTravelTime(const std::vector<T>& path, 
                          const std::unordered_map<std::pair<T, T>, double>& trafficFactors) {
        if (path.size() < 2) return 0;
        
        int totalTime = 0;
        
        for (size_t i = 0; i < path.size() - 1; ++i) {
            int baseDistance = roadNetwork.getWeight(path[i], path[i + 1]);
            
            double trafficFactor = 1.0;
            auto factorIt = trafficFactors.find({path[i], path[i + 1]});
            if (factorIt != trafficFactors.end()) {
                trafficFactor = factorIt->second;
            }
            
            totalTime += static_cast<int>(baseDistance * trafficFactor);
        }
        
        return totalTime;
    }
};
```

## 📊 性能分析和比较

### 图表示方法比较

| 表示方法 | 空间复杂度 | 添加顶点 | 添加边 | 删除顶点 | 删除边 | 查找边 | 遍历邻居 | 适用场景 |
|----------|------------|----------|--------|----------|--------|--------|----------|----------|
| 邻接矩阵 | O(V²) | O(V²) | O(1) | O(V²) | O(1) | O(1) | O(V) | 稠密图、频繁查边 |
| 邻接表 | O(V+E) | O(1) | O(1) | O(V+E) | O(degree) | O(degree) | O(degree) | 稀疏图、一般用途 |
| 边列表 | O(E) | O(1) | O(1) | O(E) | O(E) | O(E) | O(E) | 简单图、排序需求 |

### 算法复杂度比较

| 算法 | 时间复杂度 | 空间复杂度 | 适用图类型 | 说明 |
|------|------------|------------|------------|------|
| DFS | O(V+E) | O(V) | 所有图 | 递归/迭代实现 |
| BFS | O(V+E) | O(V) | 所有图 | 最短路径(无权) |
| Dijkstra | O((V+E)logV) | O(V) | 非负权图 | 单源最短路径 |
| Bellman-Ford | O(VE) | O(V) | 所有图 | 检测负环 |
| Floyd-Warshall | O(V³) | O(V²) | 所有图 | 全源最短路径 |
| Kruskal | O(ElogE) | O(V) | 无向图 | 最小生成树 |
| Prim | O((V+E)logV) | O(V) | 无向图 | 最小生成树 |

## 🧪 测试和验证

```cpp
void testGraphImplementations() {
    std::cout << "=== 图数据结构测试 ===" << std::endl;
    
    // 测试邻接表
    std::cout << "\n1. 邻接表测试:" << std::endl;
    AdjacencyList<std::string> adjList(true); // 有向图
    
    adjList.addEdge("A", "B", 5);
    adjList.addEdge("A", "C", 3);
    adjList.addEdge("B", "C", 2);
    adjList.addEdge("B", "D", 4);
    adjList.addEdge("C", "D", 7);
    adjList.addEdge("C", "E", 1);
    adjList.addEdge("D", "E", 6);
    
    adjList.printGraph();
    
    std::cout << "顶点数: " << adjList.getVertexCount() << std::endl;
    std::cout << "边数: " << adjList.getEdgeCount() << std::endl;
    
    // 测试遍历
    std::cout << "\n2. 图遍历测试:" << std::endl;
    std::cout << "DFS (递归): ";
    std::unordered_set<std::string> visited;
    GraphTraversal<std::string>::dfsRecursive(adjList, "A", visited);
    std::cout << std::endl;
    
    std::cout << "DFS (迭代): ";
    GraphTraversal<std::string>::dfsIterative(adjList, "A");
    std::cout << std::endl;
    
    std::cout << "BFS: ";
    GraphTraversal<std::string>::bfs(adjList, "A");
    std::cout << std::endl;
    
    // 测试拓扑排序
    std::cout << "\n3. 拓扑排序测试:" << std::endl;
    auto topoOrder = TopologicalSort<std::string>::kahnAlgorithm(adjList);
    
    if (!topoOrder.empty()) {
        std::cout << "拓扑排序结果: ";
        for (const auto& vertex : topoOrder) {
            std::cout << vertex << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "图中存在环，无法进行拓扑排序" << std::endl;
    }
    
    // 测试最短路径
    std::cout << "\n4. 最短路径测试:" << std::endl;
    auto distances = ShortestPath<std::string>::dijkstra(adjList, "A");
    
    std::cout << "从A到各点的最短距离:" << std::endl;
    for (const auto& [vertex, distance] : distances) {
        std::cout << "  到" << vertex << ": " << distance << std::endl;
    }
    
    // 测试社交网络
    std::cout << "\n5. 社交网络测试:" << std::endl;
    SocialNetwork<std::string> socialNet;
    
    socialNet.addUser("Alice");
    socialNet.addUser("Bob");
    socialNet.addUser("Charlie");
    socialNet.addUser("David");
    socialNet.addUser("Eve");
    
    socialNet.addFriendship("Alice", "Bob");
    socialNet.addFriendship("Alice", "Charlie");
    socialNet.addFriendship("Bob", "Charlie");
    socialNet.addFriendship("Bob", "David");
    socialNet.addFriendship("Charlie", "Eve");
    socialNet.addFriendship("David", "Eve");
    
    socialNet.analyzeNetwork();
    
    auto mutualFriends = socialNet.findMutualFriends("Alice", "David");
    std::cout << "Alice和David的共同朋友: ";
    for (const auto& friend_ : mutualFriends) {
        std::cout << friend_ << " ";
    }
    std::cout << std::endl;
    
    auto recommendations = socialNet.recommendFriends("Alice");
    std::cout << "为Alice推荐的朋友: ";
    for (const auto& recommendation : recommendations) {
        std::cout << recommendation << " ";
    }
    std::cout << std::endl;
    
    int pathLength = socialNet.getShortestPathLength("Alice", "Eve");
    std::cout << "Alice到Eve的最短路径长度: " << pathLength << std::endl;
}
```

## 💡 图的应用领域

### 1. 网络分析
- **社交网络**: 用户关系、影响力传播
- **计算机网络**: 路由协议、网络拓扑
- **生物网络**: 蛋白质交互、基因调控

### 2. 路径和调度
- **GPS导航**: 最短路径、实时路况
- **任务调度**: 依赖关系、关键路径
- **资源分配**: 匹配问题、流量控制

### 3. 数据挖掘
- **推荐系统**: 协同过滤、相似性计算
- **聚类分析**: 社区发现、图分割
- **知识图谱**: 实体关系、语义网络

### 4. 优化问题
- **网络设计**: 最小生成树、最小费用流
- **电路设计**: 布线优化、电路分析
- **供应链**: 物流优化、库存管理

## 🔧 实现优化建议

### 1. 内存优化
- 使用紧凑的数据结构
- 考虑图的稀疏性选择表示方法
- 实现内存池管理

### 2. 性能优化
- 利用缓存局部性
- 并行算法实现
- 近似算法应用

### 3. 可扩展性
- 分布式图处理
- 流式图算法
- 外存图算法

图是计算机科学中最重要和最灵活的数据结构之一，掌握其各种表示方法和算法对于解决复杂的现实问题至关重要！🎯

# 哈希表数据结构实现详解

## 🎯 哈希表的基本概念

哈希表(Hash Table)是根据键(Key)直接访问内存存储位置的数据结构。它通过哈希函数将键映射到表中的位置来访问记录，以加快查找的速度。

### 核心组件
1. **哈希函数**: 将键映射到数组索引
2. **桶数组**: 存储键值对的数组
3. **冲突解决**: 处理不同键映射到同一位置的情况

### 性能特征
- **平均查找时间**: O(1)
- **最坏查找时间**: O(n)
- **空间复杂度**: O(n)

## 📚 哈希函数设计

### 基础哈希函数

```cpp
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <functional>
#include <algorithm>
#include <cmath>

// 通用哈希函数类
template<typename Key>
class HashFunction {
public:
    virtual ~HashFunction() = default;
    virtual size_t operator()(const Key& key, size_t tableSize) const = 0;
};

// 整数哈希函数
class IntegerHash : public HashFunction<int> {
public:
    size_t operator()(const int& key, size_t tableSize) const override {
        // 乘法哈希法
        const double A = 0.6180339887; // (√5 - 1) / 2，黄金比例
        double fractional = key * A - std::floor(key * A);
        return static_cast<size_t>(std::floor(tableSize * fractional));
    }
};

// 字符串哈希函数
class StringHash : public HashFunction<std::string> {
public:
    size_t operator()(const std::string& key, size_t tableSize) const override {
        // DJB2哈希算法
        size_t hash = 5381;
        for (char c : key) {
            hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
        }
        return hash % tableSize;
    }
};

// 通用哈希函数（使用std::hash）
template<typename Key>
class StandardHash : public HashFunction<Key> {
public:
    size_t operator()(const Key& key, size_t tableSize) const override {
        return std::hash<Key>{}(key) % tableSize;
    }
};

// MurmurHash3哈希函数（高质量哈希）
class MurmurHash3 : public HashFunction<std::string> {
private:
    static constexpr uint32_t C1 = 0xcc9e2d51;
    static constexpr uint32_t C2 = 0x1b873593;
    static constexpr uint32_t C3 = 0xe6546b64;
    
    uint32_t rotateLeft(uint32_t x, int8_t r) const {
        return (x << r) | (x >> (32 - r));
    }
    
    uint32_t fmix32(uint32_t h) const {
        h ^= h >> 16;
        h *= 0x85ebca6b;
        h ^= h >> 13;
        h *= 0xc2b2ae35;
        h ^= h >> 16;
        return h;
    }
    
public:
    size_t operator()(const std::string& key, size_t tableSize) const override {
        const uint8_t* data = reinterpret_cast<const uint8_t*>(key.c_str());
        int len = static_cast<int>(key.length());
        uint32_t seed = 0;
        
        uint32_t h1 = seed;
        const int nblocks = len / 4;
        
        // Body
        const uint32_t* blocks = reinterpret_cast<const uint32_t*>(data + nblocks * 4);
        for (int i = -nblocks; i; i++) {
            uint32_t k1 = blocks[i];
            
            k1 *= C1;
            k1 = rotateLeft(k1, 15);
            k1 *= C2;
            
            h1 ^= k1;
            h1 = rotateLeft(h1, 13);
            h1 = h1 * 5 + C3;
        }
        
        // Tail
        const uint8_t* tail = data + nblocks * 4;
        uint32_t k1 = 0;
        
        switch (len & 3) {
            case 3: k1 ^= tail[2] << 16;
            case 2: k1 ^= tail[1] << 8;
            case 1: k1 ^= tail[0];
                k1 *= C1;
                k1 = rotateLeft(k1, 15);
                k1 *= C2;
                h1 ^= k1;
        }
        
        // Finalization
        h1 ^= len;
        h1 = fmix32(h1);
        
        return h1 % tableSize;
    }
};
```

## 📚 链地址法实现

```cpp
template<typename Key, typename Value, typename Hash = StandardHash<Key>>
class ChainedHashTable {
private:
    struct Entry {
        Key key;
        Value value;
        
        Entry(const Key& k, const Value& v) : key(k), value(v) {}
    };
    
    std::vector<std::list<Entry>> table;
    size_t tableSize;
    size_t numElements;
    Hash hashFunction;
    
    static constexpr double MAX_LOAD_FACTOR = 0.75;
    
    // 判断是否为质数
    bool isPrime(size_t n) const {
        if (n < 2) return false;
        if (n == 2) return true;
        if (n % 2 == 0) return false;
        
        for (size_t i = 3; i * i <= n; i += 2) {
            if (n % i == 0) return false;
        }
        return true;
    }
    
    // 获取下一个质数
    size_t nextPrime(size_t n) const {
        while (!isPrime(n)) {
            n++;
        }
        return n;
    }
    
    // 重新哈希
    void rehash() {
        std::vector<std::list<Entry>> oldTable = std::move(table);
        size_t oldSize = tableSize;
        
        tableSize = nextPrime(tableSize * 2);
        table = std::vector<std::list<Entry>>(tableSize);
        numElements = 0;
        
        // 重新插入所有元素
        for (const auto& bucket : oldTable) {
            for (const auto& entry : bucket) {
                insert(entry.key, entry.value);
            }
        }
    }
    
public:
    // 构造函数
    explicit ChainedHashTable(size_t initialSize = 11) 
        : tableSize(nextPrime(initialSize)), numElements(0) {
        table.resize(tableSize);
    }
    
    // 插入或更新
    void insert(const Key& key, const Value& value) {
        size_t index = hashFunction(key, tableSize);
        auto& bucket = table[index];
        
        // 查找是否已存在
        for (auto& entry : bucket) {
            if (entry.key == key) {
                entry.value = value; // 更新
                return;
            }
        }
        
        // 插入新元素
        bucket.emplace_back(key, value);
        numElements++;
        
        // 检查负载因子
        if (static_cast<double>(numElements) / tableSize > MAX_LOAD_FACTOR) {
            rehash();
        }
    }
    
    // 查找
    bool find(const Key& key, Value& value) const {
        size_t index = hashFunction(key, tableSize);
        const auto& bucket = table[index];
        
        for (const auto& entry : bucket) {
            if (entry.key == key) {
                value = entry.value;
                return true;
            }
        }
        return false;
    }
    
    // 删除
    bool remove(const Key& key) {
        size_t index = hashFunction(key, tableSize);
        auto& bucket = table[index];
        
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->key == key) {
                bucket.erase(it);
                numElements--;
                return true;
            }
        }
        return false;
    }
    
    // 检查是否存在
    bool contains(const Key& key) const {
        size_t index = hashFunction(key, tableSize);
        const auto& bucket = table[index];
        
        for (const auto& entry : bucket) {
            if (entry.key == key) {
                return true;
            }
        }
        return false;
    }
    
    // 获取大小
    size_t size() const { return numElements; }
    bool empty() const { return numElements == 0; }
    
    // 获取负载因子
    double loadFactor() const {
        return static_cast<double>(numElements) / tableSize;
    }
    
    // 获取统计信息
    void printStats() const {
        size_t maxChainLength = 0;
        size_t totalChainLength = 0;
        size_t nonEmptyBuckets = 0;
        
        for (const auto& bucket : table) {
            if (!bucket.empty()) {
                nonEmptyBuckets++;
                maxChainLength = std::max(maxChainLength, bucket.size());
                totalChainLength += bucket.size();
            }
        }
        
        std::cout << "哈希表统计信息:" << std::endl;
        std::cout << "  表大小: " << tableSize << std::endl;
        std::cout << "  元素数量: " << numElements << std::endl;
        std::cout << "  负载因子: " << loadFactor() << std::endl;
        std::cout << "  非空桶数: " << nonEmptyBuckets << std::endl;
        std::cout << "  最大链长: " << maxChainLength << std::endl;
        std::cout << "  平均链长: " << (nonEmptyBuckets > 0 ? static_cast<double>(totalChainLength) / nonEmptyBuckets : 0) << std::endl;
    }
    
    // 清空
    void clear() {
        for (auto& bucket : table) {
            bucket.clear();
        }
        numElements = 0;
    }
};
```

## 📚 开放寻址法实现

```cpp
template<typename Key, typename Value, typename Hash = StandardHash<Key>>
class OpenAddressingHashTable {
private:
    enum class EntryState {
        EMPTY,
        OCCUPIED,
        DELETED
    };
    
    struct Entry {
        Key key;
        Value value;
        EntryState state;
        
        Entry() : state(EntryState::EMPTY) {}
        Entry(const Key& k, const Value& v) : key(k), value(v), state(EntryState::OCCUPIED) {}
    };
    
    std::vector<Entry> table;
    size_t tableSize;
    size_t numElements;
    size_t numDeleted;
    Hash hashFunction;
    
    static constexpr double MAX_LOAD_FACTOR = 0.5;
    
    // 线性探测
    size_t linearProbe(size_t index) const {
        return (index + 1) % tableSize;
    }
    
    // 二次探测
    size_t quadraticProbe(size_t originalIndex, size_t attempt) const {
        return (originalIndex + attempt * attempt) % tableSize;
    }
    
    // 双重哈希
    size_t doubleHash(const Key& key, size_t attempt) const {
        size_t hash1 = hashFunction(key, tableSize);
        size_t hash2 = 7 - (std::hash<Key>{}(key) % 7); // 第二个哈希函数
        return (hash1 + attempt * hash2) % tableSize;
    }
    
    // 查找位置（插入或查找）
    size_t findPosition(const Key& key, bool forInsertion = false) const {
        size_t index = hashFunction(key, tableSize);
        size_t originalIndex = index;
        size_t attempt = 0;
        
        while (table[index].state != EntryState::EMPTY) {
            if (table[index].state == EntryState::OCCUPIED && table[index].key == key) {
                return index; // 找到
            }
            
            if (forInsertion && table[index].state == EntryState::DELETED) {
                return index; // 可以插入到删除的位置
            }
            
            // 使用线性探测
            index = linearProbe(index);
            attempt++;
            
            // 防止无限循环
            if (attempt >= tableSize) {
                break;
            }
        }
        
        return index;
    }
    
    // 重新哈希
    void rehash() {
        std::vector<Entry> oldTable = std::move(table);
        size_t oldSize = tableSize;
        
        tableSize = nextPrime(tableSize * 2);
        table = std::vector<Entry>(tableSize);
        numElements = 0;
        numDeleted = 0;
        
        // 重新插入所有有效元素
        for (const auto& entry : oldTable) {
            if (entry.state == EntryState::OCCUPIED) {
                insert(entry.key, entry.value);
            }
        }
    }
    
    bool isPrime(size_t n) const {
        if (n < 2) return false;
        if (n == 2) return true;
        if (n % 2 == 0) return false;
        
        for (size_t i = 3; i * i <= n; i += 2) {
            if (n % i == 0) return false;
        }
        return true;
    }
    
    size_t nextPrime(size_t n) const {
        while (!isPrime(n)) {
            n++;
        }
        return n;
    }
    
public:
    // 构造函数
    explicit OpenAddressingHashTable(size_t initialSize = 11) 
        : tableSize(nextPrime(initialSize)), numElements(0), numDeleted(0) {
        table.resize(tableSize);
    }
    
    // 插入或更新
    void insert(const Key& key, const Value& value) {
        // 检查是否需要重新哈希
        if (static_cast<double>(numElements + numDeleted) / tableSize > MAX_LOAD_FACTOR) {
            rehash();
        }
        
        size_t index = findPosition(key, true);
        
        if (table[index].state == EntryState::OCCUPIED && table[index].key == key) {
            // 更新现有元素
            table[index].value = value;
        } else {
            // 插入新元素
            if (table[index].state == EntryState::DELETED) {
                numDeleted--;
            }
            table[index] = Entry(key, value);
            numElements++;
        }
    }
    
    // 查找
    bool find(const Key& key, Value& value) const {
        size_t index = findPosition(key);
        
        if (table[index].state == EntryState::OCCUPIED && table[index].key == key) {
            value = table[index].value;
            return true;
        }
        return false;
    }
    
    // 删除（惰性删除）
    bool remove(const Key& key) {
        size_t index = findPosition(key);
        
        if (table[index].state == EntryState::OCCUPIED && table[index].key == key) {
            table[index].state = EntryState::DELETED;
            numElements--;
            numDeleted++;
            return true;
        }
        return false;
    }
    
    // 检查是否存在
    bool contains(const Key& key) const {
        size_t index = findPosition(key);
        return table[index].state == EntryState::OCCUPIED && table[index].key == key;
    }
    
    // 获取大小
    size_t size() const { return numElements; }
    bool empty() const { return numElements == 0; }
    
    // 获取负载因子
    double loadFactor() const {
        return static_cast<double>(numElements) / tableSize;
    }
    
    // 清空
    void clear() {
        for (auto& entry : table) {
            entry.state = EntryState::EMPTY;
        }
        numElements = 0;
        numDeleted = 0;
    }
    
    // 打印统计信息
    void printStats() const {
        size_t emptySlots = 0;
        size_t occupiedSlots = 0;
        size_t deletedSlots = 0;
        
        for (const auto& entry : table) {
            switch (entry.state) {
                case EntryState::EMPTY: emptySlots++; break;
                case EntryState::OCCUPIED: occupiedSlots++; break;
                case EntryState::DELETED: deletedSlots++; break;
            }
        }
        
        std::cout << "开放寻址哈希表统计信息:" << std::endl;
        std::cout << "  表大小: " << tableSize << std::endl;
        std::cout << "  占用槽: " << occupiedSlots << std::endl;
        std::cout << "  删除槽: " << deletedSlots << std::endl;
        std::cout << "  空槽: " << emptySlots << std::endl;
        std::cout << "  负载因子: " << loadFactor() << std::endl;
    }
};
```

## 📚 布谷鸟哈希实现

```cpp
template<typename Key, typename Value>
class CuckooHashTable {
private:
    struct Entry {
        Key key;
        Value value;
        bool occupied;
        
        Entry() : occupied(false) {}
        Entry(const Key& k, const Value& v) : key(k), value(v), occupied(true) {}
    };
    
    std::vector<Entry> table1;
    std::vector<Entry> table2;
    size_t tableSize;
    size_t numElements;
    
    static constexpr size_t MAX_ITERATIONS = 10;
    static constexpr double MAX_LOAD_FACTOR = 0.5;
    
    // 第一个哈希函数
    size_t hash1(const Key& key) const {
        return std::hash<Key>{}(key) % tableSize;
    }
    
    // 第二个哈希函数
    size_t hash2(const Key& key) const {
        return (std::hash<Key>{}(key) / tableSize) % tableSize;
    }
    
    // 重新哈希
    void rehash() {
        std::vector<Entry> oldTable1 = std::move(table1);
        std::vector<Entry> oldTable2 = std::move(table2);
        
        tableSize *= 2;
        table1 = std::vector<Entry>(tableSize);
        table2 = std::vector<Entry>(tableSize);
        numElements = 0;
        
        // 重新插入所有元素
        for (const auto& entry : oldTable1) {
            if (entry.occupied) {
                insert(entry.key, entry.value);
            }
        }
        
        for (const auto& entry : oldTable2) {
            if (entry.occupied) {
                insert(entry.key, entry.value);
            }
        }
    }
    
    // 布谷鸟插入
    bool cuckooInsert(const Key& key, const Value& value) {
        Entry newEntry(key, value);
        
        for (size_t i = 0; i < MAX_ITERATIONS; ++i) {
            // 尝试插入到第一个表
            size_t pos1 = hash1(newEntry.key);
            if (!table1[pos1].occupied) {
                table1[pos1] = newEntry;
                return true;
            }
            
            // 第一个表被占用，踢出元素
            std::swap(newEntry, table1[pos1]);
            
            // 尝试插入到第二个表
            size_t pos2 = hash2(newEntry.key);
            if (!table2[pos2].occupied) {
                table2[pos2] = newEntry;
                return true;
            }
            
            // 第二个表也被占用，继续踢出
            std::swap(newEntry, table2[pos2]);
        }
        
        // 达到最大迭代次数，需要重新哈希
        return false;
    }
    
public:
    // 构造函数
    explicit CuckooHashTable(size_t initialSize = 11) 
        : tableSize(initialSize), numElements(0) {
        table1.resize(tableSize);
        table2.resize(tableSize);
    }
    
    // 插入或更新
    void insert(const Key& key, const Value& value) {
        // 首先检查是否已存在
        if (contains(key)) {
            // 更新现有值
            size_t pos1 = hash1(key);
            if (table1[pos1].occupied && table1[pos1].key == key) {
                table1[pos1].value = value;
                return;
            }
            
            size_t pos2 = hash2(key);
            if (table2[pos2].occupied && table2[pos2].key == key) {
                table2[pos2].value = value;
                return;
            }
        }
        
        // 检查负载因子
        if (static_cast<double>(numElements) / tableSize > MAX_LOAD_FACTOR) {
            rehash();
        }
        
        // 尝试插入
        if (!cuckooInsert(key, value)) {
            // 插入失败，重新哈希后再试
            rehash();
            if (!cuckooInsert(key, value)) {
                throw std::runtime_error("Failed to insert after rehashing");
            }
        }
        
        numElements++;
    }
    
    // 查找
    bool find(const Key& key, Value& value) const {
        size_t pos1 = hash1(key);
        if (table1[pos1].occupied && table1[pos1].key == key) {
            value = table1[pos1].value;
            return true;
        }
        
        size_t pos2 = hash2(key);
        if (table2[pos2].occupied && table2[pos2].key == key) {
            value = table2[pos2].value;
            return true;
        }
        
        return false;
    }
    
    // 删除
    bool remove(const Key& key) {
        size_t pos1 = hash1(key);
        if (table1[pos1].occupied && table1[pos1].key == key) {
            table1[pos1].occupied = false;
            numElements--;
            return true;
        }
        
        size_t pos2 = hash2(key);
        if (table2[pos2].occupied && table2[pos2].key == key) {
            table2[pos2].occupied = false;
            numElements--;
            return true;
        }
        
        return false;
    }
    
    // 检查是否存在
    bool contains(const Key& key) const {
        size_t pos1 = hash1(key);
        if (table1[pos1].occupied && table1[pos1].key == key) {
            return true;
        }
        
        size_t pos2 = hash2(key);
        if (table2[pos2].occupied && table2[pos2].key == key) {
            return true;
        }
        
        return false;
    }
    
    // 获取大小
    size_t size() const { return numElements; }
    bool empty() const { return numElements == 0; }
    
    // 获取负载因子
    double loadFactor() const {
        return static_cast<double>(numElements) / (2 * tableSize);
    }
    
    // 清空
    void clear() {
        for (auto& entry : table1) {
            entry.occupied = false;
        }
        for (auto& entry : table2) {
            entry.occupied = false;
        }
        numElements = 0;
    }
    
    // 打印统计信息
    void printStats() const {
        size_t occupied1 = 0, occupied2 = 0;
        
        for (const auto& entry : table1) {
            if (entry.occupied) occupied1++;
        }
        
        for (const auto& entry : table2) {
            if (entry.occupied) occupied2++;
        }
        
        std::cout << "布谷鸟哈希统计信息:" << std::endl;
        std::cout << "  总表大小: " << 2 * tableSize << std::endl;
        std::cout << "  表1占用: " << occupied1 << "/" << tableSize << std::endl;
        std::cout << "  表2占用: " << occupied2 << "/" << tableSize << std::endl;
        std::cout << "  总元素: " << numElements << std::endl;
        std::cout << "  负载因子: " << loadFactor() << std::endl;
    }
};
```

## 📚 一致性哈希实现

```cpp
template<typename Key>
class ConsistentHashRing {
private:
    struct Node {
        std::string id;
        size_t hash;
        
        Node(const std::string& nodeId, size_t h) : id(nodeId), hash(h) {}
        
        bool operator<(const Node& other) const {
            return hash < other.hash;
        }
    };
    
    std::vector<Node> ring;
    size_t virtualNodes;
    std::hash<std::string> hasher;
    
public:
    explicit ConsistentHashRing(size_t vnodes = 150) : virtualNodes(vnodes) {}
    
    // 添加节点
    void addNode(const std::string& nodeId) {
        for (size_t i = 0; i < virtualNodes; ++i) {
            std::string virtualNodeId = nodeId + ":" + std::to_string(i);
            size_t hash = hasher(virtualNodeId);
            ring.emplace_back(nodeId, hash);
        }
        
        std::sort(ring.begin(), ring.end());
    }
    
    // 删除节点
    void removeNode(const std::string& nodeId) {
        ring.erase(
            std::remove_if(ring.begin(), ring.end(),
                [&nodeId](const Node& node) { return node.id == nodeId; }),
            ring.end()
        );
    }
    
    // 查找key应该分配到哪个节点
    std::string getNode(const Key& key) const {
        if (ring.empty()) {
            throw std::runtime_error("No nodes in the ring");
        }
        
        size_t keyHash = std::hash<Key>{}(key);
        
        // 找到第一个hash值大于等于keyHash的节点
        auto it = std::lower_bound(ring.begin(), ring.end(), Node("", keyHash));
        
        if (it == ring.end()) {
            // 环形结构，返回第一个节点
            return ring[0].id;
        }
        
        return it->id;
    }
    
    // 获取key的后续N个节点（用于复制）
    std::vector<std::string> getNodes(const Key& key, size_t count) const {
        if (ring.empty()) {
            return {};
        }
        
        std::vector<std::string> nodes;
        std::set<std::string> uniqueNodes;
        
        size_t keyHash = std::hash<Key>{}(key);
        auto startIt = std::lower_bound(ring.begin(), ring.end(), Node("", keyHash));
        
        if (startIt == ring.end()) {
            startIt = ring.begin();
        }
        
        auto it = startIt;
        while (uniqueNodes.size() < count && uniqueNodes.size() < getUniqueNodeCount()) {
            uniqueNodes.insert(it->id);
            
            ++it;
            if (it == ring.end()) {
                it = ring.begin();
            }
            
            // 防止无限循环
            if (it == startIt && uniqueNodes.size() < count) {
                break;
            }
        }
        
        nodes.assign(uniqueNodes.begin(), uniqueNodes.end());
        return nodes;
    }
    
    // 获取唯一节点数量
    size_t getUniqueNodeCount() const {
        std::set<std::string> uniqueNodes;
        for (const auto& node : ring) {
            uniqueNodes.insert(node.id);
        }
        return uniqueNodes.size();
    }
    
    // 打印环状态
    void printRing() const {
        std::cout << "一致性哈希环状态:" << std::endl;
        std::cout << "虚拟节点数: " << virtualNodes << std::endl;
        std::cout << "环中总节点: " << ring.size() << std::endl;
        std::cout << "唯一节点数: " << getUniqueNodeCount() << std::endl;
        
        std::map<std::string, size_t> nodeCount;
        for (const auto& node : ring) {
            nodeCount[node.id]++;
        }
        
        std::cout << "节点分布:" << std::endl;
        for (const auto& [nodeId, count] : nodeCount) {
            std::cout << "  " << nodeId << ": " << count << " 个虚拟节点" << std::endl;
        }
    }
};
```

## 📚 布隆过滤器实现

```cpp
#include <bitset>

template<size_t Size>
class BloomFilter {
private:
    std::bitset<Size> bits;
    size_t numHashFunctions;
    size_t numInserted;
    
    // 多个哈希函数
    std::vector<std::function<size_t(const std::string&)>> hashFunctions;
    
    void initializeHashFunctions() {
        hashFunctions.clear();
        
        // DJB2哈希
        hashFunctions.push_back([](const std::string& str) {
            size_t hash = 5381;
            for (char c : str) {
                hash = ((hash << 5) + hash) + c;
            }
            return hash;
        });
        
        // SDBM哈希
        hashFunctions.push_back([](const std::string& str) {
            size_t hash = 0;
            for (char c : str) {
                hash = c + (hash << 6) + (hash << 16) - hash;
            }
            return hash;
        });
        
        // FNV哈希
        hashFunctions.push_back([](const std::string& str) {
            const size_t FNV_PRIME = 1099511628211ULL;
            const size_t FNV_OFFSET_BASIS = 14695981039346656037ULL;
            
            size_t hash = FNV_OFFSET_BASIS;
            for (char c : str) {
                hash ^= c;
                hash *= FNV_PRIME;
            }
            return hash;
        });
        
        // 如果需要更多哈希函数，使用组合方式
        while (hashFunctions.size() < numHashFunctions) {
            size_t seed = hashFunctions.size();
            hashFunctions.push_back([seed](const std::string& str) {
                size_t hash = std::hash<std::string>{}(str);
                return hash ^ (seed + 0x9e3779b9 + (hash << 6) + (hash >> 2));
            });
        }
    }
    
public:
    explicit BloomFilter(size_t numHashes = 3) 
        : numHashFunctions(numHashes), numInserted(0) {
        initializeHashFunctions();
    }
    
    // 添加元素
    void add(const std::string& item) {
        for (size_t i = 0; i < numHashFunctions; ++i) {
            size_t hash = hashFunctions[i](item) % Size;
            bits.set(hash);
        }
        numInserted++;
    }
    
    // 检查元素是否可能存在
    bool mightContain(const std::string& item) const {
        for (size_t i = 0; i < numHashFunctions; ++i) {
            size_t hash = hashFunctions[i](item) % Size;
            if (!bits.test(hash)) {
                return false; // 肯定不存在
            }
        }
        return true; // 可能存在
    }
    
    // 获取误判率
    double getFalsePositiveRate() const {
        if (numInserted == 0) return 0.0;
        
        double bitsPerElement = static_cast<double>(Size) / numInserted;
        double exponent = -numHashFunctions / bitsPerElement;
        return std::pow(1.0 - std::exp(exponent), numHashFunctions);
    }
    
    // 清空过滤器
    void clear() {
        bits.reset();
        numInserted = 0;
    }
    
    // 获取统计信息
    void printStats() const {
        size_t setBits = bits.count();
        double fillRatio = static_cast<double>(setBits) / Size;
        
        std::cout << "布隆过滤器统计信息:" << std::endl;
        std::cout << "  总位数: " << Size << std::endl;
        std::cout << "  设置位数: " << setBits << std::endl;
        std::cout << "  填充率: " << fillRatio * 100 << "%" << std::endl;
        std::cout << "  哈希函数数: " << numHashFunctions << std::endl;
        std::cout << "  插入元素数: " << numInserted << std::endl;
        std::cout << "  预期误判率: " << getFalsePositiveRate() * 100 << "%" << std::endl;
    }
    
    // 估算最优哈希函数数量
    static size_t optimalHashFunctions(size_t expectedElements, size_t bitArraySize) {
        double ratio = static_cast<double>(bitArraySize) / expectedElements;
        return static_cast<size_t>(std::round(ratio * std::log(2)));
    }
    
    // 估算所需位数组大小
    static size_t optimalBitArraySize(size_t expectedElements, double targetFPR) {
        return static_cast<size_t>(
            std::ceil(-expectedElements * std::log(targetFPR) / (std::log(2) * std::log(2)))
        );
    }
};
```

## 🧪 测试和应用示例

```cpp
void testHashTableImplementations() {
    std::cout << "=== 哈希表实现测试 ===" << std::endl;
    
    // 测试链地址法哈希表
    std::cout << "\n1. 链地址法哈希表测试:" << std::endl;
    ChainedHashTable<std::string, int> chainedTable;
    
    std::vector<std::pair<std::string, int>> testData = {
        {"apple", 5}, {"banana", 3}, {"orange", 8}, {"grape", 2},
        {"kiwi", 7}, {"mango", 4}, {"peach", 6}, {"cherry", 9}
    };
    
    for (const auto& [key, value] : testData) {
        chainedTable.insert(key, value);
        std::cout << "插入: " << key << " -> " << value << std::endl;
    }
    
    chainedTable.printStats();
    
    // 测试查找
    std::cout << "\n查找测试:" << std::endl;
    for (const std::string& key : {"apple", "banana", "nonexistent"}) {
        int value;
        if (chainedTable.find(key, value)) {
            std::cout << "找到: " << key << " -> " << value << std::endl;
        } else {
            std::cout << "未找到: " << key << std::endl;
        }
    }
    
    // 测试开放寻址法哈希表
    std::cout << "\n2. 开放寻址法哈希表测试:" << std::endl;
    OpenAddressingHashTable<std::string, int> openTable;
    
    for (const auto& [key, value] : testData) {
        openTable.insert(key, value);
    }
    
    openTable.printStats();
    
    // 测试删除
    std::cout << "\n删除测试:" << std::endl;
    openTable.remove("banana");
    openTable.remove("grape");
    std::cout << "删除 banana 和 grape 后:" << std::endl;
    openTable.printStats();
    
    // 测试布谷鸟哈希
    std::cout << "\n3. 布谷鸟哈希测试:" << std::endl;
    CuckooHashTable<std::string, int> cuckooTable;
    
    for (const auto& [key, value] : testData) {
        try {
            cuckooTable.insert(key, value);
            std::cout << "成功插入: " << key << std::endl;
        } catch (const std::exception& e) {
            std::cout << "插入失败: " << key << " - " << e.what() << std::endl;
        }
    }
    
    cuckooTable.printStats();
    
    // 测试一致性哈希
    std::cout << "\n4. 一致性哈希测试:" << std::endl;
    ConsistentHashRing<std::string> hashRing;
    
    // 添加节点
    hashRing.addNode("Server1");
    hashRing.addNode("Server2");
    hashRing.addNode("Server3");
    
    hashRing.printRing();
    
    std::vector<std::string> keys = {"key1", "key2", "key3", "key4", "key5"};
    std::cout << "\n键分配测试:" << std::endl;
    for (const std::string& key : keys) {
        std::string assignedNode = hashRing.getNode(key);
        std::cout << key << " -> " << assignedNode << std::endl;
    }
    
    // 测试布隆过滤器
    std::cout << "\n5. 布隆过滤器测试:" << std::endl;
    BloomFilter<1000> bloomFilter(3);
    
    std::vector<std::string> items = {"item1", "item2", "item3", "item4", "item5"};
    
    // 添加元素
    for (const std::string& item : items) {
        bloomFilter.add(item);
        std::cout << "添加: " << item << std::endl;
    }
    
    bloomFilter.printStats();
    
    // 测试查询
    std::cout << "\n查询测试:" << std::endl;
    std::vector<std::string> queryItems = {"item1", "item3", "item6", "nonexistent"};
    for (const std::string& item : queryItems) {
        bool might = bloomFilter.mightContain(item);
        std::cout << item << ": " << (might ? "可能存在" : "肯定不存在") << std::endl;
    }
}
```

## 📊 性能对比和选择指南

### 各种哈希表实现对比

| 实现方式 | 查找时间 | 插入时间 | 删除时间 | 空间利用率 | 缓存友好性 | 适用场景 |
|----------|----------|----------|----------|------------|------------|----------|
| 链地址法 | O(1)~O(n) | O(1) | O(1)~O(n) | 低 | 差 | 通用场景 |
| 开放寻址 | O(1)~O(n) | O(1)~O(n) | O(1)~O(n) | 高 | 好 | 缓存敏感 |
| 布谷鸟哈希 | O(1) | O(1)均摊 | O(1) | 中 | 好 | 实时系统 |
| 一致性哈希 | O(log n) | O(log n) | O(log n) | - | - | 分布式系统 |

### 选择建议

1. **链地址法**: 
   - 适用于负载因子可能较高的场景
   - 删除操作频繁的应用
   - 对内存使用不敏感的情况

2. **开放寻址法**:
   - 缓存敏感的应用
   - 内存使用需要优化的场景
   - 删除操作相对较少

3. **布谷鸟哈希**:
   - 需要最坏情况O(1)查找的实时系统
   - 查找操作远多于插入操作
   - 对性能要求极高的场景

4. **一致性哈希**:
   - 分布式缓存系统
   - 负载均衡
   - 需要动态添加/删除节点的系统

## 💡 实现优化技巧

### 1. 哈希函数优化
- 选择高质量的哈希函数
- 避免哈希碰撞过多
- 考虑硬件特性(如SIMD指令)

### 2. 内存布局优化
- 使用内存池减少分配开销
- 考虑缓存行对齐
- 减少指针间接访问

### 3. 负载因子控制
- 动态调整表大小
- 选择合适的重哈希时机
- 平衡空间和时间效率

### 4. 并发优化
- 使用读写锁提高并发性
- 考虑无锁数据结构
- 分段锁技术

哈希表是现代计算机系统中最重要的数据结构之一，掌握其各种实现方式和优化技巧对于高性能系统开发至关重要！🎯

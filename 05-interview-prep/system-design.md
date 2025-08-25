# 系统设计面试题详解

## 📋 目录

1. [系统设计基础](#系统设计基础)
2. [缓存系统设计](#缓存系统设计)
3. [消息队列系统](#消息队列系统)
4. [数据库设计](#数据库设计)
5. [分布式系统](#分布式系统)
6. [微服务架构](#微服务架构)
7. [经典系统设计题](#经典系统设计题)

---

## 系统设计基础

### 1. 系统设计的核心原则

**可扩展性 (Scalability)**:
- **水平扩展**: 增加更多服务器
- **垂直扩展**: 提升单台服务器性能
- **负载均衡**: 请求分发策略

**可靠性 (Reliability)**:
- **冗余备份**: 多副本数据存储
- **故障恢复**: 自动failover机制
- **监控告警**: 实时系统健康检查

**可用性 (Availability)**:
- **SLA标准**: 99.9%、99.99%、99.999%
- **故障隔离**: 局部故障不影响整体
- **降级策略**: 保证核心功能可用

### 2. 系统设计面试流程

```
1. 需求澄清 (5-10分钟)
   ├── 功能需求: 系统要做什么
   ├── 非功能需求: 性能、可靠性等
   └── 约束条件: 用户规模、数据量等

2. 估算规模 (5-10分钟)  
   ├── DAU (Daily Active Users)
   ├── QPS (Queries Per Second)
   ├── 存储需求
   └── 带宽需求

3. 高层设计 (10-15分钟)
   ├── 核心组件识别
   ├── 组件间交互
   └── API设计

4. 详细设计 (10-15分钟)
   ├── 数据模型
   ├── 算法选择
   └── 技术栈

5. 扩展讨论 (5-10分钟)
   ├── 瓶颈分析
   ├── 扩展方案
   └── 监控运维
```

### 3. 常用技术组件

**负载均衡器**:
```cpp
class LoadBalancer {
public:
    // 轮询算法
    string roundRobin() {
        current = (current + 1) % servers.size();
        return servers[current];
    }
    
    // 一致性哈希
    string consistentHash(string key) {
        uint32_t hash = hashFunction(key);
        auto it = hashRing.lower_bound(hash);
        if (it == hashRing.end()) {
            it = hashRing.begin();
        }
        return it->second;
    }
    
    // 加权轮询
    string weightedRoundRobin() {
        for (auto& server : servers) {
            server.currentWeight += server.weight;
            if (maxWeightServer == nullptr || 
                server.currentWeight > maxWeightServer->currentWeight) {
                maxWeightServer = &server;
            }
        }
        
        maxWeightServer->currentWeight -= totalWeight;
        return maxWeightServer->address;
    }

private:
    vector<string> servers;
    int current = -1;
    map<uint32_t, string> hashRing;  // 一致性哈希环
    int totalWeight;
};
```

---

## 缓存系统设计

### 1. LRU缓存实现

**问题**: 设计一个支持get和put的LRU缓存

**解答**:
```cpp
class LRUCache {
private:
    struct Node {
        int key, value;
        Node* prev;
        Node* next;
        Node(int k = 0, int v = 0) : key(k), value(v), prev(nullptr), next(nullptr) {}
    };
    
    int capacity;
    unordered_map<int, Node*> cache;
    Node* head;
    Node* tail;
    
    void addToHead(Node* node) {
        node->prev = head;
        node->next = head->next;
        head->next->prev = node;
        head->next = node;
    }
    
    void removeNode(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    
    void moveToHead(Node* node) {
        removeNode(node);
        addToHead(node);
    }
    
    Node* removeTail() {
        Node* last = tail->prev;
        removeNode(last);
        return last;
    }
    
public:
    LRUCache(int capacity) : capacity(capacity) {
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->prev = head;
    }
    
    int get(int key) {
        if (cache.count(key)) {
            Node* node = cache[key];
            moveToHead(node);
            return node->value;
        }
        return -1;
    }
    
    void put(int key, int value) {
        if (cache.count(key)) {
            Node* node = cache[key];
            node->value = value;
            moveToHead(node);
        } else {
            Node* newNode = new Node(key, value);
            
            if (cache.size() >= capacity) {
                Node* tail_node = removeTail();
                cache.erase(tail_node->key);
                delete tail_node;
            }
            
            cache[key] = newNode;
            addToHead(newNode);
        }
    }
};
```

### 2. 分布式缓存设计

**一致性哈希实现**:
```cpp
class ConsistentHash {
private:
    map<uint32_t, string> hashRing;
    set<string> nodes;
    int virtualNodes;
    
    uint32_t hashFunction(const string& key) {
        // 使用MD5或其他哈希函数
        hash<string> hasher;
        return hasher(key);
    }
    
public:
    ConsistentHash(int vnodes = 3) : virtualNodes(vnodes) {}
    
    void addNode(const string& node) {
        nodes.insert(node);
        for (int i = 0; i < virtualNodes; i++) {
            string vnode = node + "#" + to_string(i);
            uint32_t hash = hashFunction(vnode);
            hashRing[hash] = node;
        }
    }
    
    void removeNode(const string& node) {
        nodes.erase(node);
        for (int i = 0; i < virtualNodes; i++) {
            string vnode = node + "#" + to_string(i);
            uint32_t hash = hashFunction(vnode);
            hashRing.erase(hash);
        }
    }
    
    string getNode(const string& key) {
        if (hashRing.empty()) return "";
        
        uint32_t hash = hashFunction(key);
        auto it = hashRing.lower_bound(hash);
        if (it == hashRing.end()) {
            it = hashRing.begin();
        }
        return it->second;
    }
};
```

### 3. 缓存更新策略

**Cache-Aside模式**:
```cpp
class CacheAsidePattern {
private:
    LRUCache cache;
    Database database;
    
public:
    string get(const string& key) {
        // 1. 先查缓存
        string value = cache.get(key);
        if (!value.empty()) {
            return value;
        }
        
        // 2. 缓存未命中，查数据库
        value = database.get(key);
        if (!value.empty()) {
            // 3. 写入缓存
            cache.put(key, value);
        }
        
        return value;
    }
    
    void put(const string& key, const string& value) {
        // 1. 先更新数据库
        database.put(key, value);
        
        // 2. 删除缓存（而不是更新）
        cache.remove(key);
    }
};
```

---

## 消息队列系统

### 1. 简单消息队列实现

```cpp
template<typename T>
class MessageQueue {
private:
    queue<T> messages;
    mutex mtx;
    condition_variable cv;
    bool shutdown = false;
    
public:
    void send(const T& message) {
        lock_guard<mutex> lock(mtx);
        if (!shutdown) {
            messages.push(message);
            cv.notify_one();
        }
    }
    
    bool receive(T& message, int timeoutMs = -1) {
        unique_lock<mutex> lock(mtx);
        
        if (timeoutMs < 0) {
            cv.wait(lock, [this] { return !messages.empty() || shutdown; });
        } else {
            if (!cv.wait_for(lock, chrono::milliseconds(timeoutMs),
                           [this] { return !messages.empty() || shutdown; })) {
                return false;  // 超时
            }
        }
        
        if (shutdown && messages.empty()) {
            return false;
        }
        
        message = messages.front();
        messages.pop();
        return true;
    }
    
    void close() {
        lock_guard<mutex> lock(mtx);
        shutdown = true;
        cv.notify_all();
    }
};
```

### 2. 分布式消息队列

**发布-订阅模式**:
```cpp
class PubSubSystem {
private:
    struct Subscriber {
        string id;
        function<void(const string&)> callback;
        queue<string> messageQueue;
        mutex queueMutex;
    };
    
    unordered_map<string, vector<shared_ptr<Subscriber>>> topicSubscribers;
    mutex topicMutex;
    
public:
    string subscribe(const string& topic, function<void(const string&)> callback) {
        lock_guard<mutex> lock(topicMutex);
        
        string subscriberId = generateId();
        auto subscriber = make_shared<Subscriber>();
        subscriber->id = subscriberId;
        subscriber->callback = callback;
        
        topicSubscribers[topic].push_back(subscriber);
        return subscriberId;
    }
    
    void publish(const string& topic, const string& message) {
        lock_guard<mutex> lock(topicMutex);
        
        if (topicSubscribers.count(topic)) {
            for (auto& subscriber : topicSubscribers[topic]) {
                // 异步通知订阅者
                thread([subscriber, message]() {
                    subscriber->callback(message);
                }).detach();
            }
        }
    }
    
    void unsubscribe(const string& topic, const string& subscriberId) {
        lock_guard<mutex> lock(topicMutex);
        
        if (topicSubscribers.count(topic)) {
            auto& subscribers = topicSubscribers[topic];
            subscribers.erase(
                remove_if(subscribers.begin(), subscribers.end(),
                         [&subscriberId](const shared_ptr<Subscriber>& sub) {
                             return sub->id == subscriberId;
                         }),
                subscribers.end()
            );
        }
    }

private:
    string generateId() {
        return to_string(chrono::steady_clock::now().time_since_epoch().count());
    }
};
```

---

## 数据库设计

### 1. 数据库分片策略

**范围分片**:
```cpp
class RangeSharding {
private:
    struct ShardInfo {
        string shardId;
        int minKey;
        int maxKey;
        string connectionString;
    };
    
    vector<ShardInfo> shards;
    
public:
    string getShardId(int key) {
        for (const auto& shard : shards) {
            if (key >= shard.minKey && key <= shard.maxKey) {
                return shard.shardId;
            }
        }
        return "";  // 未找到合适的分片
    }
    
    void addShard(const string& shardId, int minKey, int maxKey, 
                  const string& connectionString) {
        shards.push_back({shardId, minKey, maxKey, connectionString});
        // 保持分片按范围排序
        sort(shards.begin(), shards.end(), 
             [](const ShardInfo& a, const ShardInfo& b) {
                 return a.minKey < b.minKey;
             });
    }
};
```

**哈希分片**:
```cpp
class HashSharding {
private:
    vector<string> shards;
    
    uint32_t hashFunction(const string& key) {
        hash<string> hasher;
        return hasher(key);
    }
    
public:
    string getShardId(const string& key) {
        if (shards.empty()) return "";
        
        uint32_t hash = hashFunction(key);
        int shardIndex = hash % shards.size();
        return shards[shardIndex];
    }
    
    void addShard(const string& shardId) {
        shards.push_back(shardId);
    }
};
```

### 2. 读写分离实现

```cpp
class ReadWriteSeparation {
private:
    string masterDb;
    vector<string> slaveDb;
    int currentSlave = 0;
    
public:
    string getWriteConnection() {
        return masterDb;
    }
    
    string getReadConnection() {
        if (slaveDb.empty()) {
            return masterDb;  // 降级到主库
        }
        
        // 轮询选择从库
        currentSlave = (currentSlave + 1) % slaveDb.size();
        return slaveDb[currentSlave];
    }
    
    void addSlave(const string& slaveConnection) {
        slaveDb.push_back(slaveConnection);
    }
    
    void removeSlave(const string& slaveConnection) {
        slaveDb.erase(
            remove(slaveDb.begin(), slaveDb.end(), slaveConnection),
            slaveDb.end()
        );
    }
};
```

---

## 分布式系统

### 1. 分布式锁实现

**基于Redis的分布式锁**:
```cpp
class RedisDistributedLock {
private:
    RedisClient redis;
    string lockKey;
    string lockValue;
    int expireTimeMs;
    
public:
    RedisDistributedLock(const string& key, int expireMs = 30000) 
        : lockKey(key), expireTimeMs(expireMs) {
        lockValue = generateUniqueValue();
    }
    
    bool tryLock() {
        // 使用SET命令的NX和PX选项实现原子性
        string result = redis.set(lockKey, lockValue, "NX", "PX", expireTimeMs);
        return result == "OK";
    }
    
    bool tryLock(int timeoutMs) {
        auto startTime = chrono::steady_clock::now();
        
        while (chrono::duration_cast<chrono::milliseconds>(
                chrono::steady_clock::now() - startTime).count() < timeoutMs) {
            
            if (tryLock()) {
                return true;
            }
            
            this_thread::sleep_for(chrono::milliseconds(10));
        }
        
        return false;
    }
    
    void unlock() {
        // 使用Lua脚本保证原子性
        string luaScript = R"(
            if redis.call("get", KEYS[1]) == ARGV[1] then
                return redis.call("del", KEYS[1])
            else
                return 0
            end
        )";
        
        redis.eval(luaScript, {lockKey}, {lockValue});
    }
    
    bool renewLock() {
        string luaScript = R"(
            if redis.call("get", KEYS[1]) == ARGV[1] then
                return redis.call("pexpire", KEYS[1], ARGV[2])
            else
                return 0
            end
        )";
        
        int result = redis.eval(luaScript, {lockKey}, {lockValue, to_string(expireTimeMs)});
        return result == 1;
    }

private:
    string generateUniqueValue() {
        return to_string(chrono::system_clock::now().time_since_epoch().count()) +
               "_" + to_string(this_thread::get_id());
    }
};
```

### 2. 分布式ID生成器

**雪花算法实现**:
```cpp
class SnowflakeIdGenerator {
private:
    static const long long EPOCH = 1609459200000LL;  // 2021-01-01 00:00:00
    static const int MACHINE_ID_BITS = 10;
    static const int SEQUENCE_BITS = 12;
    
    static const long long MAX_MACHINE_ID = (1LL << MACHINE_ID_BITS) - 1;
    static const long long MAX_SEQUENCE = (1LL << SEQUENCE_BITS) - 1;
    
    static const int MACHINE_ID_SHIFT = SEQUENCE_BITS;
    static const int TIMESTAMP_SHIFT = SEQUENCE_BITS + MACHINE_ID_BITS;
    
    long long machineId;
    long long sequence = 0;
    long long lastTimestamp = -1;
    mutex mtx;
    
    long long getCurrentTimestamp() {
        return chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch()).count();
    }
    
    long long waitNextMillis(long long lastTimestamp) {
        long long timestamp = getCurrentTimestamp();
        while (timestamp <= lastTimestamp) {
            timestamp = getCurrentTimestamp();
        }
        return timestamp;
    }
    
public:
    SnowflakeIdGenerator(long long machineId) : machineId(machineId) {
        if (machineId > MAX_MACHINE_ID || machineId < 0) {
            throw invalid_argument("Machine ID out of range");
        }
    }
    
    long long generateId() {
        lock_guard<mutex> lock(mtx);
        
        long long timestamp = getCurrentTimestamp();
        
        if (timestamp < lastTimestamp) {
            throw runtime_error("Clock moved backwards");
        }
        
        if (timestamp == lastTimestamp) {
            sequence = (sequence + 1) & MAX_SEQUENCE;
            if (sequence == 0) {
                timestamp = waitNextMillis(lastTimestamp);
            }
        } else {
            sequence = 0;
        }
        
        lastTimestamp = timestamp;
        
        return ((timestamp - EPOCH) << TIMESTAMP_SHIFT) |
               (machineId << MACHINE_ID_SHIFT) |
               sequence;
    }
};
```

---

## 微服务架构

### 1. 服务注册与发现

```cpp
class ServiceRegistry {
private:
    struct ServiceInstance {
        string serviceId;
        string host;
        int port;
        unordered_map<string, string> metadata;
        chrono::steady_clock::time_point lastHeartbeat;
    };
    
    unordered_map<string, vector<ServiceInstance>> services;
    mutex servicesMutex;
    thread heartbeatChecker;
    atomic<bool> running{true};
    
    void checkHeartbeats() {
        while (running) {
            this_thread::sleep_for(chrono::seconds(30));
            
            lock_guard<mutex> lock(servicesMutex);
            auto now = chrono::steady_clock::now();
            
            for (auto& [serviceName, instances] : services) {
                instances.erase(
                    remove_if(instances.begin(), instances.end(),
                             [now](const ServiceInstance& instance) {
                                 auto elapsed = chrono::duration_cast<chrono::seconds>(
                                     now - instance.lastHeartbeat).count();
                                 return elapsed > 90;  // 90秒超时
                             }),
                    instances.end()
                );
            }
        }
    }
    
public:
    ServiceRegistry() {
        heartbeatChecker = thread(&ServiceRegistry::checkHeartbeats, this);
    }
    
    ~ServiceRegistry() {
        running = false;
        if (heartbeatChecker.joinable()) {
            heartbeatChecker.join();
        }
    }
    
    void registerService(const string& serviceName, const string& serviceId,
                        const string& host, int port,
                        const unordered_map<string, string>& metadata = {}) {
        lock_guard<mutex> lock(servicesMutex);
        
        ServiceInstance instance;
        instance.serviceId = serviceId;
        instance.host = host;
        instance.port = port;
        instance.metadata = metadata;
        instance.lastHeartbeat = chrono::steady_clock::now();
        
        services[serviceName].push_back(instance);
    }
    
    vector<ServiceInstance> getServices(const string& serviceName) {
        lock_guard<mutex> lock(servicesMutex);
        
        if (services.count(serviceName)) {
            return services[serviceName];
        }
        return {};
    }
    
    void heartbeat(const string& serviceName, const string& serviceId) {
        lock_guard<mutex> lock(servicesMutex);
        
        if (services.count(serviceName)) {
            for (auto& instance : services[serviceName]) {
                if (instance.serviceId == serviceId) {
                    instance.lastHeartbeat = chrono::steady_clock::now();
                    break;
                }
            }
        }
    }
    
    void deregisterService(const string& serviceName, const string& serviceId) {
        lock_guard<mutex> lock(servicesMutex);
        
        if (services.count(serviceName)) {
            auto& instances = services[serviceName];
            instances.erase(
                remove_if(instances.begin(), instances.end(),
                         [&serviceId](const ServiceInstance& instance) {
                             return instance.serviceId == serviceId;
                         }),
                instances.end()
            );
        }
    }
};
```

### 2. 熔断器模式

```cpp
class CircuitBreaker {
private:
    enum State { CLOSED, OPEN, HALF_OPEN };
    
    State currentState = CLOSED;
    int failureCount = 0;
    int successCount = 0;
    chrono::steady_clock::time_point lastFailureTime;
    
    int failureThreshold;
    int successThreshold;
    chrono::milliseconds timeout;
    mutex mtx;
    
public:
    CircuitBreaker(int failureThreshold = 5, int successThreshold = 3,
                  chrono::milliseconds timeout = chrono::milliseconds(60000))
        : failureThreshold(failureThreshold)
        , successThreshold(successThreshold)
        , timeout(timeout) {}
    
    template<typename Func>
    auto execute(Func&& func) -> decltype(func()) {
        lock_guard<mutex> lock(mtx);
        
        if (currentState == OPEN) {
            auto now = chrono::steady_clock::now();
            if (now - lastFailureTime >= timeout) {
                currentState = HALF_OPEN;
                successCount = 0;
            } else {
                throw runtime_error("Circuit breaker is OPEN");
            }
        }
        
        try {
            auto result = func();
            onSuccess();
            return result;
        } catch (...) {
            onFailure();
            throw;
        }
    }
    
private:
    void onSuccess() {
        if (currentState == HALF_OPEN) {
            successCount++;
            if (successCount >= successThreshold) {
                currentState = CLOSED;
                failureCount = 0;
            }
        } else if (currentState == CLOSED) {
            failureCount = 0;
        }
    }
    
    void onFailure() {
        failureCount++;
        lastFailureTime = chrono::steady_clock::now();
        
        if (currentState == CLOSED && failureCount >= failureThreshold) {
            currentState = OPEN;
        } else if (currentState == HALF_OPEN) {
            currentState = OPEN;
        }
    }
};
```

---

## 经典系统设计题

### 1. 设计短链接服务 (TinyURL)

**需求分析**:
- 功能需求: 长链接转短链接，短链接重定向
- 非功能需求: 高可用、低延迟、支持自定义
- 规模估算: 100M URLs/day, 100:1 读写比

**系统设计**:
```cpp
class TinyURLService {
private:
    class Base62Encoder {
    public:
        static string encode(long long num) {
            if (num == 0) return "a";
            
            string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
            string result;
            
            while (num > 0) {
                result = chars[num % 62] + result;
                num /= 62;
            }
            
            return result;
        }
        
        static long long decode(const string& str) {
            string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
            unordered_map<char, int> charToNum;
            
            for (int i = 0; i < chars.length(); i++) {
                charToNum[chars[i]] = i;
            }
            
            long long result = 0;
            for (char c : str) {
                result = result * 62 + charToNum[c];
            }
            
            return result;
        }
    };
    
    SnowflakeIdGenerator idGenerator;
    unordered_map<string, string> shortToLong;  // 实际应使用数据库
    unordered_map<string, string> longToShort;
    string baseUrl = "http://tiny.ly/";
    
public:
    TinyURLService(int machineId) : idGenerator(machineId) {}
    
    string shortenUrl(const string& longUrl) {
        // 检查是否已存在
        if (longToShort.count(longUrl)) {
            return baseUrl + longToShort[longUrl];
        }
        
        // 生成新的短链接
        long long id = idGenerator.generateId();
        string shortCode = Base62Encoder::encode(id);
        
        shortToLong[shortCode] = longUrl;
        longToShort[longUrl] = shortCode;
        
        return baseUrl + shortCode;
    }
    
    string expandUrl(const string& shortUrl) {
        string shortCode = shortUrl.substr(baseUrl.length());
        
        if (shortToLong.count(shortCode)) {
            return shortToLong[shortCode];
        }
        
        throw invalid_argument("Short URL not found");
    }
    
    // 自定义短链接
    string customShortenUrl(const string& longUrl, const string& customCode) {
        if (shortToLong.count(customCode)) {
            throw invalid_argument("Custom code already exists");
        }
        
        shortToLong[customCode] = longUrl;
        longToShort[longUrl] = customCode;
        
        return baseUrl + customCode;
    }
};
```

### 2. 设计聊天系统

**架构组件**:
```cpp
class ChatSystem {
private:
    struct Message {
        string messageId;
        string fromUserId;
        string toUserId;
        string content;
        chrono::system_clock::time_point timestamp;
        bool isDelivered = false;
        bool isRead = false;
    };
    
    struct User {
        string userId;
        string status;  // online, offline, away
        set<string> connectedSockets;
    };
    
    unordered_map<string, User> users;
    unordered_map<string, vector<Message>> conversations;
    PubSubSystem pubsub;
    MessageQueue<Message> messageQueue;
    
public:
    void connectUser(const string& userId, const string& socketId) {
        users[userId].userId = userId;
        users[userId].status = "online";
        users[userId].connectedSockets.insert(socketId);
        
        // 订阅用户消息
        pubsub.subscribe("user_" + userId, [this, socketId](const string& message) {
            sendToSocket(socketId, message);
        });
    }
    
    void disconnectUser(const string& userId, const string& socketId) {
        if (users.count(userId)) {
            users[userId].connectedSockets.erase(socketId);
            if (users[userId].connectedSockets.empty()) {
                users[userId].status = "offline";
            }
        }
    }
    
    void sendMessage(const string& fromUserId, const string& toUserId, 
                    const string& content) {
        Message message;
        message.messageId = generateMessageId();
        message.fromUserId = fromUserId;
        message.toUserId = toUserId;
        message.content = content;
        message.timestamp = chrono::system_clock::now();
        
        // 存储消息
        string conversationId = getConversationId(fromUserId, toUserId);
        conversations[conversationId].push_back(message);
        
        // 实时推送
        if (users.count(toUserId) && users[toUserId].status == "online") {
            string messageJson = serializeMessage(message);
            pubsub.publish("user_" + toUserId, messageJson);
            message.isDelivered = true;
        } else {
            // 离线消息队列
            messageQueue.send(message);
        }
    }
    
    vector<Message> getConversationHistory(const string& user1, const string& user2,
                                          int limit = 50, int offset = 0) {
        string conversationId = getConversationId(user1, user2);
        
        if (conversations.count(conversationId)) {
            const auto& messages = conversations[conversationId];
            int start = max(0, (int)messages.size() - offset - limit);
            int end = max(0, (int)messages.size() - offset);
            
            return vector<Message>(messages.begin() + start, messages.begin() + end);
        }
        
        return {};
    }

private:
    string getConversationId(const string& user1, const string& user2) {
        return user1 < user2 ? user1 + "_" + user2 : user2 + "_" + user1;
    }
    
    string generateMessageId() {
        return to_string(chrono::system_clock::now().time_since_epoch().count());
    }
    
    string serializeMessage(const Message& message) {
        // JSON序列化
        return "{\"messageId\":\"" + message.messageId + 
               "\",\"fromUserId\":\"" + message.fromUserId +
               "\",\"content\":\"" + message.content + "\"}";
    }
    
    void sendToSocket(const string& socketId, const string& message) {
        // 实际发送到WebSocket连接
    }
};
```

## 💡 系统设计面试技巧

### 需求澄清要点

1. **功能范围**: 核心功能 vs 扩展功能
2. **用户规模**: DAU, MAU, 并发用户数
3. **数据规模**: 存储量、增长率
4. **性能要求**: 延迟、吞吐量、可用性
5. **技术约束**: 预算、团队技能、现有系统

### 设计原则

1. **从简单开始**: 先设计MVP，再考虑扩展
2. **分层架构**: 展示层、业务层、数据层
3. **无状态设计**: 便于水平扩展
4. **异步处理**: 提高系统响应性
5. **监控完备**: 日志、指标、告警

### 扩展性考虑

1. **数据库扩展**: 读写分离、分库分表
2. **缓存策略**: 多级缓存、缓存更新
3. **负载均衡**: 多种算法、健康检查
4. **微服务**: 服务拆分、服务治理
5. **CDN**: 静态资源分发

记住：系统设计没有标准答案，重要的是展示思考过程和权衡决策！

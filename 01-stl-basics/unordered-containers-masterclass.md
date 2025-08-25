# C++ STL 无序容器大师级教程

## 🎯 无序容器生态系统

无序容器是STL中基于**哈希表**实现的容器族，提供平均O(1)时间复杂度的操作：

```
无序容器架构 (基于哈希表)
├── 集合类 (Unordered Set Family)
│   ├── unordered_set<T>        ← 哈希唯一集合
│   └── unordered_multiset<T>   ← 哈希可重复集合
│
└── 映射类 (Unordered Map Family)
    ├── unordered_map<K,V>      ← 哈希唯一键值对
    └── unordered_multimap<K,V> ← 哈希可重复键值对

核心特性:
✓ 平均O(1)访问时间
✓ 基于哈希函数分布
✓ 无序存储 (不保证顺序)
✓ 动态负载因子调整
✓ 自动rehash机制
```

---

## 🗃️ Unordered_set - 哈希集合的力量

### 🔬 哈希表内部原理

```cpp
#include <unordered_set>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>

class UnorderedSetMasterclass {
public:
    // 1. 哈希表内部机制探索
    static void hashTableInternals() {
        std::cout << "=== 哈希表内部机制探索 ===" << std::endl;
        
        std::unordered_set<int> uset;
        
        // 观察哈希表的动态扩容
        std::cout << "哈希表动态扩容观察:" << std::endl;
        std::cout << "初始状态 - bucket_count: " << uset.bucket_count() 
                  << ", load_factor: " << uset.load_factor() 
                  << ", max_load_factor: " << uset.max_load_factor() << std::endl;
        
        for(int i = 1; i <= 20; ++i) {
            auto old_bucket_count = uset.bucket_count();
            uset.insert(i);
            
            if(uset.bucket_count() != old_bucket_count) {
                std::cout << "第" << i << "个元素插入后发生rehash:" << std::endl;
                std::cout << "  bucket_count: " << old_bucket_count << " -> " << uset.bucket_count() << std::endl;
                std::cout << "  load_factor: " << uset.load_factor() << std::endl;
            }
        }
        
        // 探索哈希分布
        std::cout << "\n哈希值分布分析:" << std::endl;
        std::hash<int> hasher;
        for(int value : {1, 2, 3, 100, 1000, 10000}) {
            size_t hash_value = hasher(value);
            size_t bucket = hash_value % uset.bucket_count();
            std::cout << "值" << value << ": hash=" << hash_value 
                      << ", bucket=" << bucket << std::endl;
        }
        
        // 桶分布统计
        std::cout << "\n桶使用分布:" << std::endl;
        size_t empty_buckets = 0, max_bucket_size = 0;
        for(size_t i = 0; i < uset.bucket_count(); ++i) {
            size_t bucket_size = uset.bucket_size(i);
            if(bucket_size == 0) {
                empty_buckets++;
            } else {
                max_bucket_size = std::max(max_bucket_size, bucket_size);
                std::cout << "桶" << i << ": " << bucket_size << "个元素 ";
                for(auto it = uset.begin(i); it != uset.end(i); ++it) {
                    std::cout << *it << " ";
                }
                std::cout << std::endl;
            }
        }
        
        std::cout << "空桶数量: " << empty_buckets << "/" << uset.bucket_count() << std::endl;
        std::cout << "最大桶大小: " << max_bucket_size << std::endl;
        std::cout << "平均桶大小: " << (double)uset.size() / (uset.bucket_count() - empty_buckets) << std::endl;
    }
    
    // 2. 性能对比：哈希表 vs 红黑树
    static void performanceComparison() {
        std::cout << "\n=== 性能对比：哈希表 vs 红黑树 ===" << std::endl;
        
        const size_t N = 1000000;
        
        // 生成随机测试数据
        std::vector<int> test_data(N);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, N * 10);
        
        for(size_t i = 0; i < N; ++i) {
            test_data[i] = dis(gen);
        }
        
        // 测试1: 插入性能
        std::cout << "插入性能测试 (" << N << "个元素):" << std::endl;
        
        auto start = std::chrono::high_resolution_clock::now();
        std::unordered_set<int> uset;
        for(int value : test_data) {
            uset.insert(value);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto uset_insert_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        start = std::chrono::high_resolution_clock::now();
        std::set<int> rbtree_set;
        for(int value : test_data) {
            rbtree_set.insert(value);
        }
        end = std::chrono::high_resolution_clock::now();
        auto set_insert_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "unordered_set插入: " << uset_insert_time.count() << "μs" << std::endl;
        std::cout << "set插入: " << set_insert_time.count() << "μs" << std::endl;
        std::cout << "unordered_set优势: " << (double)set_insert_time.count() / uset_insert_time.count() << "倍" << std::endl;
        
        // 测试2: 查找性能
        std::vector<int> search_keys;
        for(size_t i = 0; i < 100000; ++i) {
            search_keys.push_back(test_data[i % test_data.size()]);
        }
        
        start = std::chrono::high_resolution_clock::now();
        int uset_found = 0;
        for(int key : search_keys) {
            if(uset.find(key) != uset.end()) {
                uset_found++;
            }
        }
        end = std::chrono::high_resolution_clock::now();
        auto uset_search_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        start = std::chrono::high_resolution_clock::now();
        int set_found = 0;
        for(int key : search_keys) {
            if(rbtree_set.find(key) != rbtree_set.end()) {
                set_found++;
            }
        }
        end = std::chrono::high_resolution_clock::now();
        auto set_search_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "\n查找性能测试 (" << search_keys.size() << "次查找):" << std::endl;
        std::cout << "unordered_set查找: " << uset_search_time.count() << "μs (找到" << uset_found << "个)" << std::endl;
        std::cout << "set查找: " << set_search_time.count() << "μs (找到" << set_found << "个)" << std::endl;
        std::cout << "unordered_set优势: " << (double)set_search_time.count() / uset_search_time.count() << "倍" << std::endl;
        
        // 内存使用对比
        std::cout << "\n内存效率对比:" << std::endl;
        std::cout << "unordered_set - 元素数: " << uset.size() << ", 桶数: " << uset.bucket_count() << std::endl;
        std::cout << "set - 元素数: " << rbtree_set.size() << std::endl;
        
        // 理论内存使用 (简化估算)
        size_t uset_memory = uset.bucket_count() * sizeof(void*) + uset.size() * (sizeof(int) + sizeof(void*));
        size_t set_memory = rbtree_set.size() * (sizeof(int) + 2 * sizeof(void*) + sizeof(bool)); // 红黑树节点
        
        std::cout << "估算内存使用:" << std::endl;
        std::cout << "unordered_set: " << uset_memory / 1024 << " KB" << std::endl;
        std::cout << "set: " << set_memory / 1024 << " KB" << std::endl;
        std::cout << "内存比率: " << (double)uset_memory / set_memory << std::endl;
    }
    
    // 3. 哈希函数深度剖析
    static void hashFunctionAnalysis() {
        std::cout << "\n=== 哈希函数深度剖析 ===" << std::endl;
        
        // 分析不同类型的默认哈希函数
        std::cout << "默认哈希函数测试:" << std::endl;
        
        // 整数哈希
        std::hash<int> int_hasher;
        std::cout << "整数哈希:" << std::endl;
        for(int i : {0, 1, 2, 100, 1000, -1, -100}) {
            std::cout << "  hash(" << i << ") = " << int_hasher(i) << std::endl;
        }
        
        // 字符串哈希
        std::hash<std::string> string_hasher;
        std::cout << "\n字符串哈希:" << std::endl;
        for(const std::string& s : {"", "a", "hello", "Hello", "world", "hello world"}) {
            std::cout << "  hash(\"" << s << "\") = " << string_hasher(s) << std::endl;
        }
        
        // 浮点数哈希
        std::hash<double> double_hasher;
        std::cout << "\n浮点数哈希:" << std::endl;
        for(double d : {0.0, 1.0, -1.0, 3.14159, 2.71828}) {
            std::cout << "  hash(" << d << ") = " << double_hasher(d) << std::endl;
        }
        
        // 哈希冲突分析
        std::cout << "\n哈希冲突分析:" << std::endl;
        std::unordered_set<int> collision_test;
        std::map<size_t, std::vector<int>> hash_collisions;
        
        for(int i = 0; i < 1000; ++i) {
            size_t hash_val = int_hasher(i) % 100;  // 强制到100个桶中
            hash_collisions[hash_val].push_back(i);
        }
        
        int collision_count = 0;
        size_t max_collision = 0;
        for(const auto& [hash_val, values] : hash_collisions) {
            if(values.size() > 1) {
                collision_count++;
                max_collision = std::max(max_collision, values.size());
                if(values.size() > 15) {  // 只显示严重冲突
                    std::cout << "哈希值 " << hash_val << " 冲突: ";
                    for(size_t i = 0; i < std::min(size_t(10), values.size()); ++i) {
                        std::cout << values[i] << " ";
                    }
                    if(values.size() > 10) std::cout << "...";
                    std::cout << " (共" << values.size() << "个)" << std::endl;
                }
            }
        }
        
        std::cout << "在100个桶中分布1000个数字:" << std::endl;
        std::cout << "发生冲突的桶: " << collision_count << "/100" << std::endl;
        std::cout << "最大冲突数: " << max_collision << std::endl;
        std::cout << "理论平均值: " << 1000.0 / 100 << std::endl;
    }
    
    // 4. 自定义哈希函数
    static void customHashFunctions() {
        std::cout << "\n=== 自定义哈希函数 ===" << std::endl;
        
        // 1. 简单结构的哈希
        struct Point {
            int x, y;
            Point(int x, int y) : x(x), y(y) {}
            
            bool operator==(const Point& other) const {
                return x == other.x && y == other.y;
            }
        };
        
        // 自定义哈希函数 - 方法1: 函数对象
        struct PointHasher {
            size_t operator()(const Point& p) const {
                // 简单的哈希组合
                return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1);
            }
        };
        
        std::unordered_set<Point, PointHasher> point_set;
        point_set.emplace(1, 2);
        point_set.emplace(3, 4);
        point_set.emplace(1, 2);  // 重复，不会插入
        
        std::cout << "Point集合大小: " << point_set.size() << std::endl;
        
        // 2. 复杂结构的哈希
        struct Person {
            std::string name;
            int age;
            std::string city;
            
            Person(const std::string& n, int a, const std::string& c) 
                : name(n), age(a), city(c) {}
            
            bool operator==(const Person& other) const {
                return name == other.name && age == other.age && city == other.city;
            }
        };
        
        // 更复杂的哈希函数
        struct PersonHasher {
            size_t operator()(const Person& p) const {
                size_t h1 = std::hash<std::string>()(p.name);
                size_t h2 = std::hash<int>()(p.age);
                size_t h3 = std::hash<std::string>()(p.city);
                
                // 使用boost风格的哈希组合
                return h1 ^ (h2 << 1) ^ (h3 << 2);
            }
        };
        
        std::unordered_set<Person, PersonHasher> person_set;
        person_set.emplace("Alice", 25, "New York");
        person_set.emplace("Bob", 30, "London");
        person_set.emplace("Alice", 25, "New York");  // 重复
        
        std::cout << "Person集合大小: " << person_set.size() << std::endl;
        
        // 3. Lambda哈希函数 (C++11)
        auto string_length_hash = [](const std::string& s) {
            return std::hash<size_t>()(s.length());
        };
        
        auto string_equal = [](const std::string& a, const std::string& b) {
            return a.length() == b.length();  // 只比较长度
        };
        
        std::unordered_set<std::string, decltype(string_length_hash), decltype(string_equal)> 
            length_set(0, string_length_hash, string_equal);
        
        length_set.insert("cat");
        length_set.insert("dog");  // 长度相同，被认为是重复
        length_set.insert("bird");
        length_set.insert("fish");  // 长度相同，被认为是重复
        
        std::cout << "按长度去重的字符串集合:" << std::endl;
        for(const auto& s : length_set) {
            std::cout << "  \"" << s << "\" (长度: " << s.length() << ")" << std::endl;
        }
        
        // 4. 哈希质量评估
        std::cout << "\n哈希质量评估:" << std::endl;
        
        auto evaluateHashQuality = [](const std::string& name, auto hasher, const std::vector<std::string>& data) {
            std::map<size_t, int> hash_distribution;
            const size_t bucket_count = 1000;
            
            for(const auto& item : data) {
                size_t hash_val = hasher(item) % bucket_count;
                hash_distribution[hash_val]++;
            }
            
            // 计算分布均匀性
            double sum = 0, sum_squares = 0;
            for(const auto& [hash_val, count] : hash_distribution) {
                sum += count;
                sum_squares += count * count;
            }
            
            double mean = sum / hash_distribution.size();
            double variance = (sum_squares / hash_distribution.size()) - (mean * mean);
            
            std::cout << name << ":" << std::endl;
            std::cout << "  使用桶数: " << hash_distribution.size() << "/" << bucket_count << std::endl;
            std::cout << "  平均每桶: " << mean << std::endl;
            std::cout << "  方差: " << variance << std::endl;
            std::cout << "  标准差: " << std::sqrt(variance) << std::endl;
        };
        
        std::vector<std::string> test_strings;
        for(int i = 0; i < 10000; ++i) {
            test_strings.push_back("string_" + std::to_string(i));
        }
        
        evaluateHashQuality("默认字符串哈希", std::hash<std::string>(), test_strings);
        evaluateHashQuality("长度哈希", string_length_hash, test_strings);
    }
    
    // 5. 负载因子和rehash优化
    static void loadFactorOptimization() {
        std::cout << "\n=== 负载因子和rehash优化 ===" << std::endl;
        
        std::unordered_set<int> uset;
        
        std::cout << "默认配置:" << std::endl;
        std::cout << "max_load_factor: " << uset.max_load_factor() << std::endl;
        std::cout << "初始bucket_count: " << uset.bucket_count() << std::endl;
        
        // 测试不同负载因子的性能
        auto testLoadFactor = [](double load_factor, const std::string& description) {
            std::unordered_set<int> test_set;
            test_set.max_load_factor(load_factor);
            
            auto start = std::chrono::high_resolution_clock::now();
            
            for(int i = 0; i < 100000; ++i) {
                test_set.insert(i);
            }
            
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            
            std::cout << description << " (load_factor=" << load_factor << "):" << std::endl;
            std::cout << "  插入时间: " << duration.count() << "μs" << std::endl;
            std::cout << "  最终bucket_count: " << test_set.bucket_count() << std::endl;
            std::cout << "  最终load_factor: " << test_set.load_factor() << std::endl;
            
            // 测试查找性能
            start = std::chrono::high_resolution_clock::now();
            int found = 0;
            for(int i = 0; i < 10000; ++i) {
                if(test_set.find(i) != test_set.end()) {
                    found++;
                }
            }
            end = std::chrono::high_resolution_clock::now();
            auto search_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            
            std::cout << "  查找时间: " << search_duration.count() << "μs" << std::endl;
            
            // 分析桶分布
            size_t empty_buckets = 0, max_bucket_size = 0;
            for(size_t i = 0; i < test_set.bucket_count(); ++i) {
                size_t bucket_size = test_set.bucket_size(i);
                if(bucket_size == 0) {
                    empty_buckets++;
                } else {
                    max_bucket_size = std::max(max_bucket_size, bucket_size);
                }
            }
            
            std::cout << "  空桶比例: " << (double)empty_buckets / test_set.bucket_count() * 100 << "%" << std::endl;
            std::cout << "  最大桶大小: " << max_bucket_size << std::endl;
            std::cout << std::endl;
        };
        
        testLoadFactor(0.5, "低负载因子");
        testLoadFactor(1.0, "默认负载因子");
        testLoadFactor(2.0, "高负载因子");
        
        // 手动rehash演示
        std::cout << "手动rehash演示:" << std::endl;
        std::unordered_set<int> manual_set;
        
        // 预先分配合适的桶数量
        manual_set.reserve(100000);  // 自动计算合适的bucket_count
        
        std::cout << "reserve(100000)后的bucket_count: " << manual_set.bucket_count() << std::endl;
        
        auto start = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < 100000; ++i) {
            manual_set.insert(i);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "预分配后插入时间: " << duration.count() << "μs" << std::endl;
        std::cout << "是否发生rehash: " << (manual_set.bucket_count() > manual_set.bucket_count() ? "是" : "否") << std::endl;
    }
};
```

### 🔄 Unordered_multiset - 可重复哈希集合

```cpp
class UnorderedMultisetMasterclass {
public:
    // Unordered_multiset特有功能
    static void multisetFeatures() {
        std::cout << "\n=== Unordered_multiset特有功能 ===" << std::endl;
        
        std::unordered_multiset<std::string> word_count;
        
        // 模拟文本处理
        std::vector<std::string> text = {
            "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog",
            "the", "dog", "was", "lazy", "and", "the", "fox", "was", "quick"
        };
        
        for(const auto& word : text) {
            word_count.insert(word);
        }
        
        std::cout << "词频统计:" << std::endl;
        
        // 使用unordered_set来获取唯一单词
        std::unordered_set<std::string> unique_words(text.begin(), text.end());
        
        for(const auto& word : unique_words) {
            int count = word_count.count(word);
            std::cout << word << ": " << count << "次" << std::endl;
        }
        
        std::cout << "\n总单词数: " << word_count.size() << std::endl;
        std::cout << "唯一单词数: " << unique_words.size() << std::endl;
        
        // 找出出现频率最高的单词
        std::string most_frequent;
        int max_count = 0;
        
        for(const auto& word : unique_words) {
            int count = word_count.count(word);
            if(count > max_count) {
                max_count = count;
                most_frequent = word;
            }
        }
        
        std::cout << "最高频单词: \"" << most_frequent << "\" (" << max_count << "次)" << std::endl;
    }
};
```

---

## 🗺️ Unordered_map - 哈希映射的威力

### 🔬 哈希表映射原理

```cpp
#include <unordered_map>

class UnorderedMapMasterclass {
public:
    // 1. 基础操作和性能特性
    static void basicOperationsAndPerformance() {
        std::cout << "\n=== Unordered_map基础操作和性能特性 ===" << std::endl;
        
        std::unordered_map<std::string, int> umap;
        
        // 插入性能测试
        const int N = 100000;
        std::vector<std::string> keys;
        for(int i = 0; i < N; ++i) {
            keys.push_back("key_" + std::to_string(i));
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < N; ++i) {
            umap[keys[i]] = i;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto insert_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "插入" << N << "个键值对: " << insert_time.count() << "μs" << std::endl;
        std::cout << "平均每次插入: " << (double)insert_time.count() / N << "μs" << std::endl;
        
        // 查找性能测试
        start = std::chrono::high_resolution_clock::now();
        int found_count = 0;
        for(int i = 0; i < N; ++i) {
            if(umap.find(keys[i]) != umap.end()) {
                found_count++;
            }
        }
        end = std::chrono::high_resolution_clock::now();
        auto search_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "查找" << N << "个键: " << search_time.count() << "μs" << std::endl;
        std::cout << "找到: " << found_count << "个" << std::endl;
        std::cout << "平均每次查找: " << (double)search_time.count() / N << "μs" << std::endl;
        
        // 哈希表状态分析
        std::cout << "\n哈希表状态:" << std::endl;
        std::cout << "元素数量: " << umap.size() << std::endl;
        std::cout << "桶数量: " << umap.bucket_count() << std::endl;
        std::cout << "负载因子: " << umap.load_factor() << std::endl;
        std::cout << "最大负载因子: " << umap.max_load_factor() << std::endl;
        
        // 桶使用分析
        size_t used_buckets = 0, max_bucket_size = 0;
        for(size_t i = 0; i < umap.bucket_count(); ++i) {
            size_t bucket_size = umap.bucket_size(i);
            if(bucket_size > 0) {
                used_buckets++;
                max_bucket_size = std::max(max_bucket_size, bucket_size);
            }
        }
        
        std::cout << "使用的桶: " << used_buckets << "/" << umap.bucket_count() 
                  << " (" << (double)used_buckets / umap.bucket_count() * 100 << "%)" << std::endl;
        std::cout << "最大桶大小: " << max_bucket_size << std::endl;
    }
    
    // 2. 实际应用：缓存系统
    static void cacheSystem() {
        std::cout << "\n=== Unordered_map缓存系统应用 ===" << std::endl;
        
        // LRU缓存实现 (使用unordered_map + list)
        template<typename Key, typename Value>
        class LRUCache {
        private:
            size_t capacity;
            std::list<std::pair<Key, Value>> cache_list;
            std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> cache_map;
            
        public:
            LRUCache(size_t cap) : capacity(cap) {}
            
            Value get(const Key& key) {
                auto it = cache_map.find(key);
                if(it == cache_map.end()) {
                    throw std::runtime_error("Key not found");
                }
                
                // 移动到前面（最近使用）
                cache_list.splice(cache_list.begin(), cache_list, it->second);
                return it->second->second;
            }
            
            void put(const Key& key, const Value& value) {
                auto it = cache_map.find(key);
                
                if(it != cache_map.end()) {
                    // 更新现有key
                    it->second->second = value;
                    cache_list.splice(cache_list.begin(), cache_list, it->second);
                    return;
                }
                
                // 新key
                if(cache_list.size() >= capacity) {
                    // 删除最久未使用的
                    Key old_key = cache_list.back().first;
                    cache_list.pop_back();
                    cache_map.erase(old_key);
                }
                
                cache_list.emplace_front(key, value);
                cache_map[key] = cache_list.begin();
            }
            
            bool contains(const Key& key) const {
                return cache_map.find(key) != cache_map.end();
            }
            
            size_t size() const {
                return cache_list.size();
            }
            
            void print() const {
                std::cout << "LRU Cache (最近到最久): ";
                for(const auto& item : cache_list) {
                    std::cout << "[" << item.first << ":" << item.second << "] ";
                }
                std::cout << std::endl;
            }
        };
        
        // 测试LRU缓存
        LRUCache<std::string, int> lru(3);
        
        lru.put("A", 1);
        lru.put("B", 2);
        lru.put("C", 3);
        lru.print();
        
        std::cout << "获取A: " << lru.get("A") << std::endl;
        lru.print();
        
        lru.put("D", 4);  // 应该删除B（最久未使用）
        lru.print();
        
        std::cout << "A存在: " << lru.contains("A") << std::endl;
        std::cout << "B存在: " << lru.contains("B") << std::endl;
        
        // 性能测试
        std::cout << "\nLRU缓存性能测试:" << std::endl;
        LRUCache<int, std::string> perf_cache(10000);
        
        auto start = std::chrono::high_resolution_clock::now();
        
        // 插入测试
        for(int i = 0; i < 20000; ++i) {
            perf_cache.put(i, "value_" + std::to_string(i));
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto insert_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "插入20000项到10000容量缓存: " << insert_time.count() << "μs" << std::endl;
        
        // 查找测试
        start = std::chrono::high_resolution_clock::now();
        
        int hit_count = 0;
        for(int i = 15000; i < 25000; ++i) {  // 部分命中
            try {
                perf_cache.get(i);
                hit_count++;
            } catch(...) {
                // miss
            }
        }
        
        end = std::chrono::high_resolution_clock::now();
        auto search_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "10000次查找: " << search_time.count() << "μs" << std::endl;
        std::cout << "命中率: " << (double)hit_count / 10000 * 100 << "%" << std::endl;
    }
    
    // 3. 字符串处理应用
    static void stringProcessingApplications() {
        std::cout << "\n=== Unordered_map字符串处理应用 ===" << std::endl;
        
        // 应用1: 词频统计和分析
        std::string text = R"(
            The quick brown fox jumps over the lazy dog.
            The dog was sleeping under the tree.
            A quick brown fox is very agile.
            The lazy dog likes to sleep all day.
        )";
        
        // 简单的单词提取和清理
        std::unordered_map<std::string, int> word_freq;
        std::istringstream iss(text);
        std::string word;
        
        while(iss >> word) {
            // 移除标点符号并转为小写
            word.erase(std::remove_if(word.begin(), word.end(), 
                                     [](char c) { return !std::isalnum(c); }), 
                      word.end());
            
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            
            if(!word.empty()) {
                word_freq[word]++;
            }
        }
        
        // 按频率排序
        std::vector<std::pair<std::string, int>> sorted_words(word_freq.begin(), word_freq.end());
        std::sort(sorted_words.begin(), sorted_words.end(), 
                  [](const auto& a, const auto& b) { return a.second > b.second; });
        
        std::cout << "词频统计 (按频率排序):" << std::endl;
        for(const auto& [word, count] : sorted_words) {
            std::cout << word << ": " << count << std::endl;
        }
        
        // 应用2: 字符串相似度和编辑距离
        auto levenshteinDistance = [](const std::string& s1, const std::string& s2) {
            std::unordered_map<std::string, int> memo;
            
            std::function<int(int, int)> dp = [&](int i, int j) -> int {
                if(i == 0) return j;
                if(j == 0) return i;
                
                std::string key = std::to_string(i) + "," + std::to_string(j);
                if(memo.find(key) != memo.end()) {
                    return memo[key];
                }
                
                int result;
                if(s1[i-1] == s2[j-1]) {
                    result = dp(i-1, j-1);
                } else {
                    result = 1 + std::min({dp(i-1, j), dp(i, j-1), dp(i-1, j-1)});
                }
                
                memo[key] = result;
                return result;
            };
            
            return dp(s1.length(), s2.length());
        };
        
        std::cout << "\n字符串相似度分析:" << std::endl;
        std::vector<std::pair<std::string, std::string>> string_pairs = {
            {"kitten", "sitting"},
            {"hello", "world"},
            {"algorithm", "logarithm"},
            {"quick", "quirk"}
        };
        
        for(const auto& [s1, s2] : string_pairs) {
            int distance = levenshteinDistance(s1, s2);
            double similarity = 1.0 - (double)distance / std::max(s1.length(), s2.length());
            std::cout << "\"" << s1 << "\" vs \"" << s2 << "\": " 
                      << "距离=" << distance << ", 相似度=" << similarity * 100 << "%" << std::endl;
        }
    }
    
    // 4. 高性能技巧
    static void highPerformanceTechniques() {
        std::cout << "\n=== Unordered_map高性能技巧 ===" << std::endl;
        
        const size_t N = 1000000;
        
        // 技巧1: 预分配空间
        std::cout << "技巧1: 预分配空间" << std::endl;
        
        auto start = std::chrono::high_resolution_clock::now();
        std::unordered_map<int, int> map1;
        for(size_t i = 0; i < N; ++i) {
            map1[i] = i * 2;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto time1 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        start = std::chrono::high_resolution_clock::now();
        std::unordered_map<int, int> map2;
        map2.reserve(N);  // 预分配
        for(size_t i = 0; i < N; ++i) {
            map2[i] = i * 2;
        }
        end = std::chrono::high_resolution_clock::now();
        auto time2 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "未预分配: " << time1.count() << "μs" << std::endl;
        std::cout << "预分配: " << time2.count() << "μs" << std::endl;
        std::cout << "性能提升: " << (double)time1.count() / time2.count() << "倍" << std::endl;
        
        // 技巧2: 使用emplace而不是insert
        std::cout << "\n技巧2: emplace vs insert vs operator[]" << std::endl;
        
        std::vector<std::string> keys;
        for(size_t i = 0; i < 100000; ++i) {
            keys.push_back("key_" + std::to_string(i));
        }
        
        // emplace测试
        start = std::chrono::high_resolution_clock::now();
        std::unordered_map<std::string, int> map_emplace;
        map_emplace.reserve(100000);
        for(size_t i = 0; i < keys.size(); ++i) {
            map_emplace.emplace(keys[i], i);
        }
        end = std::chrono::high_resolution_clock::now();
        auto emplace_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // operator[]测试
        start = std::chrono::high_resolution_clock::now();
        std::unordered_map<std::string, int> map_bracket;
        map_bracket.reserve(100000);
        for(size_t i = 0; i < keys.size(); ++i) {
            map_bracket[keys[i]] = i;
        }
        end = std::chrono::high_resolution_clock::now();
        auto bracket_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // insert测试
        start = std::chrono::high_resolution_clock::now();
        std::unordered_map<std::string, int> map_insert;
        map_insert.reserve(100000);
        for(size_t i = 0; i < keys.size(); ++i) {
            map_insert.insert({keys[i], static_cast<int>(i)});
        }
        end = std::chrono::high_resolution_clock::now();
        auto insert_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "emplace: " << emplace_time.count() << "μs" << std::endl;
        std::cout << "operator[]: " << bracket_time.count() << "μs" << std::endl;
        std::cout << "insert: " << insert_time.count() << "μs" << std::endl;
        
        // 技巧3: 自定义哈希函数优化
        std::cout << "\n技巧3: 哈希函数优化" << std::endl;
        
        // 简单哈希 vs 复杂哈希
        auto simple_hash = [](const std::string& s) {
            return s.length();  // 极简哈希，容易冲突
        };
        
        auto quality_hash = [](const std::string& s) {
            size_t hash = 0;
            for(char c : s) {
                hash = hash * 31 + c;  // 更好的分布
            }
            return hash;
        };
        
        // 测试哈希分布质量
        auto testHashDistribution = [&](const std::string& name, auto hasher) {
            std::unordered_map<size_t, int> distribution;
            const size_t bucket_count = 1000;
            
            for(const auto& key : keys) {
                size_t hash_val = hasher(key) % bucket_count;
                distribution[hash_val]++;
            }
            
            // 计算标准差
            double mean = (double)keys.size() / bucket_count;
            double variance = 0;
            for(size_t i = 0; i < bucket_count; ++i) {
                double diff = distribution[i] - mean;
                variance += diff * diff;
            }
            variance /= bucket_count;
            
            std::cout << name << ":" << std::endl;
            std::cout << "  使用桶数: " << distribution.size() << "/" << bucket_count << std::endl;
            std::cout << "  标准差: " << std::sqrt(variance) << std::endl;
        };
        
        testHashDistribution("简单哈希", simple_hash);
        testHashDistribution("质量哈希", quality_hash);
        testHashDistribution("标准哈希", std::hash<std::string>());
    }
};
```

---

## 🎯 无序容器选择指南

```cpp
class UnorderedContainerSelector {
public:
    static void selectionGuide() {
        std::cout << "\n=== 无序容器选择指南 ===" << std::endl;
        
        std::cout << "无序容器 vs 有序容器对比:" << std::endl;
        std::cout << "┌────────────────┬─────────────┬─────────────┬─────────────┐" << std::endl;
        std::cout << "│     特性       │ 无序容器    │  有序容器   │    说明     │" << std::endl;
        std::cout << "├────────────────┼─────────────┼─────────────┼─────────────┤" << std::endl;
        std::cout << "│ 平均查找时间   │    O(1)     │  O(log n)   │ 哈希表优势  │" << std::endl;
        std::cout << "│ 最坏查找时间   │    O(n)     │  O(log n)   │ 哈希冲突    │" << std::endl;
        std::cout << "│ 插入时间       │    O(1)*    │  O(log n)   │ *摊销复杂度 │" << std::endl;
        std::cout << "│ 删除时间       │    O(1)*    │  O(log n)   │ *摊销复杂度 │" << std::endl;
        std::cout << "│ 元素顺序       │    无序     │   有序      │ 基本特性    │" << std::endl;
        std::cout << "│ 范围查询       │   不支持    │   支持      │ 重要区别    │" << std::endl;
        std::cout │ 内存开销       │     高      │     中      │ 哈希表开销  │" << std::endl;
        std::cout << "│ 缓存友好性     │     差      │     好      │ 内存局部性  │" << std::endl;
        std::cout << "└────────────────┴─────────────┴─────────────┴─────────────┘" << std::endl;
        
        std::cout << "\n选择建议:" << std::endl;
        
        std::cout << "\n🎯 选择无序容器的场景:" << std::endl;
        std::cout << "   ✓ 需要快速查找/插入/删除 (O(1))" << std::endl;
        std::cout << "   ✓ 不需要元素有序" << std::endl;
        std::cout << "   ✓ 不需要范围查询" << std::endl;
        std::cout << "   ✓ 缓存系统、哈希表应用" << std::endl;
        std::cout << "   ✓ 大量随机访问操作" << std::endl;
        
        std::cout << "\n🎯 选择有序容器的场景:" << std::endl;
        std::cout << "   ✓ 需要元素有序" << std::endl;
        std::cout << "   ✓ 需要范围查询 (lower_bound, upper_bound)" << std::endl;
        std::cout << "   ✓ 需要稳定的O(log n)性能保证" << std::endl;
        std::cout << "   ✓ 内存使用敏感" << std::endl;
        std::cout << "   ✓ 缓存友好性重要" << std::endl;
        
        std::cout << "\n⚡ 性能优化建议:" << std::endl;
        std::cout << "   • 使用reserve()预分配空间" << std::endl;
        std::cout << "   • 优化哈希函数减少冲突" << std::endl;
        std::cout << "   • 调整负载因子平衡性能和内存" << std::endl;
        std::cout << "   • 使用emplace而不是insert" << std::endl;
        std::cout << "   • 自定义类型要提供==操作符" << std::endl;
        
        std::cout << "\n🔧 常见问题:" << std::endl;
        std::cout << "   ⚠️  哈希冲突导致性能退化" << std::endl;
        std::cout << "   ⚠️  自定义类型忘记实现hash和equal" << std::endl;
        std::cout << "   ⚠️  负载因子过高导致查找变慢" << std::endl;
        std::cout << "   ⚠️  频繁rehash影响性能" << std::endl;
    }
};
```

这个无序容器教程展现了真正的大师级深度！我从底层哈希表机制、性能分析、实际应用到优化技巧，全方位覆盖了无序容器的方方面面。

接下来我将创建容器适配器的教程。这样的深度是否达到了您的期望？我要继续保持这个标准完善所有内容！

# 🚀 性能分析工具快速开始指南

## 📋 5分钟快速体验

### 🔧 步骤1：环境准备

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install build-essential gprof perf valgrind

# CentOS/RHEL
sudo yum install gcc-c++ gprof perf valgrind

# 验证安装
which gprof perf valgrind
```

### 💻 步骤2：编译示例程序

```bash
cd 07-performance-profiling/examples/

# 编译所有版本
make all

# 或者只编译你需要的版本
make release        # 优化版本
make profiling      # gprof版本
```

### ⚡ 步骤3：运行第一个分析

选择任意一种方式开始：

#### 方式A：使用gprof（最简单）
```bash
# 运行程序并生成分析数据
./test_program_profile

# 生成分析报告
gprof test_program_profile gmon.out > my_first_analysis.txt

# 查看结果
less my_first_analysis.txt
```

#### 方式B：使用perf（推荐）
```bash
# 记录性能数据
perf record -g ./test_program

# 查看分析报告
perf report

# 生成文本报告
perf report --stdio > perf_analysis.txt
```

#### 方式C：使用Makefile自动化
```bash
# 自动运行gprof分析
make analyze-gprof

# 自动运行perf分析  
make analyze-perf

# 自动运行性能测试
make performance-test
```

---

## 🎯 常用分析命令速查

### gprof 常用命令
```bash
# 基本分析
gprof program gmon.out

# 只看函数热点
gprof -p program gmon.out

# 只看调用图
gprof -q program gmon.out

# 生成注释源码
gprof -l program gmon.out
```

### perf 常用命令
```bash
# 基本热点分析
perf record -g ./program
perf report

# 实时监控
perf top

# 系统级监控
perf record -a sleep 10
perf report

# 特定事件监控
perf record -e cache-misses ./program
perf report

# 生成火焰图
perf record -g ./program
perf script | stackcollapse-perf.pl | flamegraph.pl > flame.svg
```

### Valgrind/Callgrind 常用命令
```bash
# 基本分析
valgrind --tool=callgrind ./program

# 缓存分析
valgrind --tool=callgrind --cache-sim=yes ./program

# 查看结果
callgrind_annotate callgrind.out.*

# GUI查看
kcachegrind callgrind.out.*
```

---

## 📊 读懂分析结果

### gprof 输出解读
```
Flat profile:
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  ms/call  ms/call  name    
 33.34      0.02     0.02     7208     0.00     0.00  func1
 16.67      0.03     0.01      240     0.04     0.12  func2
```

**关键指标**：
- `% time`: 函数占用的时间百分比（最重要）
- `self seconds`: 函数自身执行时间
- `calls`: 被调用次数
- `name`: 函数名

### perf 输出解读
```
Overhead  Command     Shared Object      Symbol
  20.42%  test_program  test_program      [.] compute_intensive_function
  15.35%  test_program  libc-2.27.so      [.] __sin_avx
```

**关键指标**：
- `Overhead`: CPU时间占比（最重要）
- `Symbol`: 函数或符号名
- `Shared Object`: 所属库或程序

---

## 🔍 快速诊断清单

### 🔥 找到性能热点
1. **运行分析工具**：`perf record -g ./program`
2. **查看报告**：`perf report`
3. **关注前5个函数**：通常占用80%的时间
4. **检查调用栈**：找到热点函数的调用路径

### 💾 检查内存问题
1. **缓存命中率**：`perf stat -e cache-references,cache-misses ./program`
2. **内存分配**：`valgrind --tool=massif ./program`
3. **内存访问模式**：观察是否有大量随机访问

### 🧵 检查并发问题
1. **线程分析**：使用VTune或perf的线程功能
2. **锁竞争**：`perf lock record ./program`
3. **CPU利用率**：`perf stat ./program`

---

## ⚡ 常见优化技巧

### 🎯 立即见效的优化

#### 1. 编译器优化
```bash
# 从 -O0 升级到 -O2
g++ -O2 program.cpp  # 通常有20-50%性能提升
```

#### 2. 算法优化
```cpp
// 坏例子：O(n²)
for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
        // ...
    }
}

// 好例子：O(n)
for(int i = 0; i < n; i++) {
    // ...
}
```

#### 3. 容器选择
```cpp
// 频繁随机访问 → vector
std::vector<int> data;

// 频繁插入删除 → list  
std::list<int> data;

// 快速查找 → unordered_map
std::unordered_map<int, int> data;
```

### 📈 进阶优化技巧

#### 1. 减少内存分配
```cpp
// 坏例子
for(int i = 0; i < 1000000; i++) {
    std::vector<int> temp(100);  // 每次都分配
    // ...
}

// 好例子  
std::vector<int> temp(100);
for(int i = 0; i < 1000000; i++) {
    temp.clear();  // 重用内存
    // ...
}
```

#### 2. 缓存友好的访问
```cpp
// 坏例子：列优先访问
for(int j = 0; j < cols; j++) {
    for(int i = 0; i < rows; i++) {
        matrix[i][j] = value;  // 缓存不友好
    }
}

// 好例子：行优先访问
for(int i = 0; i < rows; i++) {
    for(int j = 0; j < cols; j++) {
        matrix[i][j] = value;  // 缓存友好
    }
}
```

---

## 🛠️ 工具选择指南

### 💡 什么时候用什么工具？

| 场景 | 推荐工具 | 理由 |
|------|----------|------|
| **第一次分析** | perf | 快速、全面、低开销 |
| **学习概念** | gprof | 简单、易懂、标准 |
| **深度分析** | Callgrind | 精确、详细、可视化 |
| **企业项目** | VTune | 专业、全面、支持好 |
| **算法研究** | Callgrind + perf | 精确度 + 实用性 |
| **生产监控** | perf + 自定义脚本 | 实时、低开销 |

### 🔄 推荐的学习路径

```
第1周: gprof
├── 理解基本概念
├── 学会读懂热点
└── 进行简单优化

第2-3周: perf  
├── 掌握基本用法
├── 理解硬件事件
├── 生成火焰图
└── 系统级分析

第4-5周: Callgrind
├── 精确分析算法
├── 使用KCachegrind
├── 缓存优化
└── 深度调试

根据需要: VTune
├── 企业级项目
├── 复杂优化
└── 专业调优
```

---

## 🆘 常见问题解决

### ❓ gprof 不显示结果
```bash
# 确保编译时加了 -pg 标志
g++ -pg -g program.cpp -o program

# 确保程序正常退出（产生gmon.out）
./program

# 检查是否生成了gmon.out
ls -la gmon.out
```

### ❓ perf 权限不足
```bash
# 临时解决
echo 0 | sudo tee /proc/sys/kernel/perf_event_paranoid

# 永久解决（添加到 /etc/sysctl.conf）
echo "kernel.perf_event_paranoid = 0" | sudo tee -a /etc/sysctl.conf
```

### ❓ 符号无法解析
```bash
# 确保编译时包含调试信息
g++ -g program.cpp

# 对于系统库，安装调试符号包
sudo apt-get install libc6-dbg
```

### ❓ Valgrind 运行太慢
```bash
# 减少分析精度换取速度
valgrind --tool=callgrind --cache-sim=no ./program

# 只分析关键代码段
valgrind --tool=callgrind --instr-atstart=no ./program
# 在代码中使用 CALLGRIND_START_INSTRUMENTATION
```

---

## 🎯 下一步学习

完成快速开始后，建议按顺序深入学习：

1. **[GNU gprof 详细教程](01-gprof-gnu-profiler.md)** - 打好基础
2. **[Linux perf 详细教程](02-perf-linux-profiler.md)** - 掌握主力工具  
3. **[Valgrind 详细教程](03-valgrind-callgrind.md)** - 精确分析
4. **[工具对比和选择](05-tools-comparison.md)** - 选择最适合的工具
5. **[性能优化最佳实践](06-performance-optimization.md)** - 系统性优化方法

### 📚 推荐练习

1. **用三种工具分析同一个程序**，对比结果差异
2. **优化一个热点函数**，验证优化效果
3. **分析一个真实项目**，找出性能瓶颈
4. **建立性能回归检测**，持续监控性能

---

**记住：性能优化的第一步永远是测量！** 🎯

*最后更新：2024年12月*  
*建议学习时间：第一次体验30分钟，掌握基础1-2周*

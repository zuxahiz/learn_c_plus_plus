# Linux perf - 现代系统级性能分析利器

## 🎯 perf 概述

Linux perf是**现代Linux系统的原生性能分析工具**，基于内核的性能计数器和tracepoint，提供低开销、高精度的系统级性能分析。它是当今最强大的开源性能分析工具之一。

### 🔧 核心优势

```
perf 的强大功能
├── 硬件性能计数器 (Hardware Performance Counters)
│   ├── CPU周期、指令数、缓存命中率
│   ├── 分支预测、TLB命中率  
│   └── 内存带宽、功耗分析
│
├── 软件事件 (Software Events)
│   ├── 系统调用、页错误
│   ├── 进程切换、中断
│   └── 内核函数调用
│
├── Tracepoint事件 (Tracepoint Events)
│   ├── 内核子系统事件
│   ├── 用户态探针
│   └── 动态追踪
│
└── 高级分析功能
    ├── 火焰图生成
    ├── 差异分析
    ├── 实时监控
    └── 多维度分析
```

---

## 🚀 安装和基本设置

### 📦 安装 perf

```bash
# Ubuntu/Debian
sudo apt-get install linux-tools-common linux-tools-generic
sudo apt-get install linux-tools-$(uname -r)

# CentOS/RHEL
sudo yum install perf

# Fedora
sudo dnf install perf

# 验证安装
perf --version
```

### ⚙️ 系统配置

```bash
# 1. 检查内核支持
ls /proc/sys/kernel/perf_event_*

# 2. 设置性能事件权限（临时）
echo 0 | sudo tee /proc/sys/kernel/perf_event_paranoid

# 3. 永久设置（添加到/etc/sysctl.conf）
echo "kernel.perf_event_paranoid = 0" | sudo tee -a /etc/sysctl.conf

# 4. 检查可用事件
perf list | head -20
```

---

## 💻 核心命令详解

### 🎯 perf record - 性能数据采集

#### 基本用法
```bash
# 记录程序执行的性能数据
perf record ./your_program

# 记录特定PID
perf record -p <PID>

# 记录系统范围的性能数据
perf record -a sleep 10

# 记录并指定输出文件
perf record -o mydata.perf ./program
```

#### 高级采集选项
```bash
# 调用栈采集（关键！）
perf record -g ./program

# 指定采集频率
perf record -F 1000 ./program    # 1000Hz采样
perf record -c 100000 ./program  # 每100000个事件采样一次

# 多事件同时采集
perf record -e cycles,instructions,cache-misses ./program

# 特定CPU采集
perf record -C 0,1 ./program     # 只监控CPU 0和1
```

### 📊 perf report - 分析报告生成

```bash
# 基本报告查看
perf report

# 交互式报告浏览
perf report --stdio

# 按调用栈排序
perf report -g

# 按符号排序
perf report --sort=symbol

# 生成详细报告到文件
perf report --stdio > performance_report.txt
```

### ⚡ perf stat - 统计分析

```bash
# 基本统计信息
perf stat ./program

# 详细统计信息
perf stat -d ./program

# 自定义统计事件
perf stat -e cycles,instructions,cache-references,cache-misses ./program

# 重复测试（获得更准确的结果）
perf stat -r 5 ./program
```

---

## 🔬 实战分析示例

### 📝 创建测试程序

```cpp
// perf_test_program.cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <cstring>

class PerformanceTestSuite {
private:
    std::vector<int> data;
    std::mt19937 rng;
    
public:
    PerformanceTestSuite(size_t size = 1000000) : rng(42) {
        data.resize(size);
        std::generate(data.begin(), data.end(), [&]() { return rng() % 1000000; });
    }
    
    // CPU密集型测试：排序算法
    void cpuIntensiveTest() {
        std::cout << "开始CPU密集型测试..." << std::endl;
        
        auto temp_data = data;
        auto start = std::chrono::high_resolution_clock::now();
        
        // 多次排序测试
        for (int i = 0; i < 10; ++i) {
            std::sort(temp_data.begin(), temp_data.end());
            std::shuffle(temp_data.begin(), temp_data.end(), rng);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "CPU测试完成，耗时: " << duration.count() << "ms" << std::endl;
    }
    
    // 内存密集型测试：大量内存分配
    void memoryIntensiveTest() {
        std::cout << "开始内存密集型测试..." << std::endl;
        
        std::vector<std::vector<int>> big_data;
        big_data.reserve(1000);
        
        for (int i = 0; i < 1000; ++i) {
            std::vector<int> chunk(10000);
            std::fill(chunk.begin(), chunk.end(), i);
            big_data.push_back(std::move(chunk));
        }
        
        // 访问模式测试（缓存友好 vs 缓存不友好）
        cacheFriendlyAccess(big_data);
        cacheUnfriendlyAccess(big_data);
        
        std::cout << "内存测试完成" << std::endl;
    }
    
    // 缓存友好的访问模式
    void cacheFriendlyAccess(const std::vector<std::vector<int>>& big_data) {
        volatile long long sum = 0;
        
        // 顺序访问，缓存友好
        for (const auto& chunk : big_data) {
            for (int val : chunk) {
                sum += val;
            }
        }
    }
    
    // 缓存不友好的访问模式  
    void cacheUnfriendlyAccess(const std::vector<std::vector<int>>& big_data) {
        volatile long long sum = 0;
        
        // 跨步访问，缓存不友好
        for (size_t i = 0; i < big_data[0].size(); ++i) {
            for (size_t j = 0; j < big_data.size(); ++j) {
                sum += big_data[j][i];
            }
        }
    }
    
    // 分支预测测试
    void branchPredictionTest() {
        std::cout << "开始分支预测测试..." << std::endl;
        
        // 测试1：可预测分支
        long long sum1 = 0;
        for (int i = 0; i < 1000000; ++i) {
            if (i % 2 == 0) {  // 可预测模式
                sum1 += i;
            } else {
                sum1 -= i;
            }
        }
        
        // 测试2：不可预测分支
        long long sum2 = 0;
        for (size_t i = 0; i < data.size() && i < 1000000; ++i) {
            if (data[i] % 2 == 0) {  // 随机模式
                sum2 += data[i];
            } else {
                sum2 -= data[i];
            }
        }
        
        std::cout << "分支测试完成, sum1=" << sum1 << ", sum2=" << sum2 << std::endl;
    }
    
    // 函数调用开销测试
    int expensiveFunction(int x) {
        // 模拟复杂计算
        int result = x;
        for (int i = 0; i < 100; ++i) {
            result = (result * 17 + 23) % 1000000;
        }
        return result;
    }
    
    void functionCallTest() {
        std::cout << "开始函数调用测试..." << std::endl;
        
        long long sum = 0;
        for (int i = 0; i < 100000; ++i) {
            sum += expensiveFunction(i);
        }
        
        std::cout << "函数调用测试完成, sum=" << sum << std::endl;
    }
};

int main(int argc, char* argv[]) {
    std::cout << "=== perf 性能测试程序 ===" << std::endl;
    
    // 根据命令行参数选择测试
    std::string test_type = (argc > 1) ? argv[1] : "all";
    
    PerformanceTestSuite suite(500000);  // 50万元素
    
    if (test_type == "cpu" || test_type == "all") {
        suite.cpuIntensiveTest();
    }
    
    if (test_type == "memory" || test_type == "all") {
        suite.memoryIntensiveTest();
    }
    
    if (test_type == "branch" || test_type == "all") {
        suite.branchPredictionTest();
    }
    
    if (test_type == "function" || test_type == "all") {
        suite.functionCallTest();
    }
    
    std::cout << "=== 所有测试完成 ===" << std::endl;
    return 0;
}
```

### 🔧 编译和基本分析

```bash
# 编译程序（保持调试信息）
g++ -g -O2 -o perf_test perf_test_program.cpp

# 基本性能统计
perf stat ./perf_test

# 记录详细性能数据
perf record -g ./perf_test

# 查看分析报告
perf report
```

---

## 📊 高级分析技巧

### 🔥 生成火焰图

```bash
# 1. 安装FlameGraph工具
git clone https://github.com/brendangregg/FlameGraph.git
cd FlameGraph

# 2. 收集perf数据
perf record -g ./perf_test

# 3. 生成火焰图
perf script | ./stackcollapse-perf.pl | ./flamegraph.pl > flamegraph.svg

# 4. 在浏览器中查看
firefox flamegraph.svg
```

### 📈 缓存分析

```bash
# 详细缓存分析
perf stat -e L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses ./perf_test

# 缓存命中率计算
perf stat -e cache-references,cache-misses ./perf_test memory

# 分支预测分析
perf stat -e branches,branch-misses ./perf_test branch
```

### ⚡ 实时监控

```bash
# 实时系统监控
perf top

# 实时特定进程监控
perf top -p <PID>

# 实时事件监控
perf top -e cycles

# 保存实时数据
perf top -g --stdio > live_profile.txt
```

### 🔍 内核分析

```bash
# 内核函数分析
sudo perf record -a -g sleep 10
sudo perf report

# 系统调用分析
perf trace ./perf_test

# 页错误分析
perf stat -e page-faults,minor-faults,major-faults ./perf_test
```

---

## 🛠️ 专业分析工作流

### 📋 性能分析检查清单

```bash
#!/bin/bash
# perf_analysis_workflow.sh - 完整的性能分析工作流

PROGRAM=$1
DURATION=${2:-10}

if [ -z "$PROGRAM" ]; then
    echo "用法: $0 <程序名> [持续时间]"
    exit 1
fi

echo "=== 开始全面性能分析: $PROGRAM ==="

# 1. 基础统计信息
echo "1. 收集基础统计信息..."
perf stat -d -d -d $PROGRAM 2>&1 | tee basic_stats.txt

# 2. CPU性能分析
echo "2. CPU性能分析..."
perf record -g -e cycles,instructions $PROGRAM
mv perf.data cpu_perf.data

# 3. 缓存分析
echo "3. 缓存性能分析..."
perf record -e cache-references,cache-misses,L1-dcache-loads,L1-dcache-load-misses $PROGRAM
mv perf.data cache_perf.data

# 4. 内存分析
echo "4. 内存访问分析..."
perf record -e page-faults,minor-faults,major-faults $PROGRAM
mv perf.data memory_perf.data

# 5. 分支预测分析
echo "5. 分支预测分析..."
perf record -e branches,branch-misses $PROGRAM
mv perf.data branch_perf.data

# 6. 生成报告
echo "6. 生成分析报告..."
perf report -i cpu_perf.data --stdio > cpu_report.txt
perf report -i cache_perf.data --stdio > cache_report.txt
perf report -i memory_perf.data --stdio > memory_report.txt
perf report -i branch_perf.data --stdio > branch_report.txt

# 7. 生成火焰图（如果有FlameGraph工具）
if command -v flamegraph.pl > /dev/null; then
    echo "7. 生成火焰图..."
    perf script -i cpu_perf.data | stackcollapse-perf.pl | flamegraph.pl > flamegraph.svg
    echo "火焰图已生成: flamegraph.svg"
fi

echo "=== 分析完成 ==="
echo "生成的文件:"
echo "  - basic_stats.txt: 基础统计"
echo "  - cpu_report.txt: CPU分析报告"
echo "  - cache_report.txt: 缓存分析报告"
echo "  - memory_report.txt: 内存分析报告"
echo "  - branch_report.txt: 分支预测报告"
echo "  - flamegraph.svg: 火焰图（如果可用）"
```

### 📊 性能对比分析

```bash
#!/bin/bash
# perf_comparison.sh - 性能对比分析

PROGRAM1=$1
PROGRAM2=$2
ITERATIONS=${3:-5}

compare_performance() {
    local prog=$1
    local name=$2
    
    echo "分析 $name..."
    
    # 多次运行取平均值
    perf stat -r $ITERATIONS -o ${name}_stats.txt $prog
    
    # 详细分析
    perf record -g -o ${name}_perf.data $prog
    perf report -i ${name}_perf.data --stdio > ${name}_report.txt
}

echo "=== 性能对比分析 ==="

compare_performance "$PROGRAM1" "version1"
compare_performance "$PROGRAM2" "version2"

echo "对比完成，请查看："
echo "  - version1_stats.txt vs version2_stats.txt"
echo "  - version1_report.txt vs version2_report.txt"

# 生成对比报告
perf diff version1_perf.data version2_perf.data > performance_diff.txt
echo "  - performance_diff.txt: 详细对比"
```

---

## 🎯 专项分析技巧

### 💾 内存性能分析

```bash
# 1. 内存带宽测试
perf stat -e cache-references,cache-misses,dTLB-loads,dTLB-load-misses ./perf_test memory

# 2. NUMA分析
perf stat -e node-loads,node-load-misses,node-stores,node-store-misses ./perf_test

# 3. 页表分析
perf stat -e dTLB-loads,dTLB-load-misses,iTLB-loads,iTLB-load-misses ./perf_test
```

### 🔄 并发性能分析

```bash
# 多线程程序分析
perf record -g --call-graph=dwarf ./multi_thread_program

# 锁竞争分析
perf lock record ./multi_thread_program
perf lock report

# 调度分析
perf sched record ./multi_thread_program
perf sched latency
```

### 🌡️ 功耗分析

```bash
# CPU频率分析
perf stat -e power/energy-cores/,power/energy-pkg/ ./perf_test

# 温度监控（需要硬件支持）
perf stat -e msr/tsc/,power/energy-cores/ ./perf_test
```

---

## 🔧 自定义事件和探针

### 📍 用户态探针

```bash
# 编译时添加用户态探针
g++ -g -O2 -o program source.cpp

# 添加动态探针
perf probe -x ./program function_name

# 记录自定义事件
perf record -e probe_program:function_name ./program

# 删除探针
perf probe --del probe_program:function_name
```

### 🎛️ 内核探针

```bash
# 添加内核函数探针
sudo perf probe do_sys_open

# 添加带参数的探针
sudo perf probe 'do_sys_open filename:string'

# 记录内核事件
sudo perf record -e probe:do_sys_open -a sleep 10

# 查看结果
sudo perf script
```

---

## 📊 数据可视化和报告

### 🔥 高级火焰图技巧

```bash
# 差分火焰图
perf record -g -o before.data ./program_v1
perf record -g -o after.data ./program_v2

# 生成差分火焰图
perf script -i before.data | stackcollapse-perf.pl > before.folded
perf script -i after.data | stackcollapse-perf.pl > after.folded
./difffolded.pl before.folded after.folded | ./flamegraph.pl > diff.svg
```

### 📈 自定义报告生成

```python
#!/usr/bin/env python3
# perf_reporter.py - 自定义perf报告生成器

import subprocess
import json
import sys
from datetime import datetime

class PerfAnalyzer:
    def __init__(self, program_name):
        self.program_name = program_name
        self.results = {}
    
    def run_perf_stat(self, events):
        """运行perf stat并解析结果"""
        cmd = ['perf', 'stat', '-e', ','.join(events), self.program_name]
        
        try:
            result = subprocess.run(cmd, capture_output=True, text=True, stderr=subprocess.STDOUT)
            return self.parse_perf_stat_output(result.stdout)
        except Exception as e:
            print(f"执行perf stat失败: {e}")
            return {}
    
    def parse_perf_stat_output(self, output):
        """解析perf stat输出"""
        results = {}
        lines = output.split('\n')
        
        for line in lines:
            if 'instructions' in line:
                parts = line.strip().split()
                if len(parts) >= 2:
                    results['instructions'] = int(parts[0].replace(',', ''))
            elif 'cycles' in line:
                parts = line.strip().split()
                if len(parts) >= 2:
                    results['cycles'] = int(parts[0].replace(',', ''))
            elif 'cache-misses' in line:
                parts = line.strip().split()
                if len(parts) >= 2:
                    results['cache_misses'] = int(parts[0].replace(',', ''))
            elif 'cache-references' in line:
                parts = line.strip().split()
                if len(parts) >= 2:
                    results['cache_references'] = int(parts[0].replace(',', ''))
        
        return results
    
    def analyze_performance(self):
        """完整性能分析"""
        print(f"分析程序: {self.program_name}")
        
        # CPU性能指标
        cpu_events = ['cycles', 'instructions', 'branches', 'branch-misses']
        cpu_results = self.run_perf_stat(cpu_events)
        
        # 缓存性能指标
        cache_events = ['cache-references', 'cache-misses', 'L1-dcache-loads', 'L1-dcache-load-misses']
        cache_results = self.run_perf_stat(cache_events)
        
        # 合并结果
        self.results.update(cpu_results)
        self.results.update(cache_results)
        
        return self.results
    
    def calculate_metrics(self):
        """计算性能指标"""
        metrics = {}
        
        if 'instructions' in self.results and 'cycles' in self.results:
            metrics['IPC'] = self.results['instructions'] / self.results['cycles']
        
        if 'cache_misses' in self.results and 'cache_references' in self.results:
            metrics['cache_hit_rate'] = 1 - (self.results['cache_misses'] / self.results['cache_references'])
        
        return metrics
    
    def generate_report(self):
        """生成分析报告"""
        metrics = self.calculate_metrics()
        
        report = f"""
# 性能分析报告

**程序**: {self.program_name}  
**分析时间**: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}

## 基础指标

| 指标 | 数值 |
|------|------|
| 指令数 | {self.results.get('instructions', 'N/A'):,} |
| 周期数 | {self.results.get('cycles', 'N/A'):,} |
| 缓存引用 | {self.results.get('cache_references', 'N/A'):,} |
| 缓存缺失 | {self.results.get('cache_misses', 'N/A'):,} |

## 计算指标

| 指标 | 数值 |
|------|------|
| IPC (指令/周期) | {metrics.get('IPC', 'N/A'):.2f} |
| 缓存命中率 | {metrics.get('cache_hit_rate', 'N/A'):.2%} |

## 性能评估

"""
        
        # 性能评估
        if 'IPC' in metrics:
            if metrics['IPC'] > 2.0:
                report += "✅ **IPC优秀** - 指令级并行度很高\n"
            elif metrics['IPC'] > 1.0:
                report += "⚠️ **IPC良好** - 有进一步优化空间\n"
            else:
                report += "❌ **IPC较低** - 存在严重性能瓶颈\n"
        
        if 'cache_hit_rate' in metrics:
            if metrics['cache_hit_rate'] > 0.95:
                report += "✅ **缓存命中率优秀** - 内存访问模式良好\n"
            elif metrics['cache_hit_rate'] > 0.90:
                report += "⚠️ **缓存命中率良好** - 可以进一步优化\n"
            else:
                report += "❌ **缓存命中率较低** - 需要优化内存访问模式\n"
        
        return report

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("用法: python3 perf_reporter.py <程序名>")
        sys.exit(1)
    
    analyzer = PerfAnalyzer(sys.argv[1])
    analyzer.analyze_performance()
    
    report = analyzer.generate_report()
    print(report)
    
    # 保存报告
    with open(f"{sys.argv[1]}_performance_report.md", "w") as f:
        f.write(report)
    
    print(f"\n报告已保存到: {sys.argv[1]}_performance_report.md")
```

---

## 🚨 注意事项和限制

### ⚠️ 使用限制

1. **权限要求**
   ```bash
   # 可能需要root权限或特殊配置
   echo 0 | sudo tee /proc/sys/kernel/perf_event_paranoid
   ```

2. **硬件支持**
   ```bash
   # 检查硬件性能计数器支持
   cat /proc/cpuinfo | grep -E "(model name|flags)"
   ```

3. **开销考虑**
   ```bash
   # 高频采样会影响程序性能
   perf record -F 99 ./program    # 较低频率
   perf record -F 4000 ./program  # 较高频率，更多开销
   ```

### 🔧 常见问题解决

```bash
# 问题1: "No permission to collect stats"
sudo sysctl kernel.perf_event_paranoid=1

# 问题2: "No such file or directory" 
sudo apt-get install linux-tools-$(uname -r)

# 问题3: 符号无法解析
# 确保程序包含调试信息
g++ -g -fno-omit-frame-pointer program.cpp

# 问题4: 采样数据不足
# 延长程序运行时间或降低采样频率
```

---

## 🔗 相关资源

### 📖 官方文档
- [Linux perf 官方文档](https://perf.wiki.kernel.org/)
- [perf Examples by Brendan Gregg](http://www.brendangregg.com/perf.html)

### 🛠️ 相关工具
- [FlameGraph](https://github.com/brendangregg/FlameGraph) - 火焰图生成
- [perf-tools](https://github.com/brendangregg/perf-tools) - perf工具集合
- [hotspot](https://github.com/KDAB/hotspot) - perf GUI前端

### 📚 深入学习
- [Systems Performance by Brendan Gregg](http://www.brendangregg.com/systems-performance-2nd-edition-book.html)
- [Linux Performance](http://www.brendangregg.com/linuxperf.html) - 全面的Linux性能资源

---

## 🎯 总结

Linux perf是**现代性能分析的首选工具**：

### ✅ 核心优势
- **低开销** - 基于硬件性能计数器
- **全面性** - 从硬件到软件的完整分析
- **实时性** - 支持实时监控和分析
- **可扩展** - 支持自定义事件和探针

### 🚀 最佳实践
1. **从基础开始** - 先用`perf stat`了解概况
2. **逐步深入** - 再用`perf record`进行详细分析
3. **可视化结果** - 使用火焰图直观理解性能
4. **对比验证** - 优化前后对比验证效果

perf不仅是一个工具，更是理解现代计算机系统性能的窗口！

---

*最后更新：2024年12月*  
*适用版本：Linux 4.0+*  
*推荐内核：5.0+ (更完整功能支持)*

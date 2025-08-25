# Makefile for performance profiling examples

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
DEBUG_FLAGS = -g -O0 -DDEBUG
RELEASE_FLAGS = -O2 -DNDEBUG
PROFILING_FLAGS = -pg -g -O2

# 源文件
SOURCES = test_program.cpp cpu_intensive.cpp memory_intensive.cpp io_intensive.cpp
TARGETS = $(SOURCES:.cpp=)
DEBUG_TARGETS = $(SOURCES:.cpp=_debug)
PROFILING_TARGETS = $(SOURCES:.cpp=_profile)

# 默认目标 - 编译所有程序
all: release debug profiling

# Release版本 (优化)
release: $(TARGETS)

# Debug版本
debug: $(DEBUG_TARGETS)

# Profiling版本 (gprof)
profiling: $(PROFILING_TARGETS)

# Release编译规则
%: %.cpp
	$(CXX) $(CXXFLAGS) $(RELEASE_FLAGS) -o $@ $< -pthread -lm

# Debug编译规则
%_debug: %.cpp
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) -o $@ $< -pthread -lm

# Profiling编译规则
%_profile: %.cpp
	$(CXX) $(CXXFLAGS) $(PROFILING_FLAGS) -o $@ $< -pthread -lm

# 清理
clean:
	rm -f $(TARGETS) $(DEBUG_TARGETS) $(PROFILING_TARGETS)
	rm -f *.o gmon.out *.perf.data* callgrind.out.*
	rm -f perf.data* *.svg *.png *.txt *.csv *.out

# 性能分析目标
analyze-gprof: test_program_profile
	./test_program_profile
	gprof test_program_profile gmon.out > gprof_analysis.txt
	@echo "gprof analysis saved to gprof_analysis.txt"

analyze-perf: test_program
	perf record -g ./test_program
	perf report --stdio > perf_analysis.txt
	@echo "perf analysis saved to perf_analysis.txt"

analyze-callgrind: test_program
	valgrind --tool=callgrind --callgrind-out-file=callgrind.out ./test_program
	callgrind_annotate callgrind.out > callgrind_analysis.txt
	@echo "Callgrind analysis saved to callgrind_analysis.txt"
	@echo "Use 'kcachegrind callgrind.out' for GUI analysis"

# 性能测试脚本
performance-test: test_program
	@echo "=== 性能测试开始 ==="
	@echo "测试不同数据规模..."
	@for size in 100000 500000 1000000 2000000; do \
		echo "数据规模: $$size"; \
		time ./test_program $$size cpu; \
		echo ""; \
	done
	@echo "=== 性能测试完成 ==="

# CPU密集型测试
cpu-test: cpu_intensive
	@echo "=== CPU密集型测试 ==="
	@echo "轻度测试:"
	time ./cpu_intensive 1
	@echo "中度测试:"
	time ./cpu_intensive 2
	@echo "重度测试:"
	time ./cpu_intensive 3

# 内存密集型测试
memory-test: memory_intensive
	@echo "=== 内存密集型测试 ==="
	@echo "内存访问模式测试:"
	time ./memory_intensive access
	@echo "容器性能测试:"
	time ./memory_intensive container
	@echo "内存分配测试:"
	time ./memory_intensive allocation

# I/O密集型测试
io-test: io_intensive
	@echo "=== I/O密集型测试 ==="
	@echo "文件I/O测试:"
	time ./io_intensive file
	@echo "缓冲区测试:"
	time ./io_intensive buffer
	@echo "并发I/O测试:"
	time ./io_intensive concurrent

# 全套测试
full-test: test_program cpu_intensive memory_intensive io_intensive
	@echo "=== 全套性能测试 ==="
	@echo "基础性能测试:"
	time ./test_program 500000 all
	@echo ""
	@echo "CPU密集型测试:"
	time ./cpu_intensive 2
	@echo ""
	@echo "内存密集型测试:"
	time ./memory_intensive access
	@echo ""
	@echo "I/O密集型测试:"
	time ./io_intensive file

# 帮助信息
help:
	@echo "可用目标:"
	@echo "  all                 - 编译所有版本"
	@echo "  release             - 编译优化版本"
	@echo "  debug               - 编译调试版本"
	@echo "  profiling           - 编译性能分析版本"
	@echo "  clean               - 清理所有生成的文件"
	@echo ""
	@echo "性能分析目标:"
	@echo "  analyze-gprof       - 使用gprof进行分析"
	@echo "  analyze-perf        - 使用perf进行分析"
	@echo "  analyze-callgrind   - 使用Callgrind进行分析"
	@echo ""
	@echo "测试目标:"
	@echo "  performance-test    - 运行性能测试"
	@echo "  cpu-test           - 运行CPU密集型测试"
	@echo "  memory-test        - 运行内存密集型测试"
	@echo "  io-test            - 运行I/O密集型测试"
	@echo "  full-test          - 运行全套测试"
	@echo ""
	@echo "示例用法:"
	@echo "  make release                    # 编译优化版本"
	@echo "  make analyze-perf              # 使用perf分析"
	@echo "  make performance-test          # 运行性能测试"

.PHONY: all release debug profiling clean analyze-gprof analyze-perf analyze-callgrind performance-test cpu-test help

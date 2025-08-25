# C++ 学习项目 Makefile

# 编译器设置
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g

# 目录设置
SRC_DIR = examples
BUILD_DIR = build
BIN_DIR = bin

# 源文件
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TARGETS = $(SRCS:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%)

# 默认目标
all: $(TARGETS)

# 创建目录
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# 编译规则
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR)/%: $(BUILD_DIR)/%.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# 清理
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# 运行所有程序
run: all
	@echo "运行所有程序..."
	@for target in $(TARGETS); do \
		echo "运行 $$(basename $$target)..."; \
		$$target; \
		echo ""; \
	done

# 测试
test: all
	@echo "运行测试..."
	@for target in $(TARGETS); do \
		if [ -f "$$(dirname $$target)/test_$$(basename $$target).sh" ]; then \
			echo "测试 $$(basename $$target)..."; \
			bash "$$(dirname $$target)/test_$$(basename $$target).sh"; \
		fi \
	done

# 帮助
help:
	@echo "可用的目标:"
	@echo "  all     - 编译所有程序"
	@echo "  clean   - 清理编译文件"
	@echo "  run     - 运行所有程序"
	@echo "  test    - 运行测试"
	@echo "  help    - 显示此帮助信息"

# 安装依赖（Ubuntu/Debian）
install-deps:
	sudo apt-get update
	sudo apt-get install -y g++ make valgrind

# 安装依赖（CentOS/RHEL）
install-deps-centos:
	sudo yum update
	sudo yum install -y gcc-c++ make valgrind

# 安装依赖（macOS）
install-deps-mac:
	brew install gcc make valgrind

# 检查代码风格
style-check:
	@echo "检查代码风格..."
	@for src in $(SRCS); do \
		echo "检查 $$src..."; \
		clang-format -i $$src; \
	done

# 内存检查
memcheck: all
	@echo "运行内存检查..."
	@for target in $(TARGETS); do \
		echo "检查 $$(basename $$target)..."; \
		valgrind --leak-check=full --show-leak-kinds=all $$target; \
	done

.PHONY: all clean run test help install-deps install-deps-centos install-deps-mac style-check memcheck

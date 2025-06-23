CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -O2

# 目标可执行文件
TARGET = nrndp

# 源文件目录
SRC_DIR = src

# 源文件
SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/matrix.cpp $(SRC_DIR)/assign.cpp $(SRC_DIR)/decay.cpp $(SRC_DIR)/isotope.cpp

# 生成的 .o 目标文件
OBJS = $(SRCS:.cpp=.o)

# 头文件路径
INCLUDES 	= -I$(SRC_DIR)

# 目标规则
all: $(TARGET)

# 生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# 编译 .cpp 文件为 .o
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# 清理
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)

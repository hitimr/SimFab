# Workspace
OUT_DIR = ./out
BUILD_DIR ?= ./out/build
DOC_NAME = Ex2_Hiti.pdf



$(shell mkdir -p $(OUT_DIR) $(BUILD_DIR) ./out/pdflatex ./out/images ./out/vtk)

# C++
INC_DIRS += ./viennals/include ./viennahrle/include
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CXXFLAGS ?= $(INC_FLAGS) -std=c++17 -O3 -fopenmp -g
CXX = g++



#all: task1 task2 run
all: task1 task2 task3

task1: src/task1.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) src/task1.cpp -o $(BUILD_DIR)/task1 $(LDFLAGS)
	$(BUILD_DIR)/task1


task2: src/task2.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) src/task2.cpp -o $(BUILD_DIR)/task2 $(LDFLAGS)
	$(BUILD_DIR)/task2

task3: src/task3.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) src/task3.cpp -o $(BUILD_DIR)/task3 $(LDFLAGS)
	$(BUILD_DIR)/task3


images:
	make task1
	make task2
	make task3
	cp -a ./out/images/. ./doc/res


run:
	$(BUILD_DIR)/task1
	$(BUILD_DIR)/task2

clean:
	rm -rf $(OUT_DIR)

.PHONY: clean run images
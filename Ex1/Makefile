# original by JOB VRANISH
# https://spin.atomicobject.com/2016/08/26/makefile-c-projects/


TARGET_EXEC ?= grid

OUT_DIR ?= ./out
BUILD_DIR ?= $(OUT_DIR)/build
SRC_DIRS ?= ./src

TEST_DIR ?= ./test
TEST_BUILD_DIR ?= $(OUT_DIR)/test

# Directories
$(shell mkdir -p $(BUILD_DIR))
$(shell mkdir -p $(TEST_BUILD_DIR))

# Sources
HEADERS := $(shell find $(SRC_DIRS) -name *.hpp -or -name *.h)
SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
TEST_SRCS := $(shell find $(TEST_DIR) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# Include
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -std=c++11 -Wall -Wextra -pedantic -O3 -g

all: $(BUILD_DIR)/$(TARGET_EXEC) run


# Project
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

test: test_vector3D test_sdf test_grid

# Unit Test
test_vector3D: $(OBJS) $(HEADERS) $(SRCS) $(TEST_SRCS)
	$(CXX) $(CPPFLAGS) $(SRC_DIRS)/vector3d.cpp -o $(TEST_BUILD_DIR)/$@ $(TEST_DIR)/$@.cpp $(LDFLAGS)
	$(TEST_BUILD_DIR)/$@

# Unit Test
test_sdf: $(OBJS) $(HEADERS) $(SRCS) $(TEST_SRCS)
	$(CXX) $(CPPFLAGS) $(SRC_DIRS)/sdf.cpp $(SRC_DIRS)/vector3d.cpp -o $(TEST_BUILD_DIR)/$@ $(TEST_DIR)/$@.cpp $(LDFLAGS)
	$(TEST_BUILD_DIR)/$@

# Unit Test
test_grid: $(OBJS) $(HEADERS) $(SRCS) $(TEST_SRCS)
	$(CXX) $(CPPFLAGS) $(SRC_DIRS)/grid.cpp $(SRC_DIRS)/vector3d.cpp -o $(TEST_BUILD_DIR)/$@ $(TEST_DIR)/$@.cpp $(LDFLAGS)
	$(TEST_BUILD_DIR)/$@

.PHONY: clean results run test

run:
#$(BUILD_DIR)/$(TARGET_EXEC) 20 10 0.01  Rectangle 5 5 8 8 rprp  out/grid.vtk	
#$(BUILD_DIR)/$(TARGET_EXEC) 4 4 0.1 Sphere 2 2 0 1 rrrr out/grid.vtk 3 2
#$(BUILD_DIR)/$(TARGET_EXEC) 4 4 0.1 Rectangle 1 1 3 3 rrrr out/grid.vtk 2 2
	$(BUILD_DIR)/$(TARGET_EXEC) 50 50 0.25 Sphere 25 25 0 10 rrrr "out/grid.vtk"
#$(BUILD_DIR)/$(TARGET_EXEC) 50 50 1 Rectangle 22.5 15 27.5 35  rrrr "out/grid.vtk"
	
#$(BUILD_DIR)/$(TARGET_EXEC) 4 4 0.1 Sphere 2 2 0 1 rrrr out/grid.vtk 3 2
#$(BUILD_DIR)/$(TARGET_EXEC) 4 4 0.01 Rectangle 1 1 2 2 pppp 

# generate grids for documentation
results:
	$(BUILD_DIR)/$(TARGET_EXEC) 10 10 0.1 Sphere 5 5 0 3 rrrr "results/1-sphere_center.vtk"
	$(BUILD_DIR)/$(TARGET_EXEC) 10 10 0.1 Rectangle 2 2 8 8 rrrr "results/1-rectangle_center.vtk"

	$(BUILD_DIR)/$(TARGET_EXEC) 10 10 0.1 Sphere 0 5 0 3 rrrr "results/1_sphere_left_rrrr.vtk"
	$(BUILD_DIR)/$(TARGET_EXEC) 10 10 0.1 Rectangle -4 2 4 8 rrrr "results/1_rectangle_left_rrrr.vtk"

	$(BUILD_DIR)/$(TARGET_EXEC) 10 10 0.1 Sphere 0 5 0 3 pppp "results/1_sphere_left_pppp.vtk"
	$(BUILD_DIR)/$(TARGET_EXEC) 10 10 0.1 Rectangle -4 2 4 8 pppp "results/1_rectangle_left_pppp.vtk"
	
	$(BUILD_DIR)/$(TARGET_EXEC) 10 10 0.1 Sphere 1 1.5 0 3 pppp "results/1_sphere_bottom_left_pppp.vtk"
	$(BUILD_DIR)/$(TARGET_EXEC) 10 10 0.1 Rectangle -2 -2 4 4 pppp "results/1_rectangle_bottom_left_pppp.vtk"
	
	$(BUILD_DIR)/$(TARGET_EXEC) 10 10 0.1 Sphere 1 1.5 0 3 rprp "results/1_sphere_bottom_left_rprp.vtk"
	$(BUILD_DIR)/$(TARGET_EXEC) 10 10 0.1 Rectangle -2 -2 4 4 rprp "results/1_rectangle_bottom_left_rprp.vtk"
	
	$(BUILD_DIR)/$(TARGET_EXEC) 10 10 2 Sphere 5 5 0 3 rrrr "results/1_sphere_center_coarse.vtk"
	$(BUILD_DIR)/$(TARGET_EXEC) 10 10 2 Rectangle 2 2 8 8 rrrr "results/1_rectangle_center_coarse.vtk"

	
	$(BUILD_DIR)/$(TARGET_EXEC) 10 10 0.5 Rectangle 2 2 8 8 rrrr "results/1_rectangle_center_medium_coarse.vtk"


clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) -r $(TEST_BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p

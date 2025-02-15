CXX := g++
CXXFLAGS := -Wall -Wextra -Wno-unused -Wdisabled-optimization -std=c++17
LDFLAGS :=

SRC_DIR := ../src
UTILS_DIR := $(SRC_DIR)/utils
INCLUDE_DIR := $(SRC_DIR)/include
OUT_DIR := ../out

# Find all .cpp files automatically
SOURCES := \
	$(wildcard $(SRC_DIR)/*.cpp) \
	$(wildcard $(UTILS_DIR)/*.cpp) \
	$(wildcard $(INCLUDE_DIR)/*.cpp)

# Include and library paths
INCLUDES := -I. -I$(INCLUDE_DIR)
LIBRARIES := -L.

# Look in all directories after the source cpp files
vpath %.cpp $(SRC_DIR) $(UTILS_DIR) $(INCLUDE_DIR)

# Generate object files inside `out/`, keeping the directory structure
OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(OUT_DIR)/%.o)

# Output binary in `out/`
TARGET := $(OUT_DIR)/client

.PHONY: all clean build

all: build $(TARGET)

# Link object files into the `client` executable inside `out/`
$(OUT_DIR)/client: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) $(LIBRARIES) -o $@

# Rules for compiling sources from different directories
$(OUT_DIR)/%.o: $(SRC_DIR)/%.cpp | build
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OUT_DIR)/utils/%.o: $(UTILS_DIR)/%.cpp | build
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OUT_DIR)/include/%.o: $(INCLUDE_DIR)/%.cpp | build
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

build:
	mkdir -p $(OUT_DIR) $(OUT_DIR)/utils $(OUT_DIR)/include

clean:
	rm -rf $(OUT_DIR)

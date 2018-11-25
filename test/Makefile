include ../config.mk

DEP_DIRS   = ../src
BUILD_DIR = build
INCLUDE   = ../src

TARGET_SRCS := $(addprefix $(BUILD_DIR)/, $(shell find -name \*.cpp))
OBJS    := $(TARGET_SRCS:.cpp=.o)
TARGETS := $(TARGET_SRCS:.cpp=)

DEP_SRCS := $(shell find $(DEP_DIRS) -name \*.cpp)
DEPS     := $(DEP_SRCS:.cpp=.o)

all: $(BUILD_DIR) $(TARGETS)

debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: all

$(BUILD_DIR)/%: $(OBJS) $(DEPS)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE) $^ -c -o $@

# Compile source files
$(SRC_DIR)/%.o: $(SRCS)
	$(MAKE) -C $(SRC_DIR)

# Create build directory if needed
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

.PHONY: clean

clean:
	rm -rf $(BUILD_DIR) ~*


include ../config.mk

SRCS := $(shell find ./ -name \*.cpp)
OBJS := $(SRCS:.cpp=.o)

all: $(OBJS)

debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: all

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

.PHONY: clean

clean:
	rm -rf *.o ~*


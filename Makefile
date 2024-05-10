CXX := g++

CXXFLAGS := -std=c++17 -Wall -Wextra

COMPILE_DIR := Compiler/Src
VM_DIR := VM/Src

EXECUTABLE := jackcompiler

SRCS := $(wildcard $(COMPILE_DIR)/*.cpp) \
        $(wildcard $(VM_DIR)/*.cpp) \
        Src/Compiler.cpp

OBJS := $(SRCS:.cpp=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXECUTABLE)
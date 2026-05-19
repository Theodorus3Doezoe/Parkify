# Makefile for Group project

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I.
DEBUGFLAGS = -g -O0
LDFLAGS =

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
TARGET = group_project
DEBUG_TARGET = group_project_debug

.PHONY: all clean run debug

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(DEBUG_TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(DEBUG_TARGET) $(OBJS)

run: $(TARGET)
	./$(TARGET)

debug: $(DEBUG_TARGET)
	./$(DEBUG_TARGET)

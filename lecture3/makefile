CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

MAIN_SRC = graph.cpp
MAIN_BIN = bin/graph

TEST_SRC = tester.cpp
TEST_BIN = bin/tester

BIN_DIR = bin

.PHONY: all build test clean

all: build test

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

build: $(BIN_DIR) $(MAIN_BIN)

$(MAIN_BIN): $(MAIN_SRC) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

test: $(BIN_DIR) $(TEST_BIN)

$(TEST_BIN): $(TEST_SRC) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -rf $(BIN_DIR)
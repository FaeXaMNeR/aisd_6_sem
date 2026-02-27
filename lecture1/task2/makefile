CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

MAIN_SRC = main.cpp
MAIN_BIN = bin/build
TEST_SRC = test_generator.cpp
TEST_BIN = bin/test

BIN_DIR = bin

.PHONY: all build test clean

all: build

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

build: $(BIN_DIR) $(MAIN_BIN)

$(MAIN_BIN): $(MAIN_SRC) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(MAIN_BIN) $(MAIN_SRC)

test: $(BIN_DIR) $(TEST_BIN)

$(TEST_BIN): $(TEST_SRC) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TEST_BIN) $(TEST_SRC)

clean:
	rm -rf $(BIN_DIR)

run-test: test
	./$(TEST_BIN)

run: build
	./$(MAIN_BIN)
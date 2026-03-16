CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

MAIN_SRC = main.cpp
MAIN_BIN = bin/build

BIN_DIR = bin

.PHONY: all build clean

all: build

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

build: $(BIN_DIR) $(MAIN_BIN)

$(MAIN_BIN): $(MAIN_SRC) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(MAIN_BIN) $(MAIN_SRC)

clean:
	rm -rf $(BIN_DIR)

run: build
	./$(MAIN_BIN)
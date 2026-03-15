CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra
SIZEFLAGS = -D_SIZE_ -std=c++14 -Wall -Wextra
SHUFFLEFLAGS = -D_SHUFFLE_ -std=c++14 -Wall -Wextra

MAIN_SRC = main.cpp
MAIN_BIN = bin/build
SIZE_BIN = bin/size
SHUFFLE_BIN = bin/shuffle
ARGS = 

BIN_DIR = bin

.PHONY: all build size shuffle clean run

all: build size shuffle

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

build: $(BIN_DIR) $(MAIN_BIN)

$(MAIN_BIN): $(MAIN_SRC) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

size: $(BIN_DIR) $(SIZE_BIN)

$(SIZE_BIN): $(MAIN_SRC) | $(BIN_DIR)
	$(CXX) $(SIZEFLAGS) -o $@ $<

shuffle: $(BIN_DIR) $(SHUFFLE_BIN)

$(SHUFFLE_BIN): $(MAIN_SRC) | $(BIN_DIR)
	$(CXX) $(SHUFFLEFLAGS) -o $@ $<

clean:
	rm -rf $(BIN_DIR)

run: build
	./$(MAIN_BIN) $(ARGS)

run-size: size
	./$(SIZE_BIN) $(ARGS)

run-shuffle: shuffle
	./$(SHUFFLE_BIN) $(ARGS)
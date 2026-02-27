#include <iostream>
#include <cstring>
#include <fstream>
#include <string>

const int PATH_MAX_LEN = 4096;
const int MAX_DIRS = 1000;
const int MAX_NAME_LEN = 256;

class DirStack {
private:
    char dirs[MAX_DIRS][MAX_NAME_LEN];
    int top;

public:
    DirStack() : top(-1) {}

    void push(const char* name) {
        if (top < MAX_DIRS - 1) {
            strncpy(dirs[++top], name, MAX_NAME_LEN - 1);
            dirs[top][MAX_NAME_LEN - 1] = '\0';
        }
    }

    void pop() {
        if (top >= 0) {
            top--;
        }
    }

    bool isEmpty() const {
        return top == -1;
    }

    const char* get(int index) const {
        if (index >= 0 && index <= top) {
            return dirs[index];
        }
        return nullptr;
    }

    int size() const {
        return top + 1;
    }

    void clear() {
        top = -1;
    }
};

void simplifyPath(const char* path) {
    DirStack stack;
    int len = strlen(path);
    int i = 0;

    if (path[0] != '/') {
        std::cout << "Error: Path must start with '/'" << std::endl;
        return;
    }

    while (i < len) {
        while (i < len && path[i] == '/') {
            i++;
        }

        if (i >= len) {
            break;
        }

        int start = i;
        while (i < len && path[i] != '/') {
            i++;
        }

        char component[MAX_NAME_LEN];
        int compLen = i - start;
        if (compLen >= MAX_NAME_LEN) {
            compLen = MAX_NAME_LEN - 1;
        }
        strncpy(component, path + start, compLen);
        component[compLen] = '\0';

        if (strcmp(component, ".") == 0) {
            continue;
        } else if (strcmp(component, "..") == 0) {
            if (stack.isEmpty()) {
                std::cout << "Going upper than the root directory is not possible." << std::endl;
                return;
            }
            stack.pop();
        } else {
            stack.push(component);
        }
    }

    char result[PATH_MAX_LEN];
    result[0] = '\0';

    if (stack.isEmpty()) {
        std::cout << "/" << std::endl;
        return;
    }

    for (int j = 0; j < stack.size(); j++) {
        strcat(result, "/");
        strcat(result, stack.get(j));
    }

    std::cout << result << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        std::ifstream file(argv[1]);

        if (!file) {
            std::cerr << "Error: cannot open file " << argv[1] << std::endl;
            return 1;
        }

        std::string line;

        while (std::getline(file, line)) {
            if (!line.empty()) {
                simplifyPath(line.c_str());
            }
        }
    } else {
        char path[PATH_MAX_LEN];

        std::cin >> path;

        simplifyPath(path);
    }
    
    return 0;
}
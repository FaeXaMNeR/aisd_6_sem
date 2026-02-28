#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>

const int MAX_URL_LEN = 20;
const int BUFFER_SIZE = MAX_URL_LEN + 1;

struct Node {
    char url[BUFFER_SIZE];
    Node* prev;
    Node* next;

    Node(const char* input_url) {
        strncpy(url, input_url, MAX_URL_LEN);
        url[MAX_URL_LEN] = '\0';
        prev = nullptr;
        next = nullptr;
    }
};

class Browser {
private:
    Node* head;      // (homepage)
    Node* current;

    void clearForwardHistory() {
        Node* temp = current->next;
        current->next = nullptr;
        
        while (temp != nullptr) {
            Node* next_node = temp->next;
            delete temp;
            temp = next_node;
        }
    }

public:
    Browser() {
        head = new Node("homepage");
        current = head;
    }

    ~Browser() {
        Node* temp = head;

        while (temp != nullptr) {
            Node* next_node = temp->next;
            delete temp;
            temp = next_node;
        }
    }

    void visit(const char* url) {
        Node* new_node = new Node(url);
        
        clearForwardHistory();

        current->next = new_node;
        new_node->prev = current;
        
        current = new_node;
    }

    void back(int n) {
        for (int i = 0; i < n; ++i) {
            if (current->prev != nullptr) {
                current = current->prev;
            } else {
                break;
            }
        }
    }

    void forward(int n) {
        for (int i = 0; i < n; ++i) {
            if (current->next != nullptr) {
                current = current->next;
            } else {
                break; 
            }
        }
    }

    void printCurrent() const {
        std::cout << current->url;
    }
};

// Ctrl + D для корректного завершения
int main(int argc, char* argv[]) {
    Browser browser;
    browser.printCurrent();
    std::cout << "\n";

    std::ifstream inFile;
    if (argc > 1) {
        inFile.open(argv[1]);
        if (!inFile) {
            std::cerr << "Error: cannot open file " << argv[1] << std::endl;
            return 1;
        }
    }
    std::istream& input = (argc > 1) ? inFile : std::cin;

    char command[20];
    char url[BUFFER_SIZE];
    int steps;

    while (input >> command) {
        if (strcmp(command, "visit") == 0) {
            input >> url;
            browser.visit(url);
        } else if (strcmp(command, "back") == 0) {
            input >> steps;
            browser.back(steps);
        } else if (strcmp(command, "forward") == 0) {
            input >> steps;
            browser.forward(steps);
        }

        browser.printCurrent();
        std::cout << "\n";
    }

    if (argc > 1) inFile.close();

    return 0;
}
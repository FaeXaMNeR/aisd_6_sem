#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <random>

std::string random_string(int min_len, int max_len) {
    static const char alphanum[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> len_dist(min_len, max_len);
    static std::uniform_int_distribution<> char_dist(0, sizeof(alphanum) - 2);

    int len = len_dist(gen);

    std::string s;

    for (int i = 0; i < len; ++i) {
        s += alphanum[char_dist(gen)];
    }

    return s;
}

int main(int argc, char* argv[]) {
    int num_tests = 10;

    if (argc > 1) {
        num_tests = std::atoi(argv[1]);
        if (num_tests <= 0) num_tests = 10;
    }

    std::srand(std::time(nullptr));

    for (int t = 0; t < num_tests; ++t) {
        std::string path = "/";
        int num_components = rand() % 5 + 1; // от 1 до 5 компонентов

        for (int i = 0; i < num_components; ++i) {
            int type = rand() % 3;
            std::string comp;
            if (type == 0) {
                comp = random_string(1, 8); 
            } else if (type == 1) {
                comp = ".";
            } else {
                comp = "..";
            }

            int num_slashes = rand() % 3 + 1;

            for (int s = 0; s < num_slashes; ++s) {
                path += '/';
            }
            
            path += comp;
        }

        // Случайное добавление завершающих слешей (с вероятностью 50%)
        if (rand() % 2 == 0) {
            int num_trail = rand() % 3 + 1; // 1..3 слеша в конце
            for (int s = 0; s < num_trail; ++s) path += '/';
        }

        std::cout << path << std::endl;
    }
    return 0;
}
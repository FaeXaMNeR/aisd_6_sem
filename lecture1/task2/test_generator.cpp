#include <iostream>
#include <string>
#include <random>

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> cmd_dist(0, 2);
    std::uniform_int_distribution<> step_dist(1, 3);
    std::uniform_int_distribution<> len_dist(1, 10);
    std::uniform_int_distribution<> char_dist(0, 25);

    const int COMMANDS = 10;

    for (int i = 0; i < COMMANDS; ++i) {
        int cmd = cmd_dist(gen);
        if (cmd == 0) {
            int len = len_dist(gen);
            std::string name;

            for (int j = 0; j < len; ++j) {
                name += static_cast<char>('a' + char_dist(gen));
            }

            std::cout << "visit " << name << ".com\n";
        } else if (cmd == 1) {
            std::cout << "back " << step_dist(gen) << "\n";
        } else {
            std::cout << "forward " << step_dist(gen) << "\n";
        }
    }

    return 0;
}
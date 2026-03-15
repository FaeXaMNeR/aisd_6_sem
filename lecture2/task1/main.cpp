#include <iostream>
#include <cstdlib>
#include <memory>
#include <ctime>
#include <random>
#include <chrono>
#include <algorithm>

void improvedBubbleSort(int arr[], int n) {
    if (n <= 1) return;

    int gap = n;
    bool swapped = true;

    while (gap > 1 || swapped) {
        gap = (gap * 10) / 13;
        if (gap < 1) gap = 1;

        swapped = false;

        // ->
        for (int i = 0; i < n - gap; ++i) {
            if (arr[i] > arr[i + gap]) {
                std::swap(arr[i], arr[i + gap]);
                swapped = true;
            }
        }

        // <-
        if (swapped) {
            for (int i = n - gap - 1; i >= 0; --i) {
                if (arr[i] > arr[i + gap]) {
                    std::swap(arr[i], arr[i + gap]);
                }
            }
        }
    }
}

void printVector(int *arr, int N) {
    for (int i = 0; i < N; i++) {
        std::cout << arr[i] << " ";
    }

    std::cout << "\n";
}

void shuffleVector(int *arr, int N, int p) {
    if (N <= 0 || p <= 0 || p > 100) return;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, N - 1);
    
    int numToShuffle = (N * p) / 100;
    if (numToShuffle < 2) return;
    
    int* index = new int[numToShuffle];
    for (int i = 0; i < numToShuffle; i++) {
        index[i] = dis(gen);
    }
    
    std::shuffle(index, index + numToShuffle, gen);
    
    for (int i = 0; i < numToShuffle; i++) {
        int j = dis(gen) % numToShuffle;
        std::swap(arr[index[i]], arr[index[j]]);
    }
    
    delete[] index;
}

int main(const int argc, const char *argv[]) {
    int N = 10;

    // N = argv[1], p = argv[2]
    if (argc > 1) N = std::stoi(argv[1]);

    int arr[N];

    std::srand(std::time(nullptr));

    for (int i = 0; i < N; i++) {
        arr[i] = rand() % 50 + 1;
    }

    #if !defined(_SIZE_) && !defined(_SHUFFLE_)
    std::cout << "Vector before sorting:\n";
    printVector(arr, N);
    std::cout << "\n";

    improvedBubbleSort(arr, N);
    std::cout << "Vector after sorting:\n";
    printVector(arr, N);
    #endif

    #ifdef _SIZE_
    auto start = std::chrono::high_resolution_clock::now();
    improvedBubbleSort(arr, N);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << duration.count() << "\n";
    #endif

    #ifdef _SHUFFLE_
    int p = 50; // shuffle_percentage
    if (argc > 2) p = std::stoi(argv[2]);

    improvedBubbleSort(arr, N);

    shuffleVector(arr, N, p);

    auto start = std::chrono::high_resolution_clock::now();
    improvedBubbleSort(arr, N);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << duration.count() << "\n";
    #endif

    return 0;
}
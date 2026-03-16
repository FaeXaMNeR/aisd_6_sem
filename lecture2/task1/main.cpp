#include <iostream>
#include <cstdlib>
#include <memory>
#include <ctime>
#include <random>
#include <chrono>
#include <algorithm>

template <typename T>
class MyVector {
private:
    T* data;
    size_t size;
    size_t capacity;

    void grow() {
        capacity = (capacity == 0) ? 1 : capacity * 2;
        reallocate(capacity);
    }

    void reallocate(size_t new_cap) {
        T* new_data = new T[new_cap];
        for (size_t i = 0; i < size; ++i) {
            new_data[i] = data[i];
        }

        delete[] data;
        data = new_data;
        capacity = new_cap;
    }

public:
    MyVector() : data(nullptr), size(0), capacity(0) {}

    MyVector(const MyVector& other) : data(nullptr), size(0), capacity(0) {
        *this = other;
    }

    ~MyVector() {
        delete[] data;
    }

    void push_back(const T& value) {
        if (size == capacity) {
            grow();
        }

        data[size++] = value;
    }

    
    T* begin() {
        return data;
    }

    T* end() {
        return data + size;
    }

    const T* begin() const {
        return data;
    }
    const T* end() const {
        return data + size;
    }

    size_t getSize() const {
        return size;
    }

    T& operator[](size_t idx) {
        return data[idx];
    }

    const T& operator[](size_t idx) const {
        return data[idx];
    }
};

template <typename T>
void improvedBubbleSort(MyVector<T> &vec) {
    if (vec.getSize() <= 1) return;

    int gap = static_cast<int>(vec.getSize());
    bool swapped = true;

    while (gap > 1 || swapped) {
        gap = (gap * 10) / 13;
        if (gap < 1) gap = 1;

        swapped = false;

        // ->
        for (int i = 0; i < static_cast<int>(vec.getSize()) - gap; ++i) {
            if (vec[i] > vec[i + gap]) {
                std::swap(vec[i], vec[i + gap]);
                swapped = true;
            }
        }

        // <-
        if (swapped) {
            bool reverse_swapped = false;

            for (int i = static_cast<int>(vec.getSize()) - gap - 1; i >= 0; --i) {
                if (vec[i] > vec[i + gap]) {
                    std::swap(vec[i], vec[i + gap]);
                    reverse_swapped = true;
                }
            }

            swapped = reverse_swapped;
        }
    }
}

template <typename T>
void printVector(MyVector<T> &arr) {
    for (size_t i = 0; i < arr.getSize(); i++) {
        std::cout << arr[i] << " ";
    }

    std::cout << "\n";
}

template <typename T>
void shuffleVector(MyVector<T> &arr, int p) {
    const int N = arr.getSize();
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

    MyVector<int> arr;

    std::srand(std::time(nullptr));

    for (int i = 0; i < N; i++) {
        arr.push_back(rand() % 50 + 1);
    }

    #if !defined(_SIZE_) && !defined(_SHUFFLE_)
    std::cout << "Vector before sorting:\n";
    printVector(arr);
    std::cout << "\n";

    improvedBubbleSort(arr);
    std::cout << "Vector after sorting:\n";
    printVector(arr);
    #endif

    #ifdef _SIZE_
    auto start = std::chrono::high_resolution_clock::now();
    improvedBubbleSort(arr);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << duration.count() << "\n";
    #endif

    #ifdef _SHUFFLE_
    int p = 50; // shuffle_percentage
    if (argc > 2) p = std::stoi(argv[2]);

    improvedBubbleSort(arr);

    shuffleVector(arr, p);

    auto start = std::chrono::high_resolution_clock::now();
    improvedBubbleSort(arr);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << p << " " << duration.count() << "\n";
    #endif

    return 0;
}
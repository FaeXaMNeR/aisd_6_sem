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
        size = other.size;
        capacity = other.capacity;
        data = new T[capacity];

        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
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

    T& operator=(const MyVector& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            capacity = other.capacity;
            data = new T[capacity];

            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }

        return *this;
    }
};

// 1. Классическая сортировка пузырьком
void bubbleSort(MyVector<int> &arr) {
    int n = arr.getSize();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// 2. Сортировка пузырьком (проверка на отсортированность)
void improvedBubbleSort(MyVector<int>& arr) {
    int n = arr.getSize();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        if (!swapped) break;
    }
}

// 3. Шейкерная сортировка
void cocktailShakerSort(MyVector<int>& arr) {
    int n = arr.getSize();
    int start = 0;
    int end = n - 1;
    bool swapped = true;

    while (swapped) {
        swapped = false;

        // ->
        for (int i = start; i < end; ++i) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }

        if (!swapped) break;

        swapped = false;
        end--;

        // <-
        for (int i = end; i > start; --i) {
            if (arr[i] < arr[i - 1]) {
                std::swap(arr[i], arr[i - 1]);
                swapped = true;
            }
        }
        start++;
    }
}

// 4. Сортировка расчёской
void combSort(MyVector<int>& arr) {
    int n = arr.getSize();
    int gap = n;
    const double shrink = 1.3;
    bool swapped = true;

    while (gap > 1 || swapped) {
        gap = std::floor(gap / shrink);
        if (gap < 1) gap = 1;

        swapped = false;

        for (int i = 0; i + gap < n; ++i) {
            if (arr[i] > arr[i + gap]) {
                std::swap(arr[i], arr[i + gap]);
                swapped = true;
            }
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

int main(int argc, char* argv[]) {
    std::srand(std::time(nullptr));

    int N = 10;
    
    if (argc > 1) N = std::stoi(argv[1]);

    using SortFunc = void(*)(MyVector<int>&);
    SortFunc sorts[4] = { bubbleSort, improvedBubbleSort, cocktailShakerSort, combSort };

#ifdef _SIZE_
    std::vector<int> sizes;
    for (int s = 10; s <= N; s *= 10) {
        sizes.push_back(s);
    }

    if (sizes.empty() || sizes.back() != N) {
        sizes.push_back(N);
    }

    for (int size : sizes) {
        MyVector<int> base;
        for (int i = 0; i < size; i++) {
            base.push_back(std::rand() % 1000);
        }

        long long times[4] = {0};
        for (int i = 0; i < 4; i++) {
            MyVector<int> copy = base;
            auto start = std::chrono::high_resolution_clock::now();
            sorts[i](copy);
            auto end = std::chrono::high_resolution_clock::now();
            times[i] = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        }

        std::cout << size;
        for (int i = 0; i < 4; i++) {
            std::cout << " " << times[i];
        }
        std::cout << "\n";
    }

#elif defined(_SHUFFLE_)
    if (N <= 0) N = 10000;

    MyVector<int> sorted;
    for (int i = 0; i < N; i++) {
        sorted.push_back(std::rand() % 1000);
    }
    improvedBubbleSort(sorted);

    for (int percent = 0; percent <= 100; percent += 10) {
        MyVector<int> shuffled = sorted;
        shuffleVector(shuffled, percent);

        long long times[4] = {0};
        for (int i = 0; i < 4; i++) {
            MyVector<int> copy = shuffled;
            auto start = std::chrono::high_resolution_clock::now();
            sorts[i](copy);
            auto end = std::chrono::high_resolution_clock::now();
            times[i] = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        }

        std::cout << percent;
        for (int i = 0; i < 4; i++) {
            std::cout << " " << times[i];
        }
        std::cout << "\n";
    }

#else
    MyVector<int> arr;
    for (int i = 0; i < N; i++) {
        arr.push_back(std::rand() % 50 + 1);
    }
    std::cout << "Vector before sorting:\n";
    printVector(arr);
    std::cout << "\n";

    improvedBubbleSort(arr);
    std::cout << "Vector after sorting:\n";
    printVector(arr);
#endif

    return 0;
}

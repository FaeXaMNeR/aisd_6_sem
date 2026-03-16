#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

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

    explicit MyVector(size_t n) : data(nullptr), size(0), capacity(0) {
        if (n > 0) {
            data = new T[n];
            size = n;
            capacity = n;
            
            for (size_t i = 0; i < size; ++i) {
                data[i] = T();
            }
        }
    }

    void push_back(const T& value) {
        if (size == capacity) {
            grow();
        }

        data[size++] = value;
    }

    bool isEmpty() const {
        return size == 0;
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

void countingSortByDigit(MyVector<int> &arr, int exp) {
    int n = arr.getSize();
    MyVector<int> output(n);
    int count[10] = {0};

    for (int i = 0; i < n; ++i) {
        int digit = (arr[i] / exp) % 10;
        count[digit]++;
    }

    for (int i = 1; i < 10; ++i) {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; --i) {
        int digit = (arr[i] / exp) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    for (int i = 0; i < n; ++i) {
        arr[i] = output[i];
    }
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

void radixSort(MyVector<int> &arr) {
    if (arr.isEmpty()) return;

    int maxVal = *std::max_element(arr.begin(), arr.end());

    for (int exp = 1; maxVal / exp > 0; exp *= 10) {
        countingSortByDigit(arr, exp);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) return 1;
    
    int mode = std::atoi(argv[1]);
    std::random_device rd;
    std::mt19937 gen(rd());

    if (mode == 1) {
        const size_t N_values[] = {10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};
        const size_t num_N = sizeof(N_values) / sizeof(N_values[0]);
        const int p = 50;
        const int maxVal = 999999;
        
        std::cout << "N,time_radix_ms,time_std_ms\n";
        
        for (size_t iN = 0; iN < num_N; ++iN) {
            size_t N = N_values[iN];
            
            MyVector<int> original(N);
            std::uniform_int_distribution<int> dist(0, maxVal);
            for (size_t i = 0; i < N; ++i) {
                original[i] = dist(gen);
            }
            
            MyVector<int> temp(N);
            for (size_t i = 0; i < N; ++i) temp[i] = original[i];
            shuffleVector(temp, p);
            
            MyVector<int> arr_radix(N);
            for (size_t i = 0; i < N; ++i) arr_radix[i] = temp[i];
            auto start = std::chrono::high_resolution_clock::now();
            radixSort(arr_radix);
            auto end = std::chrono::high_resolution_clock::now();
            auto time_radix = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            
            MyVector<int> arr_std(N);
            for (size_t i = 0; i < N; ++i) arr_std[i] = temp[i];
            start = std::chrono::high_resolution_clock::now();
            std::sort(arr_std.begin(), arr_std.end());
            end = std::chrono::high_resolution_clock::now();
            auto time_std = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            
            std::cout << N << "," << time_radix << "," << time_std << "\n";
        }
    }
    else if (mode == 2) {
        const int percents[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
        const size_t num_p = sizeof(percents) / sizeof(percents[0]);
        const size_t N = 100000;
        const int maxVal = 999999;
        
        std::cout << "p,time_radix_ms,time_std_ms\n";
        
        MyVector<int> original(N);
        std::uniform_int_distribution<int> dist(0, maxVal);
        for (size_t i = 0; i < N; ++i) {
            original[i] = dist(gen);
        }
        
        for (size_t ip = 0; ip < num_p; ++ip) {
            int p = percents[ip];
            
            MyVector<int> temp(N);
            for (size_t i = 0; i < N; ++i) temp[i] = original[i];
            shuffleVector(temp, p);
            
            MyVector<int> arr_radix(N);
            for (size_t i = 0; i < N; ++i) arr_radix[i] = temp[i];
            auto start = std::chrono::high_resolution_clock::now();
            radixSort(arr_radix);
            auto end = std::chrono::high_resolution_clock::now();
            auto time_radix = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            
            MyVector<int> arr_std(N);
            for (size_t i = 0; i < N; ++i) arr_std[i] = temp[i];
            start = std::chrono::high_resolution_clock::now();
            std::sort(arr_std.begin(), arr_std.end());
            end = std::chrono::high_resolution_clock::now();
            auto time_std = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            
            std::cout << p << "," << time_radix << "," << time_std << "\n";
        }
    }
    else if (mode == 3) {
        const int maxDigits[] = {1, 2, 3, 4, 5, 6, 7};
        const size_t num_d = sizeof(maxDigits) / sizeof(maxDigits[0]);
        const size_t N = 100000;
        const int p = 50;
        
        std::cout << "maxVal,time_radix_ms,time_std_ms\n";
        
        for (size_t id = 0; id < num_d; ++id) {
            int maxVal = 1;
            for (int k = 0; k < maxDigits[id]; ++k) maxVal *= 10;
            maxVal -= 1;
            
            MyVector<int> original(N);
            std::uniform_int_distribution<int> dist(0, maxVal);
            for (size_t i = 0; i < N; ++i) {
                original[i] = dist(gen);
            }
            
            MyVector<int> temp(N);
            for (size_t i = 0; i < N; ++i) temp[i] = original[i];
            shuffleVector(temp, p);
            
            MyVector<int> arr_radix(N);
            for (size_t i = 0; i < N; ++i) arr_radix[i] = temp[i];
            auto start = std::chrono::high_resolution_clock::now();
            radixSort(arr_radix);
            auto end = std::chrono::high_resolution_clock::now();
            auto time_radix = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            
            MyVector<int> arr_std(N);
            for (size_t i = 0; i < N; ++i) arr_std[i] = temp[i];
            start = std::chrono::high_resolution_clock::now();
            std::sort(arr_std.begin(), arr_std.end());
            end = std::chrono::high_resolution_clock::now();
            auto time_std = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            
            std::cout << maxVal << "," << time_radix << "," << time_std << "\n";
        }
    }
    
    return 0;
}
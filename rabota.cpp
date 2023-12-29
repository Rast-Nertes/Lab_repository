#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <thread>
#include <functional>
#include <typeinfo>

template <typename T>
void bubble_sort(std::vector<T>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

template <typename T>
void odd_even_sort(std::vector<T>& arr) {
    int n = arr.size();
    bool sorted = false;
    while (!sorted) {
        sorted = true;
        for (int i = 0; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
                sorted = false;
            }
        }
        for (int i = 1; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
                sorted = false;
            }
        }
    }
}

template <typename T>
void shell_sort(std::vector<T>& arr) {
    int n = arr.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; ++i) {
            T temp = arr[i];
            int j = i;
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
    }
}

template <typename T>
void quick_sort(std::vector<T>& arr) {
    if (arr.size() <= 1) return;

    T pivot = arr[arr.size() / 2];
    std::vector<T> left, middle, right;

    for (const auto& elem : arr) {
        if (elem < pivot) left.push_back(elem);
        else if (elem == pivot) middle.push_back(elem);
        else right.push_back(elem);
    }

    quick_sort(left);
    quick_sort(right);

    arr.clear();
    arr.insert(arr.end(), left.begin(), left.end());
    arr.insert(arr.end(), middle.begin(), middle.end());
    arr.insert(arr.end(), right.begin(), right.end());
}

template <typename T>
std::vector<T> generate_data(int size) {
    std::vector<T> data;
    data.reserve(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);
    for (int i = 0; i < size; ++i) {
        data.push_back(static_cast<T>(dis(gen)));
    }
    return data;
}

template <typename T>
double measure_time(void (*sort_function)(std::vector<T>&), std::vector<T>& arr) {
    auto start_time = std::chrono::high_resolution_clock::now();
    sort_function(arr);
    auto end_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end_time - start_time).count();
}

template <typename T>
void run_experiment(int size, const std::string& data_type, const std::string& implementation) {
    std::vector<T> data = generate_data<T>(size);

    std::cout << "\nExperiment - Size: " << size << ", Data Type: " << data_type << "\n";
    std::cout << "======================================\n";

    if (implementation == "sequential") {
        for (auto sort_function : {bubble_sort<T>, odd_even_sort<T>, shell_sort<T>, quick_sort<T>}) {
            auto sorted_data = data;
            double time_taken = measure_time(sort_function, sorted_data);
            std::cout << typeid(sort_function).name() << " (Sequential) - Size: " << size << ", Time: " << time_taken << " seconds\n";
        }
    } else if (implementation == "parallel") {
        for (auto sort_function : {bubble_sort<T>, odd_even_sort<T>, shell_sort<T>, quick_sort<T>}) {
            auto sorted_data = data;
            auto thread_sort = [&sorted_data, sort_function]() {
                sort_function(sorted_data);
            };

            std::thread t1(thread_sort);
            std::thread t2(thread_sort);
            std::thread t3(thread_sort);
            std::thread t4(thread_sort);

            auto start_time = std::chrono::high_resolution_clock::now();
            t1.join();
            t2.join();
            t3.join();
            t4.join();
            auto end_time = std::chrono::high_resolution_clock::now();
            double time_taken = std::chrono::duration<double>(end_time - start_time).count();

            std::cout << typeid(sort_function).name() << " (Parallel) - Size: " << size << ", Time: " << time_taken << " seconds\n";
        }
    }
}

int main() {
    std::vector<int> sizes = {10000, 20000, 30000, 40000, 50000};
    std::vector<std::string> data_types = {"int", "double"};

    for (int size : sizes) {
        for (const std::string& data_type : data_types) {
            run_experiment<int>(size, data_type, "sequential");
            run_experiment<int>(size, data_type, "parallel");
        }
    }

    return 0;
}

#ifndef BUBBLE_SORT_CPP
#define BUBBLE_SORT_CPP

#include "bubble_sort.hpp"
#include <algorithm>
#include <stdexcept>

namespace sorting {

    template<typename T>
    void bubbleSort(std::vector<T>& arr, size_t start, size_t end) {
        // Проверка корректности индексов
        if (start >= end) {
            return; // Пустой диапазон
        }
        
        if (end > arr.size()) {
            throw std::out_of_range("Конечный индекс выходит за пределы вектора");
        }
        
        // Классическая сортировка пузырьком без оптимизаций
        for (size_t i = start; i < end - 1; ++i) {
            for (size_t j = start; j < end - (i - start) - 1; ++j) {
                if (arr[j] > arr[j + 1]) {
                    std::swap(arr[j], arr[j + 1]);
                }
            }
        }
    }

} // namespace sorting

#endif // BUBBLE_SORT_CPP
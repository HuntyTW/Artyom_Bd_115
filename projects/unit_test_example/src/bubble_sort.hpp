#ifndef BUBBLE_SORT_HPP
#define BUBBLE_SORT_HPP

#include <vector>

namespace sorting {

    /**
     * @brief Сортировка пузырьком для части вектора
     * @param arr Вектор для сортировки
     * @param start Начальный индекс (включительно)
     * @param end Конечный индекс (исключительно)
     */
    template<typename T>
    void bubbleSort(std::vector<T>& arr, size_t start, size_t end);

} // namespace sorting

#include "bubble_sort.cpp"

#endif // BUBBLE_SORT_HPP
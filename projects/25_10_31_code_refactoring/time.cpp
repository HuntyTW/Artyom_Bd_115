#include "time.hpp"
#include "tools.hpp"
#include "generateGapSequence.hpp"
#include "shellSort.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>

static const int MAX_POWER = 10;
static const int MAX_Q_POWER = 10;

void biv::time::measureAndCompareSorting(int array[], int size, const int gaps[], int gapsCount)
{
    int* shellSortArray = new int[size];
    int* stdSortArray = new int[size];
    
    biv::tools::copyArray(array, shellSortArray, size);
    biv::tools::copyArray(array, stdSortArray, size);

    const auto shellStart = std::chrono::steady_clock::now();
    biv::sort::shellSort(shellSortArray, size, gaps, gapsCount);
    const auto shellEnd = std::chrono::steady_clock::now();
    
    const auto stdStart = std::chrono::steady_clock::now();
    std::sort(stdSortArray, stdSortArray + size);
    const auto stdEnd = std::chrono::steady_clock::now();

    const std::chrono::duration<double> shellTime = shellEnd - shellStart;
    const std::chrono::duration<double> stdTime = stdEnd - stdStart;

    std::cout << "Время сортировки Шелла: " << shellTime.count() << " секунд\n";
    std::cout << "Время стандартной сортировки: " << stdTime.count() << " секунд\n";

    if (biv::tools::areArraysEqual(shellSortArray, stdSortArray, size))
    {
        std::cout << "Результаты сортировок идентичны\n";
    }
    else
    {
        std::cout << "ОШИБКА: Результаты сортировок не совпадают!\n";
    }
    
    std::cout << "----------------------------------------\n\n";

    delete[] shellSortArray;
    delete[] stdSortArray;
}
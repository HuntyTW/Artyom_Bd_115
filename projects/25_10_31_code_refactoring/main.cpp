#include "time.hpp"
#include "tools.hpp"
#include "generateGapSequence.hpp"
#include "shellSort.hpp"

#include <cmath>
#include <iostream>

static const int MAX_POWER = 10;
static const int MAX_Q_POWER = 10;
static const int MIN_SIZE_POWER = 3;
static const int MAX_SIZE_POWER = 6;

void shellSort(int array[], int size, const int gaps[], int gapsCount);
void measureAndCompareSorting(int array[], int size, const int gaps[], int gapsCount);
void fillArrayWithRandomNumbers(int array[], int size);
void generateGapSequence(int gaps[], int& gapsCount);
bool areArraysEqual(const int arr1[], const int arr2[], int size);
void copyArray(const int source[], int destination[], int size);

int main()
{
    int gaps[MAX_POWER * MAX_Q_POWER];
    int gapsCount = 0;
    
    biv::shellGaps::generateGapSequence(gaps, gapsCount);

    std::cout << "Программа сравнивает время работы сортировки Шелла и стандартной сортировки\n";
    std::cout << "для массивов разного размера (от 10^3 до 10^6 элементов).\n\n";

    for (int power = MIN_SIZE_POWER; power <= MAX_SIZE_POWER; ++power)
    {
        const int arraySize = static_cast<int>(std::pow(10, power));
        int* array = new int[arraySize];
        
        biv::shellGaps::fillArrayWithRandomNumbers(array, arraySize);
        
        std::cout << "Размер массива: 10^" << power << " (" << arraySize << " элементов)\n";
        biv::time::measureAndCompareSorting(array, arraySize, gaps, gapsCount);
        
        delete[] array;
    }

    return 0;
}
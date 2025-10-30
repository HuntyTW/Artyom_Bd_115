#include "shellSort.hpp"

#include <iostream>

void biv::sort::shellSort(int array[], int size, const int gaps[], int gapsCount)
{
    for (int gapIndex = gapsCount - 1; gapIndex >= 0; --gapIndex)
    {
        const int gap = gaps[gapIndex];
        
        if (gap > size / 2)
        {
            continue;
        }
        
        for (int currentIndex = gap; currentIndex < size; ++currentIndex)
        {
            const int key = array[currentIndex];
            int compareIndex = currentIndex - gap;
            
            while (compareIndex >= 0 && array[compareIndex] > key)
            {
                array[compareIndex + gap] = array[compareIndex];
                compareIndex -= gap;
            }
            
            array[compareIndex + gap] = key;
        }
    }
}

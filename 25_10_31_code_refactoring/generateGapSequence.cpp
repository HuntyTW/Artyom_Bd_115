#include "generateGapSequence.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>

static const int MAX_POWER = 10;
static const int MAX_Q_POWER = 10;

void biv::shellGaps::generateGapSequence(int gaps[], int& gapsCount)
{
    for (int p = 0; p < MAX_POWER; ++p)
    {
        for (int q = 0; q < MAX_Q_POWER; ++q)
        {
            gaps[gapsCount] = static_cast<int>(std::pow(2, p) * std::pow(3, q));
            ++gapsCount;
        }
    }

    std::sort(gaps, gaps + gapsCount);
}

void biv::shellGaps::fillArrayWithRandomNumbers(int array[], int size)
{
    std::mt19937 generator(std::random_device{}());
    
    for (int i = 0; i < size; ++i)
    {
        array[i] = generator();
    }
}
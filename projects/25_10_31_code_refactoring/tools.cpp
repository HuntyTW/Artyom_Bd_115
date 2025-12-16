#include "tools.hpp"

#include <iostream>

bool biv::tools::areArraysEqual(const int arr1[], const int arr2[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        if (arr1[i] != arr2[i])
        {
            return false;
        }
    }
    
    return true;
}

void biv::tools::copyArray(const int source[], int destination[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        destination[i] = source[i];
    }
}
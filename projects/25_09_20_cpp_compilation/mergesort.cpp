#include "mergesort.hpp"

#include <iostream>
#include <vector>

using namespace std;

void biv::sortings::mergesort(vector<int> &a, size_t start, size_t end){
	int m = 10000;
    if (end - start < 2)
        return;
    if (end - start == 2)
        {
            if (a[start] > a[start + 1])
                swap(a[start], a[start+1]);
            return;
        }
    mergesort(a, start, start+(end-start)/2);
    mergesort(a, start+(end-start)/2, end);
    vector<int> b;
    size_t b1 = start;
    size_t el = start + (end - start) / 2;
    size_t b2 = el;
    while (b.size() < end - start)
    {
        if (b1 >= el || ( b2 < end && a[b2] <= a[b1]))
        {
            b.push_back(a[b2]);
            ++b2;
        }
        else
        {
            b.push_back(a[b1]);
            ++b1;
        }
    }
    for (size_t i = start; i < end; i++)
        a[i] = b[i-start];
}
#include "mergevstav.hpp"

#include <iostream>
#include <vector>

using namespace std;

void biv::sortings::mergevstav(vector<int> &a, size_t start, size_t end){
	int m = 10000;
    int n = a.size();
    if (end - start < 2)
        return;
    if (end - start == m)
        {
            for (int i = 1; i < n ; ++ i ) {
                int j = i ;
                while ( j > 0 && a [ j - 1] > a [ j ]) {
                    swap ( a[j-1], a[j]);
                    j--;
                }
            }
            return;
        }
    mergevstav(a, start, start+(end-start)/2);
    mergevstav(a, start+(end-start)/2, end);
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

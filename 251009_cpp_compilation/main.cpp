#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

using namespace std;
using namespace chrono;

const int m = 10000;
void mergevstav(vector<int> &a, size_t start, size_t end)
{
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

void mergesort(vector<int> &a, size_t start, size_t end)
{
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

int main()
{
    vector<int> v;
    for (int i = 0; i < pow(10, 3); ++i)
        v.push_back(i);
    for (size_t i = 0; i < v.size(); i++)
        swap(v[i], v[rand() % (v.size() - i) + i]);
    auto start = steady_clock :: now ();
    mergesort(v, 0, v.size());
    auto end = steady_clock :: now () ;
    duration < double > elapsed_seconds = end - start ;
    cout << " elapsed time : " << elapsed_seconds . count () << "s\n";
    auto start_1 = steady_clock :: now ();
    mergevstav(v, 0, v.size());
    auto end_1 = steady_clock :: now () ;
    duration < double > elapsed_seconds_1 = end_1 - start_1 ;
    cout << " elapsed time : " << elapsed_seconds_1 . count () << "s\n";
}
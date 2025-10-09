#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include "mergesort.hpp"
#include "mergevstav.hpp"

using namespace std;
using namespace chrono;

int main()
{
	int m = 10000;
    vector<int> v;
    for (int i = 0; i < pow(10, 3); ++i)
        v.push_back(i);
    for (size_t i = 0; i < v.size(); i++)
        swap(v[i], v[rand() % (v.size() - i) + i]);
    auto start = steady_clock :: now ();
    biv::sortings::mergesort(v, 0, v.size());
    auto end = steady_clock :: now () ;
    duration < double > elapsed_seconds = end - start ;
    cout << " elapsed time : " << elapsed_seconds . count () << "s\n";
    auto start_1 = steady_clock :: now ();
    biv::sortings::mergevstav(v, 0, v.size());
    auto end_1 = steady_clock :: now () ;
    duration < double > elapsed_seconds_1 = end_1 - start_1 ;
    cout << " elapsed time : " << elapsed_seconds_1 . count () << "s\n";
}
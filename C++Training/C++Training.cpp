// C++Training.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "MikaLibrary.h"
#include "Union-Find.h"
#include <iostream>

std::ostream& operator<<(std::ostream& stream, mikalib::vector& other)
{
    stream << other.X << ", " << other.Y << ", " << other.Z;
    return stream;
}

int main()
{
    mikalib::percolation_test::run_test();
    std::cin.get();
}



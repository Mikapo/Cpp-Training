// C++Training.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "MikaLibrary.h"
#include <array>
#include <iostream>
#include <string>
#include <vector>

std::ostream& operator<<(std::ostream& stream, mikalib::vector& other)
{
    stream << other.X << ", " << other.Y << ", " << other.Z;
    return stream;
}

int main()
{

    mikalib::dynamic_array<int> array = {1, 2, 3, 4};
    mikalib::dynamic_array<int> second_array = {1, 3, 4};

    second_array = std::move(array);

    std::cin.get();
}
// C++Training.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <ctime>

#include "MikaLibrary.h"
#include <string>

std::ostream& operator<< (std::ostream& stream, mikalib::vector& other)
{
	stream << other.X << ", " << other.Y << ", " << other.Z;
	return stream;
}

int main()
{
	mikalib::array<char> new_array;
	new_array.reserve(2);
	new_array.fill(3);
	new_array.reserve(4);
	
	for (int i = 0; i < new_array.get_size(); i++)
		std::cout << (int)new_array[i] << std::endl;

	new_array.delete_array();

	std::cin.get();
}

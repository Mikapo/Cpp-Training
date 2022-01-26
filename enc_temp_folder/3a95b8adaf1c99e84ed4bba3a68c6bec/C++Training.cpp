#include <iostream>

int main() 
{ 
	char* ptr = new char[20];
    for (int i = 0; i < 20; i++)
        ptr[i] = i;

    std::cin.get();
}
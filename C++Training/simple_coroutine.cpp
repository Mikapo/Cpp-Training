#include "simple_coroutine.h"

void simple_coroutine::resume_coroutine()
{
	
	
}

void simple_coroutine::run_async(int number)
{
	return_value = std::async(std::launch::async, async_function, number);
}

int simple_coroutine::async_function(int number)
{
	std::this_thread::sleep_for(std::chrono::seconds(10));
	int new_number = number + 1;

	std::cout << "value is: " << new_number;
	return new_number;

}

std::future<int> simple_coroutine::Coroutine()
{
	return std::future<int>();
}


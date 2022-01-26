#pragma once

#include <array>
#include <functional>
#include <iostream>

class function_tests
{
public:

    function_tests() = delete;

    template <typename return_type, typename... argtypes>
    static bool test(
        return_type (*function)(argtypes...), return_type expected_return, argtypes... function_args)
    {
        return_type test_return = function(function_args...);
        return print_result(test_return, expected_return, function_args...);
    }

    template <typename object_type, typename return_type, typename... argtypes>
    static bool test(
        return_type(object_type::* function)(argtypes...), object_type* object, return_type expected_return,
        argtypes... function_args)
    {
        return_type test_return = (object->*function)(function_args...);
        return print_result(test_return, expected_return, function_args...);
    }

private:
    template <typename return_type, typename... argtypes>
    static bool print_result(return_type test_return, return_type expected_return, argtypes... function_args)
    {
        bool success = test_return == expected_return;

        if (success)
        {
            std::cout << "TEST " << ++test_amount << ": SUCCESS!" << std::endl;
        }
        else
        {
            std::cout << "TEST " << ++test_amount << ": FAILURE!" << std::endl;
        }

        if (sizeof...(function_args) != 0)
        {
            std::cout << "arguments:       ";
            std::cout << "(";
            ((std::cout << function_args), ...);
            std::cout << ")";
        }

        std::cout << std::endl;
        std::cout << "test return:     " << test_return << std::endl;
        std::cout << "expected return: " << expected_return;
        std::cout << std::endl;
        std::cout << std::endl;
        return success;
    }

    static int test_amount;
};

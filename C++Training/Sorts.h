#pragma once
#include "MikaLibrary.h"

namespace mikalib
{

    template <typename T, int size>
    static void selection_sort(mikalib::array<T, size>& input, bool (*greater_than)(T a, T b))
    {
        for (int i = 0; i < size; i++)
        {
            size_t smallest_value = i;
            for (int a = i + 1; a < size; a++)
            {
                if (greater_than(input[smallest_value], input[a]))
                    smallest_value = a;
            }
            swap<T, size>(input, i, smallest_value);
        }
    }

    template <typename T, int size>
    static void insertion_sort(mikalib::array<T, size>& input, bool (*greater_than)(T a, T b))
    {
        for (int i = 0; i < size; i++)
            for (int a = i; a > 0; a--)
            {
                if (!greater_than(input[a], input[a - 1]))
                {
                    swap(input, a, a - 1);
                }
                else
                    break;
            }
    }

    template <typename T, int size>
    static void swap(mikalib::array<T, size>& input, size_t index_a, size_t index_b)
    {
        T temp_value = input[index_a];
        input[index_a] = input[index_b];
        input[index_b] = temp_value;
    }
} // namespace mikalib

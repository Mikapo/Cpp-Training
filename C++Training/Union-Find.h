#pragma once

#include <algorithm>
#include <iostream>
#include <random>
#include <time.h>
#include <vector>

namespace mikalib
{
    class union_find
    {
    public:
        union_find(int size);

        void add_union(int a, int b);
        bool is_connected(int a, int b) { return find_root(a) == find_root(b); }

    private:
        int find_root(int value);

        int size = 0;
        std::vector<int> tree;
        std::vector<int> tree_sizes;
    };

    class percolation_test
    {
    public:
        percolation_test(int width, int height);

        void add_random_tile();
        void print_tiles();

        bool does_path_exist() { return union_data.is_connected(size(), size() + 1); }
        int size() { return m_height * m_width; }

        static void run_test();
        static void run_multiple_test(int amount, int width, int height);
        static double run_single_test(int width, int height, bool print_tiles = true);

    private:
        void add_tile_to_union(int tile);
        static void worker_thread(std::vector<double>& output, int amount, int width, int height);

        int m_width, m_height;
        int current_tile_amount = 0;
        std::vector<int> tile_order;
        std::vector<bool> tiles;
        std::vector<int> rows;
        union_find union_data;
    };

} // namespace mikalib

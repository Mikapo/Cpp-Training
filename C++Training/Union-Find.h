#pragma once

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <time.h>

namespace mikalib
{
    class union_find
    {
    public:
        union_find(int size)
        {
            tree.reserve(size);
            tree_sizes.reserve(size);

            for (int i = 0; i < size; i++)
            {
                tree.push_back(i);
                tree_sizes.push_back(1);
            }
        }

        void add_union(int a, int b)
        {
            int i = find_root(a);
            int j = find_root(b);
            tree[i] = j;
        }

        bool is_connected(int a, int b) { return find_root(a) == find_root(b); }

    private:
        int find_root(int value)
        {
            int root = tree[value];
            while (root != tree[root])
                root = tree[root];
            return root;
        }

        int size = 0;
        std::vector<int> tree;
        std::vector<int> tree_sizes;
    };

    class percolation_test
    {
    public:
        percolation_test(int width, int height) : union_data(width * height + 2)
        {
            m_width = width;
            m_height = height;

            tiles.reserve(size());
            tile_order.reserve(size());
            rows.reserve(size());

            for (int i = 0; i < size(); i++)
            {
                tiles.push_back(false);
                tile_order.push_back(i);
            }

            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(tile_order.begin(), tile_order.end(), g);

            for (int i = 0; i < width; i++)
            {
                union_data.add_union(i, size());
            }
            for (int i = 0; i < width; i++)
            {
                union_data.add_union((width * height) - i - 1, size() + 1);
            }

            for (int a = 0; a < height; a++)
            {
                for (int b = 0; b < width; b++)
                {
                    rows.push_back(a);
                }
            }
        }

        void add_random_tile()
        {
            tiles[tile_order[current_tile_amount]] = true;
            add_tile_to_union(tile_order[current_tile_amount]);
            ++current_tile_amount;
        }

        bool does_path_exist() { return union_data.is_connected(size(), size() + 1); }

        int size() { return m_height * m_width; }
        void print_tiles()
        {
            for (int a = 0; a < m_height; a++)
            {
                for (int b = 0; b < m_width; b++)
                {
                    if (tiles[b + m_width * a])
                        std::cout << "o ";

                    else
                        std::cout << "x ";
                }
                std::cout << std::endl;
            }
        }

        static void run_test()
        {
            std::string restart = "r";

            while (restart == "r")
            {
                int width;
                int height;

                std::cout << "input graph width:" << std::endl;
                std::cin >> width;
                std::cout << std::endl;
                std::cout << "Input graph height: " << std::endl;
                std::cin >> height;
                std::cout << std::endl;

                std::cout << "creating test object..." << std::endl;
                percolation_test test(width, height);

                int tries = 0;
                std::cout << "test object created" << std::endl;
                std::cout << std::endl;
                while (!test.does_path_exist())
                {
                    test.add_random_tile();
                    tries++;
                }
               
                test.print_tiles();
                std::cout << std::endl;
                std::cout << "took " << tries << " tries to make path" << std::endl;

                std::cout << std::endl;
                std::cout << "write r to try again" << std::endl;
                std::cin >> restart;
                std::cout << std::endl;
            }
        }

    private:
        void add_tile_to_union(int tile)
        {
            if (tile + 1 < size() - 1 && rows[tile] == rows[tile + 1] && tiles[tile + 1])
                union_data.add_union(tile, tile + 1);

            if (tile - 1 > -1 && rows[tile] == rows[tile - 1] && tiles[tile - 1])
                union_data.add_union(tile, tile - 1);

            if (tile + m_width < size() - 1 && tiles[tile + m_width])
                union_data.add_union(tile, tile + m_width);

            if (tile - m_width > -1 && tiles[tile - m_width])
                union_data.add_union(tile, tile - m_width);
        }

        static void run_single_test(int width, int height) 
        {

        }

        int m_width, m_height;
        int current_tile_amount = 0;
        std::vector<int> tile_order;
        std::vector<bool> tiles;
        std::vector<int> rows;
        union_find union_data;
    };

} // namespace mikalib

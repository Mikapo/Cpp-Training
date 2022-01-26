#include "Union-Find.h"

#include <future>

mikalib::union_find::union_find(int size)
{
    tree.reserve(size);
    tree_sizes.reserve(size);

    for (int i = 0; i < size; i++)
    {
        tree.push_back(i);
        tree_sizes.push_back(1);
    }
}

void mikalib::union_find::add_union(int a, int b)
{
    int i = find_root(a);
    int j = find_root(b);
    tree[i] = j;
}

int mikalib::union_find::find_root(int value)
{
    int root = tree[value];
    while (root != tree[root])
        root = tree[root];
    return root;
}

mikalib::percolation_test::percolation_test(int width, int height) : union_data(width * height + 2)
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

void mikalib::percolation_test::add_random_tile()
{
    tiles[tile_order[current_tile_amount]] = true;
    add_tile_to_union(tile_order[current_tile_amount]);
    ++current_tile_amount;
}

void mikalib::percolation_test::print_tiles()
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

void mikalib::percolation_test::add_tile_to_union(int tile)
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

double mikalib::percolation_test::run_single_test(int width, int height, bool print_tiles)
{
    if (print_tiles)
        std::cout << "creating test object..." << std::endl;
    percolation_test test(width, height);

    double tries = 0;

    if (print_tiles)
    {
        std::cout << "test object created" << std::endl;
        std::cout << std::endl;
    }

    while (!test.does_path_exist())
    {
        test.add_random_tile();
        tries++;
    }

    if (print_tiles)
        test.print_tiles();

    if (print_tiles)
    {
        std::cout << std::endl;
        std::cout << "took " << tries << " tries to make path" << std::endl;
    }

    return tries;
}

void mikalib::percolation_test::run_multiple_test(int amount, int width, int height)
{
    std::vector<double> results;
    std::vector<std::thread> threads;
    results.reserve(amount);

    for (int i = 0; i < 8; i++)
    {
        std::thread thread = std::thread(worker_thread, std::ref(results), amount / 8, width, height);
        threads.push_back(std::move(thread));
    }

    for (int i = 0; i < 8; i++)
        threads[i].join();

   double sum = 0;
    for (double value : results)
        sum += value;
   
    std::cout << "average tries that it took to find path is: " << sum / results.size() << std::endl;
}

static std::mutex lock_mutex;
void mikalib::percolation_test::worker_thread(std::vector<double>& output, int amount, int width, int height)
{
    for (int i = 0; i < amount; i++)
    {
        double test_result = run_single_test(width, height, false);
        test_result = test_result / (width * height);
        std::lock_guard<std::mutex> lock(lock_mutex);
        output.push_back(test_result);
        std::cout << "test: " << output.size() << std::endl;
    }
}

void mikalib::percolation_test::run_test()
{
    std::string restart = "r";

    while (restart == "r")
    {
        int width;
        int height;
        std::string single;

        std::cout << "input graph width:" << std::endl;
        std::cin >> width;
        std::cout << std::endl;
        std::cout << "Input graph height: " << std::endl;
        std::cin >> height;
        std::cout << std::endl;
        std::cout << "Input s for single test and m for multiple tests: " << std::endl;
        std::cin >> single;
        std::cout << std::endl;
        if (single == "s")
        {
            run_single_test(width, height);
        }
        else if (single == "m")
        {
            int test_amount;
            std::cout << "Input amount of tests you want to run:" << std::endl;
            std::cin >> test_amount;
            run_multiple_test(test_amount, width, height);
        }

        std::cout << std::endl;
        std::cout << "Input r to try again" << std::endl;
        std::cin >> restart;
        std::cout << std::endl;
    }
}

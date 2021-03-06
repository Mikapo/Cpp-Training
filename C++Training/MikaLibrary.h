#pragma once

#include <initializer_list>
#include <iostream>
#include <utility>

#define Debug 1

#if Debug
#define CHECK_FOR_VALID_INDEX(index, array_size)                                                                       \
    if (index > array_size)                                                                                            \
    {                                                                                                                  \
        std::cout << "tried to access index: " << index << " on array that is size of " << array_size;                 \
        __debugbreak();                                                                                                \
    }                                                                                                                  \
    (void)0
#else
#define CHECK_FOR_VALID_INDEX(index, array_size)
#endif

namespace mikalib
{
    // used to test other data structures
    struct test
    {
        test() { m_data = new int[5]; }
        test(const test& other) { m_data = new int[5]; }
        test(test&& other) noexcept
        {
            m_data = other.m_data;
            other.m_data = nullptr;
        }

        ~test() { delete[] m_data; }

        test& operator=(const test& other)
        {
            delete[] m_data;
            m_data = other.m_data;
            return *this;
        }

    private:
        int* m_data;
    };

    struct vector
    {
        float X, Y, Z;
        vector() : X(0), Y(0), Z(0) {}

        explicit vector(float X, float Y, float Z) : X(X), Y(Y), Z(Z) {}
        vector operator+(const vector& other) const { return vector(X + other.X, Y + other.Y, Z + other.Z); }

        vector operator*(float other) const { return vector(X * other, Y * other, Z * other); }

        vector operator/(float other) const { return vector(X / other, Y / other, Z / other); }

        double size() const;
        void normalize();
    };

    template <typename T, size_t array_size>
    class array
    {
    public:
        array() {}

        array(const std::initializer_list<T>& data)
        {
            CHECK_FOR_VALID_INDEX(data.size(), array_size);
            for (size_t i = 0; i < data.size(); i++)
                m_data[i] = *(data.begin() + i);
        }

        T& operator[](size_t index)
        {
            CHECK_FOR_VALID_INDEX(index, array_size);
            return m_data[index];
        }
        const T& operator[](size_t index) const
        {
            CHECK_FOR_VALID_INDEX(index, array_size);
            return m_data[index];
        }

        void fill(const T& value)
        {
            for (size_t i = 0; i < array_size; i++)
                m_data[i] = value;
        }

        void print_array() const
        {
            for (int i = 0; i < size(); i++)
                std::cout << m_data[i] << " ";
        }

        constexpr int size() const { return array_size; };
        T* data() { return &m_data; };

    private:
        T m_data[array_size];
    };

    template <typename dynamic_array>
    class dynamic_array_iterator
    {
    public:
        using valuetype = typename dynamic_array::valuetype;

        dynamic_array_iterator(valuetype* ptr) : m_ptr(ptr) {}

        dynamic_array_iterator& operator++()
        {
            m_ptr++;
            return *this;
        }
        dynamic_array_iterator& operator++(int)
        {
            dynamic_array_iterator temp = *this;
            ++m_ptr;
            return temp;
        }
        dynamic_array_iterator& operator--()
        {
            m_ptr--;
            return *this;
        }
        dynamic_array_iterator& operator--(int)
        {
            dynamic_array_iterator temp = *this;
            --m_ptr;
            return temp;
        }

        valuetype& operator[](size_t index) { return m_ptr[index]; }
        valuetype* operator->() { return m_ptr; }
        valuetype& operator*() { return *m_ptr; }

        bool operator==(const dynamic_array_iterator& other) { return m_ptr == other.m_ptr; }
        bool operator!=(const dynamic_array_iterator& other) { return m_ptr != other.m_ptr; }

    private:
        valuetype* m_ptr;
    };

    template <typename T>
    class dynamic_array
    {
    public:
        using valuetype = T;
        using iterator = dynamic_array_iterator<dynamic_array<T>>;

        dynamic_array() : m_data(nullptr), m_size(0), m_capacity(0) {}
        dynamic_array(std::initializer_list<T> data)
        {
            reserve(data.size());

            for (size_t i = 0; i < data.size(); i++)
            {
                add(*(data.begin() + i));
            }
        }
        explicit dynamic_array(const dynamic_array& other)
        {
            reserve(other.m_capacity);

            for (size_t i = 0; i < other.m_size; i++)
            {
                push_back(other[i]);
            }
        }
        explicit dynamic_array(const dynamic_array&& other)
            : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity)
        {
            other.m_data = nullptr;
            other.m_size = 0;
            other.m_capacity = 0;
        }

        ~dynamic_array()
        {
            clear();
            ::operator delete(m_data, sizeof(T) * m_capacity);
        }

        T& operator[](size_t index)
        {
            CHECK_FOR_VALID_INDEX(index, m_size);
            return m_data[index];
        }
        const T& operator[](size_t index) const
        {
            CHECK_FOR_VALID_INDEX(index, m_size);
            return m_data[index];
        }
        dynamic_array& operator=(const dynamic_array& other)
        {
            delete_memory();

            reserve(other.m_size);
            for (size_t i = 0; i < other.m_size; i++)
            {
                push_back(other[i]);
            }

            return *this;
        }
        dynamic_array& operator=(dynamic_array&& other)
        {
            delete_memory();

            m_data = other.m_data;
            m_size = other.m_size;
            m_capacity = other.m_capacity;

            other.m_data = nullptr;
            other.m_size = 0;
            other.m_capacity = 0;

            return *this;
        }

        template <typename... argtypes>
        void init(size_t size, argtypes... args)
        {
            delete_memory();
            reserve(size);

            for (size_t i = 0; i < m_capacity; i++)
                emplace(args...);
        }

        // changes capacity of array
        void reserve(size_t size)
        {
            T* new_data = (T*)::operator new(sizeof(T) * size);
            m_capacity = size;

            if (m_capacity < m_size)
                m_size = size;

            for (size_t i = 0; i < m_size; i++)
                new (&new_data[i]) T(std::move(m_data[i]));

            for (size_t i = 0; i < m_size; i++)
                m_data[i].~T();

            ::operator delete(m_data, sizeof(T) * m_capacity);
            m_data = new_data;
        }

        void add(const T& value)
        {
            if (m_size >= m_capacity)
            {
                reserve(m_size + 1);
            }

            m_data[m_size] = value;
            ++m_size;
        }

        void add(const T&& value)
        {
            if (m_size >= m_capacity)
            {
                reserve(m_size + 1);
            }

            m_data[m_size] = std::move(value);
            ++m_size;
        }

        template <typename... argtypes>
        void emplace(argtypes&&... args)
        {
            if (m_size >= m_capacity)
            {
                reserve(m_size + 1);
            }

            new (&m_data[m_size]) T(std::forward<argtypes>(args)...);
            ++m_size;
        }

        template <typename... argtypes>
        void emplace_at(size_t index, argtypes&&... args)
        {
            CHECK_FOR_VALID_INDEX(index, m_size);
            new (&m_data[index]) T(std::forward<argtypes>(args)...);
        }

        void fill(const T& value)
        {
            for (size_t i = 0; i < m_size; i++)
            {
                m_data[i] = value;
            }
        }

        void remove_at(size_t index)
        {
            CHECK_FOR_VALID_INDEX(index, m_size);

            m_data[index].~T();

            for (size_t i = index + 1; i < m_size; i++)
                m_data[i - 1] = std::move(m_data[i]);

            --m_size;
        }

        void remove(const T& value, bool single = false)
        {
            bool found = true;
            while (found)
            {
                found = false;
                for (size_t i = 0; i < m_size; i++)
                {
                    if (m_data[i] == value)
                    {
                        found = true;
                        remove_at(i);
                        if (single)
                            return;
                        break;
                    }
                }
            }
        }

        void clear()
        {
            for (size_t i = 0; i < m_size; i++)
                m_data[i].~T();

            m_size = 0;
        }

        void delete_memory()
        {
            clear();
            ::operator delete(m_data, sizeof(T) * m_capacity);
            m_data = nullptr;
            m_capacity = 0;
        }

        void print_array() const
        {
            for (size_t i = 0; i < m_size; i++)
                std::cout << m_data[i] << " ";
        }

        iterator begin() { return iterator(m_data); }
        iterator end() { return iterator(m_data + m_size); }

        inline size_t size() const { return m_size; };
        inline size_t capacity() const { return m_capacity; };

    private:
        T* m_data;
        size_t m_size, m_capacity;
    };
}; // namespace mikalib

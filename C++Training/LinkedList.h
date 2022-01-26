#pragma once

#include <utility>

namespace mikalib
{
    template <typename linked_list>
    class linked_list_iterator
    {
    public:
        using valuetype = typename linked_list::valuetype;

        linked_list_iterator(linked_list::node* top) : m_current_node(top) {}

        linked_list_iterator& operator++()
        {
            m_current_node = m_current_node->next;
            return *this;
        }
        linked_list_iterator& operator++(int)
        {
            linked_list_iterator temp = *this;
            m_current_node = m_current_node->next;
            return temp;
        }

        valuetype* operator->() { return &(m_current_node->value); }
        valuetype& operator*() { return m_current_node->value; }

        bool operator==(const linked_list_iterator& other) { return m_current_node == other.m_current_node; }
        bool operator!=(const linked_list_iterator& other) { return m_current_node != other.m_current_node; }

    private:
        linked_list::node* m_current_node;
    };

    template <typename T>
    class linked_list
    {
    public:
        using valuetype = T;

        struct node
        {
            node(const T& value) : value(value) {}

            T value;
            node* next = nullptr;
            node* previous = nullptr;
        };

        linked_list() : m_top(nullptr), m_last(nullptr) {}
        linked_list(const linked_list& other)
        {
            empty();

            node* top = other.m_top;
            while (top)
            {
                add_end(top->m_value);
                top = top->m_next;
            }
        }
        linked_list(linked_list&& other) noexcept
        {
            empty();
            m_top = other.m_top;
            m_last = other.m_last;
            other.m_top = nullptr;
            other.m_last = nullptr;
        }

        ~linked_list() { empty(); }

        void operator=(const linked_list& other)
        {
            empty();

            node* top = other.m_top;
            while (top)
            {
                add(top->m_value);
                top = top->m_next;
            }
        }
        void operator=(linked_list&& other)
        {
            empty();
            m_top = other.m_top;
            m_last = other.m_last;
            other.m_top = nullptr;
            other.m_last = nullptr;
        }

        void add_end(const T& value)
        {
            if (m_top == nullptr)
            {
                m_top = new node(value);
                m_last = m_top;
                return;
            }

            m_last->next = new node(value);
            m_last->next->previous = m_last;
            m_last = m_last->next;
        }

        void add_top(const T& value)
        {
            node* old_top = m_top;
            m_top = new node(value);
            m_top->next = old_top;
        }

        T continue_list()
        {
            if (!m_top)
                return T();

            T value = std::move(m_top->value);
            node* temp_ptr = m_top->next;
            delete m_top;
            m_top = temp_ptr;
            return value;
        }

        T pop()
        {
            if (!m_top)
                return T();

            T temp_value = std::move(m_last->value);
            node* new_last = m_last->previous;

            if (m_last->previous)
                m_last->previous->next = nullptr;
            else
                m_top = nullptr;

            delete m_last;
            m_last = new_last;

            return temp_value;
        }

        bool contains(const T& value) const
        {
            node* current_node = m_top;
            while (current_node)
            {
                if (current_node->value == value)
                    return true;

                current_node = current_node->next;
            }
            return false;
        }

        void empty()
        {
            while (m_top != nullptr)
            {
                continue_list();
            }
        }

        inline bool is_empty() const { return !m_top; }
        node* top() const { return m_top; }

        linked_list_iterator<linked_list<T>> begin() { return linked_list_iterator<linked_list<T>>(m_top); }
        linked_list_iterator<linked_list<T>> end() { return linked_list_iterator<linked_list<T>>(nullptr); }

    private:
        node* m_top;
        node* m_last;
    };

    template <typename T>
    class queue
    {
    public:
        void enqueue(const T& value) { data.add_end(value); }
        T dequeue() { return data.continue_list(); }
        inline bool is_empty() const { return data.is_empty(); }
        inline bool contains(const T& value) const { return data.contains(value); }

        linked_list_iterator<linked_list<T>> begin() { return linked_list_iterator<linked_list<T>>(data.top()); }
        linked_list_iterator<linked_list<T>> end() { return linked_list_iterator<linked_list<T>>(nullptr); }

    private:
        linked_list<T> data;
    };

    template <typename T>
    class stack
    {
    public:
        void add(const T& value) { data.add_end(value); }
        T pop() { return data.pop(); }
        inline bool is_empty() const { return data.is_empty(); }
        inline bool contains(const T& value) const { return data.contains(value); }

        linked_list_iterator<linked_list<T>> begin() { return linked_list_iterator<linked_list<T>>(data.top()); }
        linked_list_iterator<linked_list<T>> end() { return linked_list_iterator<linked_list<T>>(nullptr); }

    private:
        linked_list<T> data;
    };
} // namespace mikalib

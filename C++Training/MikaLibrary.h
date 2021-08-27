#pragma once

#include <iostream>
#include <signal.h>



namespace mikalib
{
	struct vector
	{
		float X, Y, Z;
		vector()
			: X(0), Y(0), Z(0)
		{
			std::cout << "constructor";
		};

		explicit vector(float X, float Y, float Z)
			: X(X), Y(Y), Z(Z)
		{};
		vector operator+ (const vector& other) const
		{
			return vector(X + other.X, Y + other.Y, Z + other.Z);
		}

		vector operator* (float other) const
		{
			return vector(X * other, Y * other, Z * other);
		}

		vector operator/ (float other) const
		{
			return vector(X / other, Y / other, Z / other);
		}

		double size() const;
		void normalize();
	};

	template<typename T>
	class array
	{

	public:

		array()
			: m_array_ptr(nullptr), m_size(0)
		{
		}

		array(int size)
			: m_size(size)
		{
			m_array_ptr = (T*)malloc(sizeof(T) * m_size);
		}

		array(const array& other)
		{
			*this = other;
		}

		~array()
		{
			delete[] m_array_ptr;
		}

		T& operator[] (int index) const
		{
			if (index < 0 || index > m_size - 1) raise(SIGSEGV);
			return *(m_array_ptr + index);
		}

		//allows copying one vector to another
		void operator= (const array& other)
		{
			if (other.is_valid())
			{
				delete[] m_array_ptr;
				m_size = other.get_size();
				m_array_ptr = (T*)malloc(sizeof(T) * m_size);

				for (int i = 0; i < m_size; i++)
					*this[i] = other[i];
			}
			else delete_array();
		}

		//creates new array with spesific size and fills it with value
		void init(const T& value, int size)
		{
			if (size == 0)
			{
				delete_array();
				return;
			}
			
			m_size = size;
			T* new_ptr = (T*)malloc(sizeof(T) * m_size);

			for (int i = 0; i < m_size; i++)
				*(new_ptr + i) = value;

			delete[] m_array_ptr;
			m_array_ptr = new_ptr;
		}

		//changes size of array
		void reserve(int size)
		{
			if (size == 0)
			{
				delete_array();
				return;
			}
			
			m_size = size;
			T* new_ptr = (T*)malloc(sizeof(T) * m_size);

			if (m_array_ptr)
			{
				for (int i = 0; i < m_size; i++)
					*(new_ptr + i) = (*this)[i];

				delete[] m_array_ptr;
			}
			
			m_array_ptr = new_ptr;
		}

		void delete_array()
		{
			delete[] m_array_ptr;
			m_array_ptr = nullptr;
			m_size = 0;
		}

		bool is_valid()
		{
			return m_array_ptr;
		}
		
		//fills current array with value
		void fill (const T& value)
		{
			for (int i = 0; i < m_size; i++)
				(*this)[i] = value;		
		}

		//adds value to array
		void add(const T& value)
		{
			++m_size;
			T* new_ptr = (T*)malloc(sizeof(T) * m_size);

			if (m_array_ptr)
			{
				for (int i = 0; i < m_size; i++)
					*(new_ptr + i) = i < m_size - 1 ? (*this)[i] : value;
			}
			else new_ptr[0] = value;
			
			delete[] m_array_ptr;
			m_array_ptr = new_ptr;
		}

		template<typename... ArgTypes>
		void construct_at_index(int index, ArgTypes... args)
		{
			if (index < 0 || index > m_size - 1)
			{
				return;
			}

			new (m_array_ptr + index) T(args...);
		}

		inline int get_size() const { return m_size; }

	private:

		T* m_array_ptr;
		int m_size;
	};

	
};




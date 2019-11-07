#include <iostream>
#include <algorithm>
#include <cassert>

namespace mvu {

template <class T>
class Vector {
    private:
        T* vector;
        int size;
        int limit;
    public:
        Vector() {
            size = -1;
            limit = 1;
            vector = new T[limit];
        }
        Vector(const int &mem) {
            size = -1;
            limit = mem;
            vector = new T[limit];
        }
		
        Vector (const Vector <T> &b) {
			size = b.size;
            limit = b.limit;
            vector = new T[limit];
            for (int i = 0; i <= size; i++) {
                vector[i] = b.vector[i];
            }
		}
		
        int const len(void) {
            return size + 1;
        }
		
        T & operator [] (const int &i) {
			if (i < 0 || i > size)
				throw std::out_of_range("Out of range!!!");
			return *(vector + i);
        }
        T & at(const int &i) {
			if (i < 0 || i > size)
				throw std::out_of_range("Out of range!!!");
			return *(vector + i);
		}
		
        void push_back(const T a) {
			const int mnoj = 2;
            size++;
            if (size == limit) {
                T* help = new T[mnoj * limit];
                for (int i = 0; i < size; i++) {
                    help[i] = vector[i];
                }
                delete [] vector;
                help[size] = a;
                vector = help;
                limit = limit * mnoj;
            } else {
                vector[size] = a;
            }
        }
		void push_front(const T a) {
			insert(a, 0);
        }
		
        void operator = (Vector <T> &b) {
            delete [] vector;
            size = b.size;
            limit = b.limit;
            vector = new T[limit];
            for (int i = 0; i <= size; i++) {
                vector[i] = b.vector[i];
            }
		}
		bool operator == (Vector <T> &b) {
			if ((*this).len() == b.len()) {
				for (int i = 0; i < b.len(); i++) {
					if ((*this)[i] != b[i]) 
						return 0;
					}
				return 1;
			}
			return 0;
		}
		
		void insert(const T num, const int &j) {
			if (j < 0 || j > size + 1)
				throw std::out_of_range("Out of range!!!");
			size++;
			if (size != limit) {
				for (int i = size; i > j; i--) {
					vector[i] = vector[i - 1];
				}
				vector[j] = num;
				return;
			}
			Vector <T> b;
			for (int i = 0; i < j; i++) {
				b.push_back(vector[i]);
			}
			b.push_back(num);
			for (int i = j; i < size; i++) {
				b.push_back(vector[i]);
			}
			*this = b;
		}
		T const erase (const int &j) {
			if (j < 0 || j > size)
				throw std::out_of_range("Out of range!!!");
			T em = vector[j];
			for (int i = j; i < size; i++) {
				vector[i] = vector[i + 1];
			}
			size--;
			return em;
		}
		
		friend void swap (Vector <T> &a, Vector <T> &b) {
			std::swap(a, b);
		}		

		void sort(void) {
			std::sort(vector, vector + size);
		}
		
        void clear(void) {
            size = -1;
            limit = 1;
            delete [] vector;
            vector = new T[limit];
        }
		
        ~ Vector () {
            delete [] vector;
        }
};

}

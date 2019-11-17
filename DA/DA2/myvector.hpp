#include <iostream>
#include <algorithm>
#include <cassert>
    
template <class T>
class TVector {
    private:
        T* vector;
        int size;
        int limit;
    public:
        TVector() {
            size = -1;
            limit = 1;
            vector = new T[limit];
        }
        TVector(const int &mem) {
            size = -1;
            limit = mem;
            vector = new T[limit];
        }
        
        TVector (const TVector <T> &b) {
            size = b.size;
            limit = b.limit;
            vector = new T[limit];
            for (int i = 0; i <= size; i++) {
                vector[i] = b.vector[i];
            }
        }
        
        const int Len(void) {
            return size + 1;
        }
        
        T & operator [] (const int &i) {
            if (i < 0 || i > size) {
                throw std::out_of_range("Out of range!!!");
            }
            return *(vector + i);
        }
        T & At(const int &i) {
            if (i < 0 || i > size) {
                throw std::out_of_range("Out of range!!!");
            }
            return *(vector + i);
        }
        
        void PushBack(const T a) {
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
        void PushFront(const T a) {
            Insert(a, 0);
        }
        
        void operator = (const TVector <T> &b) {
            delete [] vector;
            size = b.size;
            limit = b.limit;
            vector = new T[limit];
            for (int i = 0; i <= size; i++) {
                vector[i] = b.vector[i];
            }
        }
        bool operator == (TVector <T> &b) {
            if ((*this).Len() == b.Len()) {
                for (int i = 0; i < b.Len(); i++) {
                    if ((*this)[i] != b[i]) 
                        return 0;
                    }
                return 1;
            }
            return 0;
        }
        void operator = (char* str) {
            delete [] vector;
            int i = 0;
            while (true) {
                if (str[i] == '\0') {
                    break;
                }
                i++;
            }
            if (i > 0) {
                limit = i;
                vector = new char[limit];
            } else {
                limit = 1;
                size = -1;
                vector = new char[limit];
                return;
            }
            for (int j = 0; j < i; j++) {
                vector[j] = str[j];
            }
            size = i - 1;
        }
        
        void Insert(const T num, const int &j) {
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
            TVector <T> b(limit);
            for (int i = 0; i < j; i++) {
                b.PushBack(vector[i]);
            }
            b.PushBack(num);
            for (int i = j; i < size; i++) {
                b.PushBack(vector[i]);
            }
            *this = b;
        }
        T const Erase (const int &j) {
            if (j < 0 || j > size)
                throw std::out_of_range("Out of range!!!");
            T em = vector[j];
            for (int i = j; i < size; i++) {
                vector[i] = vector[i + 1];
            }
            size--;
            return em;
        }
        T const PopBack(void) {
            return Erase(size);
        }
        
        friend void Swap (TVector <T> &a, TVector <T> &b) {
            std::swap(a, b);
        }
        
        void Sort(void) {
            std::sort(vector, vector + size);
        }
    
        void Clear(void) {
            size = -1;
            limit = 1;
            delete [] vector;
            vector = new T[limit];
        }
        
        ~ TVector () {
            delete [] vector;
        }
};


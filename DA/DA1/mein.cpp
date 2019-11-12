#include <iostream>
//#include <cstdio>
#include <ctime>
#include <algorithm>
#include <cmath>
using namespace std;

class Value {
    private:
        int size;
        int limit;
    public:
        char* value;
        Value() {
            size = -1;
            limit = 1;
            value = new char[limit];
        }
        int V_size() {
            return size + 1;
        }
        bool Free_place() {
            if (size + 1 == limit) {
                return 0;
            } else {
                return 1;
            }
        }
        void Size_p() {
            size++;
        }
        char operator [] (const int &i) {
            return value[i];
        }
        void Add_va(const char &a) {
            size++;
            if (size == limit) {
                const int mnoj = 2;
                char* h = new char[mnoj * limit];
                for (int i = 0; i < size; i++) {
                    h[i] = value[i];
                }
                delete [] value;
                h[size] = a;
                value = h;
                limit = limit * mnoj;
            } else {
                value[size] = a;
            }
        }

        void operator = (Value& b) {
            delete [] value;
            size = b.size;
            limit = b.limit;
            value = new char[limit];
            for (int i = 0; i <= size; i++) {
                value[i] = b.value[i];
            }
        }

        friend void Swap(Value &a, Value &b) {
            using std::swap;

            swap(a.value, b.value);
            swap(a.size, b.size);
            swap(a.limit, b.limit);
        }

        void Clear_v() {
            size = -1;
            limit = 1;
            delete [] value;
            value = new char[limit];
        }
        ~ Value () {
            delete [] value;
        }
};

class Key {
    private:
        int size;
        int limit;
    public:
        char* key;
        Key() {
            size = -1;
            limit = 1;
            key = new char[limit];
        }
        int K_size() {
            return size + 1;
        }
        bool Free_place() {
            if (size + 1 == limit) {
                return 0;
            } else {
                return 1;
            }
        }
        void Size_p() {
            size++;
        }
        char operator [] (const int &i) {
            return key[i];
        }
        void Add_ch(const char &a) {
            size++;
            if (size == limit) {
                const int mnoj = 2;
                char* h = new char[mnoj* limit];
                for (int i = 0; i < size; i++) {
                    h[i] = key[i];
                }
                delete [] key;
                h[size] = a;
                key = h;
                limit = limit * mnoj;
            } else {
                key[size] = a;
            }
        }

        void operator = (Key& b) {
            delete [] key;
            size = b.size;
            limit = b.limit;
            key = new char[limit];
            for (int i = 0; i <= size; i++) {
                key[i] = b.key[i];
            }
        }

        friend void Swap(Key &a, Key &b) {
            using std::swap;

            swap(a.key, b.key);
            swap(a.size, b.size);
            swap(a.limit, b.limit);
        }

        void Clear_k() {
            size = -1;
            limit = 1;
            delete [] key;
            key = new char[limit];
        }
        ~ Key () {
            delete [] key;
        }
};

class Inf {
    private:
        int count;
        int last;
    public:

        Value* values;
        Key* keys;
        Inf() {
            count = 1;
            last = -1;
            values = new Value[count];
            keys = new Key[count];
        }
        Inf (int memory) {
            count = memory;
            last = memory - 1;
            values = new Value[count];
            keys = new Key[count];
        }
        int Size_i() {
            return (last + 1);
        }
        void Add_em(Key& a, Value& b) {
            last++;
            if (last == count) {
                const int mnoj = 2;
                Key* some_k = new Key[mnoj * count];
                for (int i = 0; i < count; i++) {
                    some_k[i] = keys[i];
                }
                some_k[count] = a;
                delete [] keys;
                keys = some_k;

                Value* some_v = new Value[mnoj * count];
                for (int i = 0; i < count; i++) {
                    some_v[i] = values[i];
                }
                some_v[count] = b;
                delete [] values;
                values = some_v;

                count = count * mnoj;
            }
            else {
                keys[last] = a;
                values[last] = b;
            }
        }
        void operator = (Inf& b) {
            count = b.count;
            last = b.last;
            delete [] keys;
            delete [] values;
            keys = new Key[count];
            values = new Value[count];
            for (int i = 0; i <= last; i++) {
                keys[i] = b.keys[i];
                values[i] = b.values[i];
            }
        }
        friend void Swap(Inf &a, Inf &b) {
            using std::swap;

            swap(a.keys, b.keys);
            swap(a.count, b.count);
            swap(a.last, b.last);
            swap(a.values, b.values);
        }
        ~ Inf () {
            delete [] keys;
            delete [] values;
        }
};

int Cti(const char &a) {
    return a - '0';
}

void Insert(Key &a, const int &j, const char &em) {
    if (a.Free_place()) {
        a.Size_p();
        for (int i = a.K_size() - 1; i > j; i--) {
            a.key[i] = a.key[i - 1];
        }
        a.key[j] = em;
        return;
    }
    Key b;
    for (int i = 0; i < j; i++) {
        b.Add_ch(a.key[i]);
    }
    b.Add_ch(em);
    for (int i = j + 1; i <= a.K_size(); i++) {
        b.Add_ch(a.key[i - 1]);
    }
    a = b;
}

void Print(Inf &a) {
    for (int i = 0; i < a.Size_i(); i++) {
        for (int j = 0; j < a.keys[i].K_size(); j++) {
            if (a.keys[i].key[j] != '*') {
                putchar(a.keys[i].key[j]);
            }
        }
        putchar('\t');
        for (int j = 0; j < a.values[i].V_size(); j++) {
            putchar(a.values[i].value[j]);
        }
        putchar('\n');
    }
}

void Sort_small(Inf& a, const int &j, const int &k, const int &shift, Inf &b) {
    int n = a.Size_i();
    int* c = new int[k];
    int* sum = new int[n];
    for (int i = 0; i < k; i++) {
        c[i] = 0;
    }
    for (int i = 0; i < n; i++) {
        sum[i] = 0;
        if (j == 1) {
            if (a.keys[i][0] == '*') {
                sum[i] = Cti(a.keys[i][1]);
            } else {
                sum[i] = 10 * Cti(a.keys[i][0]) + Cti(a.keys[i][1]);
            }
        }
        if (j == 2) {
            if (a.keys[i][3] == '*') {
                sum[i] = Cti(a.keys[i][4]);
            } else {
                sum[i] = 10 * Cti(a.keys[i][3]) + Cti(a.keys[i][4]);
            }
        }
        if (j == 3) {
            int q = a.keys[i].K_size() - 1 - shift;
            if (q <= 5) {
                sum[i] = 0;
            } else {
                sum[i] = Cti(a.keys[i][q]);
            }
        }
    }
    for (int i = 0; i < n; i++) {
        c[sum[i]]++;
    }
    for (int i = 0; i < k - 1; i++) {
        c[i + 1] += c[i];
    }
    for (int i = n - 1; i >= 0; i--) {
        c[sum[i]]--;
        Swap(a.keys[i], b.keys[c[sum[i]]]);
        Swap(a.values[i], b.values[c[sum[i]]]);
    }
    Swap(b, a);
    delete [] sum;
    delete [] c;
}

int pow(int a, int b) {
	int c = 1;
	for (int i = 0; i < b; i++) {
		c *= a;
	}
	return c;
}

void Redux_sort(Inf &a) {
    const int max_size_key = 5;
    //const int
    int n = a.Size_i();
    for (int i = 0; i < n; i++) {
        if (a.keys[i][1] == '.') {
            Insert(a.keys[i], 0, '*');
        }                                                     
        if (a.keys[i][4] == '.') {                          
            Insert(a.keys[i], 3, '*');                       
        }													 
    }														
    Inf b(n);
    Sort_small(a, 1, 32, 0, b);
    Sort_small(a, 2, 13, 0, b);
    for (int i = 0; i < max_size_key; i++) {
        Sort_small(a, 3, 10, i, b);
    }
}

void standart_sort(Inf &a) {
	int n = a.Size_i();
	Inf b(n);
	int* sum = new int[n];
	int* index = new int[n];
	bool swap;
	for (int i = 0; i < n; i++) {
		sum[i] = 0;
		index[i] = i;
        if (a.keys[i][1] == '.') {
            Insert(a.keys[i], 0, '*');
        }                                        
        if (a.keys[i][4] == '.') {                        
            Insert(a.keys[i], 3, '*');                    
        }
        for (int j = 1; j >= 0; j--) {
			if (a.keys[i][j] == '*') {
				continue;
			}
			sum[i] += Cti(a.keys[i][j])*pow(10, 1 - j);									
		}
		for (int j = 4; j >= 3; j--) {
			if (a.keys[i][j] == '*') {
				continue;
			}
			sum[i] += Cti(a.keys[i][j])*pow(10, 6 - j);									
		}
		int j = a.keys[i].K_size() - 1;
		while(a.keys[i][j] != '.') {
			sum[i] += Cti(a.keys[i][j]) * pow(10, 4 + a.keys[i].K_size() - 1 - j);
			j--;
		}
	}
	while (true) {
		for (int i = 0; i < n - 1; i++) {
			if (sum[i] > sum[i + 1]) {
				swap = 1;
				std::swap(sum[i], sum[i + 1]);
				std::swap(index[i], index[i + 1]);
			}
		}
		if (!swap) {
			break;
		} else {
			swap = 0;
		}
	}
	for (int i = 0; i < n; i++) {
		Swap(b.keys[i], a.keys[index[i]]);
		Swap(b.values[i], a.values[index[i]]);
	}
	Swap(a, b);
	delete [] sum;
	delete [] index;
}

int main()
{
    //ios::sync_with_stdio(false);
    //char ch;
    Inf a;
    Key ink;
    Value inv;
    char DD;
    int iter = 0;
    Inf b;
    int count = 10;
    /*
    while (true) {
        do {
            ch = getchar();
        } while (ch == '\n');
        if (ch == EOF) {
            break;
        }
        do {
            ink.Add_ch(ch);
            ch = getchar();
        } while (ch != '\t');
        ch = getchar();
        do {
            inv.Add_va(ch);
            ch = getchar();
        } while (ch != '\n');
        a.Add_em(ink, inv);
        ink.Clear_k();
        inv.Clear_v();
    }
    */
    // /*
    while (true) {
		iter++;
		if (rand() % 2) {
			ink.Add_ch((char)(rand() % 9) + '1');
		} else {
			DD = (char)(rand() % 4) + '0';
			ink.Add_ch(DD);
			if (DD == '3') {
				ink.Add_ch((char)(rand() % 2) + '0');
			} else if (DD == '0') {
				ink.Add_ch((char)(rand() % 9) + '1');
			} else { 	
				ink.Add_ch((char)(rand() % 10) + '0');
			}
		}
		ink.Add_ch('.');
		if (rand() % 2) {
			ink.Add_ch((char)(rand() % 10) + '0');
		} else {
			DD = (char)(rand() % 2) + '0';
			ink.Add_ch(DD);
			if (DD == '1') {
				ink.Add_ch((char)(rand() % 3) + '0');
			} else {
				ink.Add_ch((char)(rand() % 9) + '1');
			}
		}
		ink.Add_ch('.');
		DD = 1 + rand() % 4;
		for (int i = 0; i < DD; i++) {
			ink.Add_ch((char)(rand() % 10) + '0');
		}
		
		DD = 1 + rand() % 10;
		for (int i = 0; i < DD; i++) {
			if (rand() % 2) {
				inv.Add_va((char)(rand() % 26) + 'a');
			} else {
				inv.Add_va((char)(rand() % 26) + 'A');
			}
		}
		a.Add_em(ink, inv);
        ink.Clear_k();
        inv.Clear_v();	
        if (iter == count) {
			iter = 0;
			clock_t start_time = clock();
			Redux_sort(a);
			//standart_sort(a);
			cout << count << " elements for " << (double)(clock() - start_time)/CLOCKS_PER_SEC << " sec\n";
			//Print(a);
			count *= 10;
			a = b;
			//return 0;
			continue;
		}
	}
	// */
	//Redux_sort(a);
	//standart_sort(a);
	//Print(a);
    return 0;
}

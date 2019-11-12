#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <vector>
#include <pthread.h>
#include <string>
#include <fstream>
#include "myvector.hpp"

using namespace std;

const int MAX_THREADS = 7516;
pthread_mutex_t mut;

struct arguments_t {
	vector <string>* vec;
	int low;
	int cnt;
	vector <pthread_t>* pth;
	int limit;
	int dir;
};

bool is_bigger(string &a, string &b) {
	if (a.length() > b.length()) {
		return 1;
	} else if (a.length() < b.length()) {
		return 0;
	} else {
		for (unsigned int i = 0; i < a.length(); i++) {
			if (a[i] > b[i]) {
				return 1;
			} else if (a[i] < b[i]) {
				return 0;
			}
		}
	}
	return 0;
}

void print_v(vector <string> &a) {
	for (unsigned int i = 0; i < a.size(); i++) {
		cout << a[i] << " ";
	}
	cout << "\n";
}

void compswap(vector <string>* a, int i, int j, int dir) {
	if (dir == is_bigger(a->at(i), a->at(j))) {
		swap(a->at(i), a->at(j));
	}
}

void half_cleaner(vector <string>* a, int low, int cnt, int dir) {
	if (cnt > 1) {
		int k = cnt/2;
		for (int i = low; i < low + k; i++) {
			//pthread_mutex_lock(&mut);
			compswap(a, i, i+k, dir);
			//pthread_mutex_unlock(&mut);
		}
		half_cleaner(a, low, k, dir);
		half_cleaner(a, low + k, k, dir);
	}
}

void bitonic_sort_h(vector <string>* a, int low, int cnt, int dir) {
	if (cnt > 1) {
		int k = cnt/2;
		bitonic_sort_h(a, low, k, 1);
		bitonic_sort_h(a, low + k, k, 0);
		half_cleaner(a, low, cnt, dir);
	}
}

void* bitonic_sort_t(void* arg) {
	arguments_t* ar1 = (arguments_t*) arg;
	if (ar1->cnt > 1) {
		pthread_t t1;
		pthread_t t2;
		int low = ar1->low;
		int cnt = ar1->cnt;
		int k = cnt/2;
		arguments_t ar2 = {ar1->vec, low, k, ar1->pth, ar1->limit, 1};
		arguments_t ar3 = {ar1->vec, low + k, k, ar1->pth, ar1->limit, 0};
		pthread_mutex_lock(&mut);
		if ((int) ar1->pth->size() >= ar1->limit) {
			pthread_mutex_unlock(&mut);
			bitonic_sort_h(ar1->vec, low, k, 1);
			bitonic_sort_h(ar1->vec, low + k, k, 0);
			half_cleaner(ar1->vec, ar1->low, ar1->cnt, ar1->dir);
			return NULL;
		}
		ar1->pth->push_back(t1);
		pthread_mutex_unlock(&mut);
		//cout << ar1->pth->size();
		pthread_create(&t1, NULL, bitonic_sort_t, &ar2);
		pthread_mutex_lock(&mut);
		if ((int) ar1->pth->size() >= ar1->limit) {
			pthread_mutex_unlock(&mut);
			pthread_join(t1, NULL);
			bitonic_sort_h(ar1->vec, low + k, k, 0);
			half_cleaner(ar1->vec, ar1->low, ar1->cnt, ar1->dir);
			return NULL;
		}
		ar1->pth->push_back(t2);
		pthread_mutex_unlock(&mut);
		pthread_create(&t2, NULL, bitonic_sort_t, &ar3);
		pthread_join(t1, NULL);
		pthread_join(t2, NULL);
		half_cleaner(ar1->vec, ar1->low, ar1->cnt, ar1->dir);
	}
	return NULL;
}

void bitonic_sort(vector <string>* a, vector <pthread_t>* threads, int lim) {
	pthread_t t;
	threads->push_back(t);
	arguments_t arg = {a, 0, (int) a->size(), threads, lim, 1};
	pthread_create(&t, NULL, bitonic_sort_t, &arg);
	pthread_join(t, NULL);
}

int pow2(int a) {
	int c = 1;
	for (int i = 1; i <= a; i++) {
		c = 2*c;
	}
	return c;
} 

string modern_vector(vector <string> &a) {
	int i = 0;
	while ((int)a.size() > pow2(i)) {
		i++;
	}
	if ((int)a.size() == pow2(i) || a.size() == 0) {
		return "";
	}
	string max = a[i];
	for (int i = 0; i < (int) a.size(); i++) {
		if (is_bigger(a[i], max)) {
			max = a[i];
		}
	}
	max.append("a");
	while ((int)a.size() != pow2(i)) {
		a.push_back(max);
	}
	return max;
}

int main() {
	vector <pthread_t> threads;
	string str;
	ifstream fin("test.txt");
	vector <string> vec;
	while (fin >> str) {
		vec.push_back(str);
	}
	unsigned int min = vec.size();
	string max;
	int lim;
	cout << "Input the limit count of trades: ";
	cin >> lim;
	if (lim == -1) {
		lim = MAX_THREADS;
	} else if (lim == 0) {
		cout << "start sort: ";
		print_v(vec);
		max = modern_vector(vec);
		bitonic_sort_h(&vec, 0, (int)vec.size(), 1);
		while (vec.size() != min) {
			for (int i = 0; i < (int)vec.size(); i++) {
				if (vec[i] == max) {
					vec.erase(vec.begin() + i);
				}
			}
		}
		cout << "out sort: ";
		print_v(vec);
		cout << "count of trades: " << lim << endl;
		return 0;
	} else if (lim < 0 && lim != -1) {
		return 0;
	} else if (lim > MAX_THREADS) {
		lim = MAX_THREADS;
	}
	cout << "start sort: ";
	print_v(vec);
	max = modern_vector(vec);
	cout << "out sort: ";
	bitonic_sort(&vec, &threads, lim);
	while (vec.size() != min) {
		for (int i = 0; i < (int)vec.size(); i++) {
			if (vec[i] == max) {
				vec.erase(vec.begin() + i);
			}
		}
	}
	print_v(vec);
	cout << "count of trades: " << threads.size() << endl;
	return 0;
}

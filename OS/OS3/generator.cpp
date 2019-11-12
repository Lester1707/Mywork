#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cmath>
#include <string>

using namespace std;
typedef string elements_type;

int main() {
	int len_str = 8;
	int count_str = 30000;
	char* ch = new char[1];
	elements_type str;
	elements_type str_h;
	ofstream fout("test.txt");
	for (int i = 0; i < count_str; i++) {
		int len = rand() % len_str + 1;
		for (int j = 0; j < len; j++) {
			ch[0] = rand() % 26 + 97;
			str_h = ch;
			str.append(str_h);
		}
		str.append("\n");
		//cout << str;
		fout << str;
		str.clear();
	}
	delete [] ch;
	return 0;
}

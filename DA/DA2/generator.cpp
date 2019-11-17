#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <string>
#include <ctime>
using namespace std;
typedef string elements_type;

int main() {
    srand(time(0));
	int len_str = 3;
	int count_str = 200000;
    int len_num = 8;
	char* ch = new char[1];
	elements_type str;
	elements_type str_h;
	ofstream fout("test.txt");
	for (int i = 0; i < count_str; i++) {
        str.append("+ ");
		int len = rand() % len_str + 1;
		for (int j = 0; j < len; j++) {
			ch[0] = rand() % 26 + 97;
			str_h = ch;
			str.append(str_h);
		}
        str.append(" ");
        len = rand() % len_num + 1;
		for (int j = 0; j < len; j++) {
			ch[0] = rand() % 9 + '0';
			str_h = ch;
			str.append(str_h);
		}
		str.append("\n");
		//cout << str;
		fout << str;
		str.clear();
	}
    fout.close();
	delete [] ch;
	return 0;
}

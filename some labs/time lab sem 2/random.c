#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "string.h"

int ran(int end_point, int helper)
{
	srand(time(0) - helper);
	return (rand() % (end_point + 1));
}

void name(string* a, int help, int p)
{
	int value;
	a->name_size = ran(9, -1 + help) + 1;
	if (p)
		printf("'");
	for (int i = 0; i < a->name_size; i++) {
		value = (char) ran(61, i - a->name_size + help);
		if (value <= 9)
			a->name[i] = '0' + value;
		else if (value >= 10 && value <= 35)
			a->name[i] = 'a' + (value - 10);
		else if (value >= 36)
			a->name[i] = 'A' + (value - 36);
		if (p)
			printf("%c", a->name[i]);
	}
	if (p)
		printf("'\n");
}

int main(void) 
{
	char value;
	int is_printy = 1;
	string a;
	FILE* f;
	f = fopen("134krfkewlkew;klewl.txt", "w");
	int size = 0;
		printf("enter the count off table elements: ");
		scanf("%d", &size);
		if (size >= 2000)
			is_printy = 0;
		printf("1 file:\n");
		fwrite(&size, sizeof(size), 1, f);
		for (int i = 0; i < size; i++) {
			a.size_key = ran(4, i + size) + 1;
			for (int j = 0; j < a.size_key; j++) {
				value = (char) ran(61, j + i);
				if (value <= 9)
					a.key[j] = '0' + value;
				else if (value >= 10 && value <= 35)
					a.key[j] = 'a' + (value - 10);
				else if (value >= 36)
					a.key[j] = 'A' + (value - 36);
				if (is_printy)
					printf("%c", a.key[j]);
			}
			if (is_printy)
				printf(" ");
			name(&a, i + a.size_key, is_printy);
			fwrite(&a, sizeof(a), 1, f);
		}
	fclose(f);
	return 0;
}

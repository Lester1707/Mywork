#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
 
#include "string.h"

int powr(int a, int b)
{
	int c = 1;
	for (int i = 1; i <= b; i++)
		c *= a;
	return c;
}

int power(char* a, int asize)
{
	int c = 0;
	for (int i = 0; i < asize; i++)
		if (a[i] >= '0' && a[i] <= '9')
			c += (a[i] - '0') * powr(61, asize - i - 1);
		else if (a[i] >= 'a' && a[i] <= 'z')
			c += (10 + a[i] - 'a') * powr(61, asize - i - 1);
		else if (a[i] >= 'A' && a[i] <= 'Z')
			c += (36 + a[i] - 'A') * powr(61, asize - i - 1);	
	return c;
}
 
int main(void)
{
	
	int value;
	int count;
	char ch;
	char key[5];
	int is_printy = 1;
	FILE* in = fopen("134krfkewlkew;klewl.txt", "r"); 
	if (!in) {
		printf("file is destroy!!!");
		return 1;
	}
	fread(&count, sizeof(count), 1, in);
	if (count >= 2000)
		is_printy = 0;
	string b[count];
	string c;
	string a;
	int nomer;
	for (int i = 0; i < count; i++) {
		if (!fread(&a, sizeof(a), 1, in))
			printf("33");
		b[i] = a;
	}
	int k = 0;
	printf("2 file:\n");
	clock_t start = clock();
	clock_t end;
	printf("\tprint 's' to sort table\n\tprint 'p' to print table\n\tprint 'f' to find element on key\n");
	while (1) {
		ch = getchar();
		switch(ch) {
			case 's':
				start = clock();
				for (int i = 0; i < count; i++) {
					value = power(b[i].key, b[i].size_key);
					nomer = i;
					for (int j = i; j < count; j++)
						if (power(b[j].key, b[j].size_key) < value) {
							value = power(b[j].key, b[j].size_key);
							nomer = j;
						}
					c = b[i];
					b[i] = b[nomer];
					b[nomer] = c;
				}
				end = clock();
				printf("use time: %f\n", (double)(end - start)/CLOCKS_PER_SEC);
				break;
			case 'o':
				start = clock();
				for (int i = 0; i < count; i++) {
					value = power(b[i].key, b[i].size_key);
					nomer = i;
					for (int j = i; j < count; j++)
						if (power(b[j].key, b[j].size_key) > value) {
							value = power(b[j].key, b[j].size_key);
							nomer = j;
						}
					c = b[i];
					b[i] = b[nomer];
					b[nomer] = c;
				}
				end = clock();
				printf("use time: %f\n", (double)(end - start)/CLOCKS_PER_SEC);
				break;
			case 'p':
				if (is_printy)
					for (int i = 0; i < count; i++) {
						for (int j = 0; j < b[i].size_key; j++)
							printf("%c", b[i].key[j]);
						printf(" '");
						for (int j = 0; j < b[i].name_size; j++)
							printf("%c", b[i].name[j]);
						printf("'\n");
					}
				break;
			case 'f':
				printf("enter the key: ");
				scanf("%s", key);
				for (int i = 0; i < count; i++)
					if (!strncmp(key, b[i].key, strlen(key)) && strlen(key) == b[i].size_key) {
						k = 1;
						printf("Your element: '");
						for (int j = 0; j < b[i].name_size; j++)
							printf("%c", b[i].name[j]);
						printf("'\n");
				}
				if (k == 0)
					printf("element is not found\n");
				else
					k = 0;
				break;
			default:
				printf("Wrong format!");
		}
		getchar();
	}			
	//clock_t end = clock();
	printf("use time: %f", (double)(end - start)/CLOCKS_PER_SEC);
	remove("134krfkewlkew;klewl.txt");
	//fclose(in);
	return 0;		
}

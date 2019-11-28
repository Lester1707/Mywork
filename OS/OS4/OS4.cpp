#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>

using namespace std;

void fact(int n, int* out) {
	if (n != 1 && n != 0) {
		pid_t pid = fork();
		if (pid) {
			wait(NULL);
			*out = *out * n;
		} else if (pid == 0) {
			fact(n - 1, out);
			exit(0);
		} else {
			perror("fork");
			exit(1);
		}
	}
}

int main() {
	int input;
	const int start = 1;
    int shm = shm_open("shm", O_CREAT | O_TRUNC | O_RDWR, 0666);
    if (shm == -1) {
        exit(-1);
    }
    int r = ftruncate(shm, sysconf(_SC_PAGE_SIZE));
    if (r != 0) {
        exit(-2);
    }
    int* adr = (int*)mmap(0, sysconf(_SC_PAGE_SIZE), PROT_WRITE|PROT_READ, MAP_SHARED, shm, 0);
    if (adr == MAP_FAILED) {
        exit(-3);
    }
    close(shm);
    int* value = adr;
    *value = start;
	cout << "input the number: ";
	cin >> input;
	while (input < 0) {
		cout << "Number should be positive\nTry again: ";
		cin >> input;
	}
	fact(input, value);
	cout << "answer: " << *value << endl;
	return 0;
}

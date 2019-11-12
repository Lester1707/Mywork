#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>

using namespace std;

void if_stop_prog(size_t size) {
	if (size != sizeof(int)) {
		cout << "Wrong write pipe\n";
		exit(-1);
	} else if (size < 0) {
		cout << "Wrong read pipe\n";
		exit(-1);
	}
}

void fact(int n, int fd[]) {
	if (n != 1 && n != 0) {
		size_t size;
		pid_t pid = fork();
		if (pid) {
			wait(NULL);
			int out;
			size = read(fd[0], &out, sizeof(int));
			if_stop_prog(size);
			out = out * n;
			size = write(fd[1], &out, sizeof(int));
			if_stop_prog(size);
		} else if (pid == 0) {
			fact(n - 1, fd);
			exit(0);
		} else {
			perror("fork");
			exit(1);
		}
	}
}

int main() {
	size_t size;
	int a;
	const int start = 1;
	int fd[2];
	cout << "input the number: ";
	cin >> a;
	while (a < 0) {
		cout << "Number should be positive\nTry again: ";
		cin >> a;
	}
	if(pipe(fd) < 0) {
		cout << "Cant open pipe\n";
		exit(-1);
	}
	size = write(fd[1], &start, sizeof(int));
	if_stop_prog(size);
	fact(a, fd);
	size = read(fd[0], &a, sizeof(int));
	if_stop_prog(size);
	cout << "answer: " << a << endl;
	if (close(fd[0]) < 0) {
		cout << "Cant close input stream\n";
	}
	if (close(fd[1]) < 0) {
		cout << "Cant close output stream\n";
	}
	return 0;
}

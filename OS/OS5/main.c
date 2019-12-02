#include "tree.c"
#include "tree.h"
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

void add_method(Node* argument1, int argument2)
{
	void *dl_handle;
	void (*func)(Node*, int);
	char *error;
 
	dl_handle = dlopen("libtree.so", RTLD_LAZY);
	if (!dl_handle) {
		printf("!!! %s\n", dlerror());
		return;
	}
 
	func = dlsym(dl_handle, "add_node");
	error = dlerror();
	if (error != NULL) {
		printf("!!! %s\n", error);
		return;
	}
	
	func(argument1, argument2);

	dlclose( dl_handle );
	return;
}

void delete_method(Node* argument1, int argument2)
{
	void *dl_handle;
	Node* (*func)(Node*, int);
	char *error;
 
	dl_handle = dlopen("libtree.so", RTLD_LAZY);
	if (!dl_handle) {
		printf("!!! %s\n", dlerror());
		return;
	}
 
	func = dlsym(dl_handle, "delete_node");
	error = dlerror();
	if (error != NULL) {
		printf("!!! %s\n", error);
		return;
	}
	
	func(argument1, argument2);

	dlclose( dl_handle );
	return;
}

void print_method(Node* argument1)
{
	void *dl_handle;
	void (*func)(Node*);
	char *error;
 
	dl_handle = dlopen("libtree.so", RTLD_LAZY);
	if (!dl_handle) {
		printf("!!! %s\n", dlerror());
		return;
	}
 
	func = dlsym(dl_handle, "print_tree");
	error = dlerror();
	if (error != NULL) {
		printf("!!! %s\n", error);
		return;
	}
	
	func(argument1);

	dlclose( dl_handle );
	return;
}

int main()
{
	Node* a;
	char c;
	int value;
	int j = 0;
	printf("Choose event!\n\tprint r to the stop program\n\tprint a to add node\n\tprint p to print tree\n\tprint d to delete any node\n");
	while (scanf("%c", &c)) {
		switch(c) {
			case('a'):
				printf("in the num: ");
				scanf("%d", &value);
				if (j == 0) {
					a = create_node(value);
					j = 1;
				} else {
					add_method(a, value);	
				}
				printf("Choose event!\n");
				break;
			case('d'):
				if (j == 0) {
					printf("tree is empety!\n");
					printf("Choose event!\n");
					break;
				}
				printf("in the num: ");
				scanf("%d", &value);
				if (value == a->data) {
					a = delete_node(a, value);
				} else {
					delete_method(a, value);
				}
				printf("Choose event!\n");
				break;
			case('p'):
				if (j == 0) {
					printf("tree is empety!\n");
					printf("Choose event!\n");
					break;
				}
				print_method(a);
				printf("Choose event!\n");
				break;
			case('r'):
				if (j != 0)
					delete_tree(a);
				return 0;
			default:
				printf("Wrong format!\n");
				printf("Choose event!\n");
		}
		getchar();
	}
	delete_tree(a);
	return 0;
}

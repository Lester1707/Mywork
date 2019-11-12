#include "tree.h"

int main()
{
	Node* a;
	char c;
	int value;
	int j = 0;
	printf("Choose event!\n\tprint r to the stop program\n\tprint a to add node\n\tprint p to print tree\n\tprint d to delete any node\n\tprint l to know tree is list or no\n");
	while (scanf("%c", &c)) {
		switch(c) {
			case('a'):
				printf("in the num: ");
				scanf("%d", &value);
				if (j == 0) {
					a = create_node(value);
					j = 1;
				} else {
					add_node(a, value);	
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
					delete_node(a, value);
				}
				printf("Choose event!\n");
				break;
			case('p'):
				if (j == 0) {
					printf("tree is empety!\n");
					printf("Choose event!\n");
					break;
				}
				print_tree(a);
				printf("Choose event!\n");
				break;
			case('r'):
				if (j != 0)
					delete_tree(a);
				return 0;
			case('l'):
				if (j == 0) {
					printf("tree is empety!\n");
					printf("Choose event!\n");
					break;
				}
				printf("%d\n", list_is_tree(a));
				printf("Choose event!\n");
				break;
			default:
				printf("Wrong format!\n");
				printf("Choose event!\n");
		}
		getchar();
	}
	delete_tree(a);
	return 0;
}

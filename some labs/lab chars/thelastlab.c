#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "tree.c"
#include "list.c"

int is_number(char a) {
	switch(a) {
		case'x':
			return 1;
		case'0':
			return 1;
		case'1':
			return 1;
		case'2':
			return 1;
		case'3':
			return 1;
		case'4':
			return 1;	
		case'5':
			return 1;
		case'6':
			return 1;
		case'7':
			return 1;
		case'8':
			return 1;
		case'9':
			return 1;	
		default:
			return 0;
	}
	return 0;
}

int find_x_help(Node* node, int count)
{
	int k1 = 0, k2 = 0;
	if (l_num(node->data, 1) == 'x')
		count++;
	if (node->son_left)
		k1 = find_x_help(node->son_left, count);
	if (node->son_right)
		k2 = find_x_help(node->son_right, count);
	return k1 + k2 + count;
}

int find_x(Node* node)
{
	return find_x_help(node, 0);
}

int is_operand(char a)
{
	switch(a) {
		case'+':
			return 1;
		case'-':
			return 1;
		case'*':
			return 1;
		case'/':
			return 1;
		case'^':
			return 1;	
		case'(':
			return 1;
		case')':
			return 1;
		default:
			return 0;
	}
	return 0;
}

int power_operand(char a)
{
	switch(a) {
		case'+':
			return 2;
		case'/':
			return 3;
		case'-':
			return 2;
		case'*':
			return 3;
		case'^':
			return 4;	
		case'(':
			return 1;
		case')':
			return 1;
		default:
			return 0;
	}
	return 0;
}

int is_brakets_left(char a)
{
	if (a == '(')
		return 1;
	return 0;
}

int is_brakets_right(char a)
{
	if (a == ')')
		return 1;
	return 0;
}

int binar_operands(char a)
{
	switch(a) {
		case'+':
			return 1;
		case'-':
			return 1;
		case '*':
			return 1;
		case '^':
			return 1;
		case'/':
			return 1;
		default:
			return 0;
	}
	return 0;
}

char tree_num(Node* node)
{
	return l_num(node->data, 1);
}

void tree_change_num(Node* node, char a)
{
	while (node->data->size != 0)
		l_pop(node->data, 1);
	push_back(node->data, a);
}

List* tree_list(Node* node) {
	List* a = create_list();
	for (int i = 1; i <= node->data->size; i++)
		push_back(a, l_num(node->data, i));
	return a;
}

void tree_comply(Node* node)
{
	Node* help;
	Node* c;
	if (tree_num(node) == '*' && tree_num(node->son_right) == '^' && is_number(tree_num(node->son_right->son_right))) {
		help = copy_tree(node);
		delete_tree(node->son_left);
		delete_tree(node->son_right);
		tree_change_num(node, '+');
		node->son_left = copy_tree(help);
		node->son_right = copy_tree(help);
		c = node->son_left;
		l_destroy(c->son_left->data);
		c->son_left->data = tree_list(help->son_right->son_right);
		if (c->son_left->son_left) {
			delete_tree(c->son_left->son_left);
			c->son_left->son_left = NULL;
		}
		if (c->son_left->son_right) {
			delete_tree(c->son_left->son_right);
			c->son_left->son_right = NULL;
		}
		c = node->son_right;
		delete_tree(c->son_left);
		c->son_left = create_node(create_list());
		push_back(c->son_left->data, '-');
		c = c->son_left;
		c->son_right = create_node(create_list());
		l_destroy(c->son_right->data);
		c->son_right->data = tree_list(help->son_right->son_right);
		if (c->son_left){
			delete_tree(c->son_left);
			c->son_left = NULL;
		}
		c->son_left = copy_tree(help->son_left);
		delete_tree(help);
	} else {
		if (node->son_left)
			tree_comply(node->son_left);
		if (node->son_right)
			tree_comply(node->son_right);
	}
}

void tree_analis(Node* node)
{
	Node* help;
	if (tree_num(node) != '*') {
		if (tree_num(node) == '^' && is_number(tree_num(node->son_left))) {
			help = copy_tree(node);
			delete_tree(node->son_left);
			delete_tree(node->son_right);
			tree_change_num(node, '*');
			node->son_right = copy_tree(help);
			node->son_left = create_node(create_list());
			push_back(node->son_left->data, '1');
			delete_tree(help);
		} else if (tree_num(node) == 'x') {
			tree_change_num(node, '*');
			node->son_left = create_node(create_list());
			push_back(node->son_left->data, '1');
			node->son_right = create_node(create_list());
			push_back(node->son_right->data, 'x');
		} else {
			if (node->son_left)
				tree_analis(node->son_left);
			if (node->son_right)
				tree_analis(node->son_right);
		}
	} else if (tree_num(node->son_right) == 'x') {
		tree_change_num(node->son_right, '^');
		node->son_right->son_left = create_node(create_list());
		push_back(node->son_right->son_left->data, 'x');
		node->son_right->son_right = create_node(create_list());
		push_back(node->son_right->son_right->data, '1');
	}		
}

int left_social(char a)
{
	switch(a) {
		case'/':
			return 1;
		case '-':
			return 1;
		case '+':
			return 1;
		case '*':
			return 1;
		default:
			return 0;
	}
	return 0;
}

void my_add(List* a, char c)
{
	push_back(a, c);
	push_back(a, ' ');
}

int main() 
{
	char ch;
	List* chars = create_list();
	List* out = create_list();
	List* stack = create_list();
	Node** nodes = malloc(sizeof(Node*));
	Node* use;
	int count_nodes = 0;
	while (1) {
		ch = getchar();
		if (ch == EOF) {
			push_back(chars, '|');
			break;
		}
		push_back(chars, ch);
	}
	for (int i = 1; i <= chars->size; i++)
		if (l_num(chars, i) == ' ') {
			l_pop(chars, i);
			i--;
		}
	for (int i = 1; i <= chars->size; i++)
		if (l_num(chars, i) == '-') {
			if (i == 1) {
				push_front(chars, '0');
			} else if (is_operand(l_num(chars, i - 1))) {
				V_insert(chars, i, '0');
			}
		}
	for (int i = 1; i <= chars->size; i++) {
		if (is_number(l_num(chars, i))) {
			while (1) {
				push_back(out, l_num(chars, i));
				if (!is_number(l_num(chars, i + 1))) {
					push_back(out, ' ');
					break;
				}
				i++;
			}
		} else if (is_operand(l_num(chars, i)) || l_num(chars, i) == '|') {
			if (l_num(chars, i) == '|') {
					while (1) {
						if (stack->size == 0)
							break;
						my_add(out, l_pop(stack, stack->size));
					}
					break;
			} else if (is_brakets_right(l_num(chars, i))) {
				while (1) {
					if (is_brakets_left(l_num(stack, stack->size)))
						break;
					my_add(out, l_pop(stack, stack->size));
				}
				l_pop(stack, stack->size);
				continue;
			} else if (is_brakets_left(l_num(chars, i))) {
				push_back(stack, l_num(chars, i));
				continue;
			} else if (stack->size > 0) {
				while (1) {
					if (binar_operands(l_num(chars, i))) {
						if ( (power_operand(l_num(chars, i)) < power_operand(l_num(stack, stack->size))) || 
						(left_social(l_num(stack, stack->size)) && (power_operand(l_num(chars, i)) == power_operand(l_num(stack, stack->size))) ) ) {
							my_add(out, l_pop(stack, stack->size));
							if (stack->size == 0)
								break;
						} else {
							break;
						}
					}
				}
			}
			push_back(stack, l_num(chars, i));
		}
	}
	l_print(out);
	printf("\n");
	for (int i = 1; i <= out->size; i++)
		if (l_num(out, i) != ' ') {
			nodes[count_nodes] = create_node(create_list());
			count_nodes++;
			nodes = realloc(nodes, (count_nodes + 1)*sizeof(Node*));
			if (is_number(l_num(out, i))) {
				while (1) {
					push_back(nodes[count_nodes - 1]->data, l_num(out, i));
					if (is_number(l_num(out, i + 1)) == 0)
						break;
					i++;
				}
			} else {
				push_back(nodes[count_nodes - 1]->data, l_num(out, i));
			}
		}
	while (count_nodes != 1)
		for (int i = 0; i < count_nodes - 2; i++)
			if ((is_number(l_num(nodes[i]->data, 1)) || nodes[i]->son_left != NULL) && (is_number(l_num(nodes[i + 1]->data, 1)) || nodes[i + 1]->son_left != NULL) && (is_operand((l_num(nodes[i + 2]->data, 1))) && nodes[i + 2]->son_left == NULL)) {
				nodes[i + 2]->son_left = nodes[i];
				nodes[i + 2]->son_right = nodes[i + 1];
				for (int j = i; j < count_nodes - 1; j++)
					nodes[j] = nodes[j + 1];
				count_nodes--;
				nodes = realloc(nodes, (count_nodes)*sizeof(Node*));
				for (int j = i; j < count_nodes - 1; j++)
					nodes[j] = nodes[j + 1];
				count_nodes--;
				nodes = realloc(nodes, (count_nodes)*sizeof(Node*));
				i--;
			}
	use = copy_tree(nodes[0]);
	delete_tree(nodes[0]);
	free(nodes);
	print_tree(use);
	if (out->size == 2 || find_x(use) > 0) {
		tree_analis(use);
		tree_comply(use);
		printf("-----------------\n");
		print_tree(use);
	}
	delete_tree(use);
	l_destroy(chars);
	l_destroy(out);
	l_destroy(stack);
	return 0;
}

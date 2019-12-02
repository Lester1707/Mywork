#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

Node* create_node(use_data value)
{
	Node* node;
	node = malloc(sizeof(Node));
	node->data = value;
	node->root = 1;
	node->son_left = NULL;
	node->son_right = NULL;
	return node;
}

void add_node(Node* node, use_data value)
{
	if (node->root == 0) {
		node->data = value;
		node->root = 1;
	} else {
		Node* a;
		a = malloc(sizeof(Node));
		a->data = value;
		a->root = 2;
		a->son_left = NULL;
		a->son_right = NULL;
		Node* c;
		c = node;
		while (1) {
			if (value < c->data) {
				if (c->son_left == NULL) {
					c->son_left = a;
					break;
				}
				c = c->son_left;
			} else {
				if (c->son_right == NULL) {
					c->son_right = a;
					break;
				}
				c = c->son_right;
			}
		}
	}
}

void delete_tree(Node* node)
{
	if (node->son_left) {
		delete_tree(node->son_left);
		node->son_left = NULL;
	} 
	if (node->son_right) {
		delete_tree(node->son_right);
		node->son_right = NULL;
	}
	free(node);
}

Node* copy_tree(Node* node)
{
	Node* new = malloc(sizeof(Node));
	new->root = node->root;
	if (node->son_left)
		new->son_left = copy_tree(node->son_left);
	else
		new->son_left = NULL;
	if (node->son_right)
		new->son_right = copy_tree(node->son_right);
	else
		new->son_right = NULL;
	return new;
}

Node* delete_node(Node* node, use_data value)
{
	if (node->root == 0) {
		printf("tree is empety\n");
	} else {
		use_data var;
		int l = 0;
		int r = 0;
		Node* c = node;
		Node* d = node;
		Node* h;
		while (1) {
			if (value < c->data) {
				if (c->son_left == NULL) {
					break;
				}
				d = c;
				c = c->son_left;
				l = 1;
				r = 0;
			}
			if (value > c->data) {
				if (c->son_right == NULL) {
					break;
				}
				d = c;
				c = c->son_right;
				r = 1;
				l = 0;
			}
			if (value == c->data) {
				if (c->son_left != NULL && c->son_right != NULL && c->son_left->son_right != NULL) {
					d = c;
					c = c->son_left;
					while (c->son_right->son_right) {
						c = c->son_right;
					}
					var = c->son_right->data;
					h = c->son_right;
					c->son_right = c->son_right->son_left;
					h->son_left = NULL;
					delete_tree(h);
					d->data = var;
				} else if (c->son_right != NULL && c->son_left == NULL) {
					if (l == 1) {
						d->son_left = c->son_right;
						c->son_right = NULL;
						delete_tree(c);
					} else if (r == 1) {
						d->son_right = c->son_right;
						c->son_right = NULL;
						delete_tree(c);
					} else {
						node = node->son_right;
						node->root = 1;
						c->son_right = NULL;
						delete_tree(c);
						return node;
					}
				} else if (c->son_left != NULL) {
					if (r == 1) {
						d->son_right = c->son_left;
						c->son_left = NULL;
						if (c->son_right != NULL) {
							d->son_right->son_right = c->son_right;
							c->son_right = NULL;
						}
						delete_tree(c);
					} else if (l == 0 && r == 0) {
							node = node->son_left;
							node->root = 1;
							if (c->son_right != NULL) {
								node->son_right = c->son_right;
							}
							c->son_left = NULL;
							c->son_right = NULL;
							delete_tree(c);
							return node;
					} else if (l == 1) {
						c->data = c->son_left->data;
						d = c->son_left;
						c->son_left = c->son_left->son_left;
						d->son_left = NULL;
						delete_tree(d);
					}
				} else {
					if (c->root == 1) {
						node->root = 0;
						break;
					}
					delete_tree(c);
					if (l == 1) {
						d->son_left = NULL;
					} else {
						d->son_right = NULL;
					}
				}
				break;
				return NULL;
			}
		}
	}
	return NULL;
}

void print_tree_help(Node* node, int count)
{
	if (node->root == 0) {
		printf("tree is empety\n");
	} else {
		int l = 0;
		for (int i = 0; i < count; i++)
			printf("\t");
		printf("%d", node->data);
		printf("\n");
		if (node->son_left) {
			l = 1;
			print_tree_help(node->son_left, count + 1);
		}
		if (node->son_right) {
			if (l == 0) {
				for (int i = 0; i < count + 1; i++)
					printf("\t");
				printf("\t");
				printf("\n");
			}
			print_tree_help(node->son_right, count + 1);
		}
	}
}

void print_tree(Node* node)
{
	print_tree_help(node, 0);
}

int list_is_tree(Node* node)
{
	if (node->root != 1) {
		return 1;
	}
	Node* c = node;
	while (1) {
		if (c->son_left) {
			if (c->son_right) {
				return 0;
			}
			c = c->son_left;
		} else if (c->son_right) {
			if (c->son_left)
				return 0;
			c = c->son_right;
		} else {
			return 1;
		}
	}
}

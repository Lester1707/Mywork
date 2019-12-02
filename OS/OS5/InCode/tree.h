#ifndef __tree_h__
#define __tree_h__

//#include "list.h"

typedef int use_data;

typedef struct Node {
    use_data data;
    struct Node *son_left;
    struct Node *son_right;
    int root;
} Node;

Node* create_node(use_data value);
void add_node(Node* node, use_data value);
void delete_tree(Node* node);
Node* delete_node(Node* node, use_data value);
void print_tree(Node* node);
Node* copy_tree(Node* node);
int list_is_tree(Node* node);

#endif

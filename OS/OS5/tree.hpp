#include <iostream>

using namespace std;


class Node {
  public:
    int _value;
    Node *left;
    Node *right;
    
    Node(int value) {
        _value = value;
        left = NULL;
        right = NULL;
    } Node():_value(0), left(NULL), right(NULL) {
    }

    void SetLeft(Node * ptr) {
        left = ptr;
    }

    void SetRight(Node * ptr) {
        right = ptr;
    }

    void SetValue(int iValue) {
        _value = iValue;
    }

    int GetValue() {
        return _value;
    }

    Node *GetLeft() {
        return left;
    }

    Node *GetRight() {
        return right;
    }

};

class Tree {
  private:
    Node * head;
  public:
    Tree():head(NULL) {
    } Tree(int value) {
        Node *node = new Node(value);
        head = node;
    }

    Tree(Node * node) {
        head = node;
    }

    bool Add(Node * node, int value) {
        if (head == NULL) {
            Node *node = new Node(value);
            head = node;
            return true;
        }
        if (value >= node->GetValue()) {
            if (node->GetRight() != NULL)
                return Add(node->GetRight(), value);
            else {
                Node *newNode = new Node(value);
                node->SetRight(newNode);
                return true;
            }
        } else {
            if (node->GetLeft() != NULL)
                Add(node->GetLeft(), value);
            else {
                Node *newNode = new Node(value);
                node->SetLeft(newNode);
                return true;
            }
        }
        return true;
    }

    bool Add(int value) {
        if (head == NULL) {
            Node *node = new Node(value);
            head = node;
            return true;
        }
        if (value >= head->GetValue()) {
            if (head->GetRight() != NULL)
                return Add(head->GetRight(), value);
            else {
                Node *newNode = new Node(value);
                head->SetRight(newNode);
                return true;
            }
        } else {
            if (head->GetLeft() != NULL)
                return Add(head->GetLeft(), value);
            else {
                Node *newNode = new Node(value);
                head->SetLeft(newNode);
                return true;
            }
        }
        return true;
    }

    bool search(int ivalue) {
        if (ivalue == head->GetValue()) {
            return true;
        } else {
            return false;
        }
    }
    
    Node* RemoveNode(Node * root, int x)
    {
        Node *t = new Node;
        if (root == NULL)
            return NULL;
        if (x == root->_value) {
            if (root->left == NULL) {
                t = root->right;
                delete root;
                return t;
            }
            t = root->left;
            while (t->right) {
                t = t->right;
            }
            t->right = root->right;
            return root->left;
        }
        if (x < root->_value)
            root->left = RemoveNode(root->left, x);
        else
            root->right = RemoveNode(root->right, x);
        return root;
    }
    
    void print(void) {
        cout << "tree structure\n";
    }
};

#include <iostream>
#include "myvector.hpp"

const int red = 1;
const int black = 0;

using namespace std;

struct Data {
	TVector <char> key;
	TVector <int> value;
    bool color;
};

void PrintDateR(Data &date) {
    for (int i = 0; i < date.key.Len(); i++) {
        printf("\033[31m%c\033[0m", date.key[i]);
    }
    putchar(' ');
    for (int i = 0; i < date.value.Len(); i++) {
        printf("\033[31m%d\033[0m", date.value[i]);
    }
    putchar('\n');
}

void PrintDateB(Data &date) {
    for (int i = 0; i < date.key.Len(); i++) {
        printf("%c", date.key[i]);
    }
    putchar(' ');
    for (int i = 0; i < date.value.Len(); i++) {
        printf("%d", date.value[i]);
    }
    putchar('\n');
}

void ClearData(Data &date) {
    date.key.Clear();
    date.value.Clear();
}

bool IsBigger(TVector <char> &vec1, TVector <char> &vec2) {
    if (vec1.Len() > vec2.Len()) {
        return 1;
    } else if (vec1.Len() < vec2.Len()) {
        return 0;
    } else {
        for (int i = 0; i < vec1.Len(); i++) {
            if (vec1[i] > vec2[i]) {
                return 1;
            }
        }
    }
    return 0;
}

class RBTree {
    protected:
        Data root;
        RBTree* left;
        RBTree* right;
        RBTree* father;
        int size;
    public:
        RBTree() {
            root.color = black;
            size = 0;
            left = nullptr;
            right = nullptr;
            father = nullptr;
        }
        RBTree(Data &em) {
            root.color = red;
            root = em;
            size = 1;
            left = nullptr;
            right = nullptr;
            father = nullptr;
        }
        friend void BalanceTreeL(RBTree* tree) {
            if (!tree->father) {
                return;
            }
            if () {
                RBTree* ancle_r = tree->father->father->right;
                ancle_r->root.color = black;
                tree->father->root.color = black;
                BalanceTreeL(ancle_r->father);
            }
            
            if (tree->father->father) {
                RBTree* help = tree->father->right;
                tree->father->right = tree->father->father;
                tree->father->father = tree->father->right->father;
                tree->father->right->left = help;
            }
            
            if (tree->father->father) {
                RBTree* help = tree->father->left;
                tree->father->left = tree->father->father;
                tree->father->father = tree->father->left->father;
                tree->father->left->right = help;
            }
            
            if (1) {
                RBTree* help = tree->left;
                tree->left = tree->father;
                tree->father = tree->father->father;
                tree->left->right = help;
                tree->father->left = tree;
            }
            
            if (1) {
                RBTree* help = tree->right;
                tree->right = tree->father;
                tree->father = tree->father->father;
                tree->right->left = help;
                tree->father->right = tree;
            }
                
        }
        friend void BalanceTreeR(RBTree* tree) {
            if (tree->father->father->left) {
                RBTree* ancle_l = tree->father->father->left;
                ancle_l->root.color = black;
                tree->father->root.color = black;
                BalanceTreeR(ancle_l->father);
            }
            
            
            
        }
        void AddNode(Data &em) {
            RBTree* tree = this;
            if (!tree->size) {
                tree->root = em;
                tree->root.color = black;
                size++;
                return;
            }
            if (IsBigger(em.key, tree->root.key)) {
                if (right) {
                    tree->right->AddNode(em);
                } else {
                    RBTree* newtree = new RBTree(em);
                    tree->right = newtree;
                    tree->father = tree;
                    size++;
                }
            } else {
                 if (left) {
                    tree->left->AddNode(em);
                } else {
                    RBTree* newtree = new RBTree(em);
                    tree->left = newtree;
                    tree->father = tree;
                    size++;
                }
            }
        }
        void PrintTree(const int &count) {
            if (size == 0) {
                return;
            }
            int kl = 0;
            for (int i = 0; i < count; i++) {
                printf("    ");
            }
            if (root.color) {
                PrintDateR(root);
            } else {
                PrintDateB(root);
            }
            if (left) {
                kl = 1;
                left->PrintTree(count + 1);
            }
            if (right) {
                if (kl == 0) {
                    for (int i = 0; i < count + 1; i++) {
                        printf("    ");
                    }
                    printf("    ");
                    printf("\n");
                }
                right->PrintTree(count + 1);
            }
        }
        ~RBTree() {
            if (left) {
                delete left;
                left = nullptr;
            }
            if (right) {
                delete right;
                right = nullptr;
            }
            father = nullptr;
        }
};

int Cti(const char &em) {
    return em - '0';
}

int main() {
    RBTree tree;
    Data get_data;
    char ch;
    do {
        ch = getchar();
        switch (ch) {
            case '+':
                getchar();
                while (true) {
                    ch = getchar();
                    if (ch == ' ') {
                        break;
                    }
                    get_data.key.PushBack(ch);
                }
                while (true) {
                    ch = getchar();
                    if (ch == '\n') {
                        break;
                    }
                    get_data.value.PushBack(Cti(ch));
                }
                tree.AddNode(get_data);
                tree.PrintTree(0);
                ClearData(get_data);
                break;
            case '-':
                break;
            case '!':
                break;
            default:
                break;
        }
    } while (ch != EOF);
    return 0;
}

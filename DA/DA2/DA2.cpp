#include <iostream>
#include "myvector.hpp"

const bool red = 1;
const bool black = 0;

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
            } else if (vec1[i] < vec2[i]) {
				return 0;
			}
        }
    }
    return 1;
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
            root = em;
            root.color = red;
            size = 1;
            left = nullptr;
            right = nullptr;
            father = nullptr;
        }
        friend void BalanceTreeL(RBTree* tree) {
            if (!(tree->root.color == red && tree->father->root.color == red)) {
                return;
            }
            if (!tree->father->father) {
                tree->father->root.color = black;
                return;
            }
            if (tree->father->father->right && tree->father->father->right != tree->father && tree->father->father->right->root.color == red) {
                RBTree* ancle_r = tree->father->father->right;
                ancle_r->root.color = black;
                tree->father->root.color = black;
                if (ancle_r->father->father) {
                    tree->father->father->root.color = red;
                    if (ancle_r->father->father->left == ancle_r->father) {
                        BalanceTreeL(ancle_r->father);
                    } else if (ancle_r->father->father->right == ancle_r->father) {
                        BalanceTreeR(ancle_r->father);
                    }
                }
            } else if (tree->father->father->right && tree->father->father->left && tree->father->father->right->root.color == red && tree->father->father->left->root.color == red) {
                RBTree* ancle_r = tree->father->father->left;
                ancle_r->root.color = black;
                tree->father->root.color = black;
                if (ancle_r->father->father) {
                    tree->father->father->root.color = red;
                    if (ancle_r->father->father->left == ancle_r->father) {
                        BalanceTreeL(ancle_r->father);
                    } else if (ancle_r->father->father->right == ancle_r->father) {
                        BalanceTreeR(ancle_r->father);
                    }
                }
            } else if (tree->father->father->right == tree->father && (!tree->father->father->left || tree->father->father->left->root.color == black)) {
                RBTree* help = tree->right;
                tree->right = tree->father;
                tree->father = tree->father->father;
                tree->right->left = help;
                tree->father->right = tree;
                tree->right->father = tree;
                if (tree->right->left) {
                    tree->right->left->father = tree->right;
                }
                BalanceTreeR(tree->right);
            } else if (!tree->father->father->right || tree->father->father->right->root.color == black) {
                if (!tree->father->father->father) {
                    Data help1d;
                    RBTree* help2;
                    RBTree* general = tree->father->father;
                    help1d = general->root;
                    general->root = general->left->root;
                    general->left->root = general->left->left->root;
                    help2 = general->left->right;
                    general->left->left = tree->left;
                    general->left->right = tree->right;
                    if (general->left->left) {
                        general->left->left->father = general->left;
                    }
                    if (general->left->right) {
                        general->left->right->father = general->left;
                    }
                    if (general->right) {
                        Data help2d;
                        RBTree* help3;
                        help3 = general->right;
                        general->right = tree;
                        general->right->root = help1d;
                        general->right->left = help2;
                        if (general->right->left) {
                            general->right->left->father = general->right;
                        }
                        general->right->right = help3;
                        general->right->right->father = general->right;
                        tree->father = general;
                    } else {
                        general->right = tree;
                        general->right->father = general;
                        general->right->left = help2;
                        if (general->right->left) {
                            general->right->left->father = general->right;
                        }
                        general->right->right = nullptr;
                        general->right->root = help1d;
                    }
                    general->root.color = black;
                    general->right->root.color = red;
                    general->left->root.color = red;
                    return;
                }
                RBTree* help = tree->father->right;
                tree->father->right = tree->father->father;
                tree->father->father = tree->father->right->father;
                tree->father->right->left = help;
                if (help) {
                    help->father = tree->father->right;
                }
                tree->father->right->father = tree->father;
                if (tree->father->father->left == tree->father->right) {
                    tree->father->father->left = tree->father;
                } else if (tree->father->father->right == tree->father->right) {
                    tree->father->father->right = tree->father;
                }
                tree->father->root.color = black;
                tree->father->right->root.color = red;
            }
        }
        friend void BalanceTreeR(RBTree* tree) {
            if (!tree->father->father && tree->father->root.color == red) {
                tree->father->root.color = black;
                return;
            }
            if (!(tree->root.color == red && tree->father->root.color == red)) {
                return;
            }
            if (tree->father->father->left != nullptr && tree->father->father->left != tree->father && tree->father->father->left->root.color == red) {
                RBTree* ancle_r = tree->father->father->left;
                ancle_r->root.color = black;
                tree->father->root.color = black;
                if (ancle_r->father->father) {
                    tree->father->father->root.color = red;
                    if (ancle_r->father->father->left == ancle_r->father) {
                        BalanceTreeL(ancle_r->father);
                    } else if (ancle_r->father->father->right == ancle_r->father) {
                        BalanceTreeR(ancle_r->father);
                    }
                }
            } else if (tree->father->father->right && tree->father->father->left && tree->father->father->right->root.color == red && tree->father->father->left->root.color == red) {
                RBTree* ancle_r = tree->father->father->right;
                ancle_r->root.color = black;
                tree->father->root.color = black;
                if (ancle_r->father->father) {
                    tree->father->father->root.color = red;
                    if (ancle_r->father->father->left == ancle_r->father) {
                        BalanceTreeL(ancle_r->father);
                    } else if (ancle_r->father->father->right == ancle_r->father) {
                        BalanceTreeR(ancle_r->father);
                    }
                }
            } else if (tree->father->father->left == tree->father && (!tree->father->father->right || tree->father->father->right->root.color == black)) {
                RBTree* help = tree->left;
                tree->left = tree->father;
                tree->father = tree->father->father;
                tree->left->right = help;
                tree->father->left = tree;
                tree->left->father = tree;
                if (tree->left->right) {
                    tree->left->right->father = tree->left;
                }
                BalanceTreeL(tree->left);
            } else if (tree->father->father->left == nullptr || tree->father->father->left->root.color == black) {
                if (tree->father->father->father == nullptr) {
                    Data help1d;
                    RBTree* help2;
                    RBTree* general = tree->father->father;
                    help1d = general->root;
                    general->root = general->right->root;
                    general->right->root = general->right->right->root;
                    help2 = general->right->left;
                    general->right->right = tree->right;
                    general->right->left = tree->left;
                    if (general->right->right) {
                        general->right->right->father = general->right;
                    }
                    if (general->right->left) {
                        general->right->left->father = general->right;
                    }
                    if (general->left) {
                        Data help2d;
                        RBTree* help3;
                        help3 = general->left;
                        general->left = tree;
                        general->left->root = help1d;
                        general->left->right = help2;
                        if (general->left->right) {
                            general->left->right->father = general->left;
                        }
                        general->left->left = help3;
                        general->left->left->father = general->left;
                        tree->father = general;
                    } else {
                        general->left = tree;
                        general->left->father = general;
                        general->left->right = help2;
                        if (general->left->right) {
                            general->left->right->father = general->left;
                        }
                        general->left->left = nullptr;
                        general->left->root = help1d;
                    }
                    general->root.color = black;
                    general->left->root.color = red;
                    return;
                }
                RBTree* help = tree->father->left;
                tree->father->left = tree->father->father;
                tree->father->father = tree->father->left->father;
                tree->father->left->right = help;
                if (help) {
                    help->father = tree->father->left;
                }
                tree->father->left->father = tree->father;
                if (tree->father->father->right == tree->father->left) {
                    tree->father->father->right = tree->father;
                } else if (tree->father->father->left == tree->father->left) {
                    tree->father->father->left = tree->father;
                }
                tree->father->root.color = black;
                tree->father->left->root.color = red;
            }
        }
        void check_red() {
            if (left) {
                if (root.color == red && left->root.color == red)
                    cout << "1";
                left->check_red();
            }
            if (right) {
                if (root.color == red && right->root.color == red)
                    cout << "1";
                right->check_red();
            }
        }
        void check_size() {
            if (left) {
                if (!IsBigger(root.key, left->root.key))
                    cout << "2";
                left->check_size();
            }
            if (right) {
                if (IsBigger(root.key, right->root.key))
                    cout << "2";
                right->check_size();
            }
        }
        void AddNode(Data &em) {
            RBTree* tree = this;
            if (!tree->size) {
                tree->root = em;
                tree->root.color = black;
                tree->size++;
                return;
            }
            if (IsBigger(em.key, tree->root.key)) {
                if (right) {
                    tree->right->AddNode(em);
                } else {
                    RBTree* newtree = new RBTree(em);
                    tree->right = newtree;
                    newtree->father = tree;
                    BalanceTreeR(newtree);
                }
            } else {
                 if (left) {
                    tree->left->AddNode(em);
                } else {
                    RBTree* newtree = new RBTree(em);
                    tree->left = newtree;
                    newtree->father = tree;
                    BalanceTreeL(newtree);
                }
            }
            tree->size++;
        }
        RBTree* SearchNode(TVector <char> &em) {
            RBTree* search = nullptr;
            if (em == root.key) {
                return this;
            } else if (IsBigger(em, root.key)) {
                if (right) {
                    search = right->SearchNode(em);
                }
                if (search) {
                    return search;
                }
            } else {
                 if (left) {
                    search = left->SearchNode(em);
                }
            }
            return search;
        }
        
        void DeleteNode(TVector <char> &em) {
            RBTree* tree = SearchNode(em);
            if (!tree) {
                return;
            }
            if (tree->left && !tree->right) {
                //cout << "0";
                bool l;
                bool col = tree->root.color;
                RBTree* new_node = tree->left;
                if (tree->father && tree->father->left == tree) {
                    l = 1;
                    tree->father->left = tree->left;
                } else if (tree->father && tree->father->right == tree) {
                    l = 0;
                    tree->father->right = tree->left;
                } else {
                    tree->root = tree->left->root;
                    tree->root.color = black;
                    tree->right = tree->left->right;
                    tree->left = tree->left->left;
                    if (new_node->left) {
                        new_node->left->father = tree;
                        new_node->left = nullptr;
                    }
                     if (new_node->right) {
                        new_node->right->father = tree;
                        new_node->right = nullptr;
                    }
                    delete new_node;
                    return;
                }
                tree->left->father = tree->father;
                tree->left = nullptr;
                delete tree;
                if (col == black) {
                    if (new_node->root.color == red) {
                        new_node->root.color = black;
                    } else if (new_node->father->root.color == red) {
                        new_node->father->root.color = black;
                    } else {
                         if (l) {
                            BlackBalanceL(new_node);
                        } else {
                            BlackBalanceR(new_node);
                        }
                        new_node = nullptr;
                    }
                }
                return;
            }
            if (!tree->left && tree->right) {
                //cout << "2";
                bool col = tree->root.color;
                bool l;
                RBTree* new_node = tree->right;
                if (tree->father && tree->father->left == tree) {
                    l = 1;
                    tree->father->left = tree->right;
                } else if (tree->father && tree->father->right == tree) {
                    l = 0;
                    tree->father->right = tree->right;
                } else {
                    tree->root = tree->right->root;
                    tree->root.color = black;
                    tree->left = tree->right->left;
                    tree->right = tree->right->right;
                    if (new_node->left) {
                        new_node->left->father = tree;
                        new_node->left = nullptr;
                    }
                     if (new_node->right) {
                        new_node->right->father = tree;
                        new_node->right = nullptr;
                    }
                    delete new_node;
                    return;
                }
                tree->right->father = tree->father;
                tree->right = nullptr;
                delete tree;
               if (col == black) {
                    if (new_node->root.color == red) {
                        new_node->root.color = black;
                    } else if (new_node->father->root.color == red) {
                        new_node->father->root.color = black;
                    } else {
                        if (l) {
                            BlackBalanceL(new_node);
                        } else {
                            BlackBalanceR(new_node);
                        }
                        new_node = nullptr;
                    }
                }
                return;
            }
            if (!tree->left && !tree->right) {
                //cout << "4";
                if (!tree->father) {
                    tree->size = 0;
                    tree->root.key.Clear();
                    tree->root.value.Clear();
                    return; 
                }
                if (tree->root.color == black) {
                    bool l;
                    if (tree->father->left == tree) {
                        l = 1;
                    } else if (tree->father->right == tree) {
                        l = 0;
                    }
                    if (l) {
                       BalanceTreeL(tree);
                       tree->father->left = nullptr;
                    } else {
                        cout << "in";
                        BalanceTreeR(tree);
                        cout << "out";
                        tree->father->right = nullptr;
                    }
                } else {
                    if (tree->father->left == tree) {
                        tree->father->left = nullptr;
                    } else {
                        tree->father->right = nullptr;
                    }
                }
                delete tree;
                return;
            }
            if (tree->left && tree->right) {
                //cout << "3";
                RBTree* help = tree->right;
                while (true) {
                    if (!help->left) {
                        break;
                    }
                    help = help->left;
                }
                tree->root.key = help->root.key;
                tree->root.value = help->root.value;
                help->DeleteNode(help->root.key);
                help = nullptr;
            }
        }
        friend void BlackBalanceL(RBTree* tree) {
            if (tree == nullptr) {
                return;
            }
            if (!tree->father) {
                return;
            }
            if (tree->father->right && tree->father->right->root.color == red) {
                //cout << "1l";
                RBTree* general = tree->father;
                RBTree* help1 = general->right->left;
                RBTree* help2 = general->right->right;
                RBTree* help3 = general->left; 
                Data help = general->root;
                general->root = general->right->root;
                general->root.color = black;
                if (!help2 && !help1) {
                    general->left = general->right;
                    general->right = nullptr;
                    general->left->left = help3;
                    help3->father = general->left;
                    general->left->root = help;
                    general->left->root.color = red;
                    return;
                }
                if (!help2 && help1) {
                    general->left = general->right;
                    general->right = nullptr;
                    general->left->left = help3;
                    help3->father = general->left;
                    general->left->right = help1;
                    general->left->root = help;
                    general->left->root.color = red;
                    return;
                }
                if (general->right->right) {
                    general->right->root = general->right->right->root;
                    general->right->right = general->right->right->right;
                    general->right->left = help2->left;
                }
                if (general->right->left) {
                    general->right->left->father = general->right;
                }
                if (general->right->right) {
                    general->right->right->father = general->right;
                }
                general->left = help2;
                general->left->father = general;
                general->left->left = help3;
                help3->father = general->left;
                general->left->root = help;
                general->left->root.color = red;
                general->left->right = help1;
                if (help1) {
                    help1->father = general->left;
                }
            } else if (tree->father->right && tree->father->right->root.color == black && (tree->father->right->left == nullptr || tree->father->right->left->root.color == black) && (tree->father->right->right == nullptr || tree->father->right->right->root.color == black)) {
                //cout << "2l";
                tree->father->right->root.color = red;
                tree->father->root.color = black;
            } else if (tree->father->right && tree->father->right->root.color == black && tree->father->right->left && tree->father->right->left->root.color == red && (tree->father->right->right == nullptr || tree->father->right->right->root.color == black)) {
                //cout << "3l";
                RBTree* help1 = tree->father->right->left->right;
                tree->father->right = tree->father->right->left;
                tree->father->right->right = tree->father->right->father;
                tree->father->right->father = tree->father;
                tree->father->right->right->father = tree->father->right;
                tree->father->right->right->left = help1;
                if (help1) {
                    help1->father = tree->father->right->right;
                }
            } else if (tree->father->right && tree->father->right->root.color == black && tree->father->right->right && tree->father->right->right->root.color == red) {
                //cout << "4l";
                RBTree* general = tree->father;
                RBTree* help1 = general->right->left;
                RBTree* help2 = general->right->right;
                RBTree* help3 = general->left; 
                Data help = general->root;
                general->root = general->right->root;
                general->root.color = help.color;
                general->right->root = general->right->right->root;
                general->right->right = general->right->right->right;
                general->right->left = help2->left;
                if (general->right->left) {
                    general->right->left->father = general->right;
                }
                if (general->right->right) {
                    general->right->right->father = general->right;
                }
                general->left = help2;
                general->left->father = general;
                general->left->left = help3;
                help3->father = general->left;
                general->left->root = help;
                general->left->root.color = black;
                general->left->right = help1;
                if (help1) {
                    help1->father = general->left;
                }
            } else {
                if (tree->father->father && tree->father->father->left == tree) {
                    BlackBalanceL(tree->father);
                } else if (tree->father->father && tree->father->father->right == tree) {
                    BlackBalanceR(tree->father);
                }
            }
        }
        friend void BlackBalanceR(RBTree* tree) {
            cout << "go";
            if (tree == nullptr) {
                return;
            }
            if (!tree->father) {
                return;
            }
            if (tree->father->left && tree->father->left->root.color == red) {
                cout << "1r";
                RBTree* general = tree->father;
                RBTree* help1 = general->left->right;
                RBTree* help2 = general->left->left;
                RBTree* help3 = general->right; 
                Data help = general->root;
                general->root = general->left->root;
                general->root.color = black;
                if (!help2 && !help1) {
                    general->right = general->left;
                    general->left = nullptr;
                    general->right->right = help3;
                    help3->father = general->right;
                    general->right->root = help;
                    general->right->root.color = red;
                    return;
                }
                if (!help2 && help1) {
                    general->right = general->left;
                    general->left = nullptr;
                    general->right->right = help3;
                    help3->father = general->right;
                    general->right->left = help1;
                    general->right->root = help;
                    general->right->root.color = red;
                    return;
                }
                if (general->left->left) {
                    general->left->root = general->left->left->root;
                    general->left->left = general->left->left->left;
                    general->left->right = help2->right;
                }
                if (general->left->right) {
                    general->left->right->father = general->left;
                }
                if (general->left->left) {
                    general->left->left->father = general->left;
                }
                general->right = help2;
                general->right->father = general;
                general->right->right = help3;
                help3->father = general->right;
                general->right->root = help;
                general->right->root.color = red;
                general->right->left = help1;
                if (help1) {
                    help1->father = general->right;
                }
            } else if (tree->father->left && tree->father->left->root.color == black && (tree->father->left->right == nullptr || tree->father->left->right->root.color == black) && (tree->father->left->left == nullptr || tree->father->left->left->root.color == black)) {
                cout << "2r";
                tree->father->left->root.color = red;
                tree->father->root.color = black;
            } else if (tree->father->left && tree->father->left->root.color == black && tree->father->left->right && tree->father->left->right->root.color == red && (tree->father->left->left == nullptr || tree->father->left->left->root.color == black)) {
                cout << "3r";
                RBTree* help1 = tree->father->left->right->left;
                tree->father->left = tree->father->left->right;
                tree->father->left->left = tree->father->left->father;
                tree->father->left->father = tree->father;
                tree->father->left->left->father = tree->father->left;
                tree->father->left->left->right = help1;
                if (help1) {
                    help1->father = tree->father->left->left;
                }
            } else if (tree->father->left && tree->father->left->root.color == black && tree->father->left->left && tree->father->left->left->root.color == red) {
                cout << "4r";
                RBTree* general = tree->father;
                RBTree* help1 = general->left->right;
                RBTree* help2 = general->left->left;
                RBTree* help3 = general->right; 
                Data help = general->root;
                general->root = general->left->root;
                general->root.color = help.color;
                general->left->root = general->left->left->root;
                general->left->left = general->left->left->left;
                general->left->right = help2->right;
                if (general->left->right) {
                    general->left->right->father = general->left;
                }
                if (general->left->left) {
                    general->left->left->father = general->left;
                }
                general->right = help2;
                general->right->father = general;
                general->right->right = help3;
                help3->father = general->right;
                general->right->root = help;
                general->right->root.color = black;
                general->right->left = help1;
                if (help1) {
                    help1->father = general->right;
                }
            } else {
                cout << "5r";
                if (tree->father->father && tree->father->father->left == tree) {
                    BlackBalanceL(tree->father);
                } else if (tree->father->father && tree->father->father->right == tree) {
                    BlackBalanceR(tree->father);
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
        void ShowTree() {
            PrintTree(0);
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
    TVector <char> del;
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
                ClearData(get_data);
                break;
            case '-':
                getchar();
                while (true) {
                    ch = getchar();
                    if (ch == '\n') {
                        break;
                    } else if (ch == ' ') {
                        while (ch != '\n')
                            ch = getchar();
                        break;
                    }
                    del.PushBack(ch);
                }
                tree.DeleteNode(del);
                del.Clear();
                break;
            case '!':
                break;
            case 'p':
                cout << endl;
                tree.ShowTree();
                break;
            default:
                break;
        }
    } while (ch != EOF);
    tree.check_red();
    return 0;
}

#include <iostream>
#include <fstream>
#include <cstdio>

const bool red = 1;
const bool black = 0;
const int size_word = 258;

using namespace std;

char IgnoreReg(const char &a) {
    const char A = 'A';
    const char Z = 'Z';
    if (a >= A && a <= Z) {
        return a - A + 'a';
    } else {
        return a;
    }
}

void CheckWrongs(ofstream &f1) {
    if (f1.fail()) {
        puts("ERROR");
        puts("The read type does not match the specidied");
        exit(-2);
    }
    if (f1.bad()) {
        puts("ERROR");
        puts("Bad file formatting");
    }
}

void CheckWrongs(ifstream &f1) {
    if (f1.fail()) {
        puts("ERROR");
        puts("The read type does not match the specidied");
        exit(-2);
    }
    if (f1.bad()) {
        puts("ERROR");
        puts("Bad file formatting");
    }
}


template <class T>
class TVector {
    private:
        T* vector;
        int size;
        int limit;
    public:
        TVector() {
            size = -1;
            limit = 1;
            vector = new T[limit];
        }
        TVector(const int &mem) {
            size = -1;
            limit = mem;
            vector = new T[limit];
        }
        
        TVector (const TVector <T> &b) {
            size = b.size;
            limit = b.limit;
            vector = new T[limit];
            for (int i = 0; i <= size; i++) {
                vector[i] = b.vector[i];
            }
        }
        
        const int Len(void) {
            return size + 1;
        }
        
        T & operator [] (const int &i) {
            if (i < 0 || i > size) {
                throw std::out_of_range("Out of range!!!");
            }
            return *(vector + i);
        }
        T & At(const int &i) {
            if (i < 0 || i > size) {
                throw std::out_of_range("Out of range!!!");
            }
            return *(vector + i);
        }
        
        void PushBack(const T a) {
            const int mnoj = 2;
            size++;
            if (size == limit) {
                T* help = new T[mnoj * limit];
                for (int i = 0; i < size; i++) {
                    help[i] = vector[i];
                }
                delete [] vector;
                help[size] = a;
                vector = help;
                limit = limit * mnoj;
            } else {
                vector[size] = a;
            }
        }
        void PushFront(const T a) {
            Insert(a, 0);
        }
        
        void operator = (const TVector <T> &b) {
            delete [] vector;
            size = b.size;
            limit = b.limit;
            vector = new T[limit];
            for (int i = 0; i <= size; i++) {
                vector[i] = b.vector[i];
            }
        }
        bool operator == (TVector <T> &b) {
            if ((*this).Len() == b.Len()) {
                for (int i = 0; i < b.Len(); i++) {
                    if ((*this)[i] != b[i]) 
                        return 0;
                    }
                return 1;
            }
            return 0;
        }
        void operator = (char* str) {
            delete [] vector;
            int i = 0;
            while (true) {
                if (str[i] == '\0') {
                    break;
                }
                i++;
            }
            if (i > 0) {
                limit = i;
                vector = new char[limit];
            } else {
                limit = 1;
                size = -1;
                vector = new char[limit];
                return;
            }
            for (int j = 0; j < i; j++) {
                vector[j] = IgnoreReg(str[j]);
            }
            size = i - 1;
        }
        
        void Insert(const T num, const int &j) {
            if (j < 0 || j > size + 1)
                throw std::out_of_range("Out of range!!!");
            size++;
            if (size != limit) {
                for (int i = size; i > j; i--) {
                    vector[i] = vector[i - 1];
                }
                vector[j] = num;
                return;
            }
            TVector <T> b(limit);
            for (int i = 0; i < j; i++) {
                b.PushBack(vector[i]);
            }
            b.PushBack(num);
            for (int i = j; i < size; i++) {
                b.PushBack(vector[i]);
            }
            *this = b;
        }
        T const Erase (const int &j) {
            if (j < 0 || j > size)
                throw std::out_of_range("Out of range!!!");
            T em = vector[j];
            for (int i = j; i < size; i++) {
                vector[i] = vector[i + 1];
            }
            size--;
            return em;
        }
        T const PopBack(void) {
            return Erase(size);
        }
        
        friend void Swap (TVector <T> &a, TVector <T> &b) {
            std::swap(a, b);
        }
    
        void Clear(void) {
            size = -1;
            limit = 1;
            delete [] vector;
            vector = new T[limit];
        }
        
        ~ TVector () {
            delete [] vector;
        }
};

struct TData {
	TVector <char> key;
	unsigned long long value;
    bool color;
};

void PrintDateR(TData &date) {
    for (int i = 0; i < date.key.Len(); i++) {
        printf("\033[31m%c\033[0m", date.key[i]);
    }
    putchar(' ');
    printf("\033[31m%llu\033[0m\n", date.value);
}

void PrintDateB(TData &date) {
    for (int i = 0; i < date.key.Len(); i++) {
        printf("%c", date.key[i]);
    }
    putchar(' ');
    printf("%llu\n", date.value);
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
    return 0;
}

class TRBtree {
    protected:
        TRBtree* left;
        TRBtree* right;
        TRBtree* father;
        int size;
    public:
    TData root;
        TRBtree() {
            root.color = black;
            size = 0;
            left = nullptr;
            right = nullptr;
            father = nullptr;
        }
        TRBtree(TData &em) {
            root = em;
            root.color = red;
            size = 1;
            left = nullptr;
            right = nullptr;
            father = nullptr;
        }
        friend void BalanceTreeL(TRBtree* tree) {
            if (!(tree->root.color == red && tree->father->root.color == red)) {
                return;
            }
            if (!tree->father->father) {
                tree->father->root.color = black;
                return;
            }
            if (tree->father->father->right && tree->father->father->right != tree->father && tree->father->father->right->root.color == red) {
                TRBtree* ancle_r = tree->father->father->right;
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
                TRBtree* ancle_r = tree->father->father->left;
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
                TRBtree* help = tree->right;
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
                    TData help1d;
                    TRBtree* help2;
                    TRBtree* general = tree->father->father;
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
                        TData help2d;
                        TRBtree* help3;
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
                TRBtree* help = tree->father->right;
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
        friend void BalanceTreeR(TRBtree* tree) {
            if (!tree->father->father && tree->father->root.color == red) {
                tree->father->root.color = black;
                return;
            }
            if (!(tree->root.color == red && tree->father->root.color == red)) {
                return;
            }
            if (tree->father->father->left != nullptr && tree->father->father->left != tree->father && tree->father->father->left->root.color == red) {
                TRBtree* ancle_r = tree->father->father->left;
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
                TRBtree* ancle_r = tree->father->father->right;
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
                TRBtree* help = tree->left;
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
                    TData help1d;
                    TRBtree* help2;
                    TRBtree* general = tree->father->father;
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
                        TData help2d;
                        TRBtree* help3;
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
                TRBtree* help = tree->father->left;
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
        void AddNode(TData &em) {
            TRBtree* tree = this;
            if (!tree->size) {
                tree->root = em;
                tree->root.color = black;
                tree->size++;
                puts("OK");
                return;
            }
            if (em.key == tree->root.key) {
                puts("Exist");
                return;
            } else if (IsBigger(em.key, tree->root.key)) {
                if (right) {
                    tree->right->AddNode(em);
                } else {
                    TRBtree* newtree = new TRBtree(em);
                    tree->right = newtree;
                    newtree->father = tree;
                    BalanceTreeR(newtree);
                    puts("OK");
                    tree->size++;
                }
            } else {
                 if (left) {
                    tree->left->AddNode(em);
                } else {
                    TRBtree* newtree = new TRBtree(em);
                    tree->left = newtree;
                    newtree->father = tree;
                    BalanceTreeL(newtree);
                    puts("OK");
                    tree->size++;
                }
            }
        }
        TRBtree* SearchNode(TVector <char> &em) {
            TRBtree* search = nullptr;
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
            TRBtree* tree = SearchNode(em);
            if (!tree) {
                puts("NoSuchWord");
                return;
            }
            if (tree->left && !tree->right) {
                bool l;
                bool col = tree->root.color;
                TRBtree* new_node = tree->left;
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
                    puts("OK");
                    return;
                }
                tree->left->father = tree->father;
                tree->left = nullptr;
                delete tree;
                if (col == black) {
                    if (new_node->root.color == red) {
                        new_node->root.color = black;
                    } else {
                         if (l) {
                            BlackBalanceL(new_node);
                        } else {
                            BlackBalanceR(new_node);
                        }
                        new_node = nullptr;
                    }
                }
                puts("OK");
                return;
            }
            if (!tree->left && tree->right) {
                bool col = tree->root.color;
                bool l;
                TRBtree* new_node = tree->right;
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
                    puts("OK");
                    return;
                }
                tree->right->father = tree->father;
                tree->right = nullptr;
                delete tree;
               if (col == black) {
                    if (new_node->root.color == red) {
                        new_node->root.color = black;
                    } else {
                        if (l) {
                            BlackBalanceL(new_node);
                        } else {
                            BlackBalanceR(new_node);
                        }
                        new_node = nullptr;
                    }
                }
                puts("OK");
                return;
            }
            if (!tree->left && !tree->right) {
                bool l = -1;
                if (!tree->father) {
                    tree->size = 0;
                    tree->root.key.Clear();
                    puts("OK");
                    return; 
                }
                if (tree->root.color == black) {
                    if (tree->father->left == tree) {
                        l = 1;
                    } else if (tree->father->right == tree) {
                        l = 0;
                    }
                    if (l) {
                       BlackBalanceL(tree);
                       tree->father->left = nullptr;
                    } else {
                        BlackBalanceR(tree);
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
                puts("OK");
                return;
            }
            if (tree->left && tree->right) {
                TRBtree* help = tree->right;
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
        friend void BlackBalanceL(TRBtree* tree) {
            if (tree == nullptr) {
                return;
            }
            if (tree->root.color == red) {
                tree->root.color = black;
                return;
            }
            if (!tree->father) {
                return;
            }
            if (tree->father->right && tree->father->right->root.color == red) {
                TRBtree* general = tree->father;
                TRBtree* help1 = general->right->left;
                TRBtree* help2 = general->right->right;
                TRBtree* help3 = general->left; 
                TData help = general->root;
                general->root = general->right->root;
                general->root.color = black;
                if (!help2 && !help1) {
                    general->left = general->right;
                    general->right = nullptr;
                    general->left->left = help3;
                    help3->father = general->left;
                    general->left->root = help;
                    general->left->root.color = red;
                    BlackBalanceL(tree);
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
                    BlackBalanceL(tree);
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
                BlackBalanceL(tree);
            } else if (tree->father->right && tree->father->right->root.color == black && (tree->father->right->left == nullptr || tree->father->right->left->root.color == black) && (tree->father->right->right == nullptr || tree->father->right->right->root.color == black)) {
                tree->father->right->root.color = red;
                if (tree->father->father && tree->father->father->left == tree->father) {
                    BlackBalanceL(tree->father);
                } else if (tree->father->father && tree->father->father->right == tree->father) {
                    BlackBalanceR(tree->father);
                }
            } else if (tree->father->right && tree->father->right->root.color == black && tree->father->right->left && tree->father->right->left->root.color == red && (tree->father->right->right == nullptr || tree->father->right->right->root.color == black)) {
                TRBtree* help1 = tree->father->right->left->right;
                tree->father->right = tree->father->right->left;
                tree->father->right->right = tree->father->right->father;
                tree->father->right->father = tree->father;
                tree->father->right->right->father = tree->father->right;
                tree->father->right->right->left = help1;
                if (help1) {
                    help1->father = tree->father->right->right;
                }
                tree->father->right->root.color = black;
                tree->father->right->right->root.color = red;
                BlackBalanceL(tree);
            } else if (tree->father->right && tree->father->right->root.color == black && tree->father->right->right && tree->father->right->right->root.color == red) {
                TRBtree* general = tree->father;
                TRBtree* help1 = general->right->left;
                TRBtree* help2 = general->right->right;
                TRBtree* help3 = general->left; 
                TData help = general->root;
                general->root = general->right->root;
                general->root.color = help.color;
                general->right->root = general->right->right->root;
                general->right->root.color = black;
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
            }
        }
        friend void BlackBalanceR(TRBtree* tree) {
            if (tree == nullptr) {
                return;
            }
            if (tree->root.color == red) {
                tree->root.color = black;
                return;
            }
            if (!tree->father) {
                return;
            }
            if (tree->father->left && tree->father->left->root.color == red) {
                TRBtree* general = tree->father;
                TRBtree* help1 = general->left->right;
                TRBtree* help2 = general->left->left;
                TRBtree* help3 = general->right; 
                TData help = general->root;
                general->root = general->left->root;
                general->root.color = black;
                if (!help2 && !help1) {
                    general->right = general->left;
                    general->left = nullptr;
                    general->right->right = help3;
                    help3->father = general->right;
                    general->right->root = help;
                    general->right->root.color = red;
                    BlackBalanceR(tree);
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
                    BlackBalanceR(tree);
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
                BlackBalanceR(tree);
            } else if (tree->father->left && tree->father->left->root.color == black && (tree->father->left->right == nullptr || tree->father->left->right->root.color == black) && (tree->father->left->left == nullptr || tree->father->left->left->root.color == black)) {
                tree->father->left->root.color = red;
                if (tree->father->father && tree->father->father->left == tree->father) {
                    BlackBalanceL(tree->father);
                } else if (tree->father->father && tree->father->father->right == tree->father) {
                    BlackBalanceR(tree->father);
                }
            } else if (tree->father->left && tree->father->left->root.color == black && tree->father->left->right && tree->father->left->right->root.color == red && (tree->father->left->left == nullptr || tree->father->left->left->root.color == black)) {
                TRBtree* help1 = tree->father->left->right->left;
                tree->father->left = tree->father->left->right;
                tree->father->left->left = tree->father->left->father;
                tree->father->left->father = tree->father;
                tree->father->left->left->father = tree->father->left;
                tree->father->left->left->right = help1;
                if (help1) {
                    help1->father = tree->father->left->left;
                }
                tree->father->left->root.color = black;
                tree->father->left->left->root.color = red;
                BlackBalanceR(tree);
            } else if (tree->father->left && tree->father->left->root.color == black && tree->father->left->left && tree->father->left->left->root.color == red) {
                TRBtree* general = tree->father;
                TRBtree* help1 = general->left->right;
                TRBtree* help2 = general->left->left;
                TRBtree* help3 = general->right; 
                TData help = general->root;
                general->root = general->left->root;
                general->root.color = help.color;
                general->left->root = general->left->left->root;
                general->left->root.color = black;
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
        void WriteFile(char* filename) {
            if (!size) {
                puts("OK");
                return;
            }
            ofstream os(filename);
            if (!os.is_open()) {
                puts("ERROR");
                puts("Cant create or open file");
                exit(-1);
            }
            WriteFileP(os);
            puts("OK");
        }
        void WriteFileP(ofstream &os) {
            char key_help[size_word];
            bool ex_son_l = 0;
            bool ex_son_r = 0;
            for (int i = 0; i < root.key.Len(); i++) {
                key_help[i] = root.key[i];
            }
            key_help[root.key.Len()] = '\0';
            os << key_help << ' ';
            CheckWrongs(os);
            os << root.value << ' ';
            CheckWrongs(os);
            os << root.color << ' ';
            CheckWrongs(os);
            os << size << ' ';
            CheckWrongs(os);
            if (left) {
                ex_son_l = 1;
            }
            if (right) {
                ex_son_r = 1;
            }
            os << ex_son_l << ' ';
            CheckWrongs(os);
            os << ex_son_r << ' ';
            CheckWrongs(os);
            if (left) {
                left->WriteFileP(os);
            }
            if (right) {
                right->WriteFileP(os);
            }
        }
        void ReadFile(char* filename) {
            ifstream is(filename);
            if (!is.is_open()) {
                puts("ERROR");
                puts("Cant create or open file");
                exit(-1);
            }
            delete left;
            delete right;
            left = nullptr;
            right = nullptr;
            ReadFileP(is, this);
            puts("OK");
        }
        friend void ReadFileP(ifstream &is, TRBtree* use_tree) {
            char key_help[size_word];
            bool ex_son_l = 0;
            bool ex_son_r = 0;
            TData data;
            TRBtree* newtree = nullptr;
            if (!(is >> key_help)) {
                return;
            }
            is >> data.value;
            CheckWrongs(is);
            is >> data.color;
            CheckWrongs(is);
            data.key = key_help;
            use_tree->root = data;
            is >> use_tree->size;
            CheckWrongs(is);
            is >> ex_son_l;
            CheckWrongs(is);
            is >> ex_son_r;
            CheckWrongs(is);
            if (ex_son_l) {
                newtree = new TRBtree(data);
                use_tree->left = newtree;
                newtree->father = use_tree;
                ReadFileP(is, newtree);
            } else {
                use_tree->left = nullptr;
            }
            if (ex_son_r) {
                newtree = new TRBtree(data);
                use_tree->right = newtree;
                newtree->father = use_tree;
                ReadFileP(is, newtree);
            } else {
                use_tree->right = nullptr;
            }
        }
        
        ~TRBtree() {
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
    ios::sync_with_stdio(false);
    TRBtree tree;
    TData get_data;
    char key1[size_word];
    TRBtree* help = nullptr;
    unsigned long long value;
    char filename[size_word];
    char ch;
    int j = 0;
    do {
        ch = getchar();
        switch (ch) {
            case '+':
                if (scanf(" %s %llu", key1, &value)) {
                    get_data.key = key1;
                    get_data.value = value;
                    tree.AddNode(get_data);
                }
                break;
            case '-':
                 if (scanf(" %s", key1)) {
                    get_data.key = key1;
                    tree.DeleteNode(get_data.key);
                }
                break;
            case '!':
                j = 0;
                getchar();
                bool s_l;
                if (getchar() == 'S') {
                    s_l = 1;
                } else {
                    s_l = 0;
                }
                while (true) {
                    if (getchar() == ' ') {
                        break;
                    }
                }
                while (true) {
                    ch = getchar();
                    if (ch == '\n') {
                        break;
                    }
                    filename[j] = ch;
                    j++;
                }
                filename[j] = '\0';
                if (s_l) {
                    tree.WriteFile(filename);
                } else {
                    tree.ReadFile(filename);
                }
                break;
            case'>':
                tree.ShowTree();
                break;
            default:
                get_data.key.Clear();
                if (IgnoreReg(ch) < 'a' || IgnoreReg(ch) > 'z') {
                    break;
                }
                while (true) {
                    get_data.key.PushBack(IgnoreReg(ch));
                    ch = getchar();
                    if (ch == '\n' || ch == EOF) {
                        break;
                    }
                }
                help = tree.SearchNode(get_data.key);
                if (help) {
                    printf("OK: %llu\n", help->root.value);
                } else {
                    puts("NoSuchWord");
                }
                break;
                getchar();
        }
    } while (ch != EOF);
    return 0;
}

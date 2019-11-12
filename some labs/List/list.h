#ifndef __list_h__
#define __list_h__

typedef char Use_data;

typedef struct Element{
    Use_data data;
    struct Element *next;
    struct Element *prev;
} Element;

typedef struct List {
    int size;
    Element *barr;
} List;

List* create_list(void);
void push_front(List* a, Use_data c);
void l_print(List* a);
void push_back(List* a, Use_data c);
void V_insert(List* a, int j, Use_data value);
void V_delete(List* a, int j);
int lenght(List* a);
Use_data l_num(List* a, int j);
int Zero_list(List* a);
int One_List(List* a);
Use_data l_pop(List* a, int j);
void l_destroy(List* a);
void special_func(List* a, Use_data value);

#endif

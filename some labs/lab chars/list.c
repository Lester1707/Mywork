#include "list.h"

List* create_list(void)
{
	List* a;
	Element* b;
	a = malloc(sizeof(List));
	b = malloc(sizeof(Element));
	a->size = 0;
	a->barr = b;
	b->prev = a->barr;
	b->next = a->barr;
	return(a);
}

void push_back(List* a, Use_data c)
{
	Element* n;
	n = malloc(sizeof(Element));
	n->data = c;
	a->size++;
	n->next = a->barr;
	n->prev = (a->barr)->prev;
	(a->barr)->prev = n;
	(n->prev)->next = n;
}

int Zero_list(List* a)
{
	for (int i = 1; i <= a->size; i++) {
		if (l_num(a, i) != 0) {
			return 0;
		}
	}
	return 1;
}

void push_front(List* a, Use_data c)
{
	Element* n;
	n = malloc(sizeof(Element));
	n->data = c;
	a->size++;
	n->next = (a->barr)->next;
	n->prev = a->barr;
	(a->barr)->next = n;
	(n->next)->prev = n;
}

void l_print(List* a)
{
	if (a->size == 0) {
		printf("List is empety!\n");
	} else {
		//printf("Out:");
		Element* c;
		c = (a->barr)->next;
		for (int i = 1; i <= a->size; i++) {
			printf("%c", c->data);
			c = c->next;
		}
		//printf("\n");
	}
}

Use_data l_num(List* a, int j)
{
	if (j > a->size || j < 0) {
		printf("correct positionl!\n");
		return -1;
	} 
	if (j == 0) {
		printf("Tree is empety!l\n");
		return -1;
	}
	Element* c;
	Use_data b;
	c = (a->barr);
	for (int i = 1; i <= j; i++) {
		c = c->next;
		b = c->data;
	}
	return b;
}
	
Use_data l_pop(List* a, int j)	
{
	int k;
	if (j > a->size || j <= 0) {
		printf("in correct positiond!\n");
		return 0;
	} else if (a->size == 0) {
		printf("list is empety!\n");
		return 0;
	} else {
		Element* c;
		c = (a->barr)->next;
		for (int i = 1; i < j; i++)
			c = c->next;
		k = c->data;
		for (int i = j; i < a->size; i++) {
			c->data = (c->next)->data;
			c = c->next;
		}
		(a->barr)->prev = c->prev;
		(c->prev)->next = a->barr;
		free(c);
		a->size--;
		return k;
	}
	return k;
}

void V_insert(List* a, int j, Use_data value)
{
	if (j > a->size + 1 || j <= 0) {
		printf("in the correct positioni!\n");
	} else if (j == a->size + 1) {
		push_back(a, value);
	} else if (j == 1) {
		push_front(a, value);
	} else {
		Element* c;
		Use_data d, k;
		c = a->barr;
		for (int i = 0; i < j; i++)
			c = c->next;
		d = c->data;
		c->data = value;
		for (int i = j; i < a->size; i++) {
			k = (c->next)->data;
			(c->next)->data = d;
			d = k;
			c = c->next;
		}
		push_back(a, d);
	}
}

int One_List(List* a)
{
	for (int i = 1; i <= a->size; i++) {
		if (l_num(a, i) != 1) {
			return 0;
		}
	}
	return 1;
}

void V_delete(List* a, int j)
{
	if (j > a->size || j <= 0) {
		printf("in correct positiond!\n");
	} else if (a->size == 0) {
		printf("list is empety!\n");
	} else {
		Element* c;
		c = (a->barr)->next;
		for (int i = 1; i < j; i++)
			c = c->next;
		for (int i = j; i < a->size; i++) {
			c->data = (c->next)->data;
			c = c->next;
		}
		(a->barr)->prev = c->prev;
		(c->prev)->next = a->barr;
		free(c);
		a->size--;
	}
}

int lenght(List* a)
{
	return a->size;
}

void l_destroy(List* a)
{
	while (a->size > 0)
		V_delete(a, 1);
	free(a->barr);
	free(a);
}

void special_func(List* a, Use_data value)
{
	if (a->size != 1) {
		List* b;
		b = create_list();
		for (int i = 0; i < a->size; i++)
			push_front(b, 0);
		for (int i = 1; i <= a->size; i++) {
			if (l_num(a, i) == value) {
				if (i == 1) {
					V_delete(b, 2);
					V_insert(b, 2, 1);
					continue;
				}
				if (i == a->size) {
					V_delete(b, a->size - 1);
					V_insert(b, a->size - 1, 1);
					break;
				}
				V_delete(b, i - 1);
				V_insert(b, i - 1, 1);
				V_delete(b, i + 1);
				V_insert(b, i + 1, 1);
			}
		}
		int i = 1;
		while(1 - Zero_list(b)) {
			if (l_num(b, i) == 1) {
				V_delete(a, i);
				V_delete(b, i);
				i--;
			}
			i++;
		}
		l_destroy(b);
	}
}	
		

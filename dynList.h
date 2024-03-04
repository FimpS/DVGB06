#ifndef ARRAYLIST_HEADER_H
#define ARRAYLIST_HEADER_H

#include <stdbool.h>

typedef struct dynList
{
	unsigned size;
	unsigned capacity;
	void **body;
} dynList;

dynList* dynList_create();
int dynList_size();
void dynList_mem_alloc();
void dynList_add();
void dynList_set();
void dynList_destroy();
void dynList_del_index();
void dynList_clear();
bool dynList_is_empty();

void *dynList_pop();
void *dynList_get();

#endif

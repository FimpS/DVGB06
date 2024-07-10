#include "global.h"
#define START_CAP 4

dynList* dynList_create()
{
	dynList* new = (dynList*)malloc(sizeof(dynList));
	new->size = 0;
	new->body = (void*)malloc(sizeof(void*) * START_CAP);
	new->capacity = START_CAP;

	return new;
}

bool dynList_is_empty(dynList *a)
{
	return a->size == 0;
}

int dynList_size(dynList *a)
{
	return a->size;
}

void dynList_mem_alloc(dynList* a)
{
	if(dynList_size(a) + 1 > a->capacity)
	{
		printf("size: %d\ncapa: %d\n", a->size, a->capacity);
		a->capacity *= 2;
		a->body = realloc(a->body, sizeof(void*) * a->capacity);
	}
}

void dynList_add(dynList* a, void *key)
{
	dynList_mem_alloc(a);
	a->body[a->size++] = key;
}

void *dynList_pop(dynList* a)
{
	if(dynList_is_empty(a))
		return NULL;
	void* tmp = a->size != 0 ? a->body[--a->size] : NULL;
	free(tmp);
	return NULL;
}

void dynList_del_index(dynList* a, int index)
{
#if 0
	//void* tmp = dynList_get(a, index);
	dynList_memshift(a->body + index + 1, -1, a->size - index);
	a->size--;
#endif
#if 1
	if(index >= a->size || a->size == 0)
		return;
	int tmp_s = dynList_size(a);
	void *tmp = a->body[index];
	free(tmp);
	for(int i = index; i < tmp_s; i++)
		dynList_set(a, i, dynList_get(a, i + 1));
	a->size--;
#endif
}

void dynList_set(dynList *a, int index, void *tmp)
{
	if(dynList_is_empty(a))
		return;
	a->body[index] = tmp;
}

void *dynList_get(dynList* a, int index)
{
	//return a->size != 0 ? a->body[index] : NULL;
	return a->size > index ? a->body[index] : NULL;
}

void dynList_clear(dynList *a)
{
	
	int tmp_s = a->size;
	if(dynList_is_empty(a))
		return;
	for(int i = 0; i < tmp_s; i++)
		dynList_pop(a);
}

void dynList_destroy(dynList *a)
{
	//int s = a->size;
	if(dynList_is_empty(a))
		return;
	free(a->body);
	/*
	for(int i = 0; i < s; i++)
		dynList_pop(a);
	*/
	a->size = 0;
	a->body = malloc(sizeof(void*) * START_CAP);
	a->capacity = START_CAP;
}

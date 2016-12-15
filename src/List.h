
#ifndef DOUBLELINKEDLIST_H_
#define DOUBLELINKEDLIST_H_

#include "stm32f4xx.h"

typedef
struct _list2Node
{
	void* data;
	_list2Node* next;
	_list2Node* prev;
} List2Node;

typedef
struct _list2
{
	List2Node* first;
	List2Node* last;
	uint16_t size;
} List2;

void AddElementBack(List2* list, void* elptr);
void AddElementFront(List2* list, void* elptr);
void AddElementAfter(List2* list, List2Node* node, void* elptr);

#endif /* DOUBLELINKEDLIST_H_ */

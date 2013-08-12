#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "../stdafx.h"

typedef struct LINKEDLIST_NODE
{
	void* data;
	struct LINKEDLIST_NODE* next;
	struct LINKEDLIST_NODE* prev;
} listnode;

typedef listnode listroot;

// Starts a list.
listroot* createlist();

// Gets the next node of the list from another one.
void list_next(listnode*);

// Gets the previous node of the list from another one.
void list_prev(listnode*);

// Adds a node to the end of the list
void list_add(listroot*, void*);

// Adds a node according to a comparision.
// If the comparision returns true, the node will be added
// right after the current one.
void list_addif(listroot*, void*, bool (*comparefunc)(void*));

// Remove a specific node.
void list_remove(listnode*);

// Removes one node with given data.
void list_removeifequals(listroot*, void*);

// Removes all nodes with a given data.
void list_removeall(listroot*, void*);

// Removes all nodes according to a comparision.
// If the comparision is true, the node will be removed.
void list_removeif(listroot*, bool (*comparefunc)(void*));

#endif
#ifndef STACK_H
#define STACK_H

#include "../stdafx.h"

typedef struct STACK_NODE
{
	void* data;
	struct STACK_NODE* prev;
} stacknode;

typedef stacknode stackroot;

// Creates stack
stackroot* createstack();

// Pushes on top of stack
void  stack_push(stackroot*, void*);

// Peeks what's on top of stack
void* stack_peek(stackroot*);

// Pops whatever's on top of stack
void  stack_pop (stackroot*);

#endif
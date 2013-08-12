#include "stack.h"

stackroot* createstack()
{
	return NULL;
}

void  stack_push(stackroot* root, void* newdata)
{
	stacknode* aux = malloc(sizeof(stackroot));
	aux->data = newdata;

	if(!root)
	{
		root = aux;
		root->prev = NULL;
		return;
	}

	aux->prev = root;
	root = aux;
}

void* stack_peek(stackroot* root)
{
	if(!root) return NULL;
	return root->data;
}

void  stack_pop (stackroot* root)
{
	if(!root) return;
	stacknode* aux = root;
	root = root->prev;
	free(aux);
}
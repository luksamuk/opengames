#include "linkedlist.h"

listroot* createlist()
{
	return NULL;
}

void list_next(listnode* node)
{
	node = node->next;
}

void list_prev(listnode* node)
{
	node = node->prev;
}

void list_add(listroot* root, void* data)
{
	// Go to end of list
	listnode* aux2 = root;
	while(aux2->next) list_next(aux2);

	// Create node
	listnode* aux = malloc(sizeof(listnode));
	aux->data = data;
	// Add node
	aux->next = NULL;
	aux->prev = aux2;
	aux2->next = aux;
}

void list_addif(listroot* root, void* data, bool (*comparefunc)(void*))
{
	if(!root) return;
	listnode* aux = root;
	while(aux)
	{
		if((*comparefunc)(aux->data))
		{
			listnode* aux2 = malloc(sizeof(listnode));
			aux2->data = data;
			aux2->prev = aux;
			aux2->next = aux->next;

			aux->next->prev = aux2;
			aux->next = aux2;
			return;
		}
		list_next(aux);
	}
}

void list_remove(listnode* node)
{
	if(node->prev) node->prev->next = node->next;
	if(node->next) node->next->prev = node->prev;
	free(node->data);
	free(node);
}

void list_removeifequals(listroot* root, void* data)
{
	listnode* aux = root;
	while(aux && aux->data != data)
		list_next(aux);
	if(!aux) return;
	if(aux->prev) aux->prev->next = aux->next;
	if(aux->next) aux->next->prev = aux->prev;
	free(aux->data);
	free(aux);
}

void list_removeall(listroot* root, void* data)
{
	listnode* aux = root;
	while(aux)
	{
		if(aux->data == data)
		{
			listnode* next = aux->next;
			list_remove(aux);
			aux = next;
		}
		else list_next(aux);
	}
}

void list_removeif(listroot* root, bool (*comparefunc)(void*))
{
	listnode* aux = root;
	while(aux && !(*comparefunc)(aux->data))
		list_next(aux);
	if(!aux) return;
	if(aux->prev) aux->prev->next = aux->next;
	if(aux->next) aux->next->prev = aux->prev;
	free(aux->data);
	free(aux);
}
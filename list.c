/*
 * list.c - implementation of the integer list functions 
 */


#include <stdlib.h>
#include <stdio.h>
#include "list.h"



list_t* lst_new()
{
   list_t *list;
   list = (list_t*) malloc(sizeof(list_t));
   list->first = NULL;
   return list;
}


void lst_destroy(list_t *list)
{
	struct lst_iitem *item, *nextitem;

	item = list->first;
	while (item != NULL){
		nextitem = item->next;
		free(item);
		item = nextitem;
	}
	free(list);
}


void insert_new_process(list_t *list, int pid)
{
	lst_iitem_t *item;

	item = (lst_iitem_t *) malloc (sizeof(lst_iitem_t));
	item->pid = pid;
	item->next = list->first;
	list->first = item;
}


void update_terminated_process(list_t *list, int pid)
{
   printf("teminated process with pid: %d\n", pid);
}


void lst_print(list_t *list)
{
	lst_iitem_t *item;

	printf("Process list:\n");
	item = list->first;
	/*while(1){  */
	while (item != NULL){
		printf("%d\n", item->pid);
		item = item->next;
	}
	printf("-- end of list.\n");
}

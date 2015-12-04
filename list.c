/*
 * list.c - implementation of the integer list functions 
 */


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
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


void insert_new_process(list_t *list, int pid, time_t starttime)
{
	lst_iitem_t *item;

	item = (lst_iitem_t *) malloc (sizeof(lst_iitem_t));
	item->pid = pid;
	//item->status = status;
	item->starttime = starttime;
	item->endtime = 0;
	item->next = list->first;
	list->first = item;
}


void update_terminated_process(list_t *list, int pid, int status, time_t endtime)
{
	lst_iitem_t *item;
	item = list->first;
	while(item != NULL){
		if((item->pid)==pid){
			item->endtime=endtime;
			item->status=status;
			item = item->next;
			break;
		}
		item = item->next;
	}
}

int get_execution_time(list_t *list, int pid){
	lst_iitem_t *item;

	item = list->first;
	while(item != NULL){
		if((item->pid)==pid){
			return (int) (item->endtime-item->starttime);
		}
		item = item->next;
	}
	perror("Process not found!");
	return -1;
}


void lst_print(list_t *list)
{
	lst_iitem_t *item;

	printf("Process list:\n");
	item = list->first;
	while (item != NULL){
		printf("PID:%d STATUS:%d DURATION:%d\n", item->pid, item->status, (int) (item->endtime-item->starttime));
		item = item->next;
	}
	printf("-- END OF LIST.\n");
}

void initialize() {

	Head = NULL;
	Tail = NULL;
}

Item createProcess(int PID) {
	Item newProcess = (Item) malloc(sizeof(struct process));

	newProcess -> PID = PID;

	return newProcess;
	
}

Link newProcess(Item item, Link next) {

	Link newProcess = (Link) malloc(sizeof(struct Pool));
	
	newProcess -> item = item;
	newProcess -> next = next;

	return newProcess;
}

void insertProcess(Item item) {

	if (Head == NULL)
		Tail = (Head = newProcess(item, Head));
	else {
		Tail -> next = newProcess(item,Tail -> next);
		Tail = Tail -> next;
	}
}

Link findProcess(Link Head, int PID) {

	Link foundProcess;
	
	for (foundProcess = Head; foundProcess != NULL; foundProcess = foundProcess -> next) {
		if (PID == foundProcess -> item -> PID)
			return foundProcess;
	}
	return NULL;
}

void removeProcess(int PID) {
	
	Link foundProcess, previousProcess;

	for (foundProcess = Head, previousProcess = NULL; foundProcess != NULL && foundProcess -> item -> PID != PID;
			previousProcess = foundProcess, foundProcess = foundProcess -> next);
	
	if(foundProcess == NULL) {
		printf("Process %d does not exist\n", PID);
		return;
	}
	printf("O PID:%d foi removido\n", PID);

	if(Head == foundProcess)
		Head = foundProcess -> next;
	else 
		previousProcess -> next = foundProcess -> next;
	if(foundProcess -> next == NULL)
		Tail = previousProcess; 
	
	free(foundProcess -> item);
	free(foundProcess);
}


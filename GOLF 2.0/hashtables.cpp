#include "include/hashtables.h"
#include <functional>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void table_init(table *hashtable[]){
	for(int i=0; i<table_size; i++)
		hashtable[i] = NULL;
}

void table_print(table *hashtable[]){
	printf("Start\n");
	for(int i=0; i<table_size; i++){
		table *tmp = hashtable[i];

		if(tmp == NULL){
			printf("\t%d. null\n", i);
			continue;
		}
		
		printf("\t%d. ", i);
		while(tmp != NULL){
			printf("type: %d; index: %d", tmp->type, tmp->index);

			if(tmp->next != NULL) 
				printf(" -> ");

			tmp = tmp->next;
		}
		printf("\n");
	}
	printf("End\n");
}

int _hash(int type, int index){
	return 13 * type + index;
}

bool table_insert(table *hashtable[], int type, int ind){
	int index = _hash(type, ind) % table_size;

	table *new_item = (table*)malloc(sizeof(table));
	if(new_item == NULL)
		return false;

	new_item->type = type;
	new_item->index = ind;
	new_item->next = NULL;
	
	table *tmp = hashtable[index];
	if(tmp == NULL)
		hashtable[index] = new_item;
	else{
		while(tmp->next != NULL) 
			tmp = tmp->next;
		tmp->next = new_item;
	}

	return true;
}

bool table_lookup(table *hashtable[], int type, int ind){
	int index = _hash(type, ind) % table_size;

	if(hashtable[index] != NULL){
		table *tmp = hashtable[index];

		while(tmp != NULL){
			if(tmp->index == ind && tmp->type == type)
				return true;
			tmp = tmp->next;
		}
		
	}

	return false;
}

bool table_del(table *hashtable[], int type, int ind){
	int index = _hash(type, ind) % table_size;
	
	if(hashtable[index] != NULL){
		table *tmp = hashtable[index];
		table *prev = NULL;

		while(tmp != NULL){
			if(tmp->index == ind && tmp->type == type){
				if(prev == NULL)
					hashtable[index] = tmp->next;
				else
					prev->next = tmp->next;
				free(tmp);

				return true;
			}
			prev = tmp;
			tmp = tmp->next;
		}
	}

	return false;
}

bool table_full(table *hashtable[]){

	for(int i=0; i<table_size; i++)
		if(hashtable[i] == NULL)
			return false;
	return true;

}

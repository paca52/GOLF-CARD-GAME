#ifndef HASHTABLES_H
#define HASHTABLES_H

#include <stdbool.h>

typedef struct table{
	int type;
	int index;
	struct table *next;
}table;

const int table_size = 52;

void table_init(table *hashtable[]);
void table_print(table *hashtable[]);
int _hash(int type, int index);
bool table_insert(table *hashtable[], int type, int ind);
bool table_lookup(table *hashtable[], int type, int ind);
bool table_del(table *hashtable[], int type, int ind);
bool table_full(table *hashtable[]);

#endif /* HASHTABLES_H */

#ifndef UNDOREDO_H
#define UNDOREDO_H

#include "list.h"
#include "cmd_line.h"

typedef struct list {
	node * start;
	node * end;
	int current_index;
} list;

extern list* action_list[10];

list *  init_list(int workspace);

void free_list(int buffer);

short action(int buffer, char * line);

node * insert_tail(list * list, char * value);

short free_from_current_index(int buffer);



#endif
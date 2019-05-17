#include "undoredo.h"


list *init_list(int buffer){
	action_list[buffer] = malloc(sizeof(list));
	memset(action_list[buffer], 0, sizeof(list));
	if (action_list[buffer] == NULL) {
		fprintf(stderr, "Error : list not initialized\n");
		return NULL;
	}
	action_list[buffer]->current_index = 0;
	return action_list[buffer];
}

void free_list(int buffer){
	list *current = action_list[buffer];
	if (current == NULL) return;
	free_all(current->start);
	if (current->end != NULL) free_all(current->end);
	free(current);
}

short action(int buffer, char *line){
	list *l = action_list[buffer];
	if (l == NULL) {
		fprintf(stderr, "Null list at %d\n", buffer);
		return 1;
	}
	node * current = l->start;
	if(current==NULL){
		node * new = init_node(line);
		l->start = new;
		l->end = new;
	l->current_index++;
		return 0;
	}
	int i=0;
	printf("%d\n", l->current_index);
	while(current->next!=NULL && i<l->current_index){
		printf("DANS WHILE\n");
		current=current->next;
		i++;
	}
	current->next=init_node(line);
	l->current_index++;
	return 0;
}

/**
 * Inserts a new node in the end of the linked list
 *
 * @param list list to free
 * @param value value of the new node
 * @return NULL if failed, new node otherwise
 */
node *insert_tail(list *list, char *value){
	if (value == NULL) return NULL;
	node *current = list->start;
	node *new = init_node(value);
	if (new == NULL) return NULL;
	//if first node
	if (current == NULL) {
		list->start = new;
		list->end = new;
		return new;
	}
	//else
	while (current->next != NULL) {

		current = current->next;
	}

	current->next = new;
	list->end = new;
	return new;
}

short free_from_current_index(int buffer){
	list * l = action_list[buffer];
	if(l==NULL){
		fprintf(stderr, "Error : cant free null list\n");
		return 1;
	}
	node * current = l->start;
	int i=0;
	while((current!=NULL && current->next !=NULL) || i<=l->current_index){
		current=current->next;
		i++;
	}
	free_all(current->next);
	return 0;
}

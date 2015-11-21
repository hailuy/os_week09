#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int approxEqual(float el, float refval, float tolerance)
{
  return (refval-tolerance <= el &&
	  el <= refval+tolerance);
}


llist* add_list(llist* node, int r, int c, float val){
  llist* current = malloc(sizeof(llist));

  if (!current){
	printf("Unable to allocate memory : add_list(llist*, %d, %d, %f)\n", r, c, val);
	exit(1);
  }

  if (node){
	node->next = current;         /* add new node to the tail of the previous node */
	current->prev = node;         
  }
  else {
	node = current;               /* first node in the list */ 
	current->prev = NULL;         
  }
  current->r = r;                 /* allocate data */
  current->c = c;
  current->val = val;
  current->next = NULL;           /* indicate the end of the list */

  return current;
}


llist* get_head_list(llist* node){
  llist* head=NULL;
  llist* current=node;
  
  while(current){
	head = current;
	current = current->prev;     /* go one node back */
  }

  return head;
}

llist* clear_list(llist* node){
 llist* current = get_head_list(node);    /* go to start of the list */
 llist* temp;
 while (current){
	temp = current->next;
	free(current);
	current = temp;
 }
 return NULL;                            /* the list now points to NULL */
}

void print_list(llist* node){
 llist*	current = get_head_list(node);    /* go to start of the list */
 llist*	temp;
 while (current){
        temp = current->next;
        fprintf(stdout, "r=%d, c=%d: %.6f\n", current->r, current->c, current->val);
        current = temp;
 }


}


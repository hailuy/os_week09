#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"


int main(){
  llist* my_list=NULL;		/* Pointer to an empty list */
  int i=0;

  for (i=0;i<5;i++)           /* loop to build the list */
	 my_list = add_list(my_list,i,i, i*1.0f);   /* add an item to list */

  print_list(my_list);			
  my_list = clear_list(my_list);
  
}



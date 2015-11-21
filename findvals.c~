#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "utils.h"

#define NUM_THREADS 2


/* typedef command defines a new data type. 
   Below it creates a 'struct': think of it as a class
   without methods and only public data members. 

   As a result of this typedef, so_t is a new data type
   which referes to the struct as defined below. 

   Note: since the thread functions can only have a single argument, 
   we use a struct to encapsulate a bunch of different things and 
   pass it on to the thread together. 
*/



typedef struct sharedobject {
 float** d_matrix; /* shared data   */
 int sh_rct;   /* shared row count */
 int sh_cct;   /* shared col count */
 float sh_ref; /* shared */
 float sh_tol;
 int verbose;  /* shared flag */
} so_t;


so_t* shared_data;  /* Different threads can read the thread global data from this variable. 
                       Since the threads only "read" from it, do you think access to shared_data
                       should be synchorised with mutex locks? "
                    */


/* See below an example of how to share writable data between multiple threads 
   that require synchronised access with "mutex locks". Tailor the example to 
   this problem. 

   https://computing.llnl.gov/tutorials/pthreads/#MutexOverview
*/



/* create a mutex lock variable */ 

   pthread_mutex_t mutexsum;

/* Below, define a global variable to maintain the count approximate matches. This variable 
 needs to be accessed by the threads only with a proper lock. */
int sh_count=0;

int verbose = 0;

void *runner(void *param); /* the thread prototype. You have to provide the definition*/


int main(int argc, char **argv)
{
  // convert strings to floats and assign below to replace 0 values. 
  float ref=0;
  float tol=0;

  /* 
    Add code to handle command line parameters. Use compact code. 
    Longwinded code with an if block for each combination of parameters
    is inelegant and will be penalised. 
  */  
  int i;
  for(i = 1;i < argc;i++){
    if(strcmp(argv[i],"-r") == 0)
            ref = strtof(argv[i + 1],NULL);     
    else if(strcmp(argv[i],"-v") == 0) 
            verbose = 1;
    else if(strcmp(argv[i],"-t") == 0)            
	    tol = strtof(argv[i + 1],NULL);
  }

  if (ref == 0 || tol == 0)
  {
    fprintf(stderr, "Usage: %s -r ref -t tol\n", argv[0]);
    exit(1);
  }

  /* Add code to notice time and then modify and uncomment the 
     printf statement below to suite your code. 

     printf("# Start time and date: %s", asctime(local));

  */

  struct tm *local;
  time_t start, end;
  time(&start); // read and record clock
  local = localtime(&start);
  printf("# Start time and date: %s", asctime(local));

  struct timeval tim;
  gettimeofday(&tim, NULL);
  double t1=tim.tv_sec+(tim.tv_usec/1000000.0);

  int rct, cct; /* row count and column count to be read in */

  scanf("%d", &rct);
  scanf("%d", &cct);

  shared_data = malloc(sizeof(so_t));   /* allocate memory for shared data */
  
  
  float **sh_rows = (float **)malloc(rct * sizeof (float*));//sizeof(long int)
  if (sh_rows == 0)
  {
	  fprintf(stderr, "arr Couldn't alocate sufficient space.\n");
	  exit(1);
  }
  int k;
  for(k = 0;k < rct;k++)
  {
	  float* row = (float *) malloc(cct * sizeof(float));
	  if (row == 0)
	  {
	  fprintf(stderr, "row Couldn't alocate sufficient row space.\n");
	  exit(1);
	  }
	  sh_rows[k] = row;          
  }
  int ro,co;
  for(ro = 0;ro < rct;ro++){
	  for(co = 0;co < cct;co++){
		  scanf("%f", &sh_rows[ro][co]);
	  }
  }


  /* load all the input data into shared_data */
  shared_data->sh_rct = rct;
  shared_data->sh_cct = cct;
  shared_data->d_matrix = sh_rows;
  shared_data->sh_ref = ref;
  shared_data->sh_tol = tol;
  shared_data->verbose = verbose;


  /* multi-threading begins */

  pthread_t tid[NUM_THREADS];   /* an array to hold the thread IDs */
  int  t_index[NUM_THREADS];     /* an array to pass an index to each thread indicating the thread number.
                                 Use it to determine which thread scans which part of input data rows. */

  llist* plist=NULL;		/* to hold linked list to print out if -v option is used*/

  /* Add code to initialise lock. Again, look at the link given above if you are unsure. */

  pthread_mutex_init(&mutexsum,NULL);

  /* Create threads */
 int ti=0;
/*pthread_create(&tid[0],NULL,runner,(void *)0);
pthread_create(&tid[1],NULL,runner,(void *)1);
pthread_join(tid[0], (void **) &plist);  
pthread_join(tid[1], (void **) &plist);  */
  for (ti=0;ti<NUM_THREADS;ti++){
     t_index[ti] = ti;
   
     pthread_create(&tid[t_index[ti]],NULL,runner,&t_index[ti]);
  } 


 for (ti=0; ti<NUM_THREADS;ti++){
        /* When the threads exit, they MUST return a list of matches
           found. This list can be NULL if verbose option is NOT specified.
           Otherwise, it is a linked list of matching items, where each item is of 
           the form: row index, col index, value.

           
           Print the list ONLY if verbose (-v) is specified. HINT: look at utils.c and linkedlist.c.
           Where to put list printing code? Before or after pthread_join? */       
 
           pthread_join(tid[ti], (void **) &plist);                     
           if(verbose == 1){
            print_list(plist);
            plist = clear_list(plist);
           }   
 }

  // now output sh_count
  fprintf(stdout, "Found %d approximate matches.\n", sh_count);


  /* Add code to correctly print out elapsed seconds */ 
 // double ticks=0;  
  gettimeofday(&tim, NULL);
  double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
  printf("Elapsed time: %.6lf(s)\n", t2-t1);


  /* time to destroy lock. See the link given above for example */
  pthread_mutex_destroy(&mutexsum);

  /* free all memory you created i.e. everything that your code malloc_ed */
  free(shared_data);
  free(sh_rows);
  pthread_exit(NULL);
  exit(0);
}

void *runner(void *param){

  int tid = *((int *) param);

  /* Based on the thread index passed, decide which data rows 
    to scan in this thread. That is, decide the start-row 
    and end-row for each thread. 
    
    NOTE: Consider the possibility that number of rows is ODD. 
    Try to balance load between all the threads but if exact load 
    balancing is not possible, aim for maximum balance possible.
   */

  int r=0,c=0;
  float** sh_rows = shared_data->d_matrix;
  float ref = shared_data->sh_ref;
  float tol = shared_data->sh_tol;
  llist* print_list=NULL;  
  int row = shared_data->sh_rct;

  int s_row = 0,e_row = 0;

  if(tid == 0){
    s_row = 0;
    e_row = row / 2;
  }
  else if(tid == 1){
    s_row = row / 2;
    e_row = row;
  }
  for (r = s_row; r < e_row; r++)
  {
    for (c = 0; c < shared_data->sh_cct; c++)
    {
	/* Check if sh_rows{r][c] matches the given test. 
	   If yes, add code to increment count in a "thread safe" 
           manner i.e. avoid race conditions. 

	   Also, if -v option is given, add the matching record to 
           plist. */
	if(approxEqual(sh_rows[r][c],ref,tol))
	{
	       if(verbose)
               {
                 print_list = add_list(print_list,r,c,sh_rows[r][c]);
	       }
               pthread_mutex_lock(&mutexsum);
               sh_count++; 
               pthread_mutex_unlock(&mutexsum);
	}
                  
     }
  }
  
  pthread_exit(print_list);  /* Modify code to return the list back to the calling thread */

}

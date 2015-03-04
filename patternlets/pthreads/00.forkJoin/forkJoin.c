/* forkJoin.c demonstrates the fork-join pattern using pthreads.
 *
 * Joel Adams, Calvin College, Fall 2013.
 *
 * Usage: ./forkJoin
 */

#include <stdio.h>   // printf()
#include <pthread.h> // pthread types and functions

void* childGreetings(void * param) { 
   printf("Greetings from the child thread\n");
   return NULL;
}

int main() {
  pthread_t child;                     // struct for child-thread info
                                       // FORK child thread:
  pthread_create( &child,              //  our handle for it
                   NULL,               //  its attributes 
                   childGreetings,     //  the function it should run
                   NULL );             //  args to that function
  
  printf("Greetings from the parent (main) thread.\n");

  pthread_join( child, NULL );         // JOIN: wait for child to finish
  
  return 0;
}


/* forkJoin5.c demonstrates the fork-join pattern using pthreads,
 *   using multiple threads and successfully passes multiple arguments
 *   of different types using a struct.
 *
 * Joel Adams, Calvin College, Fall 2013.
 * Libby Shoop, Macalester College, Fall 2014.
 *      changed for using C time.h inside additional timer.h file
 *
 * Usage: ./forkJoin5 [numThreads]
 */

#include <stdio.h>   // printf()
#include <stdlib.h>  // atoi(), exit(), ...
#include <pthread.h> // pthread types and functions

#include "timer.h"

typedef struct {
     unsigned long id;
     unsigned long numThreads;
     double startTime;
} argStruct;

void* childGreetings(void * buf) { 
   double stopTime; 
   GET_TIME(stopTime);
   argStruct * args = (argStruct*) buf;
   unsigned long childID = args->id;
   unsigned long numThreads = args->numThreads;
   double startTime = args->startTime;
   double creationTime = stopTime - startTime;
   printf("Greetings from child #%lu of %lu which took %lf secs to create\n", 
           childID, numThreads, creationTime);
   free(args);
   return NULL;
}

unsigned long processCommandLine(int argc, char** argv) {
   if (argc == 2) {
      return strtoul(argv[1], 0, 10);
   } else if (argc == 1) {
      return 1;
   } else {
      fprintf(stderr, "\nUsage: ./forkJoin2 [numThreads]\n");
      exit(1);
   }
}

int main(int argc, char** argv) {
  pthread_t * children;                   // dynamic array of child threads
  unsigned long id = 0;                   // loop control variable
  unsigned long numThreads = 0;           // desired # of threads
  argStruct * args;                       // argument buffer
                                          // get desired # of threads
  numThreads = processCommandLine(argc, argv);
                                          // allocate array of handles
  children = malloc( numThreads * sizeof(pthread_t) );
                                          // FORK:
  for (id = 1; id < numThreads; id++) {
     args = malloc( sizeof( argStruct ) );
     args->id = id; 
     args->numThreads = numThreads;
     
     GET_TIME(args->startTime);           // collect time just before creating
     pthread_create( &(children[id-1]),   // our handle for the child 
                      NULL,               // attributes of the child
                      childGreetings,     // the function it should run
                      (void*) args );     // args to that function
  }
  
  printf("Greetings from the parent (main) thread.\n");

  for (id = 1; id < numThreads; id++) {   // JOIN:
     pthread_join( children[id-1], NULL );
  }
  
  free(children);                         // deallocate array

  return 0;
}


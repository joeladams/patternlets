/* forkJoin6.c demonstrates the fork-join pattern using pthreads,
 *   using multiple threads. It passes an argument to each child thread,
 *   which returns a value to the parent thread.
 *
 * Joel Adams, Calvin College, Fall 2013.
 *
 * Usage: ./forkJoin6 [numThreads]
 */

#include <stdio.h>   // printf()
#include <stdlib.h>  // atoi(), exit(), ...
#include <pthread.h> // pthread types and functions

void* child(void * buf) { 
   unsigned long childID = (unsigned long) buf;
   unsigned long idSquared = childID * childID;
   printf("Child #%lu computed %lu\n", 
           childID, idSquared);
   return (void*) idSquared;
}

unsigned long processCommandLine(int argc, char** argv) {
   if (argc == 2) {
      return strtoul(argv[1], 0, 10);
   } else if (argc == 1) {
      return 1;
   } else {
      fprintf(stderr, "\nUsage: ./forkJoin6 [numThreads]\n");
      exit(1);
   }
}

int main(int argc, char** argv) {
  pthread_t * children;                   // dynamic array of child threads
  unsigned long id = 0;                   // loop control variable
  unsigned long numThreads = 0;           // desired # of threads
  unsigned long sumSquares = 0;
  unsigned long childsSquare = 0;
                                          // get desired # of threads
  numThreads = processCommandLine(argc, argv);
                                          // allocate array of handles
  children = malloc( numThreads * sizeof(pthread_t) );
                                          // FORK:
  for (id = 1; id < numThreads; id++) {
     pthread_create( &(children[id-1]),   // our handle for the child 
                      NULL,               // attributes of the child
                      child,              // the function it should run
                      (void*) id );       // args to that function
  }
  
  printf("The parent (main) thread computed %lu.\n", sumSquares);

  for (id = 1; id < numThreads; id++) {   // JOIN:
     pthread_join( children[id-1], (void**) &childsSquare );
     sumSquares += childsSquare;
  }

  printf("\nThe sum of the squares from 1 to %lu is %lu.\n\n",
          numThreads-1, sumSquares);
  
  free(children);                         // deallocate array

  return 0;
}


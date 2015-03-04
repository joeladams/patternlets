/* forkJoin3.c demonstrates the fork-join pattern using pthreads,
 *   using multiple threads and passing multiple arguments.
 *   It contains a race condition that forJoin4.c fixes.
 *
 * Joel Adams, Calvin College, Fall 2013.
 *
 * Usage: ./forkJoin3 [numThreads]
 */

#include <stdio.h>   // printf()
#include <stdlib.h>  // atoi(), exit(), ...
#include <pthread.h> // pthread types and functions

void* childGreetings(void * id) { 
   unsigned long * buf = (unsigned long*) id;
   unsigned long childID = buf[0];
   unsigned long numThreads = buf[1];
   printf("Greetings from child #%lu of %lu\n", 
           childID, numThreads);
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
  unsigned long buffer[2];                // argument buffer
                                          // get desired # of threads
  numThreads = processCommandLine(argc, argv);
                                          // allocate array of handles
  children = malloc( numThreads * sizeof(pthread_t) );
                                          // FORK:
  for (id = 1; id < numThreads; id++) {
     buffer[0] = id, buffer[1] = numThreads;
     pthread_create( &(children[id-1]),   // our handle for the child 
                      NULL,               // attributes of the child
                      childGreetings,     // the function it should run
                      (void*) buffer );   // args to that function
  }
  
  printf("Greetings from the parent (main) thread.\n");

  for (id = 1; id < numThreads; id++) {   // JOIN:
     pthread_join( children[id-1], NULL );
  }
  
  free(children);                         // deallocate array

  return 0;
}


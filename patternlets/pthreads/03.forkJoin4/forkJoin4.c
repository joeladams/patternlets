/* forkJoin4.c demonstrates the fork-join pattern using pthreads,
 *   using multiple threads and successfully passes multiple arguments
 *   of the same type, using an array.
 *
 * Joel Adams, Calvin College, Fall 2013.
 *
 * Usage: ./forkJoin4 [numThreads]
 */

#include <stdio.h>   // printf()
#include <stdlib.h>  // atoi(), exit(), ...
#include <pthread.h> // pthread types and functions

void* childGreetings(void * args) { 
   unsigned long * buf = (unsigned long*) args;
   unsigned long childID = buf[0];
   unsigned long numThreads = buf[1];
   printf("Greetings from child #%lu of %lu\n", 
           childID, numThreads);
   free(buf);
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
  unsigned long *argBuffer;               // argument buffer
                                          // get desired # of threads
  numThreads = processCommandLine(argc, argv);
                                          // allocate array of handles
  children = malloc( numThreads * sizeof(pthread_t) );
                                          // FORK:
  for (id = 1; id < numThreads; id++) {
     argBuffer = malloc( 2 * sizeof(unsigned long) );
     argBuffer[0] = id; argBuffer[1] = numThreads;
     pthread_create( &(children[id-1]),   // our handle for the child 
                      NULL,               // attributes of the child
                      childGreetings,     // the function it should run
                      (void*) argBuffer );// args to that function
  }
  
  printf("Greetings from the parent (main) thread.\n");

  for (id = 1; id < numThreads; id++) {   // JOIN:
     pthread_join( children[id-1], NULL );
  }
  
  free(children);                         // deallocate array

  return 0;
}


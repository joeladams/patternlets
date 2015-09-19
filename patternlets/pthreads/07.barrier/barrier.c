/* barrier.c demonstrates the Barrier pattern using pthreads
 *
 * Joel Adams, Calvin College, Fall 2013.
 *
 * Usage: ./barrier [numThreads]
 * 
 * Exercise:
 *   1. Build and run several times, for each of 2, 4, 8, 16, ... threads.
 *   2. Uncomment the call to Barrier in helloGoodbye()..
 *   3. Save, rebuild, rerun, and note the difference in results.
 */

#include <stdio.h>              // printf()
#include <stdlib.h>             // strtoul(), exit(), ...

#include "pthreadBarrier.h"     // pthreadBarrier call and data

void* helloGoodbye(void * buf) { 
   unsigned long * argBuf = (unsigned long*) buf;
   unsigned long id = argBuf[0];
   unsigned long numThreads = argBuf[1];
   printf("Hello from thread #%lu of %lu.\n", id, numThreads);  

//   pthreadBarrier(numThreads);

   printf("Goodbye from thread #%lu\n", id);  
   free(argBuf);
   return NULL;
}

unsigned long processCommandLine(int argc, char** argv) {
   if (argc == 2) {
      return strtoul(argv[1], 0, 10);
   } else if (argc == 1) {
      return 1;
   } else {
      fprintf(stderr, "\nUsage: ./barrier [numThreads]\n");
      exit(1);
   }
}

int main(int argc, char** argv) {
  pthread_t * children;                   // dynamic array of child threads
  unsigned long id = 0;                   // loop control variable
  unsigned long numThreads = 0;           // number of threads
  unsigned long *args;                    // arg array
                                          // get desired # of threads
  numThreads = processCommandLine(argc, argv);
                                          // allocate array of handles
  children = malloc( numThreads * sizeof(pthread_t) );
                                          // FORK:
  for (id = 1; id < numThreads; id++) {
     args = malloc( 2 * sizeof(unsigned long) );
     args[0] = id; args[1] = numThreads;
     pthread_create( &(children[id-1]),   // our handle for the child 
                      NULL,               // attributes of the child
                      helloGoodbye,       // the function it should run
                      (void*) args );     // args to that function
  }
  
  args = malloc( 2 * sizeof(unsigned long) );
  args[0] = 0; args[1] = numThreads;
  helloGoodbye( (void*)args );

  for (id = 1; id < numThreads; id++) {   // JOIN:
     pthread_join( children[id-1], NULL );
  }

  free(children);                         // deallocate array
  barrierCleanup();                       // deallocate barrier data
  return 0;
}


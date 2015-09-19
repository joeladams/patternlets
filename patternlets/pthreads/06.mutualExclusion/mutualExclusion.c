/* mutualExclusion.c demonstrates the Mutual Exclusion pattern using pthreads
 *  to deposit and withdraw $$ from a bank account.
 *  When an even number of threads are used, the final balance should be $0.00,
 *   but this does not happen, due to a race condition.
 *
 * Joel Adams, Calvin College, Fall 2013.
 *
 * Usage: ./mutualExclusion [evenNumberOfThreads]
 * 
 * Exercise:
 *   1. Build and run several times, for each of 2, 4, 8, 16, ... threads.
 *   2. In bankAccount.h:
 *      - Surround each nonlocal access to bankAccountBalance with:
 *             pthread_mutex_lock( &lock );   // before the access
 *             pthread_mutex_unlock( &lock ); // after the access
 *   3. Save, rebuild, rerun, and note the difference in results.
 */

#include <stdio.h>        // printf()
#include <stdlib.h>       // atoi(), exit(), ...
#include <pthread.h>      // pthread types and functions

#include "bankAccount.h"  // bank account info

// utility function to identify even-odd numbers
unsigned odd(unsigned long num) { return num % 2; }

// simulate id performing 1000 transactions 
void do1000Transactions(unsigned long id) {
   for (unsigned i = 0; i < 1000; i++) {
      if ( odd( id ) ) {  
         deposit(100.00);   // odd threads deposit
      } else {
         withdraw(100.00);  // even threads withdraw
      }
   }
}

void* child(void * buf) { 
   unsigned long childID = (unsigned long) buf;
   do1000Transactions(childID);  
   return NULL;
}

unsigned long processCommandLine(int argc, char** argv) {
   if (argc == 2) {
      return strtoul(argv[1], 0, 10);
   } else if (argc == 1) {
      return 1;
   } else {
      fprintf(stderr, "\nUsage: ./mutualExclusion [evenNumberOfThreads]\n");
      exit(1);
   }
}

int main(int argc, char** argv) {
  pthread_t * children;                   // dynamic array of child threads
  unsigned long id = 0;                   // loop control variable
  unsigned long numThreads = 0;           // desired # of threads
                                          // get desired # of threads
  numThreads = processCommandLine(argc, argv);
                                          // allocate array of handles
  children = malloc( numThreads * sizeof(pthread_t) );
                                          // FORK:
  for (id = 1; id < numThreads; ++id) {
     pthread_create( &(children[id-1]),   // our handle for the child 
                      NULL,               // attributes of the child
                      child,              // the function it should run
                      (void*) id );       // args to that function
  }
  
  do1000Transactions(0);

  for (id = 1; id < numThreads; ++id) {   // JOIN:
     pthread_join( children[id-1], NULL );
  }

  printf("\nThe final balance of the account using %lu threads is $%.2f.\n\n",
          numThreads, bankAccountBalance);
  
  free(children);                         // deallocate array
  cleanup();                              // deallocate mutex
  return 0;
}


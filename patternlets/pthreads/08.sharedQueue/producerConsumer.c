/* producerConsumer.c uses a TS_Queue
 *  to solve the Producer-Consumer problem.
 *
 * Joel Adams, Calvin College, Fall 2015.
 *
 * Usage: ./producerConsumer [numProducers] [numConsumers] [numItems]
 *
 * Note: For best results: numProducers == numConsumers,
 *        or a process will be left hanging.
 */

#include "TS_Queue.h"
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>   // malloc, atoi, ...

void runProducerConsumer();

int main(int argc, char** argv) {
   runProducerConsumer(argc, argv);
}

 
void processCommandLineArgs( int argc, char** argv,
                             unsigned long* numProducers,
                             unsigned long* numConsumers,
                             unsigned long* numItems) {
   *numProducers = *numConsumers = *numItems = 1;
   switch (argc) {
      case 4: *numItems = atoi( argv[3] );
      case 3: *numConsumers = atoi( argv[2] );
      case 2: *numProducers = atoi( argv[1] ); break;
      default:
              fprintf(stderr, "\nUsage: ./producerConsumer [numProducers] [numConsumer] [numItems]\n");
   }
}

unsigned long numItems = 0;
TS_Queue* tsQueue = NULL;

void* produce( void* arg ) {
   unsigned long id = (unsigned long) arg;
   printf("\nProducer %ld: started", id); fflush(stdout);
   for (int i = 0; i < numItems; i++) {
//printf("\nProducer %ld produced: %ld\n", id, id+i+11); fflush(stdout);
      append( 10*id+i, tsQueue );
   }
   pthread_exit(NULL);
   return NULL;
}

void* consume( void* arg ) {
   unsigned long id = (unsigned long) arg;
   printf("\nConsumer %ld: started", id); fflush(stdout);
   for (int i = 0; i < numItems; i++) {
      Item it = removeFirst(tsQueue);
      printf("\nConsumer %ld removed: %ld\n", id, it); fflush(stdout);
   }
   pthread_exit(NULL);
   return NULL;
}

void runProducerConsumer( int argc, char** argv ) {
   unsigned long numProducers = 0;
   unsigned long numConsumers = 0;

   pthread_t * consumers;
   pthread_t * producers;
   unsigned long i = 0;
   unsigned long id = 0;

   processCommandLineArgs(argc, argv, 
                           &numProducers, &numConsumers,
                           &numItems);
   tsQueue = createQueue(8);

   consumers = malloc( numConsumers * sizeof(pthread_t) );
   for (id = 1; id <= numConsumers; id++) {
      pthread_create( &(consumers[id-1]), 
                      NULL,
                      consume,     // the function it should run
                      (void*) id);       // args to that function
   }
   producers = malloc( numProducers * sizeof(pthread_t) );
   for (id = 1; id <= numProducers; id++) {
      pthread_create( &(producers[id-1]), 
                      NULL,
                      produce,     // the function it should run
                      (void*) id);       // args to that function
  }

  for (i = 1; i <= numConsumers; i++) {
      pthread_join( consumers[i-1], NULL );
  }
  for (i = 1; i <= numProducers; i++) {
      pthread_join( producers[i-1], NULL );
  }

  assert( getSize(tsQueue) == 0 );

  free(producers);
  free(consumers);
  destroy(&tsQueue);
  printf("\nProducers and Consumers terminated normally \n"); fflush(stdout);
}


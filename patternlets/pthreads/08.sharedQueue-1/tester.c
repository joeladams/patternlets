
#include "SharedQueue.h"
#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

void runQueueTests();
void runMultithreadingTests();

int main() {
   printf("\nTesting SharedQueue...\n\n");
//   runQueueTests();
   runMultithreadingTests();
   printf("\nAll tests passed!\n\n");
}

const int NUM = 1000;
SharedQueue* tsQueue = NULL;

void* produce( void* arg ) {
   unsigned long id = (unsigned long) arg;
printf("\nProducer %ld: started", id); fflush(stdout);
   for (int i = 0; i < NUM; i++) {
      sqAppend(tsQueue, 10*i+id);
//printf("\nProducer: %ld appended", id+i+11); fflush(stdout);
   }
   pthread_exit(NULL);
   return NULL;
}

void* consume( void* arg ) {
   unsigned long id = (unsigned long) arg;
printf("\nConsumer %ld: started", id); fflush(stdout);
   for (int i = 0; i < NUM; i++) {
      Item it = sqRemoveFirstItem(tsQueue);
      printf("\nConsumer %ld: %ld removed\n", id, it); fflush(stdout);
   }
   pthread_exit(NULL);
   return NULL;
}

void runMultithreadingTests() {
/*
   pthread_t producer;
   pthread_t consumer;
   unsigned long producerID = 1;
   unsigned long consumerID = 2;
 
   printf("\n- multithreading... "); fflush(stdout);

   tsQueue = sqCreate();
   pthread_create( &consumer, NULL, consume, (void*) consumerID);
   printf("\nmain(): consumer launched\n"); fflush(stdout);
   pthread_create( &producer, NULL, produce, (void*) producerID);
   printf("\nmain(): producer launched\n"); fflush(stdout);

   pthread_join(producer, NULL);
   printf("\nmain(): producer finished\n"); fflush(stdout);
   pthread_join(consumer, NULL);
   printf("\nmain(): consumer finished\n"); fflush(stdout);

   sqDestroy(&tsQueue);
*/   
   pthread_t * consumers;
   pthread_t * producers;
   unsigned long numConsumers = 4;
   unsigned long numProducers = 4;
   unsigned long i = 0;
   unsigned long id = 0;

   tsQueue = sqCreate();

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

   printf(" Passed!\n"); fflush(stdout);
   free(producers);
   free(consumers);
   sqDestroy(&tsQueue);
}

void runQueueTests() {
   printf("- initialization... "); fflush(stdout);
   SharedQueue* q = sqCreate();
   assert( sqSize(q) == 0 );
   printf(" 0 "); fflush(stdout);   
   printf(" Passed!\n"); fflush(stdout);

   printf("- append(), peekFirst(), peekLast(), ... "); fflush(stdout);
   sqAppend(q, 11);
   assert( sqSize(q) == 1 );
   assert( sqPeekFirstItem(q) == 11 );
   assert( sqPeekLastItem(q) == 11 );
   printf(" 0 "); fflush(stdout);   
   sqAppend(q, 22);
   assert( sqSize(q) == 2 );
   assert( sqPeekFirstItem(q) == 11 );
   assert( sqPeekLastItem(q) == 22 );
   printf(" 1 "); fflush(stdout);   
   sqAppend(q, 33);
   assert( sqSize(q) == 3 );
   assert( sqPeekFirstItem(q) == 11 );
   assert( sqPeekLastItem(q) == 33 );
   printf(" 2 "); fflush(stdout);   
   assert( sqRemoveFirstItem(q) == 11 );
   assert( sqSize(q) == 2 );
   assert( sqPeekFirstItem(q) == 22 );
   assert( sqPeekLastItem(q) == 33 );
   printf(" 3 "); fflush(stdout);   
   assert( sqRemoveFirstItem(q) == 22 );
   assert( sqSize(q) == 1 );
   assert( sqPeekFirstItem(q) == 33 );
   assert( sqPeekLastItem(q) == 33 );
   printf(" 4 "); fflush(stdout);   
   assert( sqRemoveFirstItem(q) == 33 );
   assert( sqSize(q) == 0 );
   printf(" 5 "); fflush(stdout);   
   printf(" Passed!\n"); fflush(stdout);

   printf("- clear() and destroy()... "); fflush(stdout);
   sqAppend(q, 99);
   sqAppend(q, 98);
   sqAppend(q, 97);
   sqClear(q);
   assert( sqSize(q) == 0 );
   printf(" 0 "); fflush(stdout);
   sqAppend(q,55);
   sqAppend(q,66);
   sqDestroy(&q);
   printf(" 1 "); fflush(stdout);
   printf(" Passed!\n"); fflush(stdout);
}
   

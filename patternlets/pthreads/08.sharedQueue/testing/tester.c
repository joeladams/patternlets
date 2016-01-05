/* tester.c tests the operations on a TS_Queue.
 *
 * Joel Adams, Fall 2015.
 *
 * Usage: ./tester
 */

#include "../TS_Queue.h"
#include <pthread.h>
#include <assert.h>

void runTests() {
   printf("\nTesting TS_Queue...\n\n");
   printf("- initialization... "); fflush(stdout);
   TS_Queue* q = createQueue(4);
   assert( getSize(q) == 0 );
   assert( getCapacity(q) == 4 );
   printf(" 0 "); fflush(stdout);   
   printf(" Passed!\n"); fflush(stdout);

   printf("- append(), peekFirst(), peekLast(), ... "); fflush(stdout);
   append(11, q);                    // 11, __, __, __
   assert( getSize(q) == 1 );
   assert( getCapacity(q) == 4 );
   assert( peekFirst(q) == 11 );
   assert( peekLast(q) == 11 );
   printf(" 0 "); fflush(stdout);   
   append(22, q);                    // 11, 22, __, __
   assert( getSize(q) == 2 );
   assert( getCapacity(q) == 4 );
   assert( peekFirst(q) == 11 );
   assert( peekLast(q) == 22 );
   printf(" 1 "); fflush(stdout);   
   append(33, q);                    // 11, 22, 33, __
   assert( getSize(q) == 3 );
   assert( getCapacity(q) == 4 );
   assert( peekFirst(q) == 11 );
   assert( peekLast(q) == 33 );
   printf(" 2 "); fflush(stdout);   
   assert( removeFirst(q) == 11 );   // __, 22, 33, __
   assert( getSize(q) == 2 );
   assert( getCapacity(q) == 4 );
   assert( peekFirst(q) == 22 );
   assert( peekLast(q) == 33 );
   printf(" 3 "); fflush(stdout);   
   assert( removeFirst(q) == 22 );  // __, __, 33, __
   assert( getSize(q) == 1 );
   assert( getCapacity(q) == 4 );
   assert( peekFirst(q) == 33 );
   assert( peekLast(q) == 33 );
   printf(" 4 "); fflush(stdout);   
   assert( removeFirst(q) == 33 );  // __, __, __, __
   assert( getSize(q) == 0 );
   assert( getCapacity(q) == 4 );
   printf(" 5 "); fflush(stdout);   
   printf(" Passed!\n"); fflush(stdout);

   printf("- setCapacity()... "); fflush(stdout);
   // set up queue with a gap
   append(99, q);                   // 99, __, __, __
   append(98, q);                   // 99, 98, __, __
   append(97, q);                   // 99, 98, 97, __
   append(96, q);                   // 99, 98, 97, 96
   assert( getSize(q) == getCapacity(q) );
   assert( removeFirst(q) == 99 );  // __, 98, 97, 96
   assert( removeFirst(q) == 98 );  // __, __, 97, 96
   assert( removeFirst(q) == 97 );  // __, __, __, 96
   append(95, q);                   // 95, __, __, 96
   assert( getSize(q) == 2 );
   assert( peekFirst(q) == 96 );
   assert( peekLast(q) == 95 );
   assert( getCapacity(q) == 4 );
   setCapacity(q, 5);               // 96, 95, __, __, __
   assert( getSize(q) == 2 );
   assert( getCapacity(q) == 5 );
   assert( peekFirst(q) == 96 );
   assert( peekLast(q) == 95 );
   printf(" 0 "); fflush(stdout);
   append(94, q);                   // 96, 95, 94, __, __
   append(93, q);                   // 96, 95, 94, 93, __
   append(92, q);                   // 96, 95, 94, 93, 92
   assert( getSize(q) == getCapacity(q) );
   assert( getSize(q) == 5 );
   assert( removeFirst(q) == 96 );  // __, 95, 94, 93, 92
   assert( removeFirst(q) == 95 );  // __, __, 94, 93, 92
   assert( removeFirst(q) == 94 );  // __, __, __, 93, 92
   assert( removeFirst(q) == 93 );  // __, __, __, __, 92
   append(91, q);                   // 91, __, __, __, 92
   append(90, q);                   // 91, 90, __, __, 92
   assert( getSize(q) == 3 );
   assert( getCapacity(q) == 5 );
   setCapacity(q, 2);               // 92, 91
   assert( getSize(q) == 2 );
   assert( getCapacity(q) == 2 );
   assert( peekFirst(q) == 92 );
   assert( peekLast(q) == 91 );
   printf(" 1 "); fflush(stdout);
   printf(" Passed!\n"); fflush(stdout);

   printf("- clear() and destroy()... "); fflush(stdout);
   clear(q);
   assert( getSize(q) == 0 );
   assert( getCapacity(q) == 2 );
   printf(" 0 "); fflush(stdout);
   append(55, q);
   append(66, q);
   assert( getSize(q) == 2 );
   assert( getCapacity(q) == 2 );
   clear(q);
   assert( getSize(q) == 0 );
   assert( getCapacity(q) == 2 );
   printf(" 1 "); fflush(stdout);
   destroy(&q);
   printf(" 2 "); fflush(stdout);
   printf(" Passed!\n"); fflush(stdout);

   printf("\nAll tests passed!\n\n");
}

int main(int argc, char** argv) {
   runTests();
}


/* TS_Queue.h declares the interface to a thread-safe queue
 *  - a bounded buffer for use in the Shared Queue pattern.
 *
 * Joel Adams, Fall 2015, Calvin College
 */

#ifndef TS_QUEUE_H
#define TS_QUEUE_H

#include <stdio.h>

// the following typedef defines the
// generic Items our TS_Queue stores 
typedef long Item;

// hide the TS_Queue's implementation details
// (in TS_Queue.c)
struct ThreadSafeQueue;
typedef struct ThreadSafeQueue TS_Queue;

// --- the operations on a TS_Queue ----
// creation and initialization
TS_Queue* createQueue(unsigned capacity);

// retrieve size (# Items)
unsigned getSize(TS_Queue* tsq);

// retrieve capacity (space for Items)
unsigned getCapacity(TS_Queue* tsq);

// retrieve capacity (space for Items)
void setCapacity(TS_Queue* tsq, unsigned newCapacity);

// append an Item
void append(const Item item, TS_Queue* tsq);

// remove oldest Item
Item removeFirst(TS_Queue* tsq);

// peek at the oldest Item
Item peekFirst(TS_Queue* tsq);

// peek at the newest Item
Item peekLast(TS_Queue* tsq);

// remove all Items from a queue
void clear(TS_Queue* tsq);

// deallocate a queue created with sqCreate()
void destroy(TS_Queue** tsq);

#endif // TS_QUEUE


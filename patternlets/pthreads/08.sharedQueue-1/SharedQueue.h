/* SharedQueue.h provides the interface to
*   a thread-safe shared queue.
 *
 * Joel Adams, Fall 2015, Calvin College
 */

#ifndef SHARED_QUEUE_H
#define SHARED_QUEUE_H

#include <stdio.h>

// the following typedef defines the
// generic Items our SharedQueue stores 
typedef long Item;

// hide the SharedQueue's implementation details
// (in SharedQueue.c)
struct PrivateSharedQueue;
typedef struct PrivateSharedQueue SharedQueue;

// --- the operations on a SharedQueue ----
// creation and initialization
SharedQueue* sqCreate();

// retrieve size
unsigned sqSize(SharedQueue* sq);

// append an Item
void sqAppend(SharedQueue* sq, const Item item);

// remove oldest Item
Item sqRemoveFirstItem(SharedQueue* sq);

// peek at the oldest Item
Item sqPeekFirstItem(SharedQueue* sq);

// peek at the newest Item
Item sqPeekLastItem(SharedQueue* sq);

// remove all Items from a queue
void sqClear(SharedQueue* sq);

// deallocate a queue created with sqCreate()
void sqDestroy(SharedQueue** sq);

#endif


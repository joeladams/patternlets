/* SharedQueue.c defines a thread-safe shared queue
 *  and its operations.
 *
 * Joel Adams, Fall 2015, Calvin College
 *
 */

#include "SharedQueue.h"
#include <stdlib.h>       // malloc()
#include <pthread.h>      // various pthread functions

// # of items a SharedQueue can store at a time
#define CAPACITY 8

// SharedQueue implementation details
struct PrivateSharedQueue {
	Item          myArray[CAPACITY];
	unsigned      mySize;
	int           myFirstIndex;
	int           myLastIndex;
};

// lock for the SharedQueue
pthread_mutex_t qLock              = PTHREAD_MUTEX_INITIALIZER;
// condition variables for the SharedQueue
pthread_cond_t  qNotEmptyCondition = PTHREAD_COND_INITIALIZER;
pthread_cond_t  qNotFullCondition  = PTHREAD_COND_INITIALIZER;

/*
 * Create and initialize a SharedQueue.
 * Only the main thread should do this.
 *
 * @param: sq, the address of a SharedQueue object.
 *
 * PRE: sq is a valid SharedQueue object address
 * POST: sq->mySize == 0 &&
 *       sq->myFirstIndex == 0 &&
 *       sq->myLastIndex == -1.
 */
SharedQueue* sqCreate() {
	SharedQueue * sq = malloc( sizeof(SharedQueue) );
	if (!sq) { fprintf(stderr, "\nsqCreate(): unable to malloc SharedQueue\n"); }

	sq->mySize = 0; 
	sq->myFirstIndex = 0;
	sq->myLastIndex = -1;
	return sq;
}

/*
 * SharedQueue size accessor.
 *
 * @param: sq, the address of a SharedQueue object
 *
 * PRE: sq is a valid SharedQueue object address
 * Return: the number of Items in *sq.
 */
unsigned sqSize(SharedQueue* sq) {
	unsigned result = 0;
	pthread_mutex_lock( &qLock );
	result = sq->mySize;
	pthread_mutex_unlock( &qLock );
	return result;
}

/*
 * Append an Item to a SharedQueue.
 *
 * @param: sq, the address of a SharedQueue object
 * @param: item, an Item
 *
 * POST: item has been appended to *sq.
 */
void sqAppend(SharedQueue* sq, const Item item) {
	// lock the door
	pthread_mutex_lock( &qLock );
	// wait if the queue is full
	while (sq->mySize == CAPACITY) {
		pthread_cond_wait( &qNotFullCondition, &qLock );
	}
	// append an item to the queue
	sq->myLastIndex = (sq->myLastIndex + 1) % CAPACITY;
	sq->myArray[ sq->myLastIndex ] = item;
	(sq->mySize)++;
	// signal the queue is not empty
	pthread_cond_signal( &qNotEmptyCondition );
	// unlock the door
	pthread_mutex_unlock( &qLock );
}

/*
 * Remove oldest Item from a SharedQueue.
 *
 * @param: sq, the address of a SharedQueue.
 *
 * PRE: sq->mySize > 0.
 * POST: sq's first Item has been removed.
 * Return: the oldest Item in sq.
 */
Item sqRemoveFirstItem(SharedQueue* sq) {
	// lock the door
	pthread_mutex_lock( &qLock );
	Item result = (Item)0;
	// wait if the queue is empty
	while (sq->mySize == 0) {
		pthread_cond_wait( &qNotEmptyCondition, &qLock );
	}
	// remove the oldest item
	result = sq->myArray[sq->myFirstIndex];
	sq->myFirstIndex = (sq->myFirstIndex + 1) % CAPACITY;
	(sq->mySize)--;
	// signal the queue is not full
	pthread_cond_signal( &qNotFullCondition );
	// unlock the door
	pthread_mutex_unlock( &qLock );

	return result;
}

/*
 * Peek at oldest Item in a SharedQueue.
 *
 * @param: sq, the address of a SharedQueue.
 *
 * PRE: sq->mySize > 0
 * Return: the oldest Item in sq.
 */
Item sqPeekFirstItem(SharedQueue* sq) {
	Item result = (Item)0;
	pthread_mutex_lock( &qLock );
	while (sq->mySize == 0) {
		pthread_cond_wait( &qNotEmptyCondition, &qLock );
	}
	result = sq->myArray[sq->myFirstIndex];
	pthread_mutex_unlock( &qLock );
	return result;
}

/*
 * Peek at newest Item in a SharedQueue.
 *
 * @param: sq, the address of a SharedQueue.
 *
 * PRE: sq->mySize > 0
 * Return: the newest Item in sq.
 */
Item sqPeekLastItem(SharedQueue* sq) {
	Item result = (Item)0;
	pthread_mutex_lock( &qLock );
        while (sq->mySize == 0) {
		pthread_cond_wait( &qNotEmptyCondition, &qLock );
	}
	result = sq->myArray[sq->myLastIndex];
	pthread_mutex_unlock( &qLock );
	return result;
}

/* 
 * Clear (empty out) the queue.
 * Usually only the main thread should do this.
 *
 * @param: sq, the address of a SharedQueue
 *
 * POST: sq's Nodes have been deallocated.
 *             && sq->mySize == 0
 *             && sq->myFirstIndex == 0
 *             && sq->myLastIndex == 0.
 */
void sqClear(SharedQueue* sq) {
	pthread_mutex_lock( &qLock );
	sq->mySize = 0;
	sq->myFirstIndex = 0;
	sq->myLastIndex = -1;
	pthread_mutex_unlock( &qLock );
}

/*
 * Deallocate a SharedQueue.
 * Only the main thread should do this.
 *
 * @param: sq, the address of a pointer to a SharedQueue.
 *
 * POST: *sq has been deallocated
 *      && sq == NULL
 *      && qNotEmptyCondition has been destroyed
 *      && qNotFullCondition has been destroyed
 *      ** qLock has been destroyed.
 */ 
void sqDestroy(SharedQueue** sq) {
	// deallocate the queue itself
	free(*sq);
	sq = NULL;
	// deallocate the mutex and condition
	pthread_cond_destroy( &qNotEmptyCondition );
	pthread_cond_destroy( &qNotFullCondition );
	pthread_mutex_destroy( &qLock );
}


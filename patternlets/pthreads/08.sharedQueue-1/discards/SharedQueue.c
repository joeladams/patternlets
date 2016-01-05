/* SharedQueue.c defines a thread-safe shared queue
 *  and its operations.
 *
 * Joel Adams, Fall 2015, Calvin College
 *
 */

#include "SharedQueue.h"
#include <stdlib.h>       // malloc()

/*
 * Create and initialize a SharedQueue.
 * Only the main thread should do this.
 *
 * @param: sq, the address of a SharedQueue object.
 *
 * PRE: sq is a valid SharedQueue object address
 * POST: sq->mySize == 0 &&
 *       sq->myFirst == NULL &&
 *       sq->myLast == NULL &&
 *       sq->myLock is initialized &&
 *       sq->myNotEmptyCond is initialized.
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
 *
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
printf("\nsqAppend(): received %ld...\n", item);
	pthread_mutex_lock( &qLock );
printf("\nsqAppend(): lock locked...\n");

	while (sq->mySize == CAPACITY) {
printf("\nsqAppend(): waiting on notFull...\n");
		pthread_cond_wait( &qNotFullCondition, &qLock );
	}

printf("\nsqAppend(): appending %ld...\n", item);
	sq->myArray[ sq->myLastIndex ] = item;
	sq->myLastIndex = (sq->myLastIndex + 1) % CAPACITY;
	(sq->mySize)++;

printf("\nsqAppend(): %ld appended...\n", item);
	pthread_cond_signal( &qNotEmptyCondition );
printf("\nsqAppend(): notEmptyCond signaled...\n");
	pthread_mutex_unlock( &qLock );
printf("\nsqAppend(): lock unlocked...\n");
}

/*
 * Peek at oldest Item in a SharedQueue.
 *
 * @param: sq, the address of a SharedQueue.
 *
 * Precondition: sq->mySize > 0
 *
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
 * Precondition: sq->mySize > 0
 *
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
 * Remove oldest Item from a SharedQueue.
 *
 * @param: sq, the address of a SharedQueue.
 *
 * Precondition: sq->mySize > 0.
 *
 * Postcondition: sq's first Item has been removed.
 *
 * Return: the oldest Item in sq.
 */
Item sqRemoveFirstItem(SharedQueue* sq) {
printf("\nsqRemoveFirstItem(): running...\n");
	pthread_mutex_lock( &qLock );
printf("\nsqRemoveFirstItem(): lock locked...\n");
	Item result = (Item)0;
	while (sq->mySize == 0) {
printf("\nsqRemoveFirstItem(): waiting on not empty...\n");
		pthread_cond_wait( &qNotEmptyCondition, &qLock );
	}
printf("\nsqRemoveFirstItem(): past wait loop...\n");
	result = sq->myArray[sq->myFirstIndex];
	sq->myFirstIndex = (sq->myFirstIndex + 1) % CAPACITY;
	(sq->mySize)--;
printf("\nsqRemoveFirstItem(): Item %ld removed...\n", result);
	pthread_cond_signal( &qNotFullCondition );
printf("\nsqRemoveFirstItem(): signaling not full...\n");
	pthread_mutex_unlock( &qLock );
printf("\nsqRemoveFirstItem(): lock unlocked...\n");
	return result;
}

/* 
 * Deallocate just a SharedQueue's nodes.
 * Usually only the main thread should do this.
 *
 * @param: sq, the address of a SharedQueue
 *
 * Postcondition: sq's Nodes have been deallocated.
 *             && sq->mySize == 0
 *             && sq->myFirst == NULL
 *             && sq->myLast == NULL.
 */
void sqClear(SharedQueue* sq) {
	pthread_mutex_lock( &qLock );
	sq->mySize = 0;
	sq->myFirstIndex = 0;
	sq->myLastIndex = -1;
	pthread_mutex_unlock( &qLock );
}

/*
 * Deallocate a SharedQueue and its nodes.
 * Only the main thread should do this.
 *
 * @param: sq, the address of a pointer to a SharedQueue.
 *
 * Postcondition: all Nodes and *sq have been deallocated.
 */ 
void sqDestroy(SharedQueue** sq) {
	// deallocate the nodes
	sqClear(*sq);
	// deallocate the queue itself
	free(*sq);
	sq = NULL;
	// deallocate the mutex and cond
	pthread_cond_destroy( &qNotEmptyCondition );
	pthread_mutex_destroy( &qLock );
}


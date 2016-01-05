/* TS_Queue.c defines a thread-safe queue and its operations,
 * for use as a bounded buffer in the SharedQueue pattern.
 *
 * Joel Adams, Fall 2015, Calvin College
 *
 * Invariant: for a TS_Queue object:
 *            mySize == 0
 *            && myLastIndex == myFirstIndex-1
 *         || mySize > 0
 *            && myFirstIndex == the index of the oldest Item
 *            && myLastIndex == the index of the newest Item
 */

#include "TS_Queue.h"
#include <stdlib.h>       // malloc()
#include <pthread.h>      // various pthread functions

// SharedQueue implementation details
struct ThreadSafeQueue {
	Item *          myArray;
	unsigned        myCapacity;
	unsigned        mySize;
	int             myFirstIndex;
	int             myLastIndex;
	pthread_mutex_t myLock;
	pthread_cond_t  myNotEmptyCondition;
	pthread_cond_t  myNotFullCondition;
};


/*
 * Create and initialize a TS_Queue.
 * Only the main thread should do this.
 *
 * @param: capacity, the max number of Items TS_Queue should store.
 *
 * PRE: capacity > 0.
 * Return: tsq, the address of an empty SharedQueue object
 *               with space for 'capacity' Items..
 * POST: tsq->mySize == 0 &&
 *       tsq->myCapacity == capacity &&
 *       tsq->myArray points to an array whose length is capacity &&
 *       tsq->myFirstIndex == 0 &&
 *       tsq->myLastIndex == -1.
 */
TS_Queue* createQueue(unsigned capacity) {
	if (capacity == 0) {
		fprintf(stderr, "\ncreateQueue(n): n must be greater than zero\n");
		exit(1);
	}
	TS_Queue * tsq = malloc( sizeof(TS_Queue) );
	if (!tsq) { fprintf(stderr, "\nsqCreate(): unable to allocate TS_Queue\n"); }
	tsq->myArray = malloc( capacity * sizeof(Item) );
	if (!(tsq->myArray) ) { fprintf(stderr, "\ntsqCreate(): unable to allocate array\n"); }

	tsq->myCapacity = capacity;
	tsq->mySize = 0; 
	tsq->myFirstIndex = 0;
	tsq->myLastIndex = -1;
	pthread_mutex_init( &(tsq->myLock), NULL );
	pthread_cond_init( &(tsq->myNotEmptyCondition), NULL );
	pthread_cond_init( &(tsq->myNotFullCondition), NULL );
	return tsq;
}

/*
 * TS_Queue size accessor.
 *
 * @param: tsq, the address of a SharedQueue object
 *
 * PRE: tsq is a valid SharedQueue object address
 * Return: the number of Items in *tsq.
 */
unsigned getSize(TS_Queue* tsq) {
	unsigned result = 0;
	pthread_mutex_lock( &(tsq->myLock) );
	result = tsq->mySize;
	pthread_mutex_unlock( &(tsq->myLock) );
	return result;
}

/*
 * TS_Queue capacity accessor.
 *
 * @param: tsq, the address of a TS_Queue object
 *
 * PRE: tsq is a valid SharedQueue object address
 * Return: the space for Items in *tsq.
 */
unsigned getCapacity(TS_Queue* tsq) {
	unsigned result = 0;
	pthread_mutex_lock( &(tsq->myLock) );
	result = tsq->myCapacity;
	pthread_mutex_unlock( &(tsq->myLock) );
	return result;
}

/* resize the queue
 *
 * @param: tsq, the address of a TS_Queue object
 *         newCapacity, an unsigned int.
 *
 * PRE: newCapacity > 0
 * POST: tsq->myCapacity == newCapacity
 *     && tsq->myArray points to an array of size newCapacity
 *     && tsq->myFirst = 0
 *     && tsq->myLast = tsq->mySize-1
 *     && tsq's Items are in their correct (new) positions
 *         within tsq->myArray
 */
void setCapacity(TS_Queue* tsq, unsigned newCapacity) {
	unsigned i = 0;
	unsigned j = tsq->myFirstIndex;
	unsigned newSize = tsq->mySize;
	unsigned stop = 0;
	Item *   temp = NULL;

	// lock the door
	pthread_mutex_lock( &(tsq->myLock) );
	// check preconditions and special cases
	if ( newCapacity == 0 ) {
		printf("\nsetCapacity(q, newCapacity): capacity must be positive\n");
		exit(1);
	}
 	if ( newCapacity == tsq->myCapacity ) {
		printf("\nWARNING:\n\tsetCapacity(q, newCapacity): newCapacity == getCapacity(q)\n");
		return;
	 } else if ( newCapacity < tsq->mySize) {
		printf("\nWARNING:\n\tsetCapacity(q, newCapacity): newCapacity < getSize(q)\n\t- truncating Items\n");
		stop =  newCapacity;
		newSize = newCapacity;
	}  else {
		stop = tsq->mySize;
	}
	// allocate new array, fill it w/ items from old array
	temp = malloc( newCapacity * sizeof(Item) );
	for (i = 0; i < stop; i++) {
		temp[i] = tsq->myArray[j];
		j = (j + 1) % tsq->myCapacity;
	}
	// update all the members
	free( tsq->myArray );
	tsq->myArray = temp;
	tsq->mySize = newSize;
	tsq->myCapacity = newCapacity;
	tsq->myFirstIndex = 0;
	tsq->myLastIndex = tsq->mySize - 1;
	pthread_mutex_unlock( &(tsq->myLock) );
}

/*
 * Append an Item to a TS_Queue.
 *
 * @param: tsq, the address of a TS_Queue object
 * @param: item, an Item
 *
 * POST: item has been appended to *tsq.
 */
void append(const Item item, TS_Queue* tsq) {
	// lock the door
	pthread_mutex_lock( &(tsq->myLock) );
	// wait if the queue is full
	while (tsq->mySize == tsq->myCapacity) {
		pthread_cond_wait( &(tsq->myNotFullCondition), 
                                   &(tsq->myLock) );
	}
	// append an item to the queue
	tsq->myLastIndex = (tsq->myLastIndex + 1) % tsq->myCapacity;
	tsq->myArray[ tsq->myLastIndex ] = item;
	(tsq->mySize)++;
	// signal the queue is not empty
	pthread_cond_signal( &(tsq->myNotEmptyCondition) );
	// unlock the door
	pthread_mutex_unlock( &(tsq->myLock) );
}

/*
 * Remove oldest Item from a TS_Queue.
 *
 * @param: tsq, the address of a TS_Queue.
 *
 * PRE: tsq->mySize > 0.
 * POST: tsq's first Item has been removed.
 * Return: the oldest Item in tsq.
 */
Item removeFirst(TS_Queue* tsq) {
	// lock the door
	pthread_mutex_lock( &(tsq->myLock) );
	Item result = (Item)0;
	// wait if the queue is empty
	while (tsq->mySize == 0) {
		pthread_cond_wait( &(tsq->myNotEmptyCondition), 
                                   &(tsq->myLock) );
	}
	// remove the oldest item
	result = tsq->myArray[tsq->myFirstIndex];
	tsq->myFirstIndex = (tsq->myFirstIndex + 1) % tsq->myCapacity;
	(tsq->mySize)--;
	// signal the queue is not full
	pthread_cond_signal( &(tsq->myNotFullCondition) );
	// unlock the door
	pthread_mutex_unlock( &(tsq->myLock) );

	return result;
}

/*
 * Peek at oldest Item in a TS_Queue.
 *
 * @param: tsq, the address of a TS_Queue.
 *
 * PRE: tsq->mySize > 0
 * Return: the oldest Item in tsq.
 */
Item peekFirst(TS_Queue* tsq) {
	Item result = (Item)0;
	pthread_mutex_lock( &(tsq->myLock) );
	while (tsq->mySize == 0) {
		pthread_cond_wait( &(tsq->myNotEmptyCondition), 
                                   &(tsq->myLock) );
	}
	result = tsq->myArray[tsq->myFirstIndex];
	pthread_mutex_unlock( &(tsq->myLock) );
	return result;
}

/*
 * Peek at newest Item in a TS_Queue.
 *
 * @param: tsq, the address of a TS_Queue.
 *
 * PRE: tsq->mySize > 0
 * Return: the newest Item in tsq.
 */
Item peekLast(TS_Queue* tsq) {
	Item result = (Item)0;
	pthread_mutex_lock( &(tsq->myLock) );
        while (tsq->mySize == 0) {
		pthread_cond_wait( &(tsq->myNotEmptyCondition), 
                                   &(tsq->myLock) );
	}
	result = tsq->myArray[tsq->myLastIndex];
	pthread_mutex_unlock( &(tsq->myLock) );
	return result;
}

/* 
 * Clear (logically empty out) the queue.
 * Usually only the main thread should do this.
 *
 * @param: tsq, the address of a TS_Queue
 *
 * POST: tsq->mySize == 0
 *    && tsq->myFirstIndex == 0
 *    && tsq->myLastIndex == -1.
 */
void clear(TS_Queue* tsq) {
	pthread_mutex_lock( &(tsq->myLock) );
	tsq->mySize = 0;
	tsq->myFirstIndex = 0;
	tsq->myLastIndex = -1;
	pthread_mutex_unlock( &(tsq->myLock) );
}

/*
 * Deallocate a TS_Queue.
 * Only the main thread should do this.
 *
 * @param: tsq, the address of a pointer to a TS_Queue.
 *
 * POST: (*tsq)->myNotEmptyCondition has been destroyed
 *      && (*tsq)->myNotFullCondition has been destroyed
 *      && (*tsq)->Lock has been destroyed.
 *      && *tsq has been deallocated
 *      && *tsq == NULL
 */ 
void destroy(TS_Queue** tsq) {
	// deallocate the queue's mutex and condition
	pthread_cond_destroy( &( (*tsq)->myNotEmptyCondition ) );
	pthread_cond_destroy( &( (*tsq)->myNotFullCondition ) );
	pthread_mutex_destroy( &( (*tsq)->myLock ) );
	// deallocate the queue's array
	free( (*tsq)->myArray );
	// deallocate the queue itself
	free(*tsq);
	*tsq = NULL;
}


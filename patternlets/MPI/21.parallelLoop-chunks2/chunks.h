/* chunks.h defines getChunkStartAndStop(), a function that
 *   divides a range of integers into equal-sized chunks.
 *  This function is useful in a variety of parallel contexts, 
 *   not just looping; chunks.h makes it easy to reuse.
 *
 *  By Joel Adams, Calvin University, Sept 2025.
 *
 *  Usage: See parallelLoopChunks.c in this directory.
 */

#ifndef CHUNKS_H
#define CHUNKS_H

#include <stdio.h>  // fprintf()
#include <stdlib.h> // exit()
#include <math.h>   // ceil()
#include <mpi.h>    // MPI

/* Calculate the start and stop values for a given PE's 
 *  contiguous chunk of a range of integers (e.g., loop-iterations) 
 *  0..MAX-1, so that PEs' chunk-sizes are equal (or nearly so).
 *
 * @param: id, an int 
 * @param: numPEs, an int 
 * @param: MAX, a const unsigned long 
 * Precondition: id == this thread's id or MPI process's rank
 *            && numPEs == the number of threads or MPI processes
 *            && MAX == the size of the range: 0..MAX-1
 *                (e.g., the total number of loop iterations needed)
 *            && numPEs <= MAX 
 *            && MAX < 2^32
 * @param: start, an unsigned long* through which the 
 *          starting value of this PE's chunk should be returned
 * @param: stop, an unsigned long* through which the
 *          stopping value of this PE's chunk should be returned
 * Postcondition: *start == this PE's first iteration value 
 *             && *stop == this PE's last iteration value + 1.
 */
void getChunkStartStopValues(int id, int numPEs, const unsigned long MAX,
                              unsigned long* start, unsigned long* stop)
{
   // check precondition before proceeding
   if ((unsigned long)numPEs > MAX) {
      if (id == 0) {
         printf("\n*** In getChunkStartStopValues():\n");
         printf("***     Number of PEs (%u) exceeds MAX (%lu)\n",
                          numPEs, MAX);
         printf("***     Please run using numPEs <= %lu\n\n", MAX);
      }
      MPI_Finalize();    // Note: remove this line when mulithreading
      exit(1);
   }

   // first, compute the chunk size that works in many cases
   unsigned long chunkSize1 = (unsigned long)ceil(((double)MAX) / numPEs);
   unsigned long begin = id * chunkSize1;
   unsigned long end = begin + chunkSize1;

   // now see if there are any leftover iterations
   unsigned long remainder = MAX % numPEs;

   // if remainder == 0, chunkSize1 = chunk-size for all PEs;
   // if remainder != 0, chunkSize1 = chunk-size for p_0..p_remainder-1
   //   but for PEs p_remainder..p_numPEs-1
   //   recompute begin and end using a smaller-by-1 chunk size, chunkSize2.
   if (remainder > 0 && (unsigned)id >= remainder) {
     unsigned long chunkSize2 = chunkSize1 - 1;
     unsigned long remainderBase = remainder * chunkSize1;
     unsigned long peOffset = (id-remainder) * chunkSize2;
     begin = remainderBase + peOffset;
     end = begin + chunkSize2;
   } 

   // pass back this PE's begin and end values via params start and stop
   *start = begin;
   *stop = end;
} 

#endif


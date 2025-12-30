/* chunks.h
 *  defines getChunkStartStopValues() to divide a range of zero-based values
 *  (0..N-1, inclusive) into P contiguous chunks that differ in size by at most 1.
 *  A PE (process or thread) can call it to compute the start and stop values
 *  of its 'chunk' of a parallel loop's iterations, or in other situations
 *  where a range needs to be divided into contiguous chunks.
 *
 *  This header file makes the functionality easy to reuse.
 *
 * Joel Adams, Calvin University, Fall 2023.
 */

#ifndef CHUNKS_H
#define CHUNKS_H

#include <stdio.h>        // printf()
#include <stdlib.h>       // exit()
#include <math.h>         // ceil()

/* Calculate the start and stop values for this PE's 
 *  contiguous chunk of a range of values, 0..N-1,
 *  so that each process chunk-sizes are equal (or nearly so).
 *
 * @param: id, an int
 * @param: P, an int
 * @param: N, a const unsigned
 * Precondition: id == this process's MPI rank or thread ID.
 *            && P == the number of PEs (threads or MPI processes)
 *            && N == the total number of 0-based values in the range
 *            && P <= N 
 *            && N < 2^32
 * @param: start, the address of the unsigned variable through which the 
 *          starting value of this process's chunk should be returned
 * @param: stop, the address of the unsigned variable through which the
 *          stopping value of this process's chunk should be returned
 * Postcondition: *start = this process's first range-value 
 *             && *stop = this process's last range-value + 1.
 */
void getChunkStartStopValues(int id, int P, const unsigned N,
                              unsigned* start, unsigned* stop) {
   // check precondition before proceeding
   if (P > N) {
      if (id == 0) {
         printf("\n*** P (number of PEs: %u) exceeds N (%u)\n", P, N);
         printf("*** Please run with P less than or equal to %u\n\n", N);
      }
      return;
   }

   // compute the chunk size that works in many cases
   unsigned chunkSize1 = (long)ceil(((double)N) / P);
   unsigned begin = id * chunkSize1;
   unsigned end = begin + chunkSize1;

   // see if there are any leftover iterations
   unsigned remainder = N % P;

   // If remainder == 0, chunkSize1 = chunk-size for all PEs;
   // If remainder != 0, chunkSize1 = chunk-size for p_0..p_remainder-1
   //   but for PEs p_remainder..p_P-1
   //   recompute begin and end using a smaller-by-1 chunk size, chunkSize2.
   if (remainder > 0 && id >= remainder) {
     unsigned chunkSize2 = chunkSize1 - 1;
     unsigned remainderBase = remainder * chunkSize1;
     unsigned peOffset = (id-remainder) * chunkSize2;
     begin = remainderBase + peOffset;
     end = begin + chunkSize2;
   } 

   // pass back this PE's begin and end values via start and stop
   *start = begin;
   *stop = end;
}

#endif



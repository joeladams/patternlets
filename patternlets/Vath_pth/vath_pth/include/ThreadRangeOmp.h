// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// *********************************************
// File ThRangeOmp.h 
// ------------------
// 
// Definition of the utility function
// ThreadRangeOmp()
// -------------------------------------------

#ifndef TH_RANGE_OMP
#define TH_RANGE_OMP

#include <omp.h>         

void ThreadRangeOmp(int& Beg, int& End)
   {
   int rank, nThreads;
   int n, beg, end;
   int size, D, R;
   
   rank = omp_get_thread_num()+1;
   nThreads = omp_get_num_threads();

   size = End-Beg;
   D = (size/nThreads);
   R = size%nThreads;

   end = Beg;
   for(n=1; n<=rank; n++)
      {
      beg = end;
      end = beg+D;
      if(R)
         {
         end++;
         R--;
         }
      }
   Beg = beg;
   End = end;
   }

#endif

// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// *********************************************
// PipeThQ.h
// Template code.
//
// This class encapsulates an array of ThQueue<T> objects,
// connecting pipeline stages. 
// This class simplifies the programming of a pipeline of
// threads.
//
// Conventions: 
// ------------
// The constructor gets BEG - the first - and END -the last -
// rank of the threads engaged in the pipeline.
// Obviously, Nthreads = END - BEG + 1.
//
// The Thqueues that connect the stages are labelled in the
// range [BEG, END-1]. Boundary issues are handled : no get from 
// ThQ[BEG], no put for ThQ[END-1]
// 
// Stage n gets from ThQ[n-1] and puts to ThQ[n].

// ********************************************************

#ifndef PIPETHQ_H
#define PIPETHQ_H

#include <ThQueue.h>

template <class T>
class PipeThQ
   {
   private:
  
   int        Nc;
   int        first, last;
   ThQueue<T> **ThQ;      // array of pointers to ThQueue<T>

   public:

   // Constructor and destructor
   // --------------------------
   PipeThQ(int BEG, int END, int size = 10)
      {
      first = BEG;
      last  = END-1;
      Nc    = END - BEG;
      ThQ = new ThQueue<T>*[END];
      for(int n=first; n<=last; n++) ThQ[n] = new ThQueue<T>(size);
      }

   ~PipeThQ()
      {
      for(int n=first; n<=last; n++) delete ThQ[n];
      delete [] ThQ;
      }

   /***********************************************
    * This function posts a value  to the pipeline
    * The stage of rank n adds to ThQ[n]
    ***********************************************/

   void PostNext(T elem, int rank)
      {
      if(rank>last) return;
      ThQ[rank]->Add(elem); 
      }

   /***************************************************
    * This function waits at the synchronization
    * point and gets a T value.
    * The stage of rank n gets value from ThQueue[n-1]
    **************************************************/   
       
   T GetPrevious(int rank, bool& flag)
      {
      T retval = T();
      if(rank==first) return retval;

      // -----------------------
      // Get from previous queue
      // -----------------------
      retval = ThQ[rank-1]->Remove(flag);

      // ---------------------------------------------
      // If flag reports that previous queue is closed,
      // close the next queue
      // ---------------------------------------------
      if(!flag)
         {
         if(rank <= last) ThQ[rank]->CloseQueue();
         printf("\n Thread %d exiting ...", rank);
         pthread_exit(NULL);
         }
      return retval;
      }

   void ClosePipeline()
     {
     int n;
     //for(n=1; n<=Nc; n++) TH[n]->CloseQueue();
     ThQ[first]->CloseQueue();    // close first queue, others will follow
     }

   // end of class
   // ------------
   };
          
// *************************************************

 
#endif  // SYNCH_P

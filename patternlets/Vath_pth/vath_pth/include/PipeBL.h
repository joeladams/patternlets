// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// *********************************************
// PipeBL.h
//
// Template code. The template parameter is the Boolean
// Lock class used as connector between successive pipeline
// stages: 
//  -) BLock, for idle wait synchronization 
//  -) SpBlock, for spin wait synchronization
//  -) OBLock: a pure OpenMP implementation of SpBlock
//
// The spin block classes have all the same public interface.
// A template specialization is made for the idle BLock
// class
//
// This class encapsulates an array of BLock objects, that
// connect pipeline stages. We call them "connectors"
// This class simplifies the programming of a pipeline of
// threads.
//
// Conventions:
// ------------ 
// The constructor gets BEG - the first - and END -the last -
// rank of the threads engaged in the pipeline.
// Obviously, Nthreads = END - BEG + 1.
//
// The BLocks that connect the stages are labelled in the
// range [BEG, END-1]. Boundary issues are handled : no get from 
// BLock[BEG], no put for BLock[END-1]
// 
// Stage n gets from BLock[n-1] and puts to Block[n].
//
// ********************************************************

#ifndef PIPE_BL_H
#define PIPE_BL_H

#include <BLock.h>

template <typename T>
class PipeBL
   {
   private:
  
    T     **BL;          // array of pointers to Boolean Locks
    int   first, last;   // indices of first and last connector
    int   Nc;            // number of connectors = (nthreads-1)

   public:

    // Constructor and destructor
    // --------------------------
    PipeBL(int BEG, int END)
       {
       first = BEG;
       last  = END-1;
       Nc = END - BEG;
       BL = new T*[END];
       for(int n=first; n<=last; n++) BL[n] = new T();
       }

    ~PipeBL()
       {
       for(int n=first; n<=last; n++) delete BL[n];
       delete [] BL;
       }

   /***************************************************
    * This function releases next stage of the pipeline
    * The stage of rank n drives BLock[n]
    ***********************************************/

    void ReleaseNext(int rank)
       {
       if(rank>last) return;
       BL[rank]->Wait_Until(false);
       BL[rank]->SetState(true); 
       }

   /***********************************************
    * This function waits at the synchronization
    * point to be released by previous stage.
    * The stage of rank n is driven by BLock[n-1]
    **********************************************/   
       
    void WaitForRelease(int rank)
       {
       if(rank==first) return;
       BL[rank-1]->Wait_Until(true);
       BL[rank-1]->SetState(false);
       }

   // end of class
   };
    
// ---------------------------------
// Template specialization for BLock
// ---------------------------------

template<>
class PipeBL<BLock>
   {
   private:
  
   BLock **BL;          // array of pointers to BLocks
   int   first, last;   // indices of first and last connector
   int   Nc;            // number of connectors = (nthreads-1)

   public:

   // Constructor and destructor
   // --------------------------
   PipeBL<BLock>(int BEG, int END)
      {
      first = BEG;
      last  = END-1;
      Nc = END - BEG;
      BL = new BLock*[END];
      for(int n=first; n<=last; n++) BL[n] = new BLock();
      }

   ~PipeBL<BLock>()
      {
      for(int n=first; n<=last; n++) delete BL[n];
      delete [] BL;
      }

   void ReleaseNext(int rank)
      {
      if(rank>last) return;
      BL[rank]->Wait_Until(false, 0);
      BL[rank]->Set_And_Notify(true); 
      }

   void WaitForRelease(int rank)
      {
      if(rank==first) return;
      BL[rank-1]->Wait_Until(true, 0);
      BL[rank-1]->Set_And_Notify(false);
      }
   };
          

      
// *************************************************

 
#endif  // PIPEBL_H

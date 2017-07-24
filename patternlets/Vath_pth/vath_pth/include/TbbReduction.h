// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// *********************************************
// File TbbReduction.h
//
// Lock free algorithm to accumulate T values, 
// using the atomic<T*> TBB class
//
// -------------------------------------------
#include <tbb/atomic.h>

template<typename T>
class TbbReduction
   {
   private:
    tbb::atomic<T*> P;

   public:
    TbbReduction() { P = new T(); }
    ~TbbReduction() { delete P; }

    T Data() { return *P; }
    void Reset() 
       { 
       T value  = *P;
       *P -= value;
       }

    void Update(T d)
       {
       T *oldptr, *newptr;
       do
         {
         oldptr = P;
         newptr = new T(*P+d);
         }while(P.compare_and_swap(newptr, oldptr) != oldptr);
       delete oldptr;
       }
   };

// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// *********************************************
// RandInt.h
// *********
// This file contains a simple class that provides
// randomly distrituted integers in the range [O, N]
// where N is passed in the constructor of the class.
//
// Uses the <stdlib> function rand() that returns a
// random integer in the range [0, RAN_MAX]
//
// If N is not specified, this function falls to the
// standard rand() function in [0, RAN_MMAX].
//
// The initial seed that determines the random sequence
// can also be specified in the constructor. If it is 
// not, the initial seed is 1
// ******************************************************
#include <stdlib.h>

class RandInt
   {
   private:
   int N;

   public:
   RandInt(): N(0) {};       // first constructor
   RandInt(int n): N(n) {};  // second constructor
   RandInt(int n, int seed)  // third constructor
      {
      N = n;
      srand(seed);   // initialize rand() with seed.
      }

   int draw()
      {
      if(N==0) return rand();
      else return rand()%N;
      }
   };
     
   


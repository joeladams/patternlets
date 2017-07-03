// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// *********************************************
#ifndef RAND_H
#define RAND_H

/////////////////////////////////////////////////
// File Rand.h
// This is the easiest, quich an dirty generator
// to produce uniformly distribiued doubles in
// [0, 1]
/////////////////////////////////////////////////

#define IMUL   314159269
#define IADD   453806245
#define MASK   2147483647
#define SCALE  0.4656612873e-9

// Simple generator. Produces uniform deviates in [0,1]

class Rand
   {
   private:
   ///////

   long seed;

   public:
   //////
   
   Rand() { seed = 999; }
   Rand(unsigned long s) { seed = (long)s; }
   double draw()
      {
      seed = (seed * IMUL + IADD) & MASK;
      return (seed * SCALE);
      }
   };

#endif   // RAND_H

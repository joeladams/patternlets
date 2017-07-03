// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// *********************************************
// SafeCout.h
// Thread safe, mutex protected, output to
// stdout facility. Replaces std::cout.
// ---------------------------------------

#ifndef SAFECOUT_H
#define SAFECOUT_H

#include <pthread.h>
#include <iostream>
#include <sstream>

class SafeCout
   {
   private:
   pthread_mutex_t lock; 

   public:
   SafeCout()
      { pthread_mutex_init(&lock, NULL); }

   ~SafeCout()
      { pthread_mutex_destroy(&lock); }

   void Flush(std::ostringstream& os) 
      {
      int retval;
      pthread_mutex_lock(&lock); 
      std::string S = os.str();
      std::cout << S << std::endl; 
      os.str("");                   // reset
      pthread_mutex_unlock(&lock);
      }
   }; 

#endif

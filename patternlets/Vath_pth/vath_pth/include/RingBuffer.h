// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// *********************************************
// Ring Buffer class
// --------------------

#ifndef RING_BUFFER
#define RING_BUFFER
#include <pthread.h>

using namespace std;

template <typename T, int Size>
class RingBuffer
    {
    private:
     T buffer[Size];
     int head, tail, C;
     pthread_mutex_t rlock;
     
     int NextHead( int n)    // called with locked rlock
         { 
         if(n==(Size-1)) C++;
	 return (n+1)%Size; 
	 }

     int NextTail( int n)    // called with locked rlock
         { 
         if(n==(Size-1)) C--;
	 return (n+1)%Size; 
	 }

    public:
     RingBuffer() : head(0), tail(0) 
         { pthread_mutex_init(&rlock, NULL); }

    bool Push(const T& value)
       {
       pthread_mutex_lock(&rlock);
       int H = head;
       if(H == tail && C>0)
           {
           pthread_mutex_unlock(&rlock); 
	   return false;
           }
       else
           {
           buffer[H] = value;
           head = NextHead(H);
           pthread_mutex_unlock(&rlock); 
           return true;
           }
       }

    bool Pop(T& value)
       {
       pthread_mutex_lock(&rlock);
       int _T = tail;
       if(_T == head && C==0)
           {
           pthread_mutex_unlock(&rlock); 
	   return false;
           }
       else
           {
           value = buffer[_T];
           tail = NextTail(_T); 
           pthread_mutex_unlock(&rlock); 
           return true;
           }   
       }

    void Insert(const T& value)
       { while(Push(value)==false); }

    void Extract(T& value)
       { while(Pop(value)==false); }
   };

#endif

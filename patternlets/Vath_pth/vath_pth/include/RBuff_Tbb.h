// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// *********************************************
// -------------------------------------
// RingBuffer class, using tbb::atomic
// ------------------------------------

#ifndef RBUFF_TBB
#define RBUFF_TBB
#include <tbb/atomic.h>

using namespace std;

template <typename T, int Size>
class RBuff_Tbb
    {
    private:
     T buffer[Size];
     tbb::atomic<int> head, tail;
     tbb::atomic<int> C;
     
     int NextHead( int n)
         { 
         if(n==(Size-1)) C++;
	 return (n+1)%Size; 
	 }

     int NextTail( int n)
         { 
         if(n==(Size-1)) C--;
	 return (n+1)%Size; 
	 }

    public:
     RBuff_Tbb()
        {
        head = 0;
        tail = 0;
        }

    bool Push(const T& value)
       {
       int H = head;
       int _tail = tail;  // load, default acquire
       int _C = C;
       if(H == _tail && _C) return false;
       buffer[H] = value;
       head = NextHead(H); // default = release
       return true;
       }

    bool Pop(T& value)
       {
       int _T = tail;
       int _head = head;
       int _C = C; 
       if(_T == _head && !_C) return false;
       value = buffer[_T];
       tail = NextTail(_T); // default = release; 
       return true;
       }

    void Insert(const T& value)
       { while(Push(value)==false); }

    void Extract(T& value)
       { while(Pop(value)==false); }
   };

#endif

// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// *********************************************
// CpuTimer.h
//
// Measurine execution times. Reports wall, user
// (total computation time of all threads) and 
// system times in Pthreads. In Windows and C++11, 
// only wall times are reported
// ---------------------------------------

#ifndef CPU_TIMER_H
#define CPU_TIMER_H

#include <sys/times.h>
#include <unistd.h>
#include <iostream>

class CpuTimer
    {
    private:
    clock_t start_time; 
    clock_t end_time;
    struct tms tms_S;
    struct tms tms_E;

    public:
    void Start();
    void Stop();
    void Report();
   };

#endif

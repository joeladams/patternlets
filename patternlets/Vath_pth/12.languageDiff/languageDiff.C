
/*
 * languageDiff.c
 *
 * Using Victor Alessandrini's vath_pth library.
 * ... langugage difference between C and C++
 *
 * Modeled from code provided by Joel Adams, Calvin College, November 2009.
 * Hannah Sonsalla, Macalester College, 2017.
 *
 * Usage: ./languageDiff [numThreads]
 *
 * Exercise:
 *  - Compile, run, note resulting output is correct.
 *  - Uncomment section A and comment out line B, recompile, rerun, note results.
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <SPool.h>
#include <pthread.h>
#include <iostream>   // cout

SPool *TH;
int numThreads;

using namespace std;

// -------------------
// Worker threads code
// -------------------
void thread_fct(void *idp)  {

    int rank = TH->GetRank();

    /*
    cout << "Hello from thread #" << rank     // Section A
             << " out of " << numThreads
             << " threads\n";
   */

    printf("Hello from thread #%d of %d\n", rank, numThreads);  // Line B
}

int main(int argc, char** argv) {
    cout << "\n";

    if(argc==2) numThreads = atoi(argv[1]);
    else numThreads = 4;

    TH = new SPool(numThreads);
    TH->Dispatch(thread_fct, NULL);
    TH->WaitForIdle();
    delete TH;

    cout << "\n";
    return 0;
}

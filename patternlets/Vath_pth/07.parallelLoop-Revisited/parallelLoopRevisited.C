
/*
 * parallelLoopRevisited.C
 *
 *  Using Victor Alessandrini's vath_pth library.
 *  ... computes a table of factorial values,
 *  using Owen Astrachan's BigInt class.
 *
 * Modeled from code provided by Joel Adams, Calvin College, November 2009.
 * Hannah Sonsalla, Macalester College, 2017.
 *
 * Usage: ./parallelLoopRevisited [numThreads]
 *
 * Exercise:
 *    Note: computing factorials on the left side of the table
 *          will take less time than computing factorials on right side.
 *          Ex.  5! should be computed faster than 1000!
 *  - Build and run, record sequential run time in a spreadsheet
 *  - Comment out line A and uncomment section B, rebuild,
 *      run using 2, 4, 6, 8, ... threads, record run times.
 *      Work is split evenly into sequential chunks.
 *      Ex. [1....20][20 ... 40][40 ... 60] etc.
 *  - Comment out section C and uncomment section D, rebuild,
 *      run using 2, 4, 6, 8, ... threads, record run times.
 *      Work is split evenly based on number of threads into chunks.
 *      Ex.  4 threads: [1, 5, 9, 13 ...][2, 6, 10, 14 ...] etc.
 *  - Create a line chart plotting run times vs # of threads.
 *
 */

#include <stdlib.h>
#include <SPool.h>
#include <pthread.h>
#include <cassert>
#include <CpuTimer.h>   // CPU timer
#include "BigInt.h"

// global variables
SPool *TH;
int numThreads;
#define MAX  1024
BigInt factorialTable[MAX+1];

// -------------------
// Factorial function
// -------------------
BigInt factorial(unsigned number)  {
    BigInt result = 1;  // 0! or 1!
    if (number > 1)  {
        for (unsigned i = 2; i <= number; i ++) {
            result *= i;
        }
    }
    return result;
}

// -------------------
// Worker threads code for parallel factorial
// -------------------

void thread_fct(void *idp)  {

    int beg, end;
    beg = 0;                                    // Section C
    end = MAX;
    TH->ThreadRange(beg, end);

    for (unsigned i = beg; i < end; i++) {
	factorialTable[i] = factorial(i);       // Section C
    }

    /*
    unsigned rank = TH ->GetRank();
    for (unsigned i = rank - 1; i < MAX; i+=numThreads) {    // Section D
        factorialTable[i] = factorial(i);
    }
    */
}

// -------------------
// Sequential factorial
// -------------------
void seqFactorial(){
    for (unsigned i = 0; i <= MAX; i++) {
        factorialTable[i] = factorial(i);
    }
}

int main(int argc, char **argv)  {
    CpuTimer TR;

    if(argc==2) numThreads = atoi(argv[1]);
    else numThreads = 4;

    cout << "\nDepending on the speed of your computer,"
             << "\n  this program may take a while to complete,"
             << "\n  so please wait patiently...\n" << endl;

    /* Calculate factorial */
    BigInt result;
    TH = new SPool(numThreads);
    TR.Start();

    seqFactorial();   // ~21 secs            // Line A

    /*
    TH->Dispatch(thread_fct, NULL);      // Section B
    TH->WaitForIdle();
    */

    TR.Stop();
    TR.Report();

    // run a few tests to validate the results
    assert( factorialTable[0] == 1 );
    assert( factorialTable[1] == 1 );
    assert( factorialTable[2] == 2 );
    assert( factorialTable[3] == 6 );
    assert( factorialTable[4] == 24 );
    assert( factorialTable[5] == 120 );
    assert( factorialTable[32] == BigInt("263130836933693530167218012160000000") );
    assert( factorialTable[100] == BigInt( string("9332621544394415268169923885")
			                            + "6266700490715968264381621468"
						    + "5929638952175999932299156089"
						    + "4146397615651828625369792082"
						    + "7223758251185210916864000000"
						    + "000000000000000000" ) );

    cout << "All tests passed!\n" << flush;
    delete TH;
    return 0;
}

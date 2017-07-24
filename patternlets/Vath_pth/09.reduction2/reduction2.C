
/*
 * reduction2.C
 *
 *  Using Victor Alessandrini's vath_pth library.
 *  ... computes factorial values, using Owen Astrachan's
 *  BigInt class to explore reductions
 *
 * Modeled from code provided by Joel Adams, Calvin College, November 2009.
 * Hannah Sonsalla, Macalester College, 2017.
 *
 * Usage: ./reduction2 [numThreads] [n]
 *
 *  Exercise:
 *  - Build and run, record sequential time in a spreadsheet.
 *  - Uncomment line B and comment out line A. Rerun in parallel
 *    noting the reduction for a BigInt using 2, 4, 6, 8, ... threads,
 *    recording the wall times in the spreadsheet.
 *  - Create a chart that plots the times vs the # of threads.
 *  - Experiment with different n values
 */


#include <stdlib.h>
#include <SPool.h>
#include <pthread.h>
#include <cassert>
#include <CpuTimer.h>   // CPU timer
#include <Reduction.h>  // reduction
#include "BigInt.h"

// global variables
unsigned n;
unsigned numThreads;
Reduction<BigInt> RD;   // Reduction
SPool *TH;

// -------------------
// Worker threads code for parallel factorial
// -------------------

void thread_fct(void *idp)  {
    int beg, end;
    beg = 2;
    end = n;
    TH->ThreadRange(beg, end);   // sub-range

    BigInt result = 1;
    int rank = TH->GetRank();

    for(unsigned i=beg; i<end; i++)  {
        result *= i;
    }

    if (rank == numThreads) result *= end;  // If last thread, multiply by n = end
    RD.Multiply(result);   // Reduction: multiply global reduction variable by result
}

// -------------------
// Factorial function for multithreading
// -------------------
BigInt parFactorial(unsigned number){
    n = number;
    BigInt result = 1;    // If 0! or 1! return 1
    if (n > 1) {
        RD.Accumulate(1);
	TH->Dispatch(thread_fct, NULL);
	TH->WaitForIdle();
	result = RD.Data();
	RD.Reset();
    }
    return result;
}

// -------------------
// Sequential factorial
// -------------------
BigInt seqFactorial(unsigned number)  {
    BigInt result = 1;
    for (unsigned i = 2; i <= number; i ++) {
        result *= i;
    }
    return result;
}

int main(int argc, char **argv)  {
    n = 8192;
    numThreads = 1;
    CpuTimer TR;

    switch (argc) {
	case 3: n = atoi(argv[2]);
	case 2: numThreads = atoi(argv[1]);
	case 1: break;
	default: cout << "\nUsage: ./reduction2 [numThreads] [n]\n\n";
    }

    /* Calculate factorial */
    BigInt result;
    TH = new SPool(numThreads);
    TR.Start();
    result = seqFactorial(n);       // A
    //result = parFactorial(n);     // B
    TR.Stop();
    cout << "Computed " << n << "! using " << numThreads << " thread " << endl;
    TR.Report();


    // run a few tests to validate the results
    assert( parFactorial(0) == 1 );
    assert( parFactorial(1) == 1 );
    assert( parFactorial(2) == 2 );
    assert( parFactorial(3) == 6 );
    assert( parFactorial(4) == 24 );
    assert( parFactorial(5) == 120 );
    assert( parFactorial(32) == BigInt("263130836933693530167218012160000000") );
    assert( parFactorial(100) == BigInt( string("9332621544394415268169923885")
			                            + "6266700490715968264381621468"
						    + "5929638952175999932299156089"
						    + "4146397615651828625369792082"
						    + "7223758251185210916864000000"
						    + "000000000000000000" ) );
    cout << "All tests passed!\n" << flush;
    delete TH;
    return 0;
}

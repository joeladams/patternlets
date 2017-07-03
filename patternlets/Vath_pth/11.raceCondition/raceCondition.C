
/* raceCondition.C
 *
 * Using Victor Alessandrini's vath_pth library.
 * ... illustrates a race condition when multiple threads read from and
 *     write to a shared variable.
 *
 * Modeled from code provided by Joel Adams, Calvin College, November 2009.
 * Hannah Sonsalla, Macalester College, 2017.
 *
 * Usage: ./raceCondition [numThreads]
 *
 * Exercise:
 *   - Compile and run 10 times; note that the sequential version always
 *     produces the correct balance: $1,000,000.00
 *   - To make parallel, comment out line A and uncomment section B,
 *     recompile and rerun multiple times, compare results
 *   - To fix parallel version, comment out line C, uncomment line D,
 *     change balance to RD.Data() in print statement,
 *     recompile and rerun, compare results
 */

#include <stdlib.h>
#include <stdio.h>
#include <SPool.h>
#include <pthread.h>
#include <Reduction.h>  // reduction

// global variables
SPool *TH;
int numThreads;
double balance = 0.0;
Reduction<double> RD;   // Reduction: accumulator of doubles
const int REPS = 1000000;

// -------------------
// Worker threads code
// -------------------
void thread_fct(void *idp)  {

    int beg, end;
    beg = 0;
    end = REPS;
    TH->ThreadRange(beg, end);

    for (int i = beg; i < end; i++)  {
        balance += 1.0;                    // C
        //RD.Accumulate(1.0);              // D
    }
}

// -------------------
// Sequential balance
// -------------------

void seqBalance(int n){
    for (int i = 0; i <n; i++) {
        balance += 1.0;
    }
}

int main(int argc, char **argv)  {

    if(argc==2) numThreads = atoi(argv[1]);
    else numThreads = 4;

    seqBalance(REPS);                         //A

    /*
    TH = new SPool(numThreads);
    TH->Dispatch(thread_fct, NULL);    // Section B
    TH->WaitForIdle();
    delete TH;
    */

    printf("\n After %d $1 deposits, your balance is $%0.2f\n\n", REPS, balance);
    return 0;
}

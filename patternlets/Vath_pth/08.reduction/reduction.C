
/*
 * reduction.C
 *
 * Using Victor Alessandrini's vath_pth library.
 * ... illustrates use of reduction
 *
 * Modeled from code provided by Joel Adams, Calvin College, November 2009.
 * Hannah Sonsalla, Macalester College, 2017.
 *
 * Usage: ./reduction [numThreads]
 *
 * Exercise:
 * - Compile and run. Note that correct output is produced by
 *	   sequential sum (sequentialSum) and parallel sum with reduction
 * 	   (reductionSum & RD). Incorrect output is obtained by parallel
 *     sum without using reduction (noReduceSum).
 */

#include <stdlib.h>
#include <stdio.h>
#include <SPool.h>
#include <pthread.h>
#include <Rand.h>  // random generator
#include <Reduction.h>  // reduction

#define SIZE 1000000

// global variables
double array[SIZE];
double noReduceSum;     // used for parallel sum w/out reduction
Reduction<double> RD;   // Reduction: accumulator of doubles
SPool *TH;

// -------------------
// Worker threads code for parallel sum (with and without reduction)
// -------------------

void thread_fct(void *idp)  {
    double reductionSum = 0.0;

    int beg, end;
    beg = 0;
    end = SIZE;
    TH->ThreadRange(beg, end);

    for(int i=beg; i<end; i++)  {
        reductionSum += array[i];   // accumulate results in parallel reduction sum
	noReduceSum += array[i];    // and sum without reduction
    }

    RD.Accumulate(reductionSum);   // Reduction: accumulate double parallelSum indside RD
}

// -------------------
// Sequential sum
// -------------------
double seqSum(double *a, int n)  {
    double sum = 0;
    for(int i=0; i<n; i++) sum += array[i];
    return sum;
}

// -------------------
// Initialize array with random values in [0, 1]
// -------------------
void initialize(double *a, int n)  {
    Rand R(999);       // random generator
    for(int i=0; i<n; i++) array[i] = R.draw();
}

int main(int argc, char **argv)  {
    int numThreads;

    double sequentialSum;

    if(argc==2) numThreads = atoi(argv[1]);
    else numThreads = 4;

    /* Fill array with random numbers*/
    initialize(array, SIZE);

    /* Sum the array sequentially */
    sequentialSum = seqSum(array, SIZE);

    /* Sum array using multiple threads */
    TH = new SPool(numThreads);
    TH->Dispatch(thread_fct, NULL);
    TH->WaitForIdle();

    /* Results */
    printf("\n Sequential sum = %.2f \n", sequentialSum);
    printf("\n Parallel sum without reduction = %.2f \n", noReduceSum);
    printf("\n Parallel sum with reduction = %.2f \n\n", RD.Data());

    delete TH;
    return 0;
}

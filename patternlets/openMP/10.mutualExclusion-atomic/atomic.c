/* atomic.c
 * ... illustrates a race condition when multiple threads read from / 
 *  write to a shared variable (and explores OpenMP atomic operations).
 *
 * Joel Adams, Calvin College, November 2009.
 *
 * Usage: ./atomic
 *
 * Exercise:
 *  - Compile and run 10 times; note that it always produces the correct balance: $1,000,000.00
 *  - To parallelize, uncomment A, recompile and rerun multiple times, compare results
 *  - To fix: uncomment B, recompile and rerun, compare
 */

#include <stdio.h>  // printf()
#include <omp.h>    // OpenMP

int main() {
    const int REPS = 1000000;
    int i;
    double balance = 0.0;
  
    printf("\nYour starting bank account balance is %0.2f\n", 
               balance);

    // simulate many deposits
    // #pragma omp parallel for                      // A
    for (i = 0; i < REPS; i++) {
        // #pragma omp atomic                        // B
        balance += 1.0;
    }

    printf("\nAfter %d $1 deposits, your balance is $%0.2f\n\n", 
		REPS, balance);

    return 0;
}


/* critical.c
 * ... fixes a race condition when multiple threads read from / 
 *  write to a shared variable	using the OpenMP critical directive.
 *
 * Joel Adams, Calvin College, November 2009.
 *
 * Usage: ./critical
 *
 * Exercise:
 *  - Compile and run several times; note that it always produces the correct balance $1,000,000.00 
 *  - Comment out A; recompile/run, and note incorrect result
 *  - To fix: uncomment B1+B2+B3, recompile and rerun, compare
 */

#include<stdio.h>
#include<omp.h>

int main() {
    const int REPS = 1000000;
    int i;
    double balance = 0.0;
  
    printf("\nYour starting bank account balance is %0.2f\n", balance);

    // simulate many deposits
    #pragma omp parallel for
    for (i = 0; i < REPS; i++) {
        #pragma omp atomic                          // A
//        #pragma omp critical                      // B1
//        {                                         // B2
        balance += 1.0;
//        }                                         // B3
    }

    printf("\nAfter %d $1 deposits, your balance is %0.2f\n", 
		REPS, balance);

    return 0;
}


/* critical2.c
 * ... compares the performance of OpenMP's critical and atomic directives
 *
 * Joel Adams, Calvin College, November 2009.
 *
 * Usage: ./critical2
 *
 * Exercise:
 *  - Compile, run, compare times for critical vs. atomic
 *  - Note how much more costly critical is than atomic
 *  - Research: Create an expression that, when assigned to balance,
 *     critical can handle but atomic cannot 
 */

#include<stdio.h>
#include<omp.h>

void print(char* label, int reps, double balance, double total, double average) {
    printf("\nAfter %d $1 deposits using '%s': \
            \n\t- balance = %0.2f, \
            \n\t- total time = %0.12f, \
            \n\t- average time per deposit = %0.12f\n\n", 
               reps, label, balance, total, average);
}

int main() {
    const int REPS = 1000000;
    int i;
    double balance = 0.0,
           startTime = 0.0, 
           stopTime = 0.0,
           atomicTime = 0.0,
           criticalTime = 0.0;
  
    printf("\nYour starting bank account balance is %0.2f\n", balance);

    // simulate many deposits using atomic
    startTime = omp_get_wtime();
    #pragma omp parallel for 
    for (i = 0; i < REPS; i++) {
        #pragma omp atomic
        balance += 1.0;
    }
    stopTime = omp_get_wtime();
    atomicTime = stopTime - startTime;
    print("atomic", REPS, balance, atomicTime, atomicTime/REPS);


    // simulate the same number of deposits using critical
    balance = 0.0;
    startTime = omp_get_wtime();
    #pragma omp parallel for 
    for (i = 0; i < REPS; i++) {
         #pragma omp critical
         {
             balance += 1.0;
         }
    }
    stopTime = omp_get_wtime();
    criticalTime = stopTime - startTime;
    print("critical", REPS, balance, criticalTime, criticalTime/REPS);

    printf("criticalTime / atomicTime ratio: %0.12f\n\n", 
             criticalTime / atomicTime);

    return 0;
}


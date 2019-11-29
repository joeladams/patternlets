/* reduction2.c
 * ... illustrates the use of MPI_Reduce() using arrays...
 * Joel Adams, Calvin College, January 2015.
 *
 * Usage: mpirun -np 4 ./reduction2
 *
 * Exercise:
 * - Compile and run, comparing output to source code.
 * - Explain behavior of MPI_Reduce() in terms of
 *     srcArr and destArr.
 */

#include <mpi.h>
#include <stdio.h>

#define ARRAY_SIZE 5

void printArray(int id, char* arrayName, int* array, int SIZE);
void printSeparator(char* separator, int id);

int main(int argc, char** argv) {
    int myRank = -1;
    int srcArr[ARRAY_SIZE] = {0};
    int destArr[ARRAY_SIZE] = {0};

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    if (myRank == 0) {
        printf("\nBefore reduction: ");
        printArray(myRank, "destArr", destArr, ARRAY_SIZE);
    }

    for (unsigned i = 0; i < ARRAY_SIZE; i++) {
        srcArr[i] = myRank * i;
    }

    printSeparator("", myRank);
    printArray(myRank, "srcArr", srcArr, ARRAY_SIZE);
    printSeparator("----", myRank);

    MPI_Reduce(srcArr, destArr, ARRAY_SIZE, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (myRank == 0) {
        printf("\nAfter reduction:  ");
        printArray(myRank, "destArr", destArr, ARRAY_SIZE);
        printf("\n");
    }

    MPI_Finalize();

    return 0;
}

/* utility to display an array
 * params: id, the rank of the current process
 *         arrayName, the name of the array being displayed
 *         array, the array being displayed
 *         SIZE, the number of items in array.
 * postcondition:
 *         the id, name, and items in array have been printed to stdout.
 */
void printArray(int id, char* arrayName, int * array, int SIZE) {
    printf("Process %d, %s: [", id, arrayName);
    for (int i = 0; i < SIZE; i++) {
        printf("%3d", array[i]);
        if (i < SIZE-1) printf(",");
    }
    printf("]\n");
}

/* utility to print a separator string between before and after sections.
 * params: separator, a string
 *         id, the rank of the current process.
 * postcondition: the master process has printed the separator.
 */
void printSeparator(char* separator, int id) {
    MPI_Barrier(MPI_COMM_WORLD);
    if (id == 0) { printf("%s", separator); }
    MPI_Barrier(MPI_COMM_WORLD);
}



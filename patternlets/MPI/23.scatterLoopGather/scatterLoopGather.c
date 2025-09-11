/* scatterLoopGather.c
 * ... scatters an array of data into equal-sized chunks, 
 *      has each process use a loop to double the values in its chunk,
 *      and then gathers the chunks back to the master process.
 *
 * Joel Adams, Calvin University, December 2019.
 *
 * Precondition: ARRAY_SIZE is evenly divisible by N
 *               && N <= ARRAY_SIZE.
 *
 * Note: The output of different process's steps will be interleaved
 *       (even using barriers) b/c stdout is buffered
 *       and MPI does not guarantee FIFO output behavior.
 *
 * Usage: mpirun -np N ./scatterLoopGather
 *
 * Exercise:
 * - Compile and run, using 1, 2, 4, and 8 processes
 * - Use source code to trace execution and output
 * - Explain behavior/effect of MPI_Scatter(), MPI_Gather().
 * - Optional: change ARRAY_SIZE to be another multiple of 8, such as 16
 * - Optional: add calls to print() to display each array at each step
 */

#include <stdio.h>     // printf
#include <stdlib.h>    // malloc, exit, ...
#include <mpi.h>       // MPI functionality

#define MASTER     0
#define ARRAY_SIZE 8

void fill(int* a, int size);
void printSeparator(const char* separator, int id);
void print(char* locLabel, int id, char* aName, int* a, int numElements);

/*
 *  Main function: double the values in an array
 *  by dividing the work equally among N processes.
 */
int main(int argc, char** argv) {
    int* scatterArray = NULL;
    int* chunkArray = NULL;
    int* gatherArray = NULL;
    int numProcs = -1, myRank = -1, chunkSize = -1;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    printSeparator("", myRank);

    if (ARRAY_SIZE % numProcs || numProcs > ARRAY_SIZE) {
        if (myRank == MASTER) {
            printf("Please run with -np N divisible by and less than or equal to %d\n.", ARRAY_SIZE);
        }
        MPI_Finalize();
        exit(0);
    }

    if (myRank == MASTER) {     
        scatterArray = (int*) malloc( ARRAY_SIZE * sizeof(int) ); // allocate input array
        fill(scatterArray, ARRAY_SIZE);                           // populate it 
        gatherArray = (int*) malloc( ARRAY_SIZE * sizeof(int) );  // allocate result array
    }

    print("BEFORE Scatter", myRank, "scatterArray", scatterArray, ARRAY_SIZE);
    
    chunkSize = ARRAY_SIZE / numProcs;
    chunkArray = (int*) malloc(chunkSize * sizeof(int));          // allocate chunk array

    MPI_Scatter(scatterArray, chunkSize, MPI_INT,                 // scatter input array 
                 chunkArray, chunkSize, MPI_INT, 0, MPI_COMM_WORLD);

    print("AFTER Scatter", myRank, "chunkArray", chunkArray, chunkSize);

    for (unsigned i = 0; i < chunkSize; ++i) {                    // compute using chunk
        chunkArray[i] *= 2;
    }

    print("AFTER doubling", myRank, "chunkArray", chunkArray, chunkSize);

    MPI_Gather(chunkArray, chunkSize, MPI_INT,                   //  gather chunks
                gatherArray, chunkSize, MPI_INT,                 //   into gatherArray
                0, MPI_COMM_WORLD);

    print("AFTER gather", myRank, "gatherArray", gatherArray, ARRAY_SIZE);

    free(chunkArray);                                            // everyone clean up
    if (myRank == 0) {                                           // master clean up
        free(gatherArray); 
        free(scatterArray);
    }

    printSeparator("", myRank);

    MPI_Finalize();
    return 0;
}


/* fill an array with some easy-to-check values
 * @param: a, an int*.
 * @param: size, an int.
 * Precondition: a is the address of an array of ints.
 *              && size is the number of ints a can hold.
 * Postcondition: a has been filled with the values
 *                { 11, 12, 13, ... }.
 */
void fill(int* a, int size) {
    for (int i = 0; i < size; ++i) {
        a[i] = i+11;
    }
}

/* display a separator, synchronizing all processes
 * @param: separator, a char* 
 * @param: id, an int.
 * Precondition: separator points to a string to be be displayed
 *               && id is the MPI rank of this process.
 * Postcondition: separator has been displayed
 *                 and all MPI processes have been syncronized.
 */
void printSeparator(const char* separator, int id) {
    MPI_Barrier(MPI_COMM_WORLD);
    if (id == MASTER) { 
        printf("%s\n", separator);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

/* display a string, a process id, and its array values
 * @param: locLabel, a char*
 * @param: id, an int
 * @param: aName, a char*
 * @param: a, an int*.
 * @param: numElements, an int.
 * Precondition: locLabel points to a string describing our location
 *              && id is the rank of this MPI process
 *              && aName is the name of the array being printed
 *              && a is the address of an int array 
 *              && numElements is the number of int-values in a.
 * Postcondition: str, id, and a have all been written to stdout.
 */
void print(char* locLabel, int id, char* aName, int* a, int numElements) {
    printf("%s, process %d has this %s: {", locLabel, id, aName);
    if (a != NULL) {
        for (int i = 0; i < numElements - 1; ++i) {
            printf("%d, ", a[i]);
        }
        printf("%d}\n", a[numElements - 1]);
    } else {
        printf("}\n");
    }
}



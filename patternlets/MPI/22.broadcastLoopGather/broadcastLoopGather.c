/* broadcastLoop.c
* ... illustrates the use of MPI_Bcast() for arrays
* combined with data decomposition pattern using a parallel-for loop with
* equal chunks. Wraps up with a gather so that completed work is back in
* master process.
*
* Libby Shoop, Macalester College, July, 2017
*
* Usage: mpirun -np N ./broadcastLoop
*
* Exercise:
* - Compile and run, using 2, 4, and 8 processes
* - Use source code to trace execution and output
* - Explain behavior/effect of MPI_Bcast(), MPI_Gather().
* - optional: change MAX to be another multiple of 8, such as 16
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

/* fill an array with some arbitrary values
* @param: a, an int*.
* @param: size, an int.
* Precondition: a is the address of an array of ints.
*              && size is the number of ints a can hold.
* Postcondition: a has been filled with arbitrary values
*                { 11, 12, 13, ... }.
*/
void fill(int* a, int size) {
    int i;
    for (i = 0; i < size; i++) {
        a[i] = i+11;
    }
}

/*
* Perform the data decomposition pattern on chunk of the array.
*
* @param: reps, number of repetions to traverse array
* @param: numProcesses, total number of processes being used
* @param: id, the rank, or id of current process executing this function
* @param: array, the array of integers whose chunk this process will work on.
* @param: myChunk, a smaller array that will contain the complated work
*
* This function will work on a portion of the array by doubling the value
* at each index in the array that this process id is responsible for.
* The original array is intact and the work done is stored in a smaller
* array, myChunk.
*
* preconditions:
*        reps is divisible by numProcesses to ensure equal chunks
*        size of myChunk is reps/numProcesses
* postconditions:
*        array is unchanged
*        myChunk contains completed work
*/
void workOnChunk(int reps, int numProcesses, int id, int* array, int* myChunk) {

    int chunkSize = reps / numProcesses;      // find chunk size
    int start = id * chunkSize;               // find starting index
    int stop = start + chunkSize;             // find stopping index

    int chunkIndex = 0;
    for (int i = start; i < stop; i++) {     // iterate through our range
        printf("Process %d is performing iteration %d\n", id, i);
        // perform calculation, leaving original array intact and updating
        // local chunk with result.
        myChunk[chunkIndex] = array[i] *2;
        chunkIndex++;
    }
}


/* display a string, a process id, and its array values
* @param: str, a char*
* @param: id, an int
* @param: a, an int*.
* Precondition: str points to a string to describe this array being printed
*              && id is the rank of this MPI process
*              && a is the address of an int array with numElements.
* Postcondition: str, id, and a have all been written to stdout.
*/
void print(char* str, int id, int* a, int numElements) {
    printf("%s , process %d has: {", str, id);
    for (int i = 0; i < numElements - 1; i++) {
        printf("%d, ", a[i]);
    }
    printf("%d}\n", a[numElements - 1]);
}

#define MAX 8

/*
*  Main program that double the values in an array by dividing the work
*  equally among processes.
*/
int main(int argc, char** argv) {
    int array[MAX] = {0};
    int* myChunk = NULL;
    int* gatherArray = NULL;
    int numProcs = -1, myRank = -1;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    // check conditions for  equal-sized chunks
    if ((MAX % numProcs) == 0 && numProcs <= MAX) {
        if (myRank == 0) {     // master:
            fill(array, MAX);                                 // populate original array
            gatherArray = (int*) malloc( MAX * sizeof(int) ); // allocate result array
        }

        print("BEFORE Bcast", myRank, array, MAX);

        MPI_Bcast(array, MAX, MPI_INT, 0, MPI_COMM_WORLD);

        print("AFTER Bcast", myRank, array, MAX);

        myChunk = (int*) malloc(MAX/numProcs * sizeof(int));  // holds my work
        workOnChunk(MAX, numProcs, myRank, array, myChunk);

        print("AFTER doubling", myRank, array, MAX);          //array should not change

        MPI_Barrier(MPI_COMM_WORLD);                          // ensure all are finished

        MPI_Gather(myChunk, MAX/numProcs, MPI_INT,            //  gather chunk vals
                    gatherArray, MAX/numProcs, MPI_INT,       //   into gatherArray
                    0, MPI_COMM_WORLD);

        if (myRank == 0) {                                    // master has everything
            print("in gatherArray, AFTER gather", myRank, gatherArray, MAX);
            free(gatherArray);                                //clean up
        }
    } else {                                                  // bail if unequal chunks
        if (myRank == 0) {
            printf("Please run with -np divisible by and less than or equal to %d\n.", MAX);
        }
    }

    free(myChunk);                                            // clean up
    MPI_Finalize();
    return 0;
}

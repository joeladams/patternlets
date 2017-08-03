/* broadcastUserInput.c
 * ... illustrates the use of MPI_Bcast() with a scalar value
 *     obtained via a command line argument.
 *
 * Hannah Sonsalla, Macalester College 2017
 * Modeled from code by Joel Adams, Calvin College, April 2016.
 *
 * Usage: mpirun -np N ./broadcastUserInput <integer>
 *
 * Exercise:
 * - Compile and run several times varying the number
 *   of processes and integer value
 * - Explain the behavior you observe
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MASTER 0

/* gets value of answer from user
 * @param: argc, argument count.
 * @param: argv, argument pointer array.
 * @param: myRank, rank of current process
 * @param: answer, variable to store value given by user
 * Precondition: argc is a count of the number of arguments.
 *              && argv is a pointer array that points to the arguments.
 *              && myRank is the rank of this MPI process.
 *		&& answer is the variable to be assigned value.
 * Postcondition: answer has been filled with value from user
 *                if given, else answer remains set to 0.
 */
void getInput(int argc, char* argv[], int myRank, int* answer) {

    if (myRank == 0){  // master process
        if (argc == 2){
             *answer = atoi(argv[1]);
        }
    }
    MPI_Bcast(answer, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

int main(int argc, char** argv) {
    int answer = 0, length = 0;
    int myRank = 0;

    char myHostName[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Get_processor_name (myHostName, &length);

    printf("BEFORE the broadcast, process %d on host '%s' has answer = %d\n",
             myRank, myHostName, answer);

    getInput(argc, argv, myRank, &answer);

    printf("AFTER the broadcast, process %d on host '%s' has answer = %d\n",
             myRank, myHostName, answer);

    MPI_Finalize();

    return 0;
}

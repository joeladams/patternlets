/* messagePassing2.c
 * ... illustrates using MPI_Send() and MPI_Recv() commands on arrays...
 * While this example sends and receives char arrays (strings),
 *  the same approach works on arrays of numbers or other types.
 * Joel Adams, Calvin College, September 2013.
 *
 * Usage: mpirun -np N ./messagePassing2
 *
 * Exercise:
 * - Compile and run, varying N: 1, 2, 4, 8.
 * - Trace execution using source code.
 * - Compare to messagePassing1.c; note send/receive differences.
 */

#include <stdio.h>   // printf()
#include <mpi.h>     // MPI
#include <stdlib.h>  // malloc()
#include <string.h>  // strlen()

int odd(int number) { return number % 2; }

int main(int argc, char** argv) {
    int id = -1, numProcesses = -1, length = -1;
    char * sendString = NULL;
    char * receivedString = NULL;
    char hostName[MPI_MAX_PROCESSOR_NAME];
    MPI_Status status;
    const int SIZE = (32+MPI_MAX_PROCESSOR_NAME) * sizeof(char);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Get_processor_name (hostName, &length);

    if (numProcesses > 1 && !odd(numProcesses) ) {
        sendString = (char*) malloc( SIZE );
        receivedString = (char*) malloc( SIZE );
        // sprintf: write to string
        sprintf(sendString, "Process %d is on host \"%s\"", id, hostName);

        if ( odd(id) ) {  // odd processes send, then receive
            MPI_Send(sendString, strlen(sendString)+1,
                       MPI_CHAR, id-1, 1, MPI_COMM_WORLD);
            MPI_Recv(receivedString, SIZE, MPI_CHAR, id-1, 2,
                       MPI_COMM_WORLD, &status);
        } else {          // even processes receive, then send
            MPI_Recv(receivedString, SIZE, MPI_CHAR, id+1, 1,
                       MPI_COMM_WORLD, &status);
            MPI_Send(sendString, strlen(sendString)+1,
                       MPI_CHAR, id+1, 2, MPI_COMM_WORLD);
        }

        printf("\nProcess %d of %d received the message:\n\t'%s'\n",
                id, numProcesses, receivedString);

        free(sendString);
        free(receivedString);
    } else if ( !id) {  // only process 0 does this part
        printf("\nPlease run this program using -np N where N is positive and even.\n\n");
    }

    MPI_Finalize();
    return 0;
}

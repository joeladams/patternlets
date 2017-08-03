/* barrier.c
 *  ... illustrates the behavior of MPI_Barrier() ...
 *
 * Joel Adams, Calvin College, May 2013.
 * Bill Siever, April 2016
 *   (Converted to master/worker pattern).
 * Joel Adams, April 2016
 *   (Refactored code so that just one barrier needed).
 *
 * Usage: mpirun -np 8 ./barrier
 *
 * Exercise:
 *  - Compile; then run the program several times,
 *     noting the interleaved outputs.
 *  - Uncomment the MPI_Barrier() call; then recompile and rerun,
 *     noting how the output changes.
 *  - Explain what effect MPI_Barrier() has on process behavior.
 */

#include <stdio.h>   // printf()
#include <mpi.h>     // MPI

/* Have workers send messages to the master, which prints them.
 * @param: id, an int
 * @param: numProcesses, an int
 * @param: hostName, a char*
 * @param: position, a char*
 *
 * Precondition: this function is being called by an MPI process
 *               && id is the MPI rank of that process
 *               && numProcesses is the number of processes in the computation
 *               && hostName points to a char array containing the name of the
 *                    host on which this MPI process is running
 *               && position points to "BEFORE" or "AFTER".
 *
 * Postcondition: each process whose id > 0 has sent a message to process 0
 *                     containing id, numProcesses, hostName, and position
 *                && process 0 has received and output each message.
 */

#define BUFFER_SIZE 200
#define MASTER      0

void sendReceivePrint(int id, int numProcesses, char* hostName, char* position) {
    char buffer[BUFFER_SIZE] = {'\0'};;
    MPI_Status status;

    if (id != MASTER) {
        // Worker: Build a message and send it to the Master
        int length = sprintf(buffer,
                              "Process #%d of %d on %s is %s the barrier.\n",
                                id, numProcesses, hostName, position);
        MPI_Send(buffer, length+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    } else {
        // Master: Receive and print the messages from all Workers
        for(int i = 0; i < numProcesses-1; i++) {
           MPI_Recv(buffer, BUFFER_SIZE, MPI_CHAR, MPI_ANY_SOURCE,
                     MPI_ANY_TAG, MPI_COMM_WORLD, &status);
           printf("%s", buffer);
        }
    }
}


int main(int argc, char** argv) {
    int id = -1, numProcesses = -1, length = -1;
    char myHostName[MPI_MAX_PROCESSOR_NAME] = {'\0'};

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Get_processor_name (myHostName, &length);

    sendReceivePrint(id, numProcesses, myHostName, "BEFORE");

//    MPI_Barrier(MPI_COMM_WORLD);

    sendReceivePrint(id, numProcesses, myHostName, "AFTER");

    MPI_Finalize();
    return 0;
}

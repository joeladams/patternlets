/* sequenceNumbers.c
 *  ... shows how to acheive barrier-like behavior
 *      by using MPI message tags as sequence numbers.
 *
 * Joel Adams, Calvin College, April 2016.
 *
 * Usage: mpirun -np 8 ./sequenceNumbers
 *
 * Exercise:
 * 1. Compile; then run the program several times,
 *     noting the intermixed outputs
 * 2. Comment out the sendReceivePrint(..., "SECOND", 1); call;
 *      uncomment the sendReceivePrint(..., "SECOND", 2); call;
 *      then recompile and rerun, noting how the output changes.
 * 3. Uncomment the sendReceivePrint(..., "THIRD", 3);
 *      and sendReceivePrint(..., "FOURTH", 4); calls,
 *      then recompile and rerun, noting how the output changes.
 * 4. Explain the differences: what has caused the changes
 *      in the program's behavior, and why?
 */

#include <stdio.h>   // printf()
#include <mpi.h>     // MPI

/* Have workers send messages to the master, which prints them.
 * @param: id, an int
 * @param: numProcesses, an int
 * @param: hostName, a char*
 * @param: messageNum, a char*
 * @param: tagValue, an int
 *
 * Precondition: this routine is being called by an MPI process
 *               && id is the MPI rank of that process
 *               && numProcesses is the number of processes in the computation
 *               && hostName points to a char array containing the name of the
 *                    host on which this MPI process is running
 *               && messageNum is "FIRST", "SECOND", "THIRD", ...
 *               && tagValue is the value for the tags of the message
 *                    being sent and received this invocation of the function.
 *
 * Postcondition: each process whose id > 0 has sent a message to process 0
 *                    containing id, numProcesses, hostName, messageNum,
 *                    and tagValue
 *                && process 0 has received and output each message.
 */

#define BUFFER_SIZE 200
#define MASTER      0

void sendReceivePrint(int id, int numProcesses, char* hostName,
                        char* messageNum, int tagValue) {
    char buffer[BUFFER_SIZE] = {'\0'};;
    MPI_Status status;

    if (id != MASTER) {
        // Worker: Build a message and send it to the Master
        int length = sprintf(buffer,
                              "This is the %s message from process #%d of %d on %s.\n",
                                messageNum, id, numProcesses, hostName);
        MPI_Send(buffer, length+1, MPI_CHAR, 0, tagValue, MPI_COMM_WORLD);
    } else {
        // Master: Receive and print the messages from all Workers
        for(int i = 0; i < numProcesses-1; i++) {
           MPI_Recv(buffer, BUFFER_SIZE, MPI_CHAR, MPI_ANY_SOURCE,
                     tagValue, MPI_COMM_WORLD, &status);
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

    sendReceivePrint(id, numProcesses, myHostName, "FIRST", 1);

    sendReceivePrint(id, numProcesses, myHostName, "SECOND", 1);
//    sendReceivePrint(id, numProcesses, myHostName, "SECOND", 2);
//    sendReceivePrint(id, numProcesses, myHostName, "THIRD", 3);
//    sendReceivePrint(id, numProcesses, myHostName, "FOURTH", 4);

    MPI_Finalize();
    return 0;
}

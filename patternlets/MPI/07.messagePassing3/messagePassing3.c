/* messagePassing3.c
 * ... illustrates the use of MPI_Send() and MPI_Recv(),
 *      in combination with the master-worker pattern.
 *
 * Joel Adams, Calvin College, November 2009.
 *
 * Usage: mpirun -np N ./messagePassing3
 *
 * Exercise:
 * - Run the program, varying the value of N from 1-8.
 * - Explain the behavior you observe.
 */

#include <stdio.h>    // printf()
#include <string.h>   // strlen()
#include <mpi.h>      // MPI

#define MAX 256

int main(int argc, char** argv) {
    int id = -1, numProcesses = -1;
    char sendBuffer[MAX] = {'\0'};
    char recvBuffer[MAX] = {'\0'};
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    if (numProcesses > 1) {
        if ( id == 0 ) {                              // master:
            sprintf(sendBuffer, "%d", id);            //  create msg

            MPI_Send(sendBuffer,                      //  msg sent
                      strlen(sendBuffer) + 1,         //  num chars + NULL
                      MPI_CHAR,                       //  type
                      id+1,                           //  destination
                      1,                              //  tag
                      MPI_COMM_WORLD);                //  communicator

            MPI_Recv(recvBuffer,                      //  msg received
                      MAX,                            //  buffer size
                      MPI_CHAR,                       //  type
                      numProcesses-1,                 //  sender
                      1,                              //  tag
                      MPI_COMM_WORLD,                 //  communicator
                      &status);                       //  recv status

            printf("Process #%d of %d received %s\n", // show msg
                    id, numProcesses, recvBuffer);
        } else {                                      // workers:
            MPI_Recv(recvBuffer,                      //  msg received
                      MAX,                            //  buffer size
                      MPI_CHAR,                       //  type
                      MPI_ANY_SOURCE,                 //  sender (anyone)
                      1,                              //  tag
                      MPI_COMM_WORLD,                 //  communicator
                      &status);                       //  recv status

            printf("Process #%d of %d received %s\n", // show msg
                    id, numProcesses, recvBuffer);

            // build msg to send by appending id to msg received
            sprintf(sendBuffer, "%s %d", recvBuffer, id);

            MPI_Send(sendBuffer,                      //  msg to send
                      strlen(sendBuffer) + 1,         //  num chars + NULL
                      MPI_CHAR,                       //  type
                      (id+1) % numProcesses,          //  destination
                      1,                              //  tag
                      MPI_COMM_WORLD);                //  communicator
        }
    } else {
        printf("\nPlease run this program with at least 2 processes\n\n");
    }

    MPI_Finalize();
    return 0;
}

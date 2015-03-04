/* messagePassing.c
 * ... illustrates the use of the MPI_Send() and MPI_Recv() commands...
 * Joel Adams, Calvin College, November 2009.
 *
 * Usage: mpirun -np N ./messagePassing
 *
 * Exercise:
 * - Compile and run, using N = 4, 6, 8, and 10 processes.
 * - Use source code to trace execution.
 * - Explain what each process:
 * -- computes
 * -- sends
 * -- receives
 * -- outputs.
 */

#include <stdio.h>
#include <mpi.h>
#include <math.h>   // sqrt()

int odd(int number) { return number % 2; }

int main(int argc, char** argv) {
    int id = -1, numProcesses = -1; 
    float sendValue = -1, receivedValue = -1;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    if (numProcesses > 1 && !odd(numProcesses) ) {
        sendValue = sqrt(id);
        if ( odd(id) ) {  // odd processors send, then receive 
            MPI_Send(&sendValue, 1, MPI_FLOAT, id-1, 1, MPI_COMM_WORLD);
            MPI_Recv(&receivedValue, 1, MPI_FLOAT, id-1, 2, 
                       MPI_COMM_WORLD, &status);
        } else {          // even processors receive, then send 
            MPI_Recv(&receivedValue, 1, MPI_FLOAT, id+1, 1, 
                       MPI_COMM_WORLD, &status);
            MPI_Send(&sendValue, 1, MPI_FLOAT, id+1, 2, MPI_COMM_WORLD);
        }

        printf("Process %d of %d computed %f and received %f\n",
                id, numProcesses, sendValue, receivedValue);
    } else if ( !id) {  // only process 0 does this part 
        printf("\nPlease run this program using -np N where N is positive and even.\n\n");
    }

    MPI_Finalize();
    return 0;
}


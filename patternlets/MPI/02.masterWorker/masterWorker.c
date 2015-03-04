/* masterWorker.c
 * ... illustrates the basic master-worker pattern in MPI ...
 * Joel Adams, Calvin College, November 2009.
 *
 * Usage: mpirun -np N ./masterWorker
 *
 * Exercise:
 * - Compile and run the program, varying N from 1 through 8.
 * - Explain what stays the same and what changes as the
 *    number of processes changes.
 */

#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
  int id = -1, numWorkers = -1, length = -1;
  char hostName[MPI_MAX_PROCESSOR_NAME];

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &numWorkers);
  MPI_Get_processor_name (hostName, &length);

  if ( id == 0 ) {  // process 0 is the master 
    printf("Greetings from the master, #%d (%s) of %d processes\n",
             id, hostName, numWorkers);
  } else {          // processes with ids > 0 are workers 
    printf("Greetings from a worker, #%d (%s) of %d processes\n",
             id, hostName, numWorkers);
  }

  MPI_Finalize();
  return 0;
}


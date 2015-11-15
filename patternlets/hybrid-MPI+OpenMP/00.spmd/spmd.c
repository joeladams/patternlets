/* spmd.c
 * ... illustrates the single program multiple data
 *      (SPMD) pattern using MPI and OpenMP commands.
 *
 * Joel Adams, Calvin College, November 2009.
 *
 * Usage: mpirun -np N ./spmd 
 *
 * Exercise:
 * - Build and run the program, 
 *     varying N's value as 1, 2, 3, 4, ...
 * - Compare the results to the source code
 * - Uncomment the commented-out #pragma directive
 * - Rebuild and rerun the program, varying N as before
 * - Compare the results to the source code
 */

#include <stdio.h>    // printf()
#include <stdlib.h>   // atoi()
#include <mpi.h>      // MPI commands
#include <omp.h>      // OpenMP commands

int main(int argc, char** argv) {
	int processID= -1, numProcesses = -1, length = -1;
	char hostName[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &processID);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
	MPI_Get_processor_name (hostName, &length);

//        #pragma omp parallel 
        {
          int threadID = omp_get_thread_num();
          int numThreads = omp_get_num_threads();

	  printf("Hello from thread %d of %d from process %d of %d on %s\n",
		 threadID, numThreads,
                 processID, numProcesses, hostName);
        }

	MPI_Finalize();
	return 0;
}


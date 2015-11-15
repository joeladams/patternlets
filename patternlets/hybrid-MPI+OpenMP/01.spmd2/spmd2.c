/* spmd2.c
 * ... illustrates the single program multiple data
 *      (SPMD) pattern using MPI and OpenMP commands
 *      with the user controlling numThreads
 *      from the command line.
 *
 * Joel Adams, Calvin College, November 2009.
 *
 * Usage: mpirun -np N ./spmd [numThreads]
 *
 * Exercise:
 * - Build and run, varying N = 1, 2, 3, 4, ...
 * - Compare behavior to source code
 * - Rerun with N = 1, varying numThreads = 1, 2, 3, 4, ...
 * - Compare behavior to source code
 * - Rerun with N = 2, varying numThreads = 1, 2, 3, 4, ...
 * - Compare behavior to source code
 * - Rerun with N = 3, varying numThreads = 1, 2, 3, 4, ...
 * - Compare behavior to source code
 * - ...
 */

#include <stdio.h>    // printf()
#include <stdlib.h>   // atoi()
#include <mpi.h>      // MPI commands
#include <omp.h>      // OpenMP commands

int processCommandLine(int argc, char ** argv) {
   if (argc == 2) {
      return atoi( argv[1] );
   } else {
      return 1;
   }
}

int main(int argc, char** argv) {
	int processID= -1, numProcesses = -1, length = -1;
        int numThreads = -1;
	char hostName[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &processID);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
	MPI_Get_processor_name (hostName, &length);

        numThreads = processCommandLine(argc, argv);
        #pragma omp parallel num_threads(numThreads)
        {
          int threadID = omp_get_thread_num();

	  printf("Hello from thread %d of %d from process %d of %d on %s\n",
		 threadID, numThreads,
                 processID, numProcesses, hostName);
        }

	MPI_Finalize();
	return 0;
}


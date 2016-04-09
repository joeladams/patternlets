/* barrier+timing.c 
 *  ... illustrates the behavior of MPI_Barrier() 
 *       to coordinate process-timing.
 *
 * Joel Adams, April 2016
 *
 * Usage: mpirun -np 8 ./barrier+timing
 *
 * Exercise: 
 *  - Compile; then run the program five times, 
 *  - In a spreadsheet, compute the average,
 *     median, and minimum of the five times.
 *  - Uncomment the two MPI_Barrier() calls;
 *     then recompile, rerun five times, and
 *     compute the new average, median, and min
 *     times.
 *  - Why did uncommenting the barrier calls
 *     produce the change you observed?
 */

#include <stdio.h>   // printf()
#include <mpi.h>     // MPI
#include <unistd.h>  // sleep()

#define  MASTER 0

/* answer the ultimate question of life, the universe, 
 *  and everything, based on id and numProcs.
 * @param: id, an int
 * @param: numProcs, an int
 * Precondition: id is the MPI rank of this process
 *             && numProcs is the number of MPI processes.
 * Postcondition: The return value is 42.
 */
int solveProblem(int id, int numProcs) {

    sleep( ((double)id+1) / numProcs);

    return 42;
}

int main(int argc, char** argv) {
    int id = -1, numProcesses = -1;
    double startTime = 0.0, totalTime = 0.0;
    int answer = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

//    MPI_Barrier(MPI_COMM_WORLD);
    if ( id == MASTER) {
        startTime = MPI_Wtime();
    }

    answer = solveProblem(id, numProcesses);

//    MPI_Barrier(MPI_COMM_WORLD);
    if ( id == MASTER ) {
        totalTime = MPI_Wtime() - startTime;
        printf("\nThe answer is %d; computing it took %f secs.\n\n",
                   answer, totalTime);
    }

    MPI_Finalize();
    return 0;
}


/* ParallelLoopEqualChunks.java
 * ... illustrates the parallel loop pattern in OpenMPI+Java,
 *     in which processes perform the loop's iterations in 'chunks'
 *     whose sizes differ by at most 1 (useful when #iterations
 *     is not evenly divisible by #processes).
 *
 *  Original C logic: Libby Shoop, Macalester College, 2017
 *  Java translation: Joel Adams, Calvin University, November 2019.
 *
 * Usage: mpirun -np N java ./ParallelLoopEqualChunks
 *
 * Exercise:
 * - Compile and run, varying N: 1, 2, 3, 4, 5, 6, 7, and 8.
 * - Change REPS to 16, save, recompile, rerun, varying N again.
 * - Explain how this pattern divides the iterations of the loop
 *    among the processes.
 */

import mpi.*;

public class ParallelLoopEqualChunks {

 public static final int REPS = 8;
 public static final int MASTER = 0;

 public static void main(String [] args) throws MPIException {
    MPI.Init(args);

    int id           = MPI.COMM_WORLD.getRank();
    int numProcesses = MPI.COMM_WORLD.getSize();
    int start        = 0;
    int stop         = 0;
    String message   = "";

    // Ensure that the number of processes doesn't exceed REPS.
    // If that is not the case, have the master print an error msg and stop.
    if (numProcesses > REPS) {
      if (id == MASTER) {
          System.out.print("\nPlease run with -np N less than or equal to "
                  +  REPS + "\n\n");
      }
      MPI.Finalize();
      System.exit(0);
    } 

    // find chunkSize for some of the processes
    double doubleReps = REPS;
    int chunkSize1 = (int) Math.ceil(doubleReps / numProcesses);
    // find chunkSize to spread among the other processes
    int chunkSize2 = chunkSize1 - 1;
    int remainder = REPS % numProcesses;

    // when remainder is 0, all processes have equal-sized chunks
    if (remainder == 0 || (remainder != 0 && id < remainder)) {
        // use larger chunk-size for processes whose id < remainder
        start = id * chunkSize1;
        stop = start + chunkSize1;
    } else {
        // use smaller chunk-size spread among processes whose id >= remainder
        start = (remainder*chunkSize1) + (chunkSize2 * (id - remainder));
        stop = start + chunkSize2;
    }

    for (int i = start; i < stop; i++) {      // iterate through our range
        message = "Process " + id + " is performing iteration " + i + "\n";
        System.out.print(message);
    }

    MPI.Finalize();
  }
}


/* ParallelLoopEqualChunks.java
 * ... illustrates the parallel loop pattern in OpenMPI+Java,
 *      in which processes perform the loop's iterations in equal-sized 'chunks'
 *      (preferable when loop iterations access memory/cache locations) ...
 *
 * Joel Adams, Calvin University, November 2019.
 *  with error-handling logic by Libby Shoop, Macalester College, 2017
 *
 * Usage: mpirun -np 4 java ./ParallelLoopEqualChunks
 *
 * Exercise:
 * - Compile and run, varying N: 1, 2, 4, and 8
 * - Change REPS to 16, save, recompile, rerun, varying N again.
 * - Explain how this pattern divides the iterations of the loop
 *    among the processes.
 * - What if REPS is not evenly divisible by N?
 *    What would be a better way to handle that case?
 */

import mpi.*;

public class ParallelLoopEqualChunks {

 public static final int REPS = 8;
 public static final int MASTER = 0;

 public static void main(String [] args) throws MPIException {
    MPI.Init(args);

    int id           = MPI.COMM_WORLD.getRank();
    int numProcesses = MPI.COMM_WORLD.getSize();
    String message   = "";

    // For this first example, ensure that the REPS can be evenly divided by the
    // number of processes and that the number of processes doesn't exceed REPS.
    // If that is not the case, have the master print an error msg and stop.
    if ((REPS % numProcesses) > 0 || numProcesses > REPS) {
      if (id == MASTER) {
          System.out.print("\nPlease run with -np divisible by and less than or equal to "
                  +  REPS + "\n\n");
      }
    } else {
      int chunkSize = REPS / numProcesses;      // find chunk size
      int start = id * chunkSize;               // find starting index
      int stop = start + chunkSize;             // find stopping index

      for (int i = start; i < stop; i++) {      // iterate through our range
          message = "Process " + id + " is performing iteration " + i + "\n";
          System.out.print(message);
      }
    }

    MPI.Finalize();
  }
}


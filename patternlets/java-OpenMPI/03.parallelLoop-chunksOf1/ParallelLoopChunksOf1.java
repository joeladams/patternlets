/* ParallelLoopChunksOf1.java
 * ... illustrates the parallel loop pattern in OpenMPI+Java,
 *      in which processes perform the loop's iterations in 'chunks'
 *      of size 1 (simple, and useful when loop iterations
 *      do not access memory/cache locations) ...
 * Note this is much simpler than the 'equal chunks' loop.
 *
 * Joel Adams, Calvin University, November 2019.
 *
 * Usage: mpirun -np 4 java ./ParallelLoopChunksOf1
 *
 * Exercise:
 * - Compile and run, varying N: 1, 2, 3, 4, 5, 6, 7, 8
 * - Change REPS to 16, save, recompile, rerun, varying N again.
 * - Explain how this pattern divides the iterations of the loop
 *    among the processes.
 */

import mpi.*;

public class ParallelLoopChunksOf1 {

 public static final int REPS = 8;
 public static final int MASTER = 0;

 public static void main(String [] args) throws MPIException {
    MPI.Init(args);

    int id           = MPI.COMM_WORLD.getRank();
    int numProcesses = MPI.COMM_WORLD.getSize();
    String message   = "";

    if (numProcesses > REPS) {
      if (id == MASTER) {
          System.out.print("\nPlease run with -np less than or equal to "
                  +  REPS + "\n\n");
      }
    } else {
      for (int i = id; i < REPS; i += numProcesses) { 
          message = "Process " + id + " is performing iteration " + i + "\n";
          System.out.print(message);
      }
    }

    MPI.Finalize();
  }
}


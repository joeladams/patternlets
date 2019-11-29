/* BarrierPlusTiming.java
 * ... illustrates how a Barrier can be used to coordinate timing.
 *
 * Joel Adams, Calvin University, November 2019.
 *
 * Usage: mpirun -np 10 java ./BarrierPlusTiming
 *
 * Exercise:
 *  - Compile; then run the program five times with N=10. 
 *  - In a spreadsheet, compute the average,
 *     median, and minimum of the five times.
 *  - Uncomment the two barrier() calls; then
 *     recompile, rerun five times, and compute
 *     the new average, median, and min times.
 *  - Why did uncommenting the barrier calls
 *     produce the changes you observed?
 *     (Hint: How long does process-0 sleep
 *             inside solveProblem(),
 *             compared to process-9?)
 */

import mpi.*;

public class BarrierPlusTiming {

 public static void main(String [] args) throws MPIException {
    MPI.Init(args);

    Comm comm         = MPI.COMM_WORLD;
    int id            = comm.getRank();
    int numProcesses  = comm.getSize();
    double startTime  = 0.0;
    double totalTime  = 0.0;

//    comm.barrier();

    if (id == MASTER) {
        startTime = MPI.wtime();
    }
 
    int answer = solveProblem(id, numProcesses);

//    comm.barrier();

    if (id == MASTER) {
        totalTime = MPI.wtime() - startTime;
        String answerStr = "\nThe answer is " + answer
                           + ", computing it took "
                           + totalTime + " secs.\n\n";
        System.out.print(answerStr);
    }

    MPI.Finalize();
  }

  /* answer to the ultimate question of life, the universe, 
   *  and everything, based on id and numProcs.
   * @param: id, an int
   * @param: numProcs, an int
   * Precondition: id is the MPI rank of this process
   *             && numProcs is the number of MPI processes.
   * Postcondition: The return value is 42.
   */
  private static int solveProblem(int id, int numProcs)
  {
      try {
          Thread.sleep( (id+1) * 1000 / numProcs);   // millisecs
      } catch (InterruptedException ie) {
         System.err.println("\nsleep was interrupted!\n\n");
      }

      return 42;
  }

  private static int MASTER = 0;
}


/* ReducePlusTiming.java
 * ... illustrates how a Reduction can be used to 
 *      compute the collective time to solve a problem.
 *
 * Joel Adams, Calvin University, November 2019,
 *  using logic from Hannah Sonsalla, Macalester College 2017.
 *
 * Usage: mpirun -np 10 java ./ReducePlusTiming
 *
 * Exercise:
 *  - Compile; then run the program five times with N=10. 
 *  - In a spreadsheet, compute the average,
 *     median, and minimum of the five times.
 *  - Explain the behavior of reduce in terms of
 *     localTime and collectiveTime.
 */

import mpi.*;
import java.nio.DoubleBuffer;

public class ReducePlusTiming {

 public static void main(String [] args) throws MPIException {
    MPI.Init(args);

    Comm comm                  = MPI.COMM_WORLD;
    int id                     = comm.getRank();
    int numProcesses           = comm.getSize();
    DoubleBuffer localBuf      = MPI.newDoubleBuffer(1);
    DoubleBuffer collectiveBuf = MPI.newDoubleBuffer(1);

    comm.barrier();

    double startTime = MPI.wtime();
 
    int answer = solveProblem(id, numProcesses);

    comm.barrier();

    double localTime = MPI.wtime() - startTime;

    localBuf.put(0, localTime);
    comm.reduce(localBuf, collectiveBuf, 1, MPI.DOUBLE, MPI.SUM, MASTER);
    double collectiveTime = collectiveBuf.get(0);

    if (id == MASTER) {
        String answerStr = "\nThe answer is " + answer
                           + ";\ncomputing it took "
                           + localTime + " secs locally, and\n"
                           + collectiveTime + " secs collectively.\n\n";
        System.out.print(answerStr);
    }

    MPI.Finalize();
  }

  /* find the answer to the ultimate question of life, the universe, 
   *  and everything, based on id and numProcs.
   * @param: id, an int
   * @param: numProcs, an int
   * Precondition: id is the MPI rank of this process
   *             && numProcs is the number of MPI processes.
   * Postcondition: The return value is 42.
   */
  private static int solveProblem(int id, int numProcs) {
      try {
          Thread.sleep( (id+1) * 1000 / numProcs);   // millisecs
      } catch (InterruptedException ie) {
         System.err.println("\nsleep was interrupted!\n\n");
      }

      return 42;
  }

  private static int MASTER = 0;
}


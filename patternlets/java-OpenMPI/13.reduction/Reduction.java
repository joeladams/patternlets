/* Reduction.java
 * ... illustrates how to use the reduction pattern 
 *     (which combines distributed values in O(lg(P)) time)
 *      using OpenMPI's Java interface.
 *
 * Joel Adams, Calvin University, November 2019.
 *
 * Usage: mpirun -np 4 java ./Reduction
 *
 * Exercise:
 * - Compile and run, varying N = 1, 2, 3, 4, 6, 8, 10.
 * - Explain behavior of the reduce operation.
 */

import mpi.*;
import java.nio.IntBuffer;

public class Reduction {

 public static void main(String [] args) throws MPIException {
    MPI.Init(args);

    Comm comm        = MPI.COMM_WORLD;
    int id           = comm.getRank();
    int numProcesses = comm.getSize();

    int square       = (id+1) * (id+1);
    IntBuffer squareBuffer = MPI.newIntBuffer(BUFFER_SIZE);
    squareBuffer.put(square);

    IntBuffer sumSquaresBuffer = MPI.newIntBuffer(BUFFER_SIZE);
    comm.reduce(squareBuffer, sumSquaresBuffer, BUFFER_SIZE,
                 MPI.INT, MPI.SUM, MASTER);

    IntBuffer maxBuffer = MPI.newIntBuffer(BUFFER_SIZE);
    comm.reduce(squareBuffer, maxBuffer, BUFFER_SIZE,
                 MPI.INT, MPI.MAX, MASTER);

    if ( id == MASTER) {
        String squareMsg = "\nThe sum of the squares from 1 to "
                            + numProcesses + " is " 
                            + sumSquaresBuffer.get(0) + "\n\n";
        String maxMsg    = "The max of the squares from 1 to "
                            + numProcesses + " is " 
                            + maxBuffer.get(0) + "\n\n";
        System.out.print(squareMsg);
        System.out.print(maxMsg);
    }

    MPI.Finalize();
  }

  private static int BUFFER_SIZE = 1;
  private static int MASTER      = 0;
}


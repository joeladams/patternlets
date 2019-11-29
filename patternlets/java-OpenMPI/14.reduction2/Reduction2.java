/* Reduction2.java
 * ... illustrates the reduction pattern on multiple values,
 *      using buffers in OpenMPI's Java interface.
 *
 * Joel Adams, Calvin University, November 2019.
 *
 * Usage: mpirun -np 4 java ./Reduction2
 *
 * Exercise:
 * - Compile, then run with N = 1, 2, 3, 4, 
 *     comparing output to source code.
 * - Explain behavior of reduce() in terms of
 *     srcBuf and destBuf.
 */

import mpi.*;
import java.nio.IntBuffer;

public class Reduction2 {

 public static void main(String [] args) throws MPIException {
    MPI.Init(args);

    Comm comm        = MPI.COMM_WORLD;
    int id           = comm.getRank();
    int numProcesses = comm.getSize();

    IntBuffer srcBuf = MPI.newIntBuffer(BUFFER_SIZE);
    IntBuffer destBuf = MPI.newIntBuffer(BUFFER_SIZE);

    if (id == MASTER) {
        System.out.print("\nBefore reduction: ");
        printBuf(id, "destBuf", destBuf); 
    }

    for (int i = 0; i < BUFFER_SIZE; ++i) {
        srcBuf.put(i, id * i);
    }

    printSeparator("", id);
    printBuf(id, "srcBuf", srcBuf);
    printSeparator("----", id);

    comm.reduce(srcBuf, destBuf, BUFFER_SIZE,
                 MPI.INT, MPI.SUM, MASTER);

    if ( id == MASTER) {
        System.out.print("After reduction: ");
        printBuf(id, "destBuf", destBuf);
        System.out.println();
    }

    MPI.Finalize();
  }

  /* utility to display the contents of an IntBuffer.
   * @param: id, the int MPI rank of this process.
   * @param: bufName, a String that is the name of the buffer.
   * @param: buf, the IntBuffer.
   * @param: size, the size of buf.
   */
  private static void printBuf(int id, String bufName, IntBuffer buf) {
      String msg = "Process " + id + ", " + bufName + ": [";
      int size = buf.capacity();
      int sizeLessOne = size - 1;
      for (int i = 0; i < size; ++i) {
         msg += buf.get(i);
         if (i < sizeLessOne ) {
             msg += ",";
         }
      }
      msg += "]\n";
      System.out.print(msg);
  }

  /* utility to print a separator between before and after sections.
   * @param: separator, a String.
   * @param: id, the MPI rank of this process. 
   * POST: separator has been printed by the master process.
   */
  private static void printSeparator(String separator, int id) throws MPIException {
     MPI.COMM_WORLD.barrier();
     if (id == MASTER) { System.out.println(separator); }
     MPI.COMM_WORLD.barrier();
  }

  private static int BUFFER_SIZE = 5;
  private static int MASTER      = 0;
}


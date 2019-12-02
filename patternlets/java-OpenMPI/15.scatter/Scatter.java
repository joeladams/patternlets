/* Scatter.java
 * ... illustrates the basic scatter pattern 
 *      using buffers in OpenMPI's Java interface.
 *
 * Note: If the number of values being scattered is not
 *       evenly divisible by the number of processes,
 *       use scatterv() instead of scatter.
 *
 * Joel Adams, Calvin University, November 2019.
 *
 * Usage: mpirun -np 4 java ./Scatter
 *
 * Exercise:
 * - Compile, then run with N = 1, 2, 4, 8. 
 * - Trace execution through source code. 
 * - Explain behavior/effect of scatter. 
 * - What if BUFFER_SIZE is not evenly divisible by N?
 */

import mpi.*;
import java.nio.IntBuffer;

public class Scatter {

 public static void main(String [] args) throws MPIException {
    MPI.Init(args);

    Comm comm         = MPI.COMM_WORLD;
    int id            = comm.getRank();
    int numProcesses  = comm.getSize();

    if (numProcesses > BUFFER_SIZE) {
        if (id == MASTER) {
            System.out.println("\nPlease run this program with N <= 8 processes\n");
        }
        MPI.Finalize();
        System.exit(0);
    }
    IntBuffer sendBuf = null;
    IntBuffer recvBuf = null;

    if (id == MASTER) {
        sendBuf = MPI.newIntBuffer(BUFFER_SIZE);
        for (int i = 0; i < BUFFER_SIZE; ++i) {
           sendBuf.put(i, (i+1) * 11);
        }
        System.out.print("\nBefore scatter: ");
        printBuf(id, "sendBuf", sendBuf);
    }
 
    int numSent = BUFFER_SIZE / numProcesses;
       
    comm.barrier();                    // see comment on next barrier

    recvBuf = MPI.newIntBuffer(numSent);
    printBuf(id, "recvBuf", recvBuf);

    printSeparator("----", id);

    comm.scatter(sendBuf, numSent, MPI.INT, 
                  recvBuf, numSent, MPI.INT, MASTER); 

    if (id == MASTER) {
        System.out.print("After scatter:\n");
    }
    comm.barrier();                    // all of these barriers are here
    printBuf(id, "recvBuf", recvBuf);  //  just to make the output easier
    comm.barrier();                    //  to read; no effect on correctness
    if (id == MASTER) {
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
     if (id == MASTER) { System.out.print(separator + "\n"); }
     MPI.COMM_WORLD.barrier();
  }

  private static int BUFFER_SIZE = 8;
  private static int MASTER      = 0;
}


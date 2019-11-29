/* Gather.java
 * ... illustrates the basic gather pattern 
 *      using buffers in OpenMPI's Java interface.
 *
 * Note: If the number of values being gathered is not
 *       evenly divisible by the number of processes,
 *       use gatherv() instead of gather.
 *
 * Joel Adams, Calvin University, November 2019.
 *
 * Usage: mpirun -np 4 java ./Gather
 *
 * Exercise:
 * - Compile, then run with N = 1, 2, 3, 4, 5. 
 * - Trace execution through source code. 
 * - Explain behavior/effect of gather. 
 */

import mpi.*;
import java.nio.IntBuffer;

public class Gather {

 public static void main(String [] args) throws MPIException {
    MPI.Init(args);

    Comm comm         = MPI.COMM_WORLD;
    int id            = comm.getRank();
    int numProcesses  = comm.getSize();

    IntBuffer gatherBuf = null;

    if (id == MASTER) {
        int valuesToGather = BUFFER_SIZE * numProcesses;
        gatherBuf = MPI.newIntBuffer(valuesToGather);
        System.out.print("\nBefore gather: ");
        printBuf(id, "gatherBuf", gatherBuf);
    }
 
    IntBuffer computeBuf = MPI.newIntBuffer(BUFFER_SIZE);
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        computeBuf.put(i, id * 10 + i);
    }   
    comm.barrier();                          // These barriers are just here
    printBuf(id, "computeBuf", computeBuf);  // to make the output easier to read;
    comm.barrier();                          // no effect on functional correctness

    printSeparator("----", id);

    comm.gather(computeBuf, BUFFER_SIZE, MPI.INT, 
                  gatherBuf, BUFFER_SIZE, MPI.INT, MASTER); 

    if (id == MASTER) {
        System.out.print("After gather: ");
        printBuf(id, "gatherBuf", gatherBuf); 
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

  private static int BUFFER_SIZE = 3;
  private static int MASTER      = 0;
}


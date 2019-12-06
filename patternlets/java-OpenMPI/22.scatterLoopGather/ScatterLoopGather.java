/* ScatterLoopGather.java
 * ... uses MPI to scatter a buffer of values into chunks, 
 *      a loop to process those chunks, and 
 *      a gather to combine the piecemeal values.
 *
 * Goal: The master process fills a buffer with values
 *        and scatters it to all the other processes.
 *       Each process doubles the values in its buffer-chunk.
 *       All processes then gather the chunks back to the master.
 *
 * Joel Adams, Calvin University, November 2019.
 *
 * Note: This assumes BUFFER_SIZE is evenly divisible by N.
 *
 * Usage: mpirun -np 4 java ./BroadcastLoopGather
 *
 * Exercise:
 * - Compile, then run, using 1, 2, 4, and 8 processes
 * - Use source code to trace execution and output
 * - Explain behavior/effect of the scatter and gather.
 * - Optional: change BUFFER_SIZE to be another multiple of 8, such as 16
 */

import java.nio.IntBuffer;
import mpi.*;

public class ScatterLoopGather {

 public static void main(String [] args) throws MPIException {
    MPI.Init(args);

    Comm comm               = MPI.COMM_WORLD;
    int id                  = comm.getRank();
    int numProcesses        = comm.getSize();
    IntBuffer scatterBuffer = null;
    IntBuffer chunkBuffer   = null;
    IntBuffer gatherBuffer  = null;

    if ( BUFFER_SIZE % numProcesses != 0 || numProcesses > BUFFER_SIZE ) {
        String errorMsg = "\nPlease run this program with -np N where N is\n"
                         + " <= " + BUFFER_SIZE + " and divides evenly into "
                         + BUFFER_SIZE + "\n\n";
        System.err.println(errorMsg);
        MPI.Finalize();
        System.exit(0);
    }

    if ( id == MASTER ) { 
        scatterBuffer = MPI.newIntBuffer(BUFFER_SIZE);
        fill(scatterBuffer);
        gatherBuffer = MPI.newIntBuffer(BUFFER_SIZE);
    }

    printBuffers("BEFORE the scatter", id, scatterBuffer, chunkBuffer, gatherBuffer);

    int chunkSize = BUFFER_SIZE / numProcesses;
    chunkBuffer = MPI.newIntBuffer(chunkSize);

    comm.scatter(scatterBuffer, chunkSize, MPI.INT, 
                  chunkBuffer, chunkSize, MPI.INT, MASTER);

    printSeparator("----", id);
    printBuffers("AFTER the scatter", id, scatterBuffer, chunkBuffer, gatherBuffer);

    doubleChunk(chunkBuffer);

    printSeparator("----", id);
    printBuffers("AFTER the doubling", id, scatterBuffer, chunkBuffer, gatherBuffer);

    comm.gather(chunkBuffer, chunkSize, MPI.INT, 
                 gatherBuffer, chunkSize, MPI.INT, MASTER);

    printSeparator("----", id);
    printBuffers("AFTER the gather:", id, scatterBuffer, chunkBuffer, gatherBuffer);

    MPI.Finalize();
  }

  /* utility to fill a Buffer with some values.
   * @param: buf, an IntBuffer.
   * POST: buf has been filled with int values.
   */
  private static void fill(IntBuffer buf) {
     for (int i = 0; i < buf.capacity(); ++i) {
         buf.put(i, i + 11);
     }
  }

  /* utility to print a buffer with labels.
   * @param: label, a String.
   * @param: id, an int containing the MPI rank of this process.
   * @param: sBuf, the IntBuffer the master fills and scatters.
   * @param: cBuf, the IntBuffer for storing a process's chunk.
   * @param: gBuf, the IntBuffer for storing the gathered results.
   * POST: The buffers' contents have been printed, with labels.
   */
  private static void printBuffers(String label, int id, 
                                    IntBuffer sBuf, IntBuffer cBuf, IntBuffer gBuf) {
    String msg = label + ", process " + id
                       + "'s scatterBuffer is: [";
    if (sBuf != null) {
        for (int i = 0; i < sBuf.capacity(); ++i) {
            msg += sBuf.get(i);
            if (i < sBuf.capacity()-1) msg += ",";
        }
    }
    msg += "]\n\t\t\t\tchunkBuffer is: [";
    if (cBuf != null) {
        for (int i = 0; i < cBuf.capacity(); ++i) {
            msg += cBuf.get(i);
            if (i < cBuf.capacity()-1) msg += ",";
        }
    }
    msg += "]\n\t\t\t\tgatherBuffer is: [";
    if (gBuf != null) {
        for (int i = 0; i < gBuf.capacity(); ++i) {
            msg += gBuf.get(i);
            if (i < gBuf.capacity()-1) msg += ",";
        }
    }
    msg += "]\n";
    System.out.print(msg);
  }

  /* utility to double the values in a chunk of an array.
   * @param: fullBuf, an IntBuffer containing all the values.
   * @param: id, the MPI rank of this process.
   * @param: chunkBuf, an IntBuffer into which we will write our values.
   * PRE: chunkBuf.capacity() == BUFFER_SIZE / numProcesses.
   * POST: chunkBuf contains the doubled values of this process's chunk
   *        of fullBuf.
   */
  private static void doubleChunk(IntBuffer chunkBuf) {
      for (int i = 0; i < chunkBuf.capacity(); ++i) {
          int value = chunkBuf.get(i);
          chunkBuf.put(i, value * 2);
      }
  }

  /* utility to print a separator string between the 'before' and 'after' parts.
   * @param: separator, a String.
   * @param: id, the rank of this MPI process.
   * POST: the master has printed the separator to System.out.
   */
  public static void printSeparator(String separator, int id) throws MPIException {
     MPI.COMM_WORLD.barrier();
     if (id == MASTER) { System.out.println(separator); }
     MPI.COMM_WORLD.barrier();
  }

  private static final int MASTER      = 0;
  private static final int BUFFER_SIZE = 8;
}


/* ScatterV_GatherV.java
 * ... uses MPI's scatterv to scatter a buffer of different-sized chunks, 
 *      a loop to process those chunks, 
 *      and gatherv to recombine those different-sized chunks.
 *
 * Joel Adams, Calvin University, December 2019.
 *
 * Goal: The master process fills a buffer with values
 *        and scatters it to all the other processes.
 *       Each process doubles the values in its buffer-chunk.
 *       All processes then gather the chunks back to the master.
 *
 * Note: Using scatterv and gatherv eliminates the constraint
 *        that BUFFER_SIZE needs to be evenly divisible by N.
 *
 * Usage: mpirun -np N java ./ScatterV_GatherV
 *
 * Exercise:
 * - Compile, then run, using N = 1, 2, 3, 4, and 5 processes
 * - Use source code to trace execution and output
 * - Explain how scatterv and gatherv differ from
 *    scatter and gather, respectively
 * - Optional: change BUFFER_SIZE to be another multiple of 8, such as 16
 */

import java.nio.IntBuffer;
import mpi.*;

public class ScatterV_GatherV {

 public static void main(String [] args) throws MPIException {
    MPI.Init(args);

    Comm comm               = MPI.COMM_WORLD;
    int id                  = comm.getRank();
    int numProcesses        = comm.getSize();

    if ( numProcesses > BUFFER_SIZE ) {
        String errorMsg = "\nPlease run this program with -np N where N is\n"
                         + " <= " + BUFFER_SIZE + "\n\n";
        System.err.println(errorMsg);
        MPI.Finalize();
        System.exit(0);
    }

    IntBuffer scatterBuffer = null;
    IntBuffer gatherBuffer  = null;

    if ( id == MASTER ) {                                // master allocates buffers,
        scatterBuffer = MPI.newIntBuffer(BUFFER_SIZE);   //  fills scatterBuffer
        fill(scatterBuffer);
        gatherBuffer = MPI.newIntBuffer(BUFFER_SIZE);
    }

    int [] chunkSizeArray = new int[numProcesses];       // allocate arrays used by
    int [] offsetArray    = new int[numProcesses];       //  scatterv and gatherv

    double doubleBufferSize = BUFFER_SIZE;               // compute needed values
    int chunkSize1 = (int) Math.ceil(doubleBufferSize / numProcesses);
    int chunkSize2 = chunkSize1 - 1;
    int remainder  = BUFFER_SIZE % numProcesses;

    for (int i = 0; i < numProcesses; ++i) {             // for each process i:
        if (remainder == 0 || (remainder != 0 && i < remainder)) {
            chunkSizeArray[i] = chunkSize1;              //   store its chunkSize
            offsetArray[i] = chunkSize1 * i;             //   and the index at which
        } else {                                         //   its chunk begins
            chunkSizeArray[i] = chunkSize2;              //   within scatterBuffer
            offsetArray[i] = (remainder * chunkSize1) + (chunkSize2 * (i - remainder));
        }
    }

    int chunkSize = chunkSizeArray[id];                  // save our chunk size and
    IntBuffer chunkBuffer = MPI.newIntBuffer(chunkSize); // allocate buffer for chunk

    printBuffers("BEFORE the scatterv", id, scatterBuffer, chunkBuffer, gatherBuffer);
                                                         // scatter varying-sized chunks
    comm.scatterv(scatterBuffer, chunkSizeArray, offsetArray, MPI.INT, 
                  chunkBuffer, chunkSize, MPI.INT, MASTER);

    printSeparator("----", id);
    printBuffers("AFTER the scatterv", id, scatterBuffer, chunkBuffer, gatherBuffer);

    doubleChunk(chunkBuffer);                          // process our chunk

    printSeparator("----", id);
    printBuffers("AFTER the doubling", id, scatterBuffer, chunkBuffer, gatherBuffer);

    comm.gatherv(chunkBuffer, chunkSize, MPI.INT,      // gather varying-sized chunks
                 gatherBuffer, chunkSizeArray, offsetArray, MPI.INT, MASTER);

    printSeparator("----", id);                        // display results
    printBuffers("AFTER the gatherv", id, scatterBuffer, chunkBuffer, gatherBuffer);

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


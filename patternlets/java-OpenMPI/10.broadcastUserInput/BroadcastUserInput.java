/* Broadcast.java
 * ... illustrates the use of MPI's broadcast command 
 *     to broadcast a value entered from the commandline.
 *
 * Note: This version uses an IntBuffer of length 1 to store the scalar.
 *
 * Goal: The master process "reads" an input value from the commandline
 *        and broadcasts it to all the other processes.
 *       Each process outputs its value before and after
 *        the broadcast.
 *
 * Original C version by Hannah Sonsalla, Macalester College 2017,
 * Java version by Joel Adams, Calvin University, November 2019.
 *
 * Usage: mpirun -np 4 java ./BroadcastUserInput <integer>
 *
 * Exercise:
 * - Compile and run several times, varying the number of 
 *    MPI processes and input value.
 * - Explain behavior you observe.
 */

import java.nio.IntBuffer;
import mpi.*;

public class BroadcastUserInput {

 public static void main(String [] args) throws MPIException {
    MPI.Init(args);

    Comm comm         = MPI.COMM_WORLD;
    int id            = comm.getRank();
    int numProcesses  = comm.getSize();
    String hostName   = MPI.getProcessorName();

    IntBuffer answerBuf = MPI.newIntBuffer(1); // allocate buffer

    if (id == MASTER) {
        getInput(args, answerBuf);             // MASTER: fill buffer
    }

    String beforeMsg = "BEFORE the broadcast, the answer of process " + id
                       + " on host " + hostName 
                       + " is " + answerBuf.get(0) + "\n";
    System.out.print(beforeMsg);               // all: output 'before' values

    comm.bcast(answerBuf, 1, MPI.INT, 0);      // all: participate in broadcast

    printSeparator("----", id);

    String afterMsg = "AFTER the broadcast, the answer of process " + id
                       + " on host " + hostName 
                       + " is: " + answerBuf.get(0) + "\n";
    System.out.print(afterMsg);                // all: output 'after' values

    MPI.Finalize();
  }

  /* utility to hide details of having the master read an int value
   *  from the commandline (can be adapted to read from anywhere else).
   *
   * @param: args, a String array containing the commandline arguments.
   * @param: buf, an IntBuffer in which to the input value is to be stored.
   *
   * PRE: args[0] contains an integer input value (as a String).
   * POST: buf contains the integer from args[0], or else a default value.
   */
   private static void getInput(String [] args, IntBuffer buf) {
        int result = 0;
        if (args.length >= 1) {
            result = Integer.parseInt( args[0] );
        } else {
            System.err.println("\nUsage: mpirun -np <N> java BroadcastUserInput <integer>\n");
        }
        buf.put(result);
    } 

  /* utility to print a separator between the 'before' and 'after' parts.
   * @param: separator, a String.
   * @param: id, the rank of this MPI process.
   * POST: separator has been printed to System.out.
   */
  public static void printSeparator(String separator, int id) throws MPIException {
     MPI.COMM_WORLD.barrier();
     if (id == MASTER) { System.out.println(separator); }
     MPI.COMM_WORLD.barrier();
  }


  private static final int MASTER = 0;
}


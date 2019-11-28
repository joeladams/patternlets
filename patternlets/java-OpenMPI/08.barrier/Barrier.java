/* Barrier.java
 * ... illustrates the use of MPI's barrier pattern,
 *      using OpenMPI's Java interface.
 *
 * Goal: Have MPI processes pair up and exchange their id numbers.
 *
 * Joel Adams, Calvin University, November 2019,
 *  based on code by Bill Siever (April 2016).
 *
 * Usage: mpirun -np 4 java ./Barrier
 *
 * Exercise:
 *  - Compile; then run the program several times,
 *     noting the interleaved outputs.
 *  - Uncomment the MPI_Barrier() call; then recompile and rerun,
 *     noting how the output changes.
 *  - Explain what effect MPI_Barrier() has on process behavior.

 */

import mpi.*;
import java.nio.CharBuffer;

public class Barrier {

 public static void main(String [] args) throws MPIException {
    MPI.Init(args);

    int numProcesses  = MPI.COMM_WORLD.getSize();
    int id            = MPI.COMM_WORLD.getRank();
    String hostName   = MPI.getProcessorName();

    sendReceivePrint(id, numProcesses, hostName, "BEFORE");

//    MPI.COMM_WORLD.barrier();

    sendReceivePrint(id, numProcesses, hostName, "AFTER");

    MPI.Finalize();
  }

  /* Have workers send messages to the master, which prints them.
   * @param: id, an int
   * @param: numProcesses, an int
   * @param: hostName, a char*
   * @param: position, a char*
   *
   * Precondition: this function is being called by an MPI process
   *               && id is the MPI rank of that process
   *               && numProcesses is the number of processes in the computation
   *               && hostName points to a char array containing the name of the
   *                    host on which this MPI process is running
   *               && position points to "BEFORE" or "AFTER".
   *
   * Postcondition: each process whose id > 0 has sent a message to process 0
   *                     containing id, numProcesses, hostName, and position
   *                && process 0 has received and output each message.
   */

  private static void sendReceivePrint(int id, int numProcesses, 
                                        String hostName, String position)
                                         throws MPIException
  {
      CharBuffer buffer = MPI.newCharBuffer(BUF_SIZE);
      if ( id != MASTER ) {     // Workers: build & send a message to the Master
          String msg = "Process " + id + " of " + numProcesses
                       + " on " + hostName + " is " + position
                       + " the barrier.";
          buffer.put(msg);
          MPI.COMM_WORLD.send(buffer, msg.length(), MPI.CHAR, 0, 0);
      } else {                  // Master: recv & print msg from each Worker
          for (int i = 1; i < numProcesses; ++i) {
              MPI.COMM_WORLD.recv(buffer, BUF_SIZE, MPI.CHAR,
                                   MPI.ANY_SOURCE, MPI.ANY_TAG);
              System.out.println( buffer.toString() );
          }
      }
  }

  private static final int BUF_SIZE = 128;
  private static final int MASTER = 0;
}


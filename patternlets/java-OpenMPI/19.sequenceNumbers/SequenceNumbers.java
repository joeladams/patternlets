/* SequenceNumbers.java
 *  ... shows how to achieve barrier-like behavior
 *      by using MPI message tags as sequence numbers.
 *
 * Joel Adams, Calvin University, November 2019.
 *
 * Usage: mpirun -np 8 java ./SequenceNumbers
 *
 * Exercise:
 * 1. Compile; then run the program several times with N=8,
 *     noting the intermixed outputs
 * 2. Comment out the sendReceivePrint(..., "SECOND", 1); call;
 *      uncomment the sendReceivePrint(..., "SECOND", 2); call;
 *      then recompile and rerun, noting how the output changes.
 * 3. Uncomment the sendReceivePrint(..., "THIRD", 3);
 *      and sendReceivePrint(..., "FOURTH", 4); calls,
 *      then recompile and rerun, noting how the output changes.
 * 4. Explain the differences: what has caused the changes
 *      in the program's behavior, and why?
 */

import mpi.*;
import java.nio.CharBuffer;

public class SequenceNumbers {

 public static void main(String [] args) throws MPIException {
    MPI.Init(args);

    int numProcesses  = MPI.COMM_WORLD.getSize();
    int id            = MPI.COMM_WORLD.getRank();
    String hostName   = MPI.getProcessorName();

    sendReceivePrint(id, numProcesses, hostName, "FIRST", 1);
    sendReceivePrint(id, numProcesses, hostName, "SECOND", 1);
//    sendReceivePrint(id, numProcesses, hostName, "SECOND", 2);
//    sendReceivePrint(id, numProcesses, hostName, "THIRD", 3);
//    sendReceivePrint(id, numProcesses, hostName, "FOURTH", 4);

    MPI.Finalize();
  }

/* Have workers send messages to the master, which prints them.
 * @param: id, an int
 * @param: numProcesses, an int
 * @param: hostName, a String
 * @param: messageNum, a String
 * @param: tagValue, an int
 *
 * Precondition: this routine is being called by an MPI process
 *               && id is the MPI rank of that process
 *               && numProcesses is the number of processes in the computation
 *               && hostName contains the name of the
 *                    host on which this MPI process is running
 *               && messageNum is "FIRST", "SECOND", "THIRD", ...
 *               && tagValue is the value for the tags of the message
 *                   being sent and received this invocation of the function.
 *
 * Postcondition: each process whose id > 0 has sent a message to process 0
 *                    containing id, numProcesses, hostName, messageNum,
 *                    and tagValue
 *                && process 0 has received and output each message.
 */


  private static void sendReceivePrint(int id, int numProcesses, String hostName,
                                        String messageNum, int tagValue)
                                         throws MPIException
  {
      CharBuffer buffer = MPI.newCharBuffer(BUF_SIZE);
      if ( id != MASTER ) {     // Workers: build & send a message to the Master
          String msg = "This is the " + messageNum + " message from process #"
                        + id + " of " + numProcesses + " on " + hostName + ".\n";
          buffer.put(msg);
          MPI.COMM_WORLD.send(buffer, msg.length(), MPI.CHAR, MASTER, tagValue);
      } else {                  // Master: recv & print msg from each Worker
          for (int i = 1; i < numProcesses; ++i) {
              MPI.COMM_WORLD.recv(buffer, BUF_SIZE, MPI.CHAR,
                                   MPI.ANY_SOURCE, tagValue);
              System.out.print( buffer.toString() );
          }
          System.out.println();
      }
  }

  private static final int BUF_SIZE = 256;
  private static final int MASTER = 0;
}


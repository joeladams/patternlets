/* MessagePassing2.java
 * ... illustrates the use of MPI's send and receive commands
 *      to send Strings using CharBuffers,
 *      using OpenMPI's Java interface.
 *
 * Goal: Have MPI processes pair up and exchange their host-names.
 *
 * Note: Values are sent/received in Java using arrays or buffers.
 *       This example uses chars but the same approach works with numbers.
 *
 * Joel Adams, Calvin University, November 2019,
 *  error-handling adapted from Hannah Sonsalla, Macalester College 2017.
 *
 * Usage: mpirun -np 4 java ./MessagePassing2
 *
 * Exercise:
 * - Compile and run, using N = 1, 2, 4, and 8 processes.
 * - Use source code to trace execution.
 * - Compare to MessagePassing.java; note send-receive differences.
 */

import mpi.*;
import java.nio.CharBuffer;

public class MessagePassing2 {

 public static void main(String [] args) throws MPIException {
    MPI.Init(args);

    Comm comm         = MPI.COMM_WORLD;
    int numProcesses  = comm.getSize();
    int id            = comm.getRank();

    if ( numProcesses <= 1 || (numProcesses % 2) != 0)  {
        if (id == MASTER) {
            System.out.print("\nPlease run this program using -np N where N is positive and even.\n\n");
        }
        MPI.Finalize();
        System.exit(0);
    } 

    String hostName   = MPI.getProcessorName();
    CharBuffer sendBuf = MPI.newCharBuffer(BUFFER_MAX);
    //sendBuf.put(hostName);  // this is supposed to work but doesn't for me,
                              // (UTF-16 vs UTF-8?) so we'll do it the hard way
    for (int i = 0; i < hostName.length(); ++i) {
         sendBuf.put(i, hostName.charAt(i));
    }
    CharBuffer receiveBuf = MPI.newCharBuffer(BUFFER_MAX);
    Status status;

    if ( odd(id) ) { // odd processes send, then receive
        comm.send(sendBuf, sendBuf.length(), MPI.CHAR, id-1, 0);
        status = comm.recv(receiveBuf, BUFFER_MAX, MPI.CHAR, id-1, 0); 
    } else {         // even processes receive then send
        status = comm.recv(receiveBuf, BUFFER_MAX, MPI.CHAR, id+1, 0); 
        comm.send(sendBuf, sendBuf.length(), MPI.CHAR, id+1, 0);
    }

    String sentString = sendBuf.toString();
    String receivedString = receiveBuf.toString();
    String message = "Process " + id + " computed " + sentString
                      + " and received " + receivedString + "\n";
    System.out.print(message);

    MPI.Finalize();
  }

  private static boolean odd(int number) { return number % 2 != 0; }

  private static final int MASTER = 0;
  private static final int BUFFER_MAX = 256;
}


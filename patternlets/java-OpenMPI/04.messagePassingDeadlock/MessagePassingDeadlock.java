/* MessagePassingDeadlock.java
 * ... illustrates deadlock with MPI send and receive commands,
 *      using OpenMPI's Java interface.
 *
 * Goal: Have MPI processes pair up and exchange their id numbers.
 *
 * Note: In Java, messages are passed via arrays or buffers.
 *
 * Joel Adams, Calvin University, November 2019,
 *  based on work by Hannah Sonsalla, Macalester College 2017.
 *
 * Usage: mpirun -np 4 java ./MessagePassingDeadlock
 *
 * Exercise:
 * - Compile and run, using more than one process.
 * - Use source code to trace execution.
 * - Why does this fail?
 */

import mpi.*;

public class MessagePassingDeadlock {

 public static void main(String [] args) throws MPIException {
    MPI.Init(args);

    Comm comm         = MPI.COMM_WORLD;
    int numProcesses  = comm.getSize();
    int id            = comm.getRank();

    if ( numProcesses <= 1 || (numProcesses % 2) != 0)  {
        if (id == MASTER) {
            System.out.print("\nPlease run this program using -np N where N is positive and even.\n\n");
        }
    } else {
        int [] sendValue = new int[1];     // create length-1 int arrays for sending
        sendValue[0] = id;
        int [] receivedValue = new int[1]; //  and receiving
        Status status;

        if ( odd(id) ) { // odd processes receive from their 'left' neighbor, then send
            status = comm.recv(receivedValue, 1, MPI.INT, id-1, 0); 
            comm.send(sendValue, 1, MPI.INT, id-1, 0);
        } else {         // even processes receive from their 'right' neighbor, then send
            status = comm.recv(receivedValue, 1, MPI.INT, id+1, 0); 
            comm.send(sendValue, 1, MPI.INT, id+1, 0);
        }

        String message = "Process " + id + " computed " + sendValue[0]
                         + " and received " + receivedValue[0] + "\n";
        System.out.print(message);
    }

    MPI.Finalize();
  }

  public static boolean odd(int number) { return number % 2 != 0; }

  private static final int MASTER = 0;
}


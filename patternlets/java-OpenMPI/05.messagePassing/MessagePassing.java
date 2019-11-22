/* MessagePassing.java
 * ... illustrates the use of MPI's send and receive commands,
 *      using OpenMPI's Java interface.
 *
 * Goal: Have MPI processes pair up and exchange their id numbers.
 *
 * Note: Values are sent/received in Java using arrays or buffers.
 *
 * Joel Adams, Calvin University, November 2019,
 *  with error-handling from Hannah Sonsalla, Macalester College 2017.
 *
 * Usage: mpirun -np 4 java ./MessagePassing
 *
 * Exercise:
 * - Compile and run, using N = 4, 6, 8, and 10 processes.
 * - Use source code to trace execution.
 * - Explain what each process:
 * -- sends
 * -- receives
 * -- outputs.
 * - Run using N = 5 processes. What happens?
 */

import mpi.*;

public class MessagePassing {

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
        int [] sendValue = new int[1];
        sendValue[0] = id;
        int [] receivedValue = new int[1]; 
        Status status;

        if ( odd(id) ) { // odd processes send, then receive
            comm.send(sendValue, 1, MPI.INT, id-1, 0);
            status = comm.recv(receivedValue, 1, MPI.INT, id-1, 0); 
        } else {         // even processes receive then send
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


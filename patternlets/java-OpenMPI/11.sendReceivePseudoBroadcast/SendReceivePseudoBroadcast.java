/* SendReceivePseudoBroadcast.java
 * ... uses MPI's send and receive commands to perform a pseudo-broadcast.
 *
 * Goal: The master process sends an array to each worker process.
 *
 * Note: You do *not* want to "broadcast" an array or buffer this way,
 *       b/c its time-complexity is linear in the number of processes (O(P)),
 *       whereas the time-complexity of an MPI broadcast is logarithmic
 *        in the number of processes (O(lg(P)).
 *
 *  Hannah Sonsalla, Macalester College 2017,
 *   translated into Java by Joel Adams, Calvin University, Nov. 2019.
 *
 * Usage: mpirun -np 4 java ./SendReceivePseudoBroadcast
 *
 * Exercise:
 * - Compile and run, using N = 2, 4, 6, 8, and 10 processes.
 * - Use source code to trace execution.
 */

import mpi.*;

public class SendReceivePseudoBroadcast {

 public static void main(String [] args) throws MPIException {
    MPI.Init(args);

    Comm comm         = MPI.COMM_WORLD;
    int numProcesses  = comm.getSize();
    int id            = comm.getRank();

    int [] array      = new int[ARRAY_SIZE];

    if (id == MASTER) {
        fill(array);
    }

    print("BEFORE:", id, array);  // everyone print what they have

    if ( id == MASTER ) {         // Master: send array to each worker
        for (int i = 1; i < numProcesses; ++i) {
            comm.send(array, ARRAY_SIZE, MPI.INT, i, 0);
        }
    } else {                      // Workers: receive arrays
        comm.recv(array, ARRAY_SIZE, MPI.INT, MASTER, 0); 
    }

    printSeparator("----", id);

    print("AFTER:", id, array);   // everyone print what they have

    MPI.Finalize();
  }

  /* utility to fill an array with some values.
   * @param: a, an int array.
   * POST: a has been filled with int values.
   */
  private static void fill(int [] a) {
     for (int i = 0; i < a.length; ++i) {
         a[i] = i + 11;
     }
  }

  /* utility to print an array with a label.
   * @param: label, a String.
   * @param, id, the rank of this MPI process.
   * @param: a, an int array.
   * POST: label and a have been printed to the console.
   */
  private static void print(String label, int id, int [] a) {
      String msg = label + " array sent, process " + id
                    + " has: {" + a[0] + " " + a[1] + " " 
                    + a[2] + " " + a[3] + " " + a[4] + " " 
                    + a[5] + " "  + a[6] + " "  + a[7] + "}\n";
      System.out.print(msg);
  }

  /* utility to print a string between 'before' and 'after' sections.
   * @param: id, the MPI rank of this process.
   * @param: separator, a String.
   * POST: separator has been printed.
   */
  private static void printSeparator(String separator, int id) throws MPIException {
     MPI.COMM_WORLD.barrier();
     if (id == 0) { System.out.println(separator); }
     MPI.COMM_WORLD.barrier();
  }


  private static final int MASTER = 0;
  private static final int ARRAY_SIZE = 8;
}


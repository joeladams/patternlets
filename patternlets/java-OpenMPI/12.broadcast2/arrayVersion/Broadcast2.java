/* Broadcast2.java
 * ... illustrates the use of MPI's broadcast command with multiple values.
 *
 * Note: This version uses an array to store the values.
 *
 * Goal: The master process fills an array with values
 *        and broadcasts it to all the other processes.
 *       Each process outputs its array before and after
 *        the broadcast.
 *
 * Joel Adams, Calvin University, November 2019,
 *
 * Usage: mpirun -np 4 java ./Broadcast2
 *
 * Exercise:
 * - Compile, then run several times,
 *     using 2, 4, and 8 processes
 * - Use source code to trace execution and output
 * - Explain behavior/effect of MPI_Bcast().
 */

import mpi.*;

public class Broadcast2 {

 public static void main(String [] args) throws MPIException {
    MPI.Init(args);

    Comm comm         = MPI.COMM_WORLD;
    int id            = comm.getRank();

    int [] array = new int[ARRAY_SIZE];            // all: allocate array 

    if ( id == MASTER ) {                          // MASTER: fill its array
        fill(array);
    }

    print("BEFORE", id, array);                    // all: print buffers before

    printSeparator("----", id, comm);

    comm.bcast(array, array.length, MPI.INT, 0);   // all: participate in broadcast

    print("AFTER", id, array);                     // all: print buffers after

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

  /* utility to print a buffer with descriptive labels.
   * @param: label, a String.
   * @param: id, this process's MPI rank.
   * @param, a, an int array.
   * POST: label, id, and a have been displayed via System.out.
   */
  private static void print(String label, int id, int [] a) {
    String msg = label + " the broadcast, process " + id
                       + "'s array contains:";
    for (int i = 0; i < a.length; ++i) {
      msg += (" " + a[i]);
    }
    msg += "\n";
    System.out.print(msg); 
  }
 
  /* utility to print a separator string between the 'before' and 'after' parts.
   * @param: separator, a String.
   * @param: id, the rank of this MPI process.
   * @param: comm, the Communicator for the processes involved.
   * POST: the master has printed the separator to System.out.
   */
  public static void printSeparator(String separator, int id, Comm comm) 
                                      throws MPIException {
     comm.barrier();
     if (id == MASTER) { System.out.println(separator); }
     comm.barrier();
  }

  private static final int MASTER = 0;
  private static final int ARRAY_SIZE = 8;
}


/* MessagePassing3.java
 * ... illustrates the use of MPI's send and receive commands
 *      in combination with the master-worker pattern.
 *
 * Goal: The master process sends its id to process 1
 *        and receives an array of ids from process N-1.
 *       Every other process i receives an array of ids from process i-1,
 *        appends its id to the array, and sends the array to process (i+1)%N.
 *
 * Joel Adams, Calvin University, November 2019,
 *
 * Usage: mpirun -np 4 java ./MessagePassing3
 *
 * Exercise:
 * - Compile and run, varying N from 1-8.
 * - Explain the behavior you observe.
 */

import mpi.*;

public class MessagePassing3 {

 public static void main(String [] args) throws MPIException {
    MPI.Init(args);

    Comm comm         = MPI.COMM_WORLD;
    int numProcesses  = comm.getSize();
    int id            = comm.getRank();

    if ( numProcesses <= 1 )  {
        if (id == MASTER) {
            System.out.print("\nPlease run this program using -np N where N is at least 2.\n\n");
        }
        MPI.Finalize();
        System.exit(0);
    } 

    int [] sendArray = new int[ARRAY_SIZE];            // allocate arrays
    int [] receiveArray = new int [ARRAY_SIZE];
    int numValuesRecvd = 0;
    Status status;

    if ( id == MASTER ) {                              // MASTER:
        sendArray[0] = id;                             // 1. put id in the array
        comm.send(sendArray,                           // 2. send: array,
                        1,                             //          number of values,
                        MPI.INT,                       //          type of values,
                        id+1,                          //          destination id,
                        0);                            //          tag
        status = comm.recv(receiveArray,               // 3. recv: array,
                            ARRAY_SIZE,                //          array capacity,
                            MPI.INT,                   //          type of values,
                            numProcesses-1,            //          sender id,
                            0);                        //          tag
        numValuesRecvd = status.getCount(MPI.INT);     // 4. how many ints did we get?
        output(receiveArray, numValuesRecvd);          // 5. output what we got
    } else {                                           // WORKERS:
        status = comm.recv(receiveArray,               // 1. receive: array,
                            ARRAY_SIZE,                //             array capacity,
                            MPI.INT,                   //             type of values,
                            id-1,                      //             sender id,
                             0);                       //             tag
        numValuesRecvd = status.getCount(MPI.INT);     // 2. how many ints did we get?
        output(receiveArray, numValuesRecvd);          // 3. output what we got
        receiveArray[numValuesRecvd] = id;             // 4. append id to buffer
        comm.send(receiveArray,                        // 5. send: buffer,
                   numValuesRecvd+1,                   //          number of values,
                   MPI.INT,                            //          type of values,
                   (id+1) % numProcesses,              //          destination id,
                    0);                                //          tag
    }

    MPI.Finalize();
  }

  /* utility to print an array with useful labels.
   * @param: arr, an array.
   * @param: size, the number of ints in the array.
   * POST: The ints in buf have been displayed on System.out,
   *        preceded by spaces, and with a newline at the end.
   */
  private static void output(int [] arr, int size) throws MPIException {
      System.out.printf("Process %d of %d received:",
                          MPI.COMM_WORLD.getRank(),
                          MPI.COMM_WORLD.getSize());
      for (int i = 0; i < size; ++i) {
          System.out.print( " " );
          System.out.print( arr[i] );
      }
      System.out.print("\n");
  }

  private static final int MASTER = 0;
  private static final int ARRAY_SIZE = 256;
}


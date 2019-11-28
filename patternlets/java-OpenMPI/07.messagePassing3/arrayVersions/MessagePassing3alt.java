/* MessagePassing3.java
 * ... illustrates the use of MPI's send and receive commands
 *      in combination with the master-worker pattern.
 *
 * Goal: The master process sends its id to process 1
 *        and receives an array of ids from process N-1.
 *       Every other process i receives an array of ids from process i-1,
 *        appends its id to the buffer, and sends the array to process (i+1)%N.
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
import java.nio.IntBuffer;

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

    int [] sendArray = null;
    int [] receiveArray = null;
    Status status;

    if ( id == MASTER ) {                              // MASTER:
        sendArray = new int[1];                        // 0. allocate array
        sendArray[0] = id;                             // 1. put id in the array
        comm.send(sendArray,                           // 2. send: array,
                        1,                             //          number of values,
                        MPI.INT,                       //          type of values,
                        id+1,                          //          destination id,
                        0);                            //          tag.
        receiveArray = new int[numProcesses];
        status = comm.recv(receiveArray,               // 3. recv: array,
                            numProcesses,              //          array capacity,
                            MPI.INT,                   //          type of values,
                            numProcesses-1,            //          sender id,
                            0);                        //          tag.
        output(receiveArray);                          // 4. output what we got.
    } else {                                           // WORKERS:
        int receiveArraySize = id;                     // 0. how many ints are coming?
        receiveArray = new int[receiveArraySize];      // 1. allocate an array for them
        status = comm.recv(receiveArray,               // 2. receive: array,
                            receiveArraySize,          //             array capacity,
                            MPI.INT,                   //             type of values,
                            id-1,                      //             sender id,
                             0);                       //             tag.
        output(receiveArray);                          // 3. output what we got.
        int valuesToSend = receiveArraySize+1;         // 4. how many are we sending?
        sendArray = new int[valuesToSend];             // 5. allocate the array
        copy(receiveArray, sendArray);                 // 6. copy recv'd values
        sendArray[valuesToSend-1] = id;                // 7. append id to buffer
        comm.send(sendArray,                           // 8. send: buffer,
                   valuesToSend,                       //          number of values,
                   MPI.INT,                            //          type of values,
                   (id+1) % numProcesses,              //          destination id,
                    0);                                //          tag.
    }

    MPI.Finalize();
  }

  /* utility to copy the received array into the sending array.
   * @param: src, an int array from which we are copying values.
   * @param: dest, an int array into which we are copying them.
   * PRE:  dest.length > src.length.
   * POST: dest contains the values of src.
   */
  private static void copy(int [] src, int [] dest) {
      assert( dest.length > src.length );
      for (int i = 0; i < src.length; ++i) {
          dest[i] = src[i];
      }
  }

  /* utility to print an array with useful labels.
   * @param: arr, an array.
   * POST: The ints in buf have been displayed on System.out,
   *        preceded by spaces, and with a newline at the end.
   */
  private static void output(int [] arr) throws MPIException {
      System.out.printf("Process %d of %d received:",
                          MPI.COMM_WORLD.getRank(),
                          MPI.COMM_WORLD.getSize());
      for (int i = 0; i < arr.length; ++i) {
          System.out.print( " " );
          System.out.print( arr[i] );
      }
      System.out.print("\n");
  }

  private static final int MASTER = 0;
  private static final int BUFFER_MAX = 256;
}


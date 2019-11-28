/* MessagePassing3.java
 * ... illustrates the use of MPI's send and receive commands
 *      in combination with the master-worker pattern.
 *
 * Goal: The master process sends its id to process 1
 *        and receives a buffer of ids from process N-1.
 *       Every other process i receives a buffer of ids from process i-1,
 *        appends its id to the buffer, and sends the buffer to process (i+1)%N.
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

    IntBuffer sendBuf = MPI.newIntBuffer(BUFFER_SIZE);
    IntBuffer receiveBuf = MPI.newIntBuffer(BUFFER_SIZE);
    Status status;

    if ( id == MASTER ) {                              // MASTER:
        sendBuf.put(0, id);                            // 1. put id in buffer
        comm.send(sendBuf,                             // 2. send: buffer,
                        1,                             //          number of values,
                        MPI.INT,                       //          type of values,
                        id+1,                          //          destination id,
                        0);                            //          tag.
        status = comm.recv(receiveBuf,                 // 3. recv: buffer,
                            BUFFER_SIZE,               //          buffer capacity,
                            MPI.INT,                   //          type of values,
                            numProcesses-1,            //          sender id,
                            0);                        //          tag.
        int valuesReceived = status.getCount(MPI.INT); // 4. how many did we get?
        output(receiveBuf, valuesReceived);            // 5. output what we got.
    } else {                                           // WORKERS:
        status = comm.recv(receiveBuf,                 // 1. receive: buffer,
                            BUFFER_SIZE,               //             buffer capacity,
                            MPI.INT,                   //             type of values,
                            id-1,                      //             sender id,
                             0);                       //             tag.
        int valuesReceived = status.getCount(MPI.INT); // 2. how many did we get?
        output(receiveBuf, valuesReceived);            // 3. output what we got.
        receiveBuf.put(valuesReceived, id);            // 4. append id to buffer
        comm.send(receiveBuf,                          // 5. send: buffer,
                   valuesReceived+1,                   //          number of values,
                   MPI.INT,                            //          type of values,
                   (id+1) % numProcesses,              //          destination id,
                    0);                                //          tag.
    }

    MPI.Finalize();
  }

  /* utility to print an IntBuffer with descriptive labels.
   * @param: buf, an IntBuffer.
   * @param: size, the number of ints in IntBuffer
   *          (b/c IntBuffer has no length() method,
   *            whose bright idea was that?).
   * POST: The ints in buf have been displayed on System.out,
   *        preceded by spaces, and with a newline at the end.
   */
  private static void output(IntBuffer buf, int size) throws MPIException {
      System.out.printf("Process %d of %d received:",
                          MPI.COMM_WORLD.getRank(),
                          MPI.COMM_WORLD.getSize());
      for (int i = 0; i < size; ++i) {
          System.out.print( " " );
          System.out.print( buf.get(i) );
      }
      System.out.print("\n");
  }

  private static final int MASTER = 0;
  private static final int BUFFER_SIZE = 256;
}


/* Broadcast.java
 * ... illustrates the use of MPI's broadcast command with a scalar value.
 *
 * Note: This version uses an IntBuffer of length 1 to store the scalar.
 *
 * Goal: The master process reads an 'answer' value from a file
 *        and broadcasts it to all the other processes.
 *       Each process outputs its 'answer' value before and after
 *        the broadcast.
 *
 * Joel Adams, Calvin University, November 2019,
 *
 * Usage: mpirun -np 4 java ./Broadcast
 *
 * Exercise:
 * - Compile, then run several times,
 *     using 2, 4, and 8 processes
 * - Use source code to trace execution and output
 *     (noting contents of file "data.txt");
 * - Explain behavior/effect of MPI_Bcast().
 */

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.nio.IntBuffer;
import mpi.*;

public class Broadcast {

 public static void main(String [] args) throws MPIException {
    MPI.Init(args);

    Comm comm         = MPI.COMM_WORLD;
    int id            = comm.getRank();

    IntBuffer answerBuf = MPI.newIntBuffer(1);

    if ( id == MASTER ) {                      // MASTER: read data from file
        int answer = readAnswerFromFile("data.txt");
        answerBuf.put(answer);
    }

    String beforeMsg = "BEFORE the broadcast, process " + id
                       + "'s answer is: " + answerBuf.get(0) + "\n";
    System.out.print(beforeMsg);               // all: output 'before' values

    printSeparator("----", id);

    comm.bcast(answerBuf, 1, MPI.INT, 0);      // all: participate in broadcast

    String afterMsg = "AFTER the broadcast, process " + id
                       + "'s answer is: " + answerBuf.get(0) + "\n";
    System.out.print(afterMsg);                // all: output 'after' values

    MPI.Finalize();
  }

  /* utility to hide details of reading an int value from an input file.
   *
   * @param: fileName, a String containing the input file's name.
   * PRE: the first thing in the input file is an integer.
   *
   * @return: the integer from the file.
   * POST: this function has read and returned the integer from fileName.
   */
   private static int readAnswerFromFile(String fileName) throws MPIException {
        assert( fileName != null && fileName != "" );
        File inFile = new File(fileName);               // 1. open input file
        assert( inFile != null );
        Scanner scanner = null;                         // 2. create a Scanner to it
        try {
            scanner = new Scanner(inFile);
        } catch ( FileNotFoundException e) {
            System.err.println("\nUnable to create Scanner for 'data.txt'\n\n");
            MPI.Finalize();
            System.exit(0);
        }

        int intValue = scanner.nextInt();               // 3. read answer from the file

        scanner.close();                                // 4. clean up and return

        return intValue;
  } 

  /* utility to print a separator string between the 'before' and 'after' parts.
   * @param: separator, a String.
   * @param: id, the rank of this MPI process.
   * POST: the master has printed the separator to System.out.
   */
  public static void printSeparator(String separator, int id) throws MPIException {
     MPI.COMM_WORLD.barrier();
     if (id == MASTER) { System.out.println(separator); }
     MPI.COMM_WORLD.barrier();
  }

  private static final int MASTER = 0;
}


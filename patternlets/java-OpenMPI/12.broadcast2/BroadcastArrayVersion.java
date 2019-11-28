/* Broadcast.java
 * ... illustrates the use of MPI's broadcast command with a scalar value.
 * This version uses an array of length 1 to store the scalar value.
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
 * - Compile and run several times,
 *     using 2, 4, and 8 processes
 * - Use source code to trace execution and output
 *     (noting contents of file "data.txt");
 * - Explain behavior/effect of MPI_Bcast().
 */

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import mpi.*;

public class Broadcast {

 public static void main(String [] args) throws MPIException {
    MPI.Init(args);

    Comm comm         = MPI.COMM_WORLD;
    int id            = comm.getRank();

    int [] answer = new int[1]; 

    if ( id == MASTER ) {                 // MASTER: read data from file
        answer[0] = readAnswerFromFile("data.txt");
    }

    String beforeMsg = "BEFORE the broadcast, process " + id
                       + "'s answer is: " + answer[0] + "\n";
    System.out.print(beforeMsg);

    if ( id == MASTER) {                 // MASTER: separate before from after
        System.out.println("----");
    }

    comm.bcast(answer, 1, MPI.INT, 0);

    String afterMsg = "AFTER the broadcast, process " + id
                       + "'s answer is: " + answer[0] + "\n";
    System.out.print(afterMsg);

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

  private static final int MASTER = 0;
}


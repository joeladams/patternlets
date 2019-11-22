/* Spmd.java
 * ... illustrates the single program multiple data
 *      (SPMD) pattern using basic MPI commands
 *      and OpenMPI's Java interface.
 *
 * Joel Adams, Calvin University, November 2019.
 *
 * Usage: mpirun -np 4 java ./Spmd
 *
 * Exercise:
 * - Compile and run.
 * - Compare source code to output.
 * - Rerun, using varying numbers of processes
 *    (i.e., vary the argument to 'mpirun -np').
 * - Explain what "multiple data" values this
 *    "single program" is generating.
 */

import mpi.*;

public class Spmd {

 public static void main(String [] args) throws MPIException {
    MPI.Init(args);

    int id           = MPI.COMM_WORLD.getRank();
    int numProcesses = MPI.COMM_WORLD.getSize();
    String hostName  = MPI.getProcessorName();

    String message   = "Greetings from process #" + id
                         + " of " + numProcesses 
                         + " on " + hostName + "\n";
    System.out.print(message);

    MPI.Finalize();
  }
}


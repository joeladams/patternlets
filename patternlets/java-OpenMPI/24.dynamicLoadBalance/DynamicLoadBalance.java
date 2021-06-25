/* DynamicLoadBalance.java
 * ... illustrates how to use the dynamic load balancing pattern 
 *     using OpenMPI's Java interface.
 * This code was based on the dynamicLoadBalance.py script,
 * that was originally written by Libby Shoop (Macalester College)
 * 
 * Ruth Kurniawati, Westfield State University, June 2021.
 *
 * Usage: mpirun -np 4 java ./DynamicLoadBalance
 *
 * Exercise:
 * - Compile and run, varying N = 4, 8.
 * - Explain behavior of the dynamic load balancing of the available work
 */

import mpi.*;
import java.nio.IntBuffer;
import java.util.Arrays;
import java.util.Random;

public class DynamicLoadBalance {
    public static final int MASTER = 0;

    // tags that can be applied to messages
    public static final int WORKTAG = 1;
    public static final int DIETAG = 2;

    public static void main(String [] args) throws MPIException {
       MPI.Init(args);
   
       int id           = MPI.COMM_WORLD.getRank();
       int numProcesses = MPI.COMM_WORLD.getSize();
       //String hostName  = MPI.getProcessorName();
   
       if (id == MASTER) {
            // create an arbitrary array of numbers for how long each
            // worker task will 'work', by sleeping that amount of seconds
            int numTasks = (numProcesses-1) * 4; // avg 4 tasks per worker process
            int[] workTimes = genTasks(numTasks);
            System.out.println("master created " + workTimes.length + " values for sleep times:" + Arrays.toString(workTimes));
            handOutWork(MPI.COMM_WORLD, workTimes, numProcesses);
            
       } else {
            worker(MPI.COMM_WORLD);
       }   
       MPI.Finalize();
    }

    private static int[] genTasks(int numTasks) {
        int[] tasks = new int[numTasks];
        Random r = new Random(1000); // use the same seed
        for(int i = 0; i < numTasks; i++) {
            tasks[i] = r.nextInt(8) + 1;
        }
        return tasks;
    }

    private static void worker(Comm comm) throws MPIException {
        // keep receiving messages and do work, unless tagged to 'die'
        IntBuffer buf = MPI.newIntBuffer(1);
        while(true) {
            Status stat = comm.recv(buf, 1, MPI.INT, 0, MPI.ANY_TAG);
            int waitTime = buf.get(0);
            System.out.println("worker "+comm.getRank()+" got "+ waitTime);
            if (stat.getTag() == DIETAG) {
                System.out.println("worker "+comm.getRank()+" dying");
                return;
            }
            // simulate work by sleeping
            try {
                Thread.sleep(1000*waitTime); // sleep for waitTime seconds
            } catch (InterruptedException e) {
                e.printStackTrace();
            } 

            // indicate done with work by sending to Master
            //System.out.println("worker "+comm.getRank()+" completed work!");
            buf.put(0, waitTime);
            comm.send(buf, 1, MPI.INT, 0, WORKTAG);
        }
    }

    private static void handOutWork(Comm comm, int[] workTimes, int numProcesses) throws MPIException {
        int totalWork = workTimes.length;
        int workCount = 0, recvCount = 0;
        System.out.println("master sending first tasks");
        IntBuffer sendBuf = MPI.newIntBuffer(1);
         
        for(int id = 1; id < numProcesses; id++) {
            int work = workTimes[workCount++];
            sendBuf.put(0, work);
            comm.send(sendBuf, 1, MPI.INT, id, WORKTAG);
            System.out.println("master sent "+ work +" to "+id);
        }

        // while there is still work,
        // receive result from a worker, which also
        // signals they would like some new work
        IntBuffer recvBuf = MPI.newIntBuffer(1);
        while (workCount < totalWork) {
            // System.out.println("Master workcount " + workCount + ", total "+ totalWork);
            // receive next finished result
            Status stat = comm.recv(recvBuf, 1, MPI.INT, MPI.ANY_SOURCE, WORKTAG);
            recvCount++;
            int workerId = stat.getSource();
            int completedWorkTime = recvBuf.get(0);
            System.out.println("master received "+completedWorkTime+" from "+ workerId);
            // send next work
            int newWorkTime = workTimes[workCount++];
            sendBuf.put(0, newWorkTime);
            comm.send(sendBuf, 1, MPI.INT, workerId, WORKTAG);
            System.out.println("master sent "+newWorkTime+" to "+ workerId);
        }
        // Receive results for outstanding work requests.
        while (recvCount < totalWork) {
            Status stat = comm.recv(recvBuf, 1, MPI.INT, MPI.ANY_SOURCE, WORKTAG);
            recvCount++;
            int workerId = stat.getSource();
            int completedWorkTime = recvBuf.get(0);
            System.out.println("end: master received "+completedWorkTime+" from "+ workerId);
        }

        // Tell all workers to stop
        sendBuf.put(0, -1);
        for(int id =1; id < numProcesses; id++) {
            comm.send(sendBuf, 1, MPI.INT, id, DIETAG);
        }
    }       
}

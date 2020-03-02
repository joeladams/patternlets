#
#    Illustrates dynamic assignment of tasks that take
#    varying amounts of time.
#    The master task is solely responsible for handing out 'work'
#    to each worker task, who receives it. The time for each worker
#    task varies between 1 and 6 seconds. As soon as a worker finishes,
#    they send a message asking for more work.
#    Workers who have shorter tasks can do more while workers with longer
#    tasks complete theirs.
#
#  Libby Shoop, Macalester College, February 2020
#
# Example usage:
#      python run.py ./17dynamicLoadBalance.py N
#  Here the N signifies the number of processes to start up in mpi.
#
#  run.py executes this program within mpirun using
#         the number of processes given.
#
# Exercise:
#
#    - Run, using N = 4 processes.
#    - Study the execution carefully

from mpi4py import MPI
import numpy as np
import time

def genTasks(numTasks):
    np.random.seed(1000)  # run the same set of timed tasks
    return np.random.randint(low=1, high=9, size=numTasks)

# tags that can be applied to messages
WORKTAG = 1
DIETAG = 2

def main():
    comm = MPI.COMM_WORLD
    id = comm.Get_rank()            #number of the process running the code
    numProcesses = comm.Get_size()  #total number of processes running
    myHostName = MPI.Get_processor_name()  #machine name running the code

    if (id == 0) :
        # create an arbitrary array of numbers for how long each
        # worker task will 'work', by sleeping that amount of seconds
        numTasks = (numProcesses-1)*4 # avg 4 tasks per worker process
        workTimes = genTasks(numTasks)
        print("master created {} values for sleep times:".format(workTimes.size), flush=True)
        print(workTimes, flush=True)
        handOutWork(workTimes, comm, numProcesses)
    else:
        worker(comm)

def handOutWork(workTimes, comm, numProcesses):
    totalWork = workTimes.size
    workcount = 0
    recvcount = 0
    print("master sending first tasks", flush=True)
    # send out the first tasks to all workers
    for id in range(1, numProcesses):
        if workcount < totalWork:
            work=workTimes[workcount]
            comm.send(work, dest=id, tag=WORKTAG)
            workcount += 1
            print("master sent {} to {}".format(work, id), flush=True)

    # while there is still work,
    # receive result from a worker, which also
    # signals they would like some new work
    while (workcount < totalWork) :
        # receive next finished result
        stat = MPI.Status()
        workTime = comm.recv(source=MPI.ANY_SOURCE, status=stat)
        recvcount += 1
        workerId = stat.Get_source()
        print("master received {} from {}".format(workTime, workerId), flush=True)
        #send next work
        comm.send(workTimes[workcount], dest=workerId, tag=WORKTAG)
        workcount += 1
        print("master sent {} to {}".format(work, workerId), flush=True)

    # Receive results for outstanding work requests.
    while (recvcount < totalWork):
        stat = MPI.Status()
        workTime = comm.recv(source=MPI.ANY_SOURCE, status=stat)
        recvcount += 1
        workerId = stat.Get_source()
        print("end: master received {} from {}".format(workTime, workerId), flush=True)

    # Tell all workers to stop
    for id in range(1, numProcesses):
        comm.send(-1, dest=id, tag=DIETAG)


def worker(comm):
    # keep receiving messages and do work, unless tagged to 'die'
    while(True):
        stat = MPI.Status()
        waitTime = comm.recv(source=0, tag=MPI.ANY_TAG, status=stat)
        print("worker {} got {}".format(comm.Get_rank(), waitTime), flush=True)
        if (stat.Get_tag() == DIETAG):
            print("worker {} dying".format(comm.Get_rank()), flush=True)
            return
        # simulate work by sleeping
        time.sleep(waitTime)
        # indicate done with work by sending to Master
        comm.send(waitTime, dest=0)

########## Run the main function
main()

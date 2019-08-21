#
#  Libby Shoop, Macalester College, July 2019
#
# Example usage:
#      python run.py ./03parallelLoopChunksOf1.py N
#  Here the N signifies the number of processes to start up in mpi.
#  run.py executes this program within mpirun using
#         the number of processes given.
#
# Exercise:
#
#    - Run, using these numbers of processes, N: 1, 2, 4, and 8
#    (i.e., vary the last argument to run.py).
#    - Compare source code to output.
#    - Change REPS to 16, save, rerun, varying N again.
#    - Explain how this pattern divides the iterations of the loop
#    among the processes.
#

from mpi4py import MPI

def main():
    comm = MPI.COMM_WORLD
    id = comm.Get_rank()            #number of the process running the code
    numProcesses = comm.Get_size()  #total number of processes running
    myHostName = MPI.Get_processor_name()  #machine name running the code

    REPS = 8

    if (numProcesses <= REPS):

        for i in range(id, REPS, numProcesses):
            print("On {}: Process {} is performing iteration {}"\
            .format(myHostName, id, i))

    else:
        # can't hove more processes than work; one process reports the error
        if id == 0 :
            print("Please run with number of processes less than \
or equal to {}.".format(REPS))

########## Run the main function
main()

# 01masterWorker.py
# Multiple processes will run this same program concurrently,
# each creating different output data.
# Illustrates the master-worker pattern
# using basic MPI commands.
#
#  Libby Shoop, Macalester College, July 2019
#
# Example usage:
#      python run.py 01masterWorker.py 4
#  Here the 4 signifies the number of processes to start up in mpi.
#  run.py executes this program within mpirun using
#         the number of processes given.
#
# Exercise:
#    - Compare source code to output.
#    - Rerun, using varying numbers of processes from 1 through 8
#    (i.e., vary the last argument to run.py).
#    - Explain what stays the same and what changes as the
#    number of processes changes.
#

from mpi4py import MPI

def main():
    comm = MPI.COMM_WORLD
    id = comm.Get_rank()            #number of the process running the code
    numProcesses = comm.Get_size()  #total number of processes running
    myHostName = MPI.Get_processor_name()  #machine name running the code

    if id == 0:
        print("Greetings from the master, {} of {} on {}"\
        .format(id, numProcesses, myHostName))
    else:
        print("Greetings from a worker, {} of {} on {}"\
        .format(id, numProcesses, myHostName))

########## Run the main function
main()


#    Illustrates scatter list of lists of computed values created
#    in the master process.
#
#  Libby Shoop, Macalester College, July 2019
#
# Example usage:
#      python run.py ./15scattter.py N
#  Here the N signifies the number of processes to start up in mpi.
#
#  run.py executes this program within mpirun using
#         the number of processes given.
#
# Exercise:
#
#    - Run, using N = from 2 through 8 processes.
#    - Try with different values of SMALL_LIST_SIZE, perhaps changing printing
#      of result for readability
#    - Visualize what the comm.gather function is doing.
#

from mpi4py import MPI

SMALL_LIST_SIZE = 3

# create a small list whose values contain id times multiples of 10
def genSmallList(id):
    smallerList = []
    for j in range(1, SMALL_LIST_SIZE+1):
        smallerList = smallerList + [(id * 10)*j]
    return smallerList

def main():
    comm = MPI.COMM_WORLD
    id = comm.Get_rank()            #number of the process running the code
    numProcesses = comm.Get_size()  #total number of processes running
    myHostName = MPI.Get_processor_name()  #machine name running the code

    #all processes create small lists
    sendData = genSmallList(id)
    print("Process {} of {} on {} starts with {}"\
    .format(id, numProcesses, myHostName, sendData))

    # gather the small lists at the master node:
    # final result is a list whose length == the number of processes
    result = comm.gather(sendData, root=0)

    # only the master node has all of the small lists
    if id == 0:
        print("Process {} of {} on {} has result after gather {}"\
        .format(id, numProcesses, myHostName, result))

########## Run the main function
main()

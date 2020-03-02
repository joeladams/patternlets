#
#    Illustrates scatter list of lists of computed values created
#    in the master process.
#
#  Libby Shoop, Macalester College, July 2019
#
# Example usage:
#      python run.py ./14scatter.py N
#  Here the N signifies the number of processes to start up in mpi.
#
#  run.py executes this program within mpirun using
#         the number of processes given.
#
# Exercise:
#
#    - Run, using N = from 2 through 8 processes.
#    - Explain to yourself what genListofLists does.
#    - Visualize what the comm.scatter function is doing.
#

from mpi4py import MPI

# Create a list of lists to be scattered.
def genListOfLists(numElements):
    data = [[0]*3 for i in range(numElements)]
    for i in range(numElements):
        #make small lists of 3 distinct elements
        smallerList = []
        for j in range(1,4):
            smallerList = smallerList + [(i+1)*j]
        # place the small list in the larger list
        data[i] = smallerList
    return data

def main():
    comm = MPI.COMM_WORLD
    id = comm.Get_rank()            #number of the process running the code
    numProcesses = comm.Get_size()  #total number of processes running
    myHostName = MPI.Get_processor_name()  #machine name running the code

    # in mpi4py, the lowercase scatter method only works on lists whose size
    # is the total number of processes.
    numElements = numProcesses      #total elements in list created by master process

    # however, the list can contain lists, like this list of 3-element lists,
    # for example this list of four 3-element lists:
    #     [[1, 2, 3], [2, 4, 6], [3, 6, 9], [4, 8, 12]]

    if id == 0:
        data = genListOfLists(numElements)
        print("Master {} of {} on {} has created list: {}"\
        .format(id, numProcesses, myHostName, data))
    else:
        data = None
        print("Worker Process {} of {} on {} starts with {}"\
        .format(id, numProcesses, myHostName, data))

    #scatter one small list in the large list on node 0 to each of the processes
    result = comm.scatter(data, root=0)

    print("Process {} of {} on {} has result after scatter {}"\
    .format(id, numProcesses, myHostName, result))

    if id == 0:
        print("Master {} of {} on {} has original list after scatter: {}"\
        .format(id, numProcesses, myHostName, data))

########## Run the main function
main()

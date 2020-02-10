#
#    Illustrates reducing lists of computed values created
#    in each process. Both sum and max of lists of
#    values are illustratrated.
#
#  Libby Shoop, Macalester College, July 2019
#
# Example usage:
#      python run.py ./13reductionList.py N
#  Here the N signifies the number of processes to start up in mpi.
#
#  run.py executes this program within mpirun using
#         the number of processes given.
#
# Exercise:
#
#    - Run, using N = from 1 through 4 processes.
#     - Use source code to trace execution- note which processes execute
#       the reduce command.
#     - Explain the behavior you observe.
#     - Uncomment the two lines of code that are commented in the main() function.
#       Observe the new results and explain why the MPI.SUM (using + underneath)
#       behaves the way it does on lists, and what the new function called
#       sumListByElements is doing instead.
#

from mpi4py import MPI

# Exercise: Can you explain what this function returns,
#           given two lists as input?
def sumListByElements(x,y):
    return [a+b for a, b in zip(x, y)]

def main():
    comm = MPI.COMM_WORLD
    id = comm.Get_rank()            #number of the process running the code
    numProcesses = comm.Get_size()  #total number of processes running
    myHostName = MPI.Get_processor_name()  #machine name running the code

    srcList = [1*id, 2*id, 3*id, 4*id, 5*id]

    destListMax = comm.reduce(srcList, op=MPI.MAX)
    destListSum = comm.reduce(srcList, op=MPI.SUM)
    #destListSumByElement = comm.reduce(srcList, op=sumListByElements)

    if id == 0:        # master/root process will print result
        print("The resulting reduce max list is  {}".format(destListMax))
        print("The resulting reduce sum list is  {}".format(destListSum))
        #print("The resulting reduce sum list is  {}".format(destListSumByElement))

########## Run the main function
main()

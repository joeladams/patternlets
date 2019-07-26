#
#    Illustrates reducing single computed values
#    in each process to one final value. Both sum and max
#    of the value are illustratrated.
#
#  Libby Shoop, Macalester College, July 2019
#
# Example usage:
#      python run.py ./12reduction.py N
#  Here the N signifies the number of processes to start up in mpi.
#
#  run.py executes this program within mpirun using
#         the number of processes given.
#
# Exercise:
#
#    - Run, using N = from 1 through 8 processes.
#     - Use source code to trace execution- note which processes execute
#       the reduce command.
#     - Note the similarities to and differentces from previous examples.
#     - Explain the behavior you observe.
#

from mpi4py import MPI

def main():
    comm = MPI.COMM_WORLD
    id = comm.Get_rank()            #number of the process running the code
    numProcesses = comm.Get_size()  #total number of processes running
    myHostName = MPI.Get_processor_name()  #machine name running the code

    square = (id+1) * (id+1)

    if numProcesses > 1 :
        #initiate and complete the reductions
        sum = comm.reduce(square, op=MPI.SUM)
        max = comm.reduce(square, op=MPI.MAX)
    else :
        sum = square
        max = square

    if id == 0:        # master/root process will print result
        print("The sum of the squares is  {}".format(sum))
        print("The max of the squares is  {}".format(max))

########## Run the main function
main()

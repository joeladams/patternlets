#
# ... illustrates deadlock with send and receive commands...
#
#  Libby Shoop, Macalester College, July 2019
#
# Example usage:
#      python run.py ./04messagePassingDeadlock.py N
#  Here the N signifies the number of processes to start up in mpi,
#  which must be positive and an even number.
#
#  run.py executes this program within mpirun using
#         the number of processes given.
#
# Exercise:
#
#    - run, using more than one process.
#    - Use source code to trace execution.
#    - Why does this fail?
#

from mpi4py import MPI

# function to return whether a number of a process is odd or even
def odd(number):
    if (number % 2) == 0:
        return False
    else :
        return True

def main():
    comm = MPI.COMM_WORLD
    id = comm.Get_rank()            #number of the process running the code
    numProcesses = comm.Get_size()  #total number of processes running
    myHostName = MPI.Get_processor_name()  #machine name running the code

    if numProcesses > 1 and not odd(numProcesses):
        sendValue = id
        if odd(id):
            #odd processes receive from their paired 'neighbor', then send
            receivedValue = comm.recv(source=id-1)
            comm.send(sendValue, dest=id-1)
        else :
            #even processes receive from their paired 'neighbor', then send
            receivedValue = comm.recv(source=id+1)
            comm.send(sendValue, dest=id+1)

        print("Process {} of {} on {} computed {} and received {}"\
        .format(id, numProcesses, myHostName, sendValue, receivedValue))

    else :
        if id == 0:
            print("Please run this program with the number of processes \
positive and even")

########## Run the main function
main()

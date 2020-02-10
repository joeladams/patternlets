#
# ... illustrates proper ordering of send and receive commands
#     between pairs of processes
#
#  Libby Shoop, Macalester College, July 2019
#
# Example usage:
#      python run.py ./05messagePassing.py N
#  Here the N signifies the number of processes to start up in mpi,
#  which must be positive and an even number.
#
#  run.py executes this program within mpirun using
#         the number of processes given.
#
# Exercise:
#
#    - Run, using N = 4, 6, 8, and 10 processes.
#     - Use source code to trace execution.
#     - Explain what each process:
#     -- sends
#     -- receives
#     -- outputs.
#     - Run using N = 5 processes. What happens?
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
            #odd processes send to their paired 'neighbor', then receive from
            comm.send(sendValue, dest=id-1)
            receivedValue = comm.recv(source=id-1)
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

#
#    Illustrates broacasting a string entered at the command line from
#    the master node to the rest of the nodes.
#
#  Libby Shoop, Macalester College, July 2019
#
# Example usage:
#      python run.py ./09broadcastUserInput.py N dataString
#  Here the N signifies the number of processes to start up in mpi,
#  which must be greater than one. The dataString must be supplied
#  and represents the string that will be broadcast from the master
#  process to the workers.
#
#  run.py executes this program within mpirun using
#         the number of processes given and the dataString argument.
#
# For example, in this special instance, you can send a string with spaces
# and other special characters it it in it like this:
#     python run.py ./09broadcastUserInput.py 2 "hello\ world\!"
#
# Exercise:
#
#    - Run, using N = from 1 through 8 processes, with a string of your choosing.
#     - Use source code to trace execution- note which processes execute
#       the broadcast command.
#     - Explain the behavior you observe.
#

from mpi4py import MPI
import sys

# Determine if user provided a string to be broadcast.
# If not, quit with a warning.
def checkInput(id):
    numArguments = len(sys.argv)
    if numArguments == 1:
        #no extra argument was given- master warns and all exit
        if id == 0:
            print("Please add a string to be broadcast from master to workers")
        sys.exit()

def main():
    comm = MPI.COMM_WORLD
    id = comm.Get_rank()            #number of the process running the code
    numProcesses = comm.Get_size()  #total number of processes running
    myHostName = MPI.Get_processor_name()  #machine name running the code

    if numProcesses > 1 :
        checkInput(id)

        if id == 0:        # master
            #master: get the command line argument
            data = sys.argv[1]
            print("Master Process {} of {} on {} broadcasts \"{}\""\
            .format(id, numProcesses, myHostName, data))

        else :
            # worker: start with empty data
            data = 'No data'
            print("Worker Process {} of {} on {} starts with \"{}\""\
            .format(id, numProcesses, myHostName, data))

        #initiate and complete the broadcast
        data = comm.bcast(data, root=0)
        #check the result
        print("Process {} of {} on {} has \"{}\" after the broadcast"\
        .format(id, numProcesses, myHostName, data))

    else :
        print("Please run this program with the number of processes \
greater than 1")

########## Run the main function
main()

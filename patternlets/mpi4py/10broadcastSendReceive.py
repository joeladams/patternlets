#
#    Illustrates broacasting a dictionary, or associative array, from
#    the master node to the rest of the nodes, but using send and receive
#    point-to-point communication functions instead of the collective
#    broadcast function.
#
#  Libby Shoop, Macalester College, July 2019
#
# Example usage:
#      python run.py ./10broadcastSendReceive.py N
#  Here the N signifies the number of processes to start up in mpi,
#  which must be greater than one.
#
#  run.py executes this program within mpirun using
#         the number of processes given.
#
# Exercise:
#
#    - Run, using N = from 1 through 8 processes.
#     - Use source code to trace execution
#     - Explain the behavior you observe.
#

from mpi4py import MPI

def main():
    comm = MPI.COMM_WORLD
    id = comm.Get_rank()            #number of the process running the code
    numProcesses = comm.Get_size()  #total number of processes running
    myHostName = MPI.Get_processor_name()  #machine name running the code

    if numProcesses > 1 :

        if id == 0:        # master
            #master: generate a dictionary with arbitrary data in it, then send
            data = {'one': 1, 'two': 2, 'three': 3}
            print("Master Process {} of {} on {} broadcasts {}"\
            .format(id, numProcesses, myHostName, data))
            for processId in range (1, numProcesses):
                comm.send(data, dest=processId)

        else :
            # worker: start with empty data, then wait to receive from master
            data = {}
            print("Worker Process {} of {} on {} starts with {}"\
            .format(id, numProcesses, myHostName, data))
            data = comm.recv(source=0)

        #check the result
        print("Process {} of {} on {} has {} after the send/recv broadcast"\
        .format(id, numProcesses, myHostName, data))

    else :
        print("Please run this program with the number of processes \
greater than 1")

########## Run the main function
main()

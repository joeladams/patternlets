#
#    Illustrates send/recv of a list, in combination
#      with the master-worker pattern.
#
#  Libby Shoop, Macalester College, July 2019
#
# Example usage:
#      python run.py ./07messagePassing3.py N
#  Here the N signifies the number of processes to start up in mpi,
#  which must be greater than one.
#
#  run.py executes this program within mpirun using
#         the number of processes given.
#
# Exercise:
#
#    - Run, using N = from 1 through 8 processes.
#     - Use source code to trace execution.
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
            #generate a list with master id in it
            sendList = [id]
            # send to the first worker
            comm.send(sendList, dest=id+1)
            print("Master Process {} of {} on {} sent {}"\
            .format(id, numProcesses, myHostName, sendList))
            # receive from the last worker
            receivedList = comm.recv(source=numProcesses-1)
            print("Master Process {} of {} on {} received {}"\
            .format(id, numProcesses, myHostName, receivedList))
        else :
            # worker: receive from any source
            receivedList = comm.recv(source=id-1)
            # add this worker's id to the list and send along to next worker,
            # or send to the master if the last worker
            sendList = receivedList + [id]
            comm.send(sendList, dest=(id+1) % numProcesses)

            print("Worker Process {} of {} on {} received {} and sent {}"\
            .format(id, numProcesses, myHostName, receivedList, sendList))

    else :
        print("Please run this program with the number of processes \
greater than 1")

########## Run the main function
main()

#
#
# Multiple processes will run this same program concurrently,
# each creating different output data.
#

from mpi4py import MPI

comm = MPI.COMM_WORLD
id = comm.Get_rank()            #number of the process running the code
numProcesses = comm.Get_size()  #total number of processes running
myHostName = MPI.Get_processor_name()  #machine name running the code

print("Greetings from process {} of {} on {}".format(id, numProcesses, myHostName))

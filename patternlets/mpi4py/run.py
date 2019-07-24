#
# Runs the spmd.py program with mpirun in the shell.
#
# Example usage:
#      python run.py program.py 4
#  here the 4 signifies the number of processes to start up in mpi.
#  and program.py is the name of an mpi4py program.
#
import sys
import subprocess
import os.path
from os import path

# Check if there are not enough arguments
if (len(sys.argv) < 3):
    print("You need more arguments. Usage:")
    print("   python ./run.py prog.py N [opt]")
    print("where prog.py should be an mpi4py python program and")
    print("      N is the number of processes to use and")
    print("     [opt] is an optional argument for the program if needed (rare)")
    sys.exit(0)

if (path.exists("./cluster_nodes")):
    print("using ./cluster_nodes file for names of machines")
    command = "mpirun -np " + sys.argv[2] + " -hostfile ./cluster_nodes -map-by node " + "python " + sys.argv[1]
else:
    command = "mpirun -np " + sys.argv[2] + " python " + sys.argv[1]

# Need to check for existence of a 3rd argument and add it
if (len(sys.argv) == 4):
    command = command + " " + sys.argv[3]

print("Command that will run is:")
print(command)
print("")
print("Results:")
subprocess.call(command, shell=True)

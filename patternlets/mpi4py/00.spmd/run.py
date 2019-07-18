#
# Runs the spmd.py program with mpirun in the shell.
#
# Example usage:
#      python run.py 4
#  here the 4 signifies the number of processes to start up in mpi.
#
import sys
import subprocess

command = "mpirun -np " + sys.argv[1] + " python ./spmd.py"

subprocess.call(command, shell=True)

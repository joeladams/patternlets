The OpenMPI implementation of the MPI standard provides a (non-standard) API
for MPI programming using Java.

To use it, OpenMPI must be installed from source, not via a binary package.

1. Make sure that gcc/g++ and Java SDK (javac) are installed on your system
2. Download the current OpenMPI tarball from openmpi.org
3. Extract the OpenMPI folder/directory from the tarball 
4. Using a Terminal, 'cd' into the OpenMPI folder/directory
5. Run: 
	./configure --prefix=/usr/local  --enable-mpi-java --disable-mpi-fortran
   (unless you want Fortran, in which case remove that last switch)
6. Run: 
	make all install
7. If you then download this repo, you should be able to 'cd' into
    any of its folders, enter 'make' to build the patternlet, and
    then enter './run' to run the patternlet.

See the following for more info about installing OpenMPI:

	https://www.open-mpi.org/faq/?category=java

and the 'java' folder at the following site for the Java classes' details:

	https://github.com/open-mpi/ompi/tree/master/ompi/mpi/java


The OpenMPI implementation of the MPI standard provides a (non-standard) API
for MPI programming using Java.

To use it, OpenMPI must be installed from source, not via a binary package.

1. Make sure both gcc and Java SDK (javac) are installed on your system
2. Download the OpenMPI tarball from openmpi.org
3. Extract current version of OpenMPI from the tarball and 'cd' into the directory.
4. Run: 
	./configure --prefix=/usr/local  --enable-mpi-java
5. Run: 
	make all install

See below for more info:
	https://www.open-mpi.org/faq/?category=java


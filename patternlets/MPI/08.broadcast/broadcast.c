/* broadcast.c
 * ... illustrates the use of MPI_Bcast()...
 * Joel Adams, Calvin College, November 2009.
 *
 * Usage: mpirun -np N ./broadcast
 *
 * Exercise:
 * - Compile and run, using 2, 4, and 8 processes
 * - Use source code to trace execution and output
 * - Explain behavior/effect of MPI_Bcast().
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

/* fill an array with values 
 */
void fill(int* a, int size) {
	int i;
	for (i = 0; i < size; i++) {
		a[i] = i+11;
	}
}

/* display a string, a process id, and its array values 
 */
void print(char* str, int id, int* a) {
	printf("process %d %s: {%d, %d, %d, %d, %d, %d, %d, %d}\n",
	   id, str, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);
}

#define MAX 8

int main(int argc, char** argv) {
	int array[MAX] = {0};
        int numProcs, myRank;

	MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
        MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	if (myRank == 0) fill(array, MAX);
     
	print("array before", myRank, array);

        MPI_Bcast(array, MAX, MPI_INT, 0, MPI_COMM_WORLD);

	print("array after", myRank, array);

 	MPI_Finalize();

	return 0;
}


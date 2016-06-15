/* broadcast2.c
 * ... illustrates the use of MPI_Bcast() for arrays...
 * Joel Adams, Calvin College, November 2009.
 *
 * Usage: mpirun -np N ./broadcast2
 *
 * Exercise:
 * - Compile and run, using 2, 4, and 8 processes
 * - Use source code to trace execution and output
 * - Explain behavior/effect of MPI_Bcast().
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

/* fill an array with some arbitrary values 
 * @param: a, an int*.
 * @param: size, an int.
 * Precondition: a is the address of an array of ints.
 *              && size is the number of ints a can hold.
 * Postcondition: a has been filled with arbitrary values 
 *                { 11, 12, 13, ... }.
 */
void fill(int* a, int size) {
	int i;
	for (i = 0; i < size; i++) {
		a[i] = i+11;
	}
}

/* display a string, a process id, and its array values 
 * @param: str, a char*
 * @param: id, an int
 * @param: a, an int*.
 * Precondition: str points to either "BEFORE" or "AFTER"
 *              && id is the rank of this MPI process
 *              && a is the address of an 8-element int array.
 * Postcondition: str, id, and a have all been written to stdout.
 *
 * Note: We print the array's values using a single printf 
 *       instead of using a loop to print a[i],
 *       b/c a loop's multiple printfs will be interleaved;
 *       using a single printf avoids that problem.
 */
void print(char* str, int id, char* host, int* a) {
	printf("%s broadcast, process %d on host '%s' has: {%d, %d, %d, %d, %d, %d, %d, %d}\n",
	   str, id, host, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);
}

#define MAX 8

int main(int argc, char** argv) {
    int array[MAX] = {0};
    int numProcs = 0, myRank = 0, length = 0;
    char myHostName[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Get_processor_name (myHostName, &length);

    if (myRank == 0) { fill(array, MAX); }
     
    print("BEFORE", myRank, myHostName, array);

    MPI_Bcast(array, MAX, MPI_INT, 0, MPI_COMM_WORLD);

    print("AFTER", myRank, myHostName, array);

    MPI_Finalize();

    return 0;
}


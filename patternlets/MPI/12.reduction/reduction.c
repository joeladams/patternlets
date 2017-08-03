/* reduction.c
* ... illustrates the use of MPI_Reduce()...
* Joel Adams, Calvin College, November 2009.
*
* Usage: mpirun -np N ./reduction
*
* Exercise:
* - Compile and run, varying N: 4, 6, 8, 10.
* - Explain behavior of MPI_Reduce().
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int numProcs = -1, myRank = -1, square = -1, max = -1, sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    square = (myRank+1) * (myRank+1);

    printf("Process %d computed %d\n", myRank, square);

    MPI_Reduce(&square, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Reduce(&square, &max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (myRank == 0) {
        printf("\nThe sum of the squares is %d\n\n", sum);
        printf("The max of the squares is %d\n\n", max);
    }

    MPI_Finalize();

    return 0;
}

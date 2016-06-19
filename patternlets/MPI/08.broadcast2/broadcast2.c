/* broadcast2.c
 * ... illustrates the use of MPI_Bcast() for arrays...
 * Joel Adams, Calvin College, November 2009.
 * Steven R. Vegdahl, University of Portland, 2016
 *   (update to print function)
 *
 * Usage: mpirun -np N ./broadcast2
 *
 * Exercise:
 * - Compile and run, using 2, 4, and 8 processes 
 * - Use source code to trace execution and output
 * - Explain behavior/effect of MPI_Bcast().
 */

#include <mpi.h>        // MPI functions
#include <stdio.h>      // printf, sprintf, ...
#include <limits.h>     // CHAR_BIT

#define MAX 8

void fill(int* a, int size);
void print(char* str, int id, int* a, int size);


int main(int argc, char** argv) {
  int array[MAX] = {0};
  int numProcs, myRank;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  
  if (myRank == 0) fill(array, MAX);
  
  print("BEFORE", myRank, array, MAX);
  
  MPI_Bcast(array, MAX, MPI_INT, 0, MPI_COMM_WORLD);
  
  print("AFTER", myRank, array, MAX);
  
  MPI_Finalize();
  
  return 0;
}


/* fill an array with some values
 * @param: a, an int*.
 * @param: size, an int.
 *
 * Precondition: a is the address of an array of ints.
 *  && size is the number of ints a can hold.
 *
 * Postcondition: a has been filled with values
 * { 11, 12, 13, ... }.
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
 * @param: a, an int*
 * @param: size, an int.
 *
 * Precondition: str points to either "BEFORE" or "AFTER"
 *  && id is the rank of this MPI process
 *  && a is the address of a MAX-element int array.
 *
 * Postcondition: str, id, and a have all been written to stdout.
 */

#define MAX_INT_PRINT_SIZE ((CHAR_BIT*sizeof(int))+1)

void print(char* str, int id, int* a, int size) {

  // sprint-buffer, including space for separators and label-strings
  // assumes that 'str' length is small (e.g., < 10)
  char buffer[(size+1)*(MAX_INT_PRINT_SIZE+2)+100];

  // internal buffer pointer 
  char *spot = buffer;
  
  // sprint the characters before the elements, updating position
  spot += sprintf(spot, "%s broadcast, process %d has: {", str, id);
  
  // separator string--set to empty string to avoid putting one
  // before the first element
  char *sep = "";
  
  // sprint each element, bumping internal buffer pointer
  for (int i = 0; i < size; i++) {
    spot += sprintf(spot, "%s%d", sep, a[i]);
    sep = ", ";
  }
  
  // sprint the stuff after the elements
  spot += sprintf(spot, "}\n");
  
  // actually print out the string to stdout
  printf("%s", buffer);
}


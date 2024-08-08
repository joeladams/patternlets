/* dataRace.c is a patternlet for exploring data races.
 * The program contains *two* race conditions; can you identify both?
 *
 * @author: Joel Adams, Calvin University.
 *
 * Usage: ./dataRace numThreads
 *
 * Exercise: 
 *  1. Run with different numbers of threads and examine the output.
 *  2. Uncomment the line marked A below.
 *  3. Re-run with different numbers of threads; how is the ouput different?
 *  4. Def: Thread i 'wins' the race if sharedVar == i at the end. 
 *  5. Run 20 times with 8 threads and tally how many times each thread wins.
 *  6. Discuss this question with your neighbor:
 *      Which thread 'wins' a data race and why?
 *  7. Why did uncommenting the line marked A make a difference?
 *      In what other way are the threads 'racing' besides changing sharedVar?
 */

#include <stdio.h>     // printf()
#include <stdlib.h>    // atoi(), exit()
#include <omp.h>       // openmp

int getCommandLineArgs(int argc, char** argv);

int main(int argc, char** argv) {
   int numThreads = getCommandLineArgs(argc, argv);
   omp_set_num_threads(numThreads);

   printf("\nRunning with %d threads; let's have a race!\n", numThreads);

   int sharedVar = -1;
   #pragma omp parallel
   {
      int id = omp_get_thread_num();
//      #pragma omp critical             // A
      {
        sharedVar = id;
        printf("-Thread %d set sharedVar to %d...\n", id, sharedVar);
      }
   }
   
   printf("\nRace is over; thread %d won!\n\n", sharedVar);

   return 0;
}

int getCommandLineArgs(int argc, char** argv) {
   int numThreads = 1;
   if (argc > 1) {
      numThreads = atoi(argv[1]);
   }

   if (numThreads < 2) {
      printf("\nUsage: ./dataRace N\n\n\tPlease run with N > 1 threads.\n\n");
      exit(1);
   }

   return numThreads;
}


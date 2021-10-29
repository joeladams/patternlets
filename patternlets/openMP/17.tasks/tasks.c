/* tasks.c
 * ... illustrates the use of OpenMP's parallel task directives,
 *      which can be used for task parallelism...
 *
 * Joel Adams, Calvin University, Nov 2021.
 *
 * Usage: ./tasks
 *   or:  ./run.sh
 *
 * Exercise: 
 *  Discussing each step with your neighbor...
 *  1. Predict what the program will do when run.
 *  2. Build, run ./run.sh 5 times; were you correct?
 *  3. Uncomment the line marked A
 *  4. Predict what the program will do when run.
 *  5. Build, run ./run.sh 5 times; were you correct?
 *  6. Uncomment the line marked B
 *  7. Predict what the program will do when run.
 *  8. Build, run ./run.sh 5 times; were you correct?
 *  9. Uncomment the lines marked C.1 and C.2
 *  10. Predict what the program will do when run.
 *  11. Build, run ./run.sh 5 times; were you correct?
 *  12. Uncomment the line marked D
 *  13. Predict what the program will do when run.
 *  14. Build, run ./run.sh 5 times; were you correct?
 *  15. Uncomment the line marked E
 *  16. Predict what the program will do when run.
 *  17. Build, run ./run.sh 5 times; were you correct?
 *  18. Describe: What do the omp task and taskwait pragmas do?
 */

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>    // atoi()

int main(int argc, char** argv) {

//    #pragma omp parallel num_threads(2)    // A
    {
//        #pragma omp single                 // B
        {
            printf("Please " );

//            #pragma omp task               // C.1
            printf("list" );

//            #pragma omp task               // C.2
            printf("en" ); 

//            #pragma omp taskwait           // E
//            printf("!" );                  // D
        }
    }

    printf("\n" );
    return 0;
}


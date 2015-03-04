/* critical3.c
 * ... a simple case where OpenMP's critical works but atomic does not.
 *
 * Joel Adams, Calvin College, November 2009.
 *
 * Usage: ./critical3
 *
 * Exercise:
 *  - Compile, run, note resulting output is correct.
 *  - Uncomment line A, recompile, rerun, note results.
 *  - Uncomment line B, recompile, note results.
 *  - Recomment line B, uncomment line C, recompile, 
 *     rerun, note change in results. 
 */

#include<iostream>   // cout
#include<omp.h>      // openmp
using namespace std;

int main(int argc, char** argv) {
    cout << "\n";

    if (argc > 1) {
        omp_set_num_threads( atoi(argv[1]) );
    }

//    #pragma omp parallel                          // A
    {
        int id = omp_get_thread_num();
        int numThreads = omp_get_num_threads();
//        #pragma omp atomic                        // B
//        #pragma omp critical                      // C
        cout << "Hello from thread #" << id
             << " out of " << numThreads
             << " threads.\n";
    }

    cout << "\n";
}


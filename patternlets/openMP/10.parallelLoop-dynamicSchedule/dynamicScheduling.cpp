/* dynamicScheduling.cpp computes a table of factorial values,
 *  using Owen Astrachan's BigInt class to explore
 *  OpenMP's schedule() clause.
 *
 *  @author: Joel Adams, Calvin College, Dec 2015.
 *
 *  Usage: ./dynamicScheduling [numThreads]
 *
 *  Exercise:
 *  - Build and run, record sequential run time in a spreadsheet
 *  - Uncomment #pragma omp parallel for, rebuild,
 *      run using 2, 4, 6, 8, ... threads, record run times.
 *  - Uncomment schedule(dynamic), rebuild,
 *      run using 2, 4, 6, 8, ... threads, record run times.
 *  - Create a line chart plotting run times vs # of threads.
 */

#include "BigInt.h"                 // class BigInt
#include <omp.h>                    // OpenMP functions
#include <cassert>                  // assert()

/* factorial(n) computes n!
 * @param: n, an unsigned int.
 * @return: n!, a BigInt.
 */
BigInt factorial(unsigned n) {
	BigInt result = 1;              // 0! or 1!

	for (unsigned i = 2; i <= n; ++i) {
		result *= i;
	}

	return result;
}

int main(int argc, char** argv) {            // on a 2 GHz i7 CPU:
	const unsigned MAX = 512;            //  ~14 secs sequentially
//	const unsigned MAX = 800;            //  ~60 secs sequentially
	BigInt factorialTable[MAX+1];

	if (argc > 1) { omp_set_num_threads( atoi(argv[1]) ); }

	cout << "\nDepending on the speed of your computer,"
             << "\n  this program may take a while to complete,"
             << "\n  so please wait patiently...\n" << endl;

	double startTime = omp_get_wtime();
//	#pragma omp parallel for // schedule(dynamic)
	for (unsigned i = 0; i <= MAX; i++) {
		factorialTable[i] = factorial(i);
	}
	double totalTime = omp_get_wtime() - startTime;

	cout << "Computing 0! .. " << MAX << "! took: " 
             << totalTime << " secs\n" << endl;

	// run a few tests to validate the results
	assert( factorialTable[0] == 1 );
	assert( factorialTable[1] == 1 );
	assert( factorialTable[2] == 2 );
	assert( factorialTable[3] == 6 );
	assert( factorialTable[4] == 24 );
	assert( factorialTable[5] == 120 );
	assert( factorialTable[32] == BigInt("263130836933693530167218012160000000") );
	assert( factorialTable[100] == BigInt( string("9332621544394415268169923885")
			                            + "6266700490715968264381621468"
						    + "5929638952175999932299156089"
						    + "4146397615651828625369792082"
						    + "7223758251185210916864000000"
						    + "000000000000000000" ) );
	cout << "All tests passed!\n" << endl;
}


/* reduction2.cpp computes a table of factorial values,
 *  using Owen Astrachan's BigInt class to explore
 *  OpenMP's user-defined reductions.
 *
 *  Joel Adams, Calvin College, December 2015.
 *
 *  Usage: ./reduction2 [numThreads] [n]
 *
 *  Exercise:
 *  - Build and run, record sequential time in a spreadsheet
 *  - Uncomment #pragma omp parallel for directive, rebuild,
 *     and read the error message carefully.
 *  - Uncomment the #pragma omp declare directive, rebuild,
 *     and note the user-defined * reduction for a BigInt.
 *  - Rerun, using 2, 4, 6, 8, ... threads, recording
 *     the times in the spreadsheet.
 *  - Create a chart that plots the times vs the # of threads.
 *  - Experiment with different n values
 */

#include "BigInt.h"
#include <cassert>
#include <omp.h>

/*
#pragma omp declare reduction(*: BigInt: \
				omp_out = omp_out * omp_in) \
				initializer( omp_priv=BigInt(1))
*/

BigInt factorial(unsigned n) {
	BigInt result = 1;

//	#pragma omp parallel for reduction(*:result) 
	for (unsigned i = 2; i <= n; i += 1) {
		result *= i;
	}

	return result;
}

int main(int argc, char** argv) {  // on a 2GHz i7 CPU:
	unsigned n = 4096;         // ~10 secs sequentially
	unsigned numThreads = 1;

	switch (argc) {
		case 3: n = atoi(argv[2]);
		case 2: numThreads = atoi(argv[1]);
		case 1: break;
		default: cout << "\nUsage: ./reduction2 [numThreads] [n]\n\n";
	}
        omp_set_num_threads(numThreads);

	double startTime = omp_get_wtime();
	BigInt nFactorial = factorial(n);
	double time = omp_get_wtime() - startTime;

	cout << "Computing " << n << "! using "
             << numThreads  << " threads took: " 
             << time << " secs" << endl;

	// run a few tests to validate the results
	assert( factorial(0) == 1 );
	assert( factorial(1) == 1 );
	assert( factorial(2) == 2 );
	assert( factorial(3) == 6 );
	assert( factorial(4) == 24 );
	assert( factorial(5) == 120 );
	assert( factorial(32) == BigInt("263130836933693530167218012160000000") );
	assert( factorial(100) == BigInt( string("9332621544394415268169923885")
			                            + "6266700490715968264381621468"
						    + "5929638952175999932299156089"
						    + "4146397615651828625369792082"
						    + "7223758251185210916864000000"
						    + "000000000000000000" ) );
	cout << "All tests passed!\n" << flush;
}

// forkjoin.cc
//    Illustrates the fork-join pattern using
//    the cilk spawn directive and cilk sync
//    to join.
//    Also demonstrates issues using cout and << in workers.
//
//  Libby Shoop, Macalester College
//
//  Usage: ./forkJoin0
//
// Exercise:
//  starting configuration:
//   - make sure line marked B is uncommented and line marked C is commented.
//   - make sure line marked A is commented
//  Try:
//   - compile and then run 10 or so times and obseerve the output.
//   - uncomment line marked A. run several times and observe.
//   - comment line marked B and uncomment C.
//        compile and then run many times again; observe occasional poor output

// Pricipal points:
//    1. order of completoin of workers spawned is indeterminite
//    2. cilk_sync ensures all workers complete before main thread continues
//    3. error conditions with use of std::cout are indeterminite

#include <cilk/cilk.h>  // cilkPlus
#include <cilk/cilk_api.h>  // __cilkrts_  functions
#include <iostream>     // cout printing
#include <string>       // c++11 to_string
#include <stdio.h>      // printf
using std::cout;
using std::endl;
using std::string;

// void announce(int w);   // to display message from each worker
// int processArguments(int argc, char *argv[]);  // command line
// void f();  // real work to do

// the function f() does some time-consuming work
void f()
{
    volatile double d = 0;
    for(int n=0; n<20000; ++n)
       for(int m=0; m<10000; ++m)
           d += d*n*m;
}
// simple announcement by each worker
void announce(int w) {
  printf("%s%d%s\n", "Worker ", __cilkrts_get_worker_number(), " working..." );    // B
  f();
  // printf("%s%lu%s\n", "Worker rank ", __cilkrts_get_pedigree().rank, "..." );    // B

  // cout << "Worker " << __cilkrts_get_worker_number() << ".......\n";    // C
}


int main(int argc, char** argv) {
  int numWorkers = __cilkrts_get_nworkers();

  cout << "There are " << numWorkers << " workers by default.\n";

  // cout << "Before... Part 1" << endl;

  int status = __cilkrts_set_param("nworkers", "2");
      if (0 != status) {
          // Failed to set the number of Cilk workers
          // return status;
          cout << "failed to set the number of workers" << endl;
      }
  // Set the number of workers to be used
  // __cilkrts_set_param("nworkers", "3");
  int nwt = __cilkrts_get_nworkers();
  std::cout << "Number of Worker Threads = "
        << nwt << '\n';

  int totalWork = 8;

  for (int i = 0; i<totalWork; i++) {
    cilk_spawn announce(i);
  }

  cilk_sync;       // A

  cout << "After... Part 1" << endl;


  // cout << "Before... Part 2" << endl;
  //
  // //__cilkrts_end_cilk();    // must end cilk 'runtime' first
  // // Set the number of workers to be used
  // __cilkrts_set_param("nworkers", "2");
  // nwt = __cilkrts_get_nworkers();
  // std::cout << "Number of Worker Threads = "
  //     << nwt << '\n';
  //
  // for (int i = 0; i<totalWork; i++) {
  //   cilk_spawn announce(i);
  // }
  //
  // cilk_sync;       // A
  // cout << "After... Part 2" << endl;


  return 0;
}


// // Use the first argument to set the number of workers in the pool,
// // as long as it is positive.
// // If not positive, return 1 as a signal to stop the program.
// // If no argument, the default number of workers is used.
// //
// int processArguments(int argc, char *argv[]) {
//   // If we've got a parameter, assume it's the number of workers to be used
//   if (argc > 1)
//   {
//       // Values less than 1, or parameters that aren't numbers aren't allowed
//       if (atoi(argv[1]) < 1)
//       {
//           printf("Usage: forkJoin1 [workers]\n");
//           return 1;
//       }
//
//       // Set the number of workers to be used
//       __cilkrts_set_param("nworkers", argv[1]);
//   }
//   return 0; // all is well
//
// }

// forkjoin.cc
//    Illustrates the fork-join pattern using
//    the cilk spawn directive and cilk sync
//    to join.
//    Also demonstrates issues using cout and << in workers.
//
//  Libby Shoop, Macalester College
//
//  Usage: ./forkJoin1
//
// Exercise:
//  Try:
//   - Compile and then run 10 or so times and obseerve the output.
//   - Note the total number of threads assigned to the runtime system (RTS)
//         on your machine.
//   - Change the number of threads in the line marked A, varying between 3
//         and 8, if that is less than that total number available.
//         Compile each time and run and observe output.

// Pricipal points:
//    1. order of completoin of workers spawned is indeterminite
//    2. cilk_sync ensures all workers complete before main thread continues
//    3. you can vary the number of threads used by your program, which will
//       often shorten the amount of time to complete the work.

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

// simple announcement by each worker as they start the task
void announceAndWork(int task) {
  printf("%s%d%s%d%s\n",
      "Worker ", __cilkrts_get_worker_number(),
      " working on task ", task, "..." );
  f();  // work
}

// Arbitrarily start several tasks for the workers
void doInParallel() {
  int totalWork = 8;

  // note that this loop is for convenience of starting multiple
  // tasks on the workers
  for (int i = 0; i<totalWork; i++) {
    cilk_spawn announceAndWork(i);
  }

  cilk_sync;
}


int main(int argc, char** argv) {
  int numWorkers = __cilkrts_get_nworkers();

  cout << "There are " << numWorkers << " workers by default.\n";

  int totalWorkers = __cilkrts_get_total_workers();
  cout << "The total number of threads assigned to the RTS is "
       << totalWorkers << endl;

  cout << "Before..." << endl;

  // Set the number of workers to be used
  int status = __cilkrts_set_param("nworkers", "8");   // A
  // Note: the above can fail on some systems, so we should always check
  //        it as in the following:
  if (0 != status) {
      // Failed to set the number of Cilk workers
      // just stop if that happens
      return status;
      cout << "failed to set the number of workers" << endl;
  }

  // __cilkrts_set_param("nworkers", "3");
  int nwt = __cilkrts_get_nworkers();
  std::cout << "Number of Worker Threads = "
        << nwt << endl;

  doInParallel();

  cout << "After... Done." << endl;

  // In cilk, once the numbere of workers is set for a program,
  // you cannot change it.


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

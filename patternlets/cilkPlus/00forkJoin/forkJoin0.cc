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
#include <iostream>     // cout printing
#include <string>       // c++11 to_string
#include <stdio.h>      // printf
using std::cout;
using std::endl;

void announce(int w);   // to display message from each worker

int main(int argc, char** argv) {
  int numWorkers = 8;

  cout << "Before..." << endl;

  for (int i = 0; i<numWorkers; i++) {
    cilk_spawn announce(i);
  }

  // cilk_sync;       // A
  cout << "After..." << endl;

  return 0;
}

// simple announcement by each worker
void announce(int w) {
  printf("%s%d%s\n", "Worker ", w, "..." );    // B
  // cout << "Worker " << w << ".......\n";    // C
}

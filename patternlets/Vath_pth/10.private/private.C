
/* private.C
 *
 * Using Victor Alessandrini's vath_pth library.
 *  ... illustrates why private variables are needed in parallel for loop
 *
 * Modeled from code provided by Joel Adams, Calvin College, November 2009.
 * Hannah Sonsalla, Macalester College, 2017.
 *
 * Usage: ./private.C
 *
 * Exercise:
 * - Run, noting that the sequential program produces correct results
 * - Comment out line A and uncomment section B in Main(), recompile/run and compare,
 *   program produces incorrect results
 * - Comment out line C and uncomment section D, recompile/run and compare,
 *   program produces correct results
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <SPool.h>
#include <pthread.h>

using namespace std;
SPool TH(4);
#define SIZE 100

int m[SIZE][SIZE];
int beg = 0, end = SIZE;     // Line C


void thread_fct(void *idp)  {

    /*
    int beg, end;
    beg = 0;               // Section D
    end = SIZE;
    */

    TH.ThreadRange(beg, end);

    for(int i=beg; i<end; i++)  {
        for(int j=0; j<SIZE; j++) {
            m[i][j] = 1;
        }
    }
}

void sequentialArrayFill(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            m[i][j] = 1;
        }
    }
}

int main(int argc, char **argv)  {
    int ok = 1;

    sequentialArrayFill(SIZE);       // Line A

    /*
    TH.Dispatch(thread_fct, NULL);    // Section B
    TH.WaitForIdle();
    */

    // test (without using threads)
    for(int i=0; i<SIZE; i++) {
        for(int j=0; j<SIZE; j++) {
            if ( m[i][j] != 1 ) {
                printf("Element [%d,%d] not set... \n", i, j);
                ok = 0;
            }
        }
    }

    if ( ok ) {
        printf("\nAll elements correctly set to 1\n\n");
    }

    return 0;
}

/* parallelForStripes.c is a graphical illustration of
 *  the 'slicing' version of the parallel for loop design pattern,
 *  using Argonne Labs' MPE graphics library for X11 systems.
 *
 * Summer 2013, Joel Adams, Calvin College.
 *
 * Usage: mpirun -np N ./parallelForStripes
 *        Click the mouse in the window to terminate the program.
 *        You must have an X11 server running.
 *
 * Exercise: Run the program, varying N from 1 - 32,
 *            and compare to the 'Blocks' version...
 */

#include <mpi.h>     // MPI
#include <mpe.h>     // MPE
#include <stdlib.h>  // getenv()
#include <string.h>  // strcmp()
#include <stdio.h>   // printf(), etc.

/* 
 * getDisplay() retrieves the DISPLAY environment info
 */

char* getDisplay() {
	char * display = getenv("DISPLAY");
	if ( strncmp(display, "(null)", 7) == 0 ) {
		fprintf(stderr, "\n*** Fatal: DISPLAY variable not set.\n");
		exit(1);
	}
	return display;
}


int main(int argc, char* argv[]) {
    const int  WINDOW_WIDTH = 800;
    const int  WINDOW_HEIGHT = 400;
    int        x = 0, y = 0;
    int        id = -1, numProcesses = -1;
    int        button = -1;
    MPE_XGraph canvas;
    MPE_Color *colors = NULL;
    MPE_Color  myColor = 0;

    // initialize environment, variables, etc.
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPE_Open_graphics( &canvas, MPI_COMM_WORLD, 
                         getDisplay(),
                         5, 5,
                         WINDOW_WIDTH, WINDOW_HEIGHT, 0 );
    colors = malloc( numProcesses * sizeof(MPE_Color) );
    MPE_Make_color_array(canvas, numProcesses, colors);
    myColor = colors[id];

    // each process does a block of the window's pixels
    for (y = id; y < WINDOW_HEIGHT; y += numProcesses) {
       for (x = 0; x < WINDOW_WIDTH; x++) {
          MPE_Draw_point(canvas, x, y, myColor);
          MPE_Update(canvas);  // update here to slow things down
       }
    }

    // pause until mouse-click so the program doesn't terminate
    if (id == 0) {
       printf("\nClick in the window to continue...\n");
       MPE_Get_mouse_press( canvas, &x, &y, &button );
    }

    // clean up 
    MPE_Close_graphics( &canvas);
    MPI_Finalize();
    free(colors);
    if (id == 0) printf("Program complete.\n\n"); 
    return 0;
}


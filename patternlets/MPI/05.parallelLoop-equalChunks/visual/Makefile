PROG    = parallelForBlocks

CC      = mpicc
CFLAGS  = -DMPE_GRAPHICS -Wall -ansi -pedantic -std=c99 
LFLAGS  = -L/usr/X11R6/lib -lmpe -lX11 -lm 

$(PROG): $(PROG).c
	$(CC) $(CFLAGS) $(PROG).c -o $(PROG) $(LFLAGS) 

clean:
	rm -f $(PROG) *.o *~ *# PI*


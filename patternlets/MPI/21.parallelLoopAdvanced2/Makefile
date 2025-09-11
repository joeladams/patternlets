PROG     = parallelLoopChunks
SRC      = $(PROG).c
INCLUDE  = chunks.h

CC       = mpicc
CFLAGS   = -Wall -ansi -pedantic -std=c99
LFLAGS   = -o $(PROG) -lm

$(PROG): $(SRC) $(INCLUDE)
	$(CC) $(CFLAGS) $(SRC) $(LFLAGS)

clean:
	rm -f $(PROG) a.out *.o *~ *#


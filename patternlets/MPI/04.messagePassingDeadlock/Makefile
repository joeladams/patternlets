PROG   = messagePassingDeadlock
CC     = mpicc
CFLAGS = -Wall -ansi -pedantic -std=c99 
LFLAGS = -o $(PROG) -lm

$(PROG): $(PROG).c
	$(CC) $(CFLAGS) $(PROG).c $(LFLAGS)

clean:
	rm -f $(PROG) a.out *~ *# *.o


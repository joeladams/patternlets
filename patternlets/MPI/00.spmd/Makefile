PROG = spmd
CC = mpicc
CFLAGS = -Wall
LFLAGS = -o $(PROG)

$(PROG): $(PROG).c
	$(CC) $(CFLAGS) $(PROG).c $(LFLAGS)

clean:
	rm -f $(PROG) a.out *~ *# *.o


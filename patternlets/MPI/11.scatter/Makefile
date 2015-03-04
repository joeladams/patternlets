OBJ = scatter
CC = mpicc
CFLAGS = -Wall -ansi -pedantic -std=c99 
LFLAGS = -o $(OBJ)

$(OBJ): $(OBJ).c
	$(CC) $(CFLAGS) $(OBJ).c $(LFLAGS)

clean:
	rm -f $(OBJ)


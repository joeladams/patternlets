OBJ    = critical3
SRC    = $(OBJ).cpp
CC     = g++
CFLAGS = -Wall -ansi -pedantic -std=c++0x
LFLAGS = -o $(OBJ) -fopenmp

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LFLAGS)

clean:
	rm -f $(OBJ)


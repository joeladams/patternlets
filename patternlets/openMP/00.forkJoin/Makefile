OBJ    = forkJoin

CC     = gcc
CFLAGS = -Wall -ansi -std=c99 
LFLAGS = -o $(OBJ) 
OS     = $(shell uname -s)

ifeq ($(OS), Darwin)
CFLAGS += -Xclang -fopenmp
LFLAGS += -lomp
else
CFLAGS += -pedantic
endif

all: $(OBJ)

$(OBJ): $(OBJ).c
	$(CC) $(CFLAGS) $(OBJ).c $(LFLAGS)

clean:
	rm -f a.out $(OBJ)


# Generic Makefile for building simple projects with gcc;
# just redefine PROG with the name of your project.

PROG   = forkJoin
SRC    = $(PROG).c
CC     = gcc
CFLAGS = -Wall -ansi -std=c99 
LFLAGS = -o $(PROG) 

# Additional flags for Apple gcc vs. GNU gcc
OS     = $(shell uname -s)
ifeq ($(OS), Darwin)
   CFLAGS += -Xclang -fopenmp
   LFLAGS += -lomp
else
   CFLAGS += -pedantic
   LFLAGS += -fopenmp
endif

all: $(PROG)

$(PROG): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LFLAGS)

clean:
	rm -f a.out $(PROG)


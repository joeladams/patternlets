PROG   = forkJoin
SRC    = $(PROG).c

CC     = gcc
CFLAGS = -Wall -ansi -std=c99 
LFLAGS = -o $(PROG) 
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


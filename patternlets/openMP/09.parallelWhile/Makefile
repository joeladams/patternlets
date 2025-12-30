PROG        = parallelWhile
SRC         = $(PROG).c
LIB         = chunks

CC          = gcc
CFLAGS      = -Wall -ansi -std=c99 
LFLAGS      = -o $(PROG) 

# Additional flags for Apple gcc vs. GNU gcc
OS          = $(shell uname -s)
ifeq ($(OS), Darwin)
   CFLAGS  += -Xclang -fopenmp
   LFLAGS  += -lomp
else
   CFLAGS  += -pedantic
   LFLAGS  += -fopenmp
endif

$(PROG): $(SRC) $(LIB).h
	$(CC) $(SRC) $(CFLAGS) $(LFLAGS)

clean:
	rm -f $(PROG) *.o *~ *#


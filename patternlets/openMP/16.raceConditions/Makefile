OBJ    = dataRace
SRC    = $(OBJ).c

CC     = gcc
CFLAGS = -Wall -ansi -std=c99 
LFLAGS = -o $(OBJ) 

OS     = $(shell uname -s)
ifeq ($(OS), Darwin)
CFLAGS += -Xclang -fopenmp
LFLAGS += -lomp
else
CFLAGS += -pedantic
LFLAGS += -fopenmp
endif

all: $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LFLAGS)

clean:
	rm -f a.out $(OBJ)

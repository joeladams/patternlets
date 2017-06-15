CC      = g++
CFLAGS  = -std=c++11
LDFLAGS =
FFLAGS  = -fcilkplus

all: forkJoin0 clean

forkJoin0: forkJoin0.o
	$(CC) -o $@ $^ $(LDFLAGS) $(FFLAGS)

%.o: %.cc
	$(CXX) -c $(CFLAGS) $<  $(FFLAGS)

.PHONY: clean cleanest

clean:
	rm -f *.o

cleanest: clean
	rm forkJoin0

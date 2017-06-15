CC      = g++
CFLAGS  = -std=c++11
LDFLAGS =
FFLAGS  = -fcilkplus

all: forkJoin1 forkJoin1a clean

forkJoin1: forkJoin1.o
	$(CC) -o $@ $^ $(LDFLAGS) $(FFLAGS)

forkJoin1a: forkJoin1a.o
	$(CC) -o $@ $^ $(LDFLAGS) $(FFLAGS)

%.o: %.cc
	$(CXX) -c $(CFLAGS) $<  $(FFLAGS)

.PHONY: clean cleanest

clean:
	rm -f *.o

cleanest: clean
	rm -f forkJoin1 forkjoin1a

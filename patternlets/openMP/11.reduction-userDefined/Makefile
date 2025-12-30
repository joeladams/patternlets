TARGET = reduction2
OBJS   = $(TARGET).o BigInt.o
CC     = g++
CFLAGS = -c -Wall -ansi -pedantic -std=c++0x -fopenmp
LFLAGS = -o $(TARGET) -fopenmp

# Linking
$(TARGET): $(OBJS)
	@echo Linking...
	$(CC) $(OBJS) $(LFLAGS)

# Compiling
.cpp.o:
	@echo Compiling...
	$(CC) $(CFLAGS) $< 

# Indirect dependencies (Multiple file/classes only)
$(TARGET).o: BigInt.h
BigInt.o: BigInt.h

# cleanup
clean:
	rm -f $(TARGET) *.o *~ *# 


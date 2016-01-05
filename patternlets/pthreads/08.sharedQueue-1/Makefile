TARGET = tester
OBJS   = $(TARGET).o SharedQueue.o
CC     = gcc
CFLAGS = -c -Wall -ansi -pedantic -std=c99 
LFLAGS = -o $(TARGET) -lpthread 

# Linking
$(TARGET): $(OBJS)
	@echo -e '\nLinking...'
	$(CC) $(OBJS) $(LFLAGS)


# Compiling
.c.o:
	@echo -e '\nCompiling...'
	$(CC) $(CFLAGS) $< 

# Indirect dependencies (Multiple file/classes only)
$(TARGET).o: SharedQueue.h
SharedQueue.o: SharedQueue.h

# cleanup
clean:
	rm -f $(TARGET) *.o *~ *# 


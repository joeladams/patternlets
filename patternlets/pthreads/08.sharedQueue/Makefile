TARGET = producerConsumer
OBJS   = $(TARGET).o TS_Queue.o
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
$(TARGET).o: TS_Queue.h
TS_Queue.o: TS_Queue.h

# cleanup
clean:
	rm -f $(TARGET) *.o *~ *# 


TARGET = barrier
CC     = gcc
CFLAGS = -Wall -ansi -pedantic -std=c99 
LFLAGS = -o $(TARGET) -lpthread 

$(TARGET): $(TARGET).c pthreadBarrier.h
	$(CC) $(CFLAGS) $(TARGET).c $(LFLAGS)

clean:
	rm -f $(TARGET) *.o *~ *# 


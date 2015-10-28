TARGET = forkJoin5
CC = gcc
CFLAGS = 
LFLAGS = -o $(TARGET) -lpthread 

$(TARGET): $(TARGET).c  timer.h
	$(CC) $(CFLAGS) $(TARGET).c $(LFLAGS)

clean:
	rm -f $(TARGET) *.o *~ *#




CC = gcc
OBJS = main.o fifo.o thread.o

EXEC = barber.out

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ -pthread

%.o: %.c
	$(CC) -c  $<

main.o: fifo.h thread.h
fifo.o: fifo.h thread.h
thread.o: thread.h

clean:
	rm *.o

.PHONY: clean

.PHONY:clean
CC=gcc
CFLAGS= -Wall -O2
LIBS=-lpthread
OBJ= test
OBJS = ringbuf.c main.c
$(OBJ):$(OBJS)
	$(CC) -o $@ $(CFLAGS) $^ $(LIBS)


clean:
	rm  $(OBJ) 


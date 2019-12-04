.PHONY:clean
CC=gcc
CFLAGS= -Wall -O2
LIBS=-lpthread
OBJ= test
OBJS = fifo.c msg_log.c write_log.c
$(OBJ):$(OBJS)
	$(CC) -o $@ $(CFLAGS) $^ $(LIBS)


clean:
	rm  $(OBJ) 


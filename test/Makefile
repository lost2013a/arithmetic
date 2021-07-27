.PHONY:clean
CC=gcc
CFLAGS= -Wall -O2
LIBS=-lpthread -lm
OBJ= test
OBJS = sort1.c
$(OBJ):$(OBJS)
	$(CC) -o $@ $(CFLAGS) $^ $(LIBS)


clean:
	rm  $(OBJ) 


.PHONY:clean
CC=gcc
CFLAGS= -Wall -O2
LIBS=-lpthread -lm
OBJ= test
OBJS = 08_xsd.c
$(OBJ):$(OBJS)
	$(CC) -o $@ $(CFLAGS) $^ $(LIBS)


clean:
	rm  $(OBJ) 


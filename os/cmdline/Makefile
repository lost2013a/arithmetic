.PHONY:clean
OBJ = test
CFLAGS= -Wall -static
LIBS=-lpthread -lm
OBJS = nstar_cmd.c
$(OBJ):$(OBJS)
	$(CC) -o $@ $(CFLAGS) $^ $(LIBS)

clean:
	rm  $(OBJ) 

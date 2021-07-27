# Makefile

EXE=file_wr
OBJ=file_wr.o
SRC=file_wr.c

CC= gcc

CFLAGS= -g
LDFLAGS=  

EXE:$(OBJ)
	$(CC) $(LDFLAGS) -o $(EXE) $^

OBJ:$(SRC)
	$(CC) $(CFLAGS) -o $(OBJ) -c $^

#%.o::%.c
#	$(CC) -c $(CFLAGS) $< -o $@

.PHONY:clean
clean:
	-$(RM) $(OBJ) $(EXE)

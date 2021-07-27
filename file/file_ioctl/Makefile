# Makefile file_ioctl

EXE=file_ioctl
OBJ=file_ioctl.o
SRC=file_ioctl.c

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

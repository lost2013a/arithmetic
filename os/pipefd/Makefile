#!/bin/bash


SRC1 = read.c
SRC2 = write.c
OBJ1 = $(SRC1:.c= )
OBJ2 = $(SRC2:.c= )

all:$(OBJ1) $(OBJ2)

$(OBJ1):$(SRC1)
	$(CC) -o $@ $(CFLAGS) $^ $(LIBS)
$(OBJ2):$(SRC2)
	$(CC) -o $@ $(CFLAGS) $^ $(LIBS)	
	

.phony += clean
clean :
	rm -rf $(OBJ1) $(OBJ2)
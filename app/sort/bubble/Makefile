
BIN=b_main

SRC=$(wildcard *.c)
INC=.

OBJ=$(patsubst %.c,%.o,$(SRC))
CFLAGS=-O0 -g
%.o: %.c
	gcc -Wall -o $@ -c $^ -I$(INC)

$(BIN):$(OBJ)
	gcc -o $@ $^ $(CFLAGS)
clean:
	rm -rf $(BIN) $(OBJ)
all:$(BIN) 

.PHYON:$(BIN) clean all

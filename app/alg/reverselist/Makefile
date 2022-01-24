
BIN=t_revers

SRC=$(wildcard *.c)
INC=../lib/single_list
LIB=../lib/single_list

OBJ=$(patsubst %.c,%.o,$(SRC))
CFLAGS=-O0 -g
%.o: %.c
	gcc -Wall -o $@ -c $^ -I$(INC) $(CFLAGS)

$(BIN):$(OBJ)
	gcc -o $@ $^ -L$(LIB) -lsingle_list  $(CFLAGS) 

clean:
	rm -rf $(BIN) $(OBJ)
all:$(BIN) 

.PHYON:$(BIN) clean all

#gcc -o $@ $^ -L$(LIB) -lsingle_list -static $(CFLAGS) 

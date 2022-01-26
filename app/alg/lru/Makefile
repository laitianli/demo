
BIN=t_lru

SRC=$(wildcard *.c)
INC=../lib/double_list
LIB=../lib/double_list

OBJ=$(patsubst %.c,%.o,$(SRC))
CFLAGS=-O0 -g
%.o: %.c
	gcc -Wall -o $@ -c $^ -I$(INC) $(CFLAGS)

$(BIN):$(OBJ)
	gcc -o $@ $^ -L$(LIB) -ldouble_list  $(CFLAGS) 

clean:
	rm -rf $(BIN) $(OBJ)
all:$(BIN) 

.PHYON:$(BIN) clean all

#gcc -o $@ $^ -L$(LIB) -ldouble_list -static $(CFLAGS) 

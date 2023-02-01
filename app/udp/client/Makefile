
BIN=t_c_udp

SRC=$(wildcard *.c)
INC=.
LIB_DIR=.

#LIB=-lsctp
LIB=
OBJ=$(patsubst %.c,%.o,$(SRC))
CFLAGS=-O0 -g
%.o: %.c
	gcc -Wall -o $@ -c $^ -I$(INC) $(CFLAGS)

$(BIN):$(OBJ)
	gcc -o $@ $^ -L$(LIB_DIR) $(LIB)  $(CFLAGS) 

clean:
	rm -rf $(BIN) $(OBJ)
all:$(BIN) 

.PHYON:$(BIN) clean all

#gcc -o $@ $^ -L$(LIB_DIR) -ldouble_list -static $(CFLAGS) 

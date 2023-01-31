#GCC:=gcc
GCC:=aarch64-linux-gnu-gcc
BIN=t_unligned

SRC=$(wildcard *.c)
#INC=../lib/double_list
#LIB_DIR=../lib/double_list
LIB=
ASAN_FLAG=-fsanitize=address -lasan 
TSAN_FLAG=-fsanitize=thread -ltsan

OBJ=$(patsubst %.c,%.o,$(SRC))
CFLAGS=-O0 -g
%.o: %.c
	$(GCC) -Wall -o $@ -c $^ -I$(INC) $(CFLAGS)

$(BIN):$(OBJ)
	$(GCC) -o $@ $^ -L$(LIB_DIR) ${LIB} $(ASAN_FLAG) $(CFLAGS) 

clean:
	rm -rf $(BIN) $(OBJ)
all:$(BIN) 

.PHYON:$(BIN) clean all

#gcc -o $@ $^ -L$(LIB_DIR) -ldouble_list -static $(CFLAGS) 

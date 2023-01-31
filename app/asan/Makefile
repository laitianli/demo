
BIN=t_asan

SRC=$(wildcard *.c)
INC=../lib/double_list
LIB=../lib/double_list

OBJ=$(patsubst %.c,%.o,$(SRC))
CFLAGS=-O0 -g
CFLAGS +=-fsanitize=address -fno-stack-protector -fno-var-tracking -fno-omit-frame-pointer
#CFLAGS += -fsanitize=address -fno-omit-frame-pointer -fno-stack-protector

#set(CMAKE_C_FLAGS	" -m64 -Wno-write-strings -fgnu89-inline -fexceptions  -fno-inline -rdynamic -fsanitize=address -fsanitize-recover=all  -fsanitize=leak")
#set(CMAKE_CXX_FLAGS	" -std=c++11 -m64 -Wno-write-strings -fexceptions  -fno-inline -rdynamic -fsanitize=address -fsanitize-recover=all  -fsanitize=leak")

#gcc -o $@ $^ -L$(LIB) -ldouble_list  $(CFLAGS) 
%.o: %.c
	gcc -Wall -o $@ -c $^ -I$(INC) $(CFLAGS) 

$(BIN):$(OBJ)
	gcc -o $@ $^  -lasan  $(CFLAGS) 

clean:
	rm -rf $(BIN) $(OBJ)
all:$(BIN) 

.PHYON:$(BIN) clean all

#gcc -o $@ $^ -L$(LIB) -ldouble_list -static $(CFLAGS) 

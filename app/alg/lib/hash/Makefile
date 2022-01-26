LIB_NAME=libhash
LIB=$(LIB_NAME).a
DLIB=$(LIB_NAME).so

SRC=$(wildcard *.c)
OBJ=$(patsubst %.c,%.o,$(SRC))
CFLAGS=-O0 -g

INC=../double_list
LIB=../double_list

all:$(LIB) $(DLIB) 

%.o: %.c
	gcc -Wall -fPIC -o $@ -c $^ -I$(INC) $(CFLAGS)

$(LIB):$(OBJ)
	ar -rcs $@ $^

$(DLIB):$(OBJ)
	gcc -fPIC -shared  $^ -o $@ -L$(LIB) -ldouble_list $(CFLAGS)

clean:
	rm -rf $(LIB) $(OBJ) $(DLIB)


.PHYON:$(LIB) $(DLIB) clean all


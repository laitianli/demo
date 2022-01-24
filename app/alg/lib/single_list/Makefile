LIB_NAME=libsingle_list
LIB=$(LIB_NAME).a
DLIB=$(LIB_NAME).so

SRC=$(wildcard *.c)
OBJ=$(patsubst %.c,%.o,$(SRC))
CFLAGS=-O0 -g
%.o: %.c
	gcc -Wall -fPIC -o $@ -c $^ -I$(INC) $(CFLAGS)

$(LIB):$(OBJ)
	ar -rcs $@ $^

$(DLIB):$(OBJ)
	gcc -fPIC -shared  $^ -o $@ $(CFLAGS)

clean:
	rm -rf $(LIB) $(OBJ) $(DLIB)
all:$(LIB) $(DLIB) 

.PHYON:$(LIB) $(DLIB) clean all



SRC_FILES=bpf_change_port.c
OBJ=bpf_change_port.o


all:
	clang -O2 -Wall -g -target bpf -c $(SRC_FILES) -o $(OBJ)

clean:
	rm -rf $(OBJ)

all:hello

hello.o:
	as -o hello.o hello.s

hello:hello.o
	ld -s -o hello hello.o

clean:
	rm -rf hello hello.o

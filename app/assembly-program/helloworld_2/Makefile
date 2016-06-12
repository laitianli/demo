all: main

main: hello_32.s hello_64.s


hello_32.s:
	gcc -S -m32 -o hello_32.s hello.c

hello_64.s:
	gcc -S -m64 -o hello_64.s hello.c

clean:
	rm -rf main *.s


all: main

main: puts_32.s puts_64.s
	gcc -o main puts.c

puts_32.s:
	gcc -S -m32 -o puts_32.s puts.c

puts_64.s:
	gcc -S -m64 -o puts_64.s puts.c

clean:
	rm -rf main *.s

all: main

main: select_sort.o
	ld -lc --dynamic-linker /lib64/ld-2.5.so -o main select_sort.o

select_sort.o:
	as --32 -o select_sort.o select_sort.s


clean:
	@rm -rf main *.o

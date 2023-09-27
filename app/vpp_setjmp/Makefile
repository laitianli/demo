all: main
 
main: main.c longjmp.S 
	gcc -g -O2 -DFORTIFY_SOURCE=2 -fstack-protector -fPIC -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast  -Wno-address-of-packed-member $^ -o $@
 
clean:
	rm main
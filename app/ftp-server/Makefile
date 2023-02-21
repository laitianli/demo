CC=gcc
CFLAGS=-g -Wall 
CLIB=-lcrypt
myftp:myftp.o ftpfun.o statser.o semphore.o hashtable.o conf.o
	$(CC) $(CFLAGS) -o $@ $^ $(CLIB)
%.o:%c
	$(CC) $(CFLAGS) -c -o $@
myftp.o:myftp.c myftp.h
ftpfun.o:ftpfun.c ftpfun.h
statser.o:statser.c statser.h
semphore.o:semphore.c semphore.h
hashtable.o:hashtable.c hashtable.h
conf.o:conf.c conf.h
clean:
	rm -rf *.o *.bak


all: c s

c:
	make -C client/ all

s:	
	make -C server/ all

clean: client_clean server_clean

client_clean:
	make -C client clean

server_clean:
	make -C server clean

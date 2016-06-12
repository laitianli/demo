CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd) 

A-objs := redirect_core.o redirect_conn.o redirect_control.o
obj-m := A.o

default: 
	make -C $(KDIR) M=$(PWD) modules 
 
clean: 
	-rm -f *.o *.ko .*.cmd .*.flags *.mod.c *.order *.markers *.symvers 
r:
	rmmod A.ko
	dmesg -c
a:
	insmod A.ko

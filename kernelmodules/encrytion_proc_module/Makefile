MODULE_NAME = encrytion-disk

ifneq ($(KERNELRELEASE),)
	obj-m = $(MODULE_NAME).o
	$(MODULE_NAME)-objs = config_encrytion_disk.o
else
	PWD=$(shell pwd)
	#KER-DIR=/home/laitianli/kernel/linux-2.6.18
	KSRC?=/home/laitianli/kernel/linux-2.6.32.9
default:
	$(MAKE) -C $(KSRC) M=$(PWD) ARCH=x86_64 CROSS_COMPILE= modules

endif

clean:
	rm -rf *.o *.ko .*.cmd *.mod.*  Module.symvers .tmp_versions modules.order	

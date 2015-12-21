MODULE_NAME=klist

_ARCH=$(ARCH)
_CROSS_COMPILE=$(CROSS_COMPILE)

ifeq ($(_ARCH),)
	_ARCH = x86_64
endif

ifeq ($(_CROSS_COMPILE),)
	_CROSS_COMPILE = 
endif

ifeq ($(KERD),)
	KERD=/home/laitianli/kernel/linux-2.6.18
endif

ifneq ($(KERNELRELEASE),)
	obj-m=$(MODULE_NAME).o
	$(MODULE_NAME)-objs=klist-demo.o
else
	PWD=$(shell pwd)
	KER-DIR=$(KERD)
default:
	$(MAKE) -C $(KER-DIR) M=$(PWD) ARCH=$(_ARCH) CROSS_COMPILE=$(_CROSS_COMPILE) modules
endif

clean:
	rm -rf *.o *.ko .*.cmd *.mod.*  Module.symvers .tmp_versions modules.order


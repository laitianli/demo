MODULE_NAME=kobject
_ARCH=$(ARCH)
_CROSS_COMPILE=$(CROSS_COMPILE)
ifeq ($(_ARCH),)
	_ARCH=x86_64
endif
ifeq ($(KERD),)
	KERD=/home/laitianli/kernel/linux-2.6.18
endif

ifneq ($(KERNELRELEASE),)
	obj-m = $(MODULE_NAME).o
	$(MODULE_NAME)-objs = kobject_demo.o
else
	#KER-DIR=/home/laitianli/kernel/linux-2.6.18
	KER-DIR=$(KERD)
	PWD=$(shell pwd)
default:
	@$(MAKE) -C $(KER-DIR) M=$(PWD) ARCH=$(_ARCH) CROSS_COMPILE=$(_CROSS_COMPILE) modules 

endif

clean:
	@rm -rf *.o *.ko .*.cmd *.mod.*  Module.symvers .tmp_versions modules.order
	@echo "clean project ok!"

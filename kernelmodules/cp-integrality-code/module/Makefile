MODULE_NAME=cp

_ARCH ?= $(ARCH)
_CROSS_COMPILE ?= $(CROSS_COMPILE)

ifeq ($(_ARCH),)
	_ARCH=mips
endif

ifeq ($(_CROSS_COMPILE),)
	_CROSS_COMPILE=mips64el-linux-
endif

ifneq ($(KERNELRELEASE),)
	obj-m = $(MODULE_NAME).o
	$(MODULE_NAME)-objs=cp-file.o cp-info-proc.o cp-task.o
else
	PWD = $(shell pwd)
	KERD ?= /home/laitianli/kernel/loongson-linux-2.6.36/
default:
	@$(MAKE) -C $(KERD) M=$(PWD) ARCH=$(_ARCH) CROSS_COMPILE=$(_CROSS_COMPILE) modules
endif

clean:
	@rm -rf *.o *.ko .*.cmd *.mod.*  Module.symvers .tmp_versions modules.order
	@echo clean OK!

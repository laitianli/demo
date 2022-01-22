
ifneq ($(KERNELRELEASE),)
MODULE_NAME=hrtimer
$(MODULE_NAME)-objs=demo_hrtimer.o
obj-m=$(MODULE_NAME).o
else
KDIR?=/lib/modules/`uname -r`/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

endif


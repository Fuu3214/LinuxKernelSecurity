ifneq ($(KERNELRELEASE),)
	obj-m := myMac.o
	myMac-objs := securityServer.o securityManager.o securityCheck.o getpath.o ibtran.o initMac.o
else
	KERNELDIR ?=/lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	rm *.mod.c *.o *.order *.symvers 
endif
.PHONY:clean
clean:
	-rm *.mod.c *.o *.order *.symvers *.ko .*
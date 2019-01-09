ifneq ($(KERNELRELEASE),)
	obj-m := SS_test_main.o
	SS_test_main-objs := securityServer.o securityManager.o ibtran.o SS_test.o
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
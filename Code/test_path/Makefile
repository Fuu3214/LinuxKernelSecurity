ifneq ($(KERNELRELEASE),)
	obj-m := test_path.o
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
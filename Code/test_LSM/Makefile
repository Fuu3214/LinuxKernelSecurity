obj-$(CONFIG_SECURITY_TEST):= test.o
subdir-$(CONFIG_SECURITY_TEST)    += test

# Must precede
capability.o in order to stack properly.
obj-$(CONFIG_SECURITY_TEST)         
+= test/test.o
obj-$(CONFIG_SECURITY_SELINUX)         
+= selinux/built-in.o
obj-$(CONFIG_SECURITY_SMACK)         
+= smack/built-in.o
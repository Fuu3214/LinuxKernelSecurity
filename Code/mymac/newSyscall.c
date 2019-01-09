#include "securityCheck.h"
/**
* mkdir 
*/
asmlinkage long (*old_mkdir)(const char __user *pathname, int mode);
asmlinkage long new_mkdir(const char __user *pathname, int mode)
{
	int flag = 0;
	//printk("%d\n",current_euid());
	flag = checkMkdir(pathname,mode);
	if(flag == 1){
		//printk("permission granted\n");
		printk("mkdir, permission granted!\n");
		return old_mkdir(pathname,mode);
	}
	else if(flag == 0){
		printk("mkdir, permission denied\n");
		return -1;
	}
	else{
		//printk("not found, assume granted\n");
		return old_mkdir(pathname,mode);
	}
}
/**
* open 
*/
asmlinkage long (*old_open) (char __user *filename, int flags, int mode);
asmlinkage long new_open(char __user *filename, int flags, int mode)
{
	int flag = 0;
	//printk("%d\n",current_euid());
	flag = checkOpen(filename,flags,mode);
	if(flag == 1){
		//printk("permission granted\n");
		printk("open, permission granted!\n");
		return old_open (filename, flags, mode);
	}
	else if(flag == 0){
		printk("open, permission denied\n");
		return -1;
		//return old_open (filename, flags, mode);
	}
	else{
		//printk("not found, assume granted\n");
		return old_open (filename, flags, mode);
	}
    
}
/**
* read 
*/
asmlinkage long (*old_read)(unsigned int fd, char __user *buf, size_t count);
asmlinkage long new_read(unsigned int fd, char __user *buf, size_t count){
	//printk("read\n");
	int flag = 0;
	//printk("%d\n",current_euid());
	flag = checkRead(fd);
	if(flag == 1){
		//printk("permission granted\n");
		printk("read, permission granted!\n");
		return old_read (fd, buf, count);
	}
	else if(flag == 0){
		printk("read, permission denied\n");
		return -1;
		//return old_read (filename, flags, mode);
	}
	else{
		//printk("not found, assume granted\n");
		return old_read (fd, buf, count);
	}
}
/**
* write 
*/
/*
asmlinkage long (*old_write)(unsigned int fd, const char __user *buf,
			  size_t count);
asmlinkage long new_write(unsigned int fd, const char __user *buf,
			  size_t count){
	//printk("write\n");
	int flag = 0;
	//printk("%d\n",current_euid());
	flag = checkWrite(fd);
	if(flag == 1){
		//printk("permission granted\n");
		printk("read, permission granted!\n");
		return old_write (fd, buf, count);
	}
	else if(flag == 0){
		printk("read, permission denied\n");
		return -1;
		//return old_read (filename, flags, mode);
	}
	else{
		//printk("not found, assume granted\n");
		return old_write (fd, buf, count);
	}		  
}
*/

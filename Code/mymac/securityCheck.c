#include <linux/kernel.h>  
#include <linux/cred.h>

#include <linux/sched.h> 
#include <linux/mm.h> 
#include <linux/fdtable.h> 
#include <linux/types.h>
#include <linux/stat.h> 
#include <linux/unistd.h>
#include <linux/dcache.h>
#include <linux/fs_struct.h>  

#include <linux/file.h>
#include <linux/limits.h>
#include <linux/fcntl.h>
#include <linux/uaccess.h>

#include <linux/mount.h>
#include <linux/netdevice.h>

#include "securityServer.h"
#include "securityCheck.h"
#include "getpath.h"

/**
* mkdir 
*/
int checkMkdir(const char __user *pathname, int mode){
	int flag;
	char at_bit[AT_LEN+1];
	
	char curpath[MAX_TMPPATH];
	//char fullpath[MAX_TMPPATH];
	
	uid_t cur_uid = current_euid();	
	
	//printk("%s\n",tmp);
	get_cur_path(curpath);
	//printk("curpath: %s\n",curpath);
	//printk("full path: %s\n",fullpath);
	flag = enquireSS(cur_uid, curpath, at_bit, sizeof(at_bit));
	if(flag == 1){
		printk("checkMkdir,get from SS: %s\n",at_bit);
		
		if(at_bit[0] == '1'){
			return 1;
		}
		else if(at_bit[0] == '0'){
			return 0;
		}
	}
	return -1;
}
/**
* open 
*/
int checkOpen(char __user *filename, int flags, int mode)
{
	int flag;
	char at_bit[AT_LEN+1];
	char path[MAX_TMPPATH];
	uid_t cur_uid = current_euid();	
	char *tmp = getname(filename);
	//printk("%s",tmp);
	if(tmp[0] != '/'){
		get_cur_path(path);
		strcat(path,"/");
	}
	strcat(path,tmp);

	flag = enquireSS(cur_uid,path, at_bit, sizeof(at_bit));
	if(flag == 1){
		printk("checkOpen,get from SS: %s\n",at_bit);
		
		if(at_bit[1] == '1'){
			return 1;
		}
		else if(at_bit[1] == '0'){
			return 0;
		}
	}
	
	return -1;
}
/**
* read 
*/
int checkRead(unsigned int fd){
	int flag;
	char at_bit[AT_LEN+1];
	uid_t cur_uid = current_euid();	
	
	char path[MAX_TMPPATH];
	get_path_from_fd(path,fd);
	//printk("%s\n",ppath);
	
	flag = enquireSS(cur_uid, path, at_bit, sizeof(at_bit));
	if(flag == 1){
		printk("checkRead, get from SS: %s\n", at_bit);
		
		if(at_bit[2] == '1'){
			return 1;
		}
		else if(at_bit[2] == '0'){
			return 0;
		}
	}
	
	return -1;
}
/**
* write
*/
/*int checkWrite(unsigned int fd){
	int flag;
	char at_bit[AT_LEN+1];
	uid_t cur_uid = current_euid();	
	
	char path[MAX_TMPPATH];
	get_path_from_fd(path,fd);
	//printk("%s\n",ppath);
	
	flag = enquireSS(cur_uid, path, at_bit, sizeof(at_bit));
	if(flag == 1){
		//printk("checkWrite, get from SS: %s\n", at_bit);
		
		if(at_bit[3] == '1'){
			return 1;
		}
		else if(at_bit[3] == '0'){
			return 0;
		}
	}
	
	return -1;
}
*/	
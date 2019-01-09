#include <linux/module.h>    
#include <linux/kernel.h>    
#include <linux/init.h>    

#include <linux/sched.h>
#include <linux/cred.h>
//#include <asm/uaccess.h>

MODULE_AUTHOR("ShangDa");  
MODULE_LICENSE("GPL");  

static int uid;

static int mygetuid(void)
{
	uid = current_uid();
	return uid;
}

static int this_init(void)  
{  
    printk(KERN_ALERT "init user: %d/n", mygetuid());  
    return 0;  
}  
  
static void this_exit(void)  
{  
    printk(KERN_ALERT "exit/n");  
}  

module_init(this_init);  
module_exit(this_exit);  

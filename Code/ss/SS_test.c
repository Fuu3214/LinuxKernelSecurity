#include <linux/module.h>    
#include <linux/kernel.h>    
#include <linux/init.h>    

#include "securityServer.h"

static int __init init_procfs_example(void)    
{  
	SS_init();
    return 0;  
}    
    
    
static void __exit cleanup_procfs_example(void)    
{    
	SS_fini();
}    
    
    
module_init(init_procfs_example);    
module_exit(cleanup_procfs_example);    
    
MODULE_AUTHOR("Erik Mouw");    
MODULE_DESCRIPTION("procfs examples");    
MODULE_LICENSE("GPL");    
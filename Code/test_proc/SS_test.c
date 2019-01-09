#include "add.c"
    
static int __init init_procfs_example(void)    
{    
    return init();  
}    
    
    
static void __exit cleanup_procfs_example(void)    
{    
	return remove();
}    
    
    
module_init(init_procfs_example);    
module_exit(cleanup_procfs_example);    
    
MODULE_AUTHOR("Erik Mouw");    
MODULE_DESCRIPTION("procfs examples");    
MODULE_LICENSE("GPL");    
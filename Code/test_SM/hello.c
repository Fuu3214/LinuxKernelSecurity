//必要的头文件  
#include <linux/module.h>  
#include <linux/kernel.h>  
#include <linux/init.h>  
#include "securityManager.c"
//模块许可证声明（必须）  
MODULE_LICENSE("Dual BSD/GPL");  
//模块加载函数（必须）  
static int hello_init(void)  
{  
	char input[25];
    printk(KERN_ALERT "Hello World enter/n"); 
	strcpy(input,"allow 1 3 R|W|O");
	setvalue(input);
    return 0;  
}  
//模块卸载函数（必须）  
static void hello_exit(void)  
{  
    printk(KERN_ALERT "Hello World exit/n");  
}  
//模块的注册  
module_init(hello_init);  
module_exit(hello_exit);  
//声明模块的作者（可选）  
MODULE_AUTHOR("ShangDa");  
//声明模块的描述（可选）  
MODULE_DESCRIPTION("This is the beginning of my kernal programming!/n");  
//声明模块的别名（可选）  
MODULE_ALIAS("A simplest example");  
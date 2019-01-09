#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

#include "securityServer.h"
#include "hackSyscall.c"

MODULE_DESCRIPTION("my MAC for COMS 552 project");
MODULE_AUTHOR("Shang Da, Sen Shen");
MODULE_LICENSE("GPL");


static int __init this_init(void)
{
	if(SS_init() == -1)
		return -1;
	else if(hack_init()<0)
		return -1;
    return 0;
}

static void __exit this_fini(void)
{
    hack_fini();
	ssleep(3);
	SS_fini();
}

module_init(this_init);
module_exit(this_fini);
#include <linux/kernel.h>   
#include <linux/module.h>   
#include <linux/moduleparam.h>  
#include <linux/unistd.h>  
#include <linux/init.h>
/* 
 * For the current (process) structure, we need
 * this to know who the current user is. 
 */
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <asm/cacheflush.h>



static  unsigned long **sys_call_table;


unsigned long* find_sys_call_table(void)
{
        struct {
                unsigned short  limit;
                unsigned int    base;
        } __attribute__ ( ( packed ) ) idtr;
 
        struct {
                unsigned short  offset_low;
                unsigned short  segment_select;
                unsigned char   reserved,   flags;
                unsigned short  offset_high;
        } __attribute__ ( ( packed ) ) * idt;
 
        unsigned long system_call = 0;        // x80中断处理程序system_call 地址
        char *call_hex = "\xff\x14\x85";        // call 指令
        char *code_ptr = NULL;
        char *p = NULL;
        unsigned long sct = 0x0;
        int i = 0;
 
        __asm__ ( "sidt %0": "=m" ( idtr ) );
        idt = ( void * ) ( idtr.base + 8 * 0x80 );
        system_call = ( idt->offset_high << 16 ) | idt->offset_low;
 
        code_ptr = (char *)system_call;
        for(i = 0;i < ( 100 - 2 ); i++) {
                if(code_ptr[i] == call_hex[0]
                                && code_ptr[i+1] == call_hex[1]
                                && code_ptr[i+2] == call_hex[2] ) {
                        p = &code_ptr[i] + 3;
                        break;
                }
        }
        if ( p ){
                sct = *(unsigned long*)p;
        }
        return (unsigned long*)sct;
}



/* FUNCTION TO DISABLE WRITE PROTECT BIT IN CPU */
static void disable_wp(void)
{
        unsigned int cr0_value;
        
        asm volatile ("movl %%cr0, %0" : "=r" (cr0_value));
        
        /* Disable WP */
        cr0_value &= ~(1 << 16);
        
        asm volatile ("movl %0, %%cr0" :: "r" (cr0_value));

}
        
/* FUNCTION TO RE-ENABLE WRITE PROTECT BIT IN CPU */
static void enable_wp(void)
{
        unsigned int cr0_value;

        asm volatile ("movl %%cr0, %0" : "=r" (cr0_value));

        /* Enable WP */
        cr0_value |= (1 << 16);

        asm volatile ("movl %0, %%cr0" :: "r" (cr0_value));

}



/* 
 * UID we want to spy on - will be filled from the
 * command line 
 */
static int uid;
module_param(uid, int, 0644);

asmlinkage int (*original_call) (const char *, int, int);


asmlinkage int our_sys_open(const char *filename, int flags, int mode)
{
    int i = 0;
    char ch;

    /* 
     * Check if this is the user we're spying on 
     */
    if (uid == current->cred->uid) {     //2.6.35 中current->cred->uid
        printk("Opened file by %d: ", uid);
        do {
            get_user(ch, filename + i);
            i++;
            printk("%c", ch);
        } while (ch != 0);
        printk("\n");
    }

    /* 
     * Call the original sys_open - otherwise, we lose
     * the ability to open files 
     */
    return original_call(filename, flags, mode);
}



/* 
 * Initialize the module - replace the system call 
 */


unsigned int cr0;

int init_module()
{
    
    sys_call_table=find_sys_call_table();
    disable_wp();
    
    original_call=sys_call_table[__NR_open];
    sys_call_table[__NR_open] = (long*)our_sys_open;
   

    printk("Spying on UID:%d\n", uid);

    return 0;
}

/* 
 * Cleanup - unregister the appropriate file from /proc 
 */
void cleanup_module()
{
    sys_call_table[__NR_open] = (long *)original_call;
    enable_wp();
}

MODULE_LICENSE("GPL");
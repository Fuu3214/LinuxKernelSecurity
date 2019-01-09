#include <linux/module.h>
#include <linux/sched.h> 
#include <linux/mm.h> 
#include <linux/fdtable.h> 
#include <linux/types.h>
#include <linux/stat.h> 
#include <linux/unistd.h>
#include <linux/dcache.h>
#include <linux/fs_struct.h>  
#include <linux/mount.h>

#define MAX_TMPPATH 512

char* get_absolute_path(char *filename)
{
	char *path=NULL, *start=NULL;
    char *fullpath=NULL;
    struct dentry *dentry = NULL;
    struct vfsmount *mnt = NULL; 
	struct path mypath;
    
    fullpath = kmalloc(MAX_TMPPATH,GFP_KERNEL);
    if(!fullpath) goto OUT; 
    memset(fullpath,0,MAX_TMPPATH);

    path = kmalloc(MAX_TMPPATH,GFP_KERNEL);
    if(!path) {
        kfree(fullpath);
        goto OUT; 
    }

    task_lock(current);
    /* Get the Process working dentry */
    dentry = current->fs->pwd.dentry; 
    /* Get the Process working vfsmount */
    mnt = current->fs->pwd.mnt;


    task_unlock(current);
	mypath.mnt = mnt;
	mypath.dentry = dentry;
	start = d_path(&mypath,path,MAX_TMPPATH);
    strcat(fullpath,start);
    strcat(fullpath,"/");
    strcat(fullpath,filename);
    printk("full path: %s\n",fullpath);
	
	kfree(path);

OUT:
    return fullpath;
}



int init_module(void)
{ 
    get_absolute_path(THIS_MODULE->name);
}

void cleanup_module(void)
{
}

MODULE_LICENSE("GPL");
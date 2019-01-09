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

#include "getpath.h"

void get_cur_path(char *curpath)
{
	char tmp_path[MAX_TMPPATH] = {0};
    char * ptr = NULL;
    struct dentry *dentry = NULL;
    struct vfsmount * mnt = NULL;

    memset(tmp_path, 0, MAX_TMPPATH);
    ptr = tmp_path + MAX_TMPPATH - 256; 
	
    task_lock(current);
    /* Get the Process working dentry */
    dentry = current->fs->pwd.dentry; 
    /* Get the Process working vfsmount */
    mnt = current->fs->pwd.mnt;
    do
    {
        /* Process the dentry */
        while(dentry && dentry->d_name.name && strcmp(dentry->d_name.name,"/"))
        {
                ptr = ptr - strlen(dentry->d_name.name);
                if(ptr <= tmp_path + strlen(dentry->d_name.name) + 1)
                {
                    break;
                }
                memcpy(ptr, dentry->d_name.name, strlen(dentry->d_name.name));
                *(--ptr) = '/';
                dentry= dentry->d_parent;
        }
        if(mnt && mnt->mnt_mountpoint && mnt->mnt_mountpoint->d_name.name && strcmp(mnt->mnt_mountpoint->d_name.name,"/"))
        {
                dentry = mnt->mnt_mountpoint;
                ptr = ptr - strlen(dentry->d_name.name);
                if(ptr <= tmp_path + strlen(dentry->d_name.name) + 1)
                {
                    break;
                }
                memcpy(ptr,dentry->d_name.name,strlen(dentry->d_name.name));
                *(--ptr) = '/';
                mnt = mnt->mnt_parent;
                dentry= dentry->d_parent;
        }
    } while( 0 !=  strcmp(mnt->mnt_mountpoint->d_name.name,"/")); /* end do */

    task_unlock(current);

    //concat the full path
    //strcat(ptr, "/");
	strcpy(curpath,ptr);
}

void get_path_from_fd(char *path, unsigned int fd){
	char tmp_path[MAX_TMPPATH] = {'\0'};
	char *tmp_ppath = tmp_path;
	struct file *myfile = NULL;
	myfile = fget(fd);
	tmp_ppath = d_path(&(myfile->f_path), tmp_ppath, MAX_TMPPATH);
	strcpy(path,tmp_ppath);
}
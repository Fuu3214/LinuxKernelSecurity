#include <linux/module.h>    
#include <linux/kernel.h>    
#include <linux/init.h>  

#include <linux/proc_fs.h>    
#include <linux/jiffies.h>    
#include <asm/uaccess.h>

#include "securityServer.h"
#include "securityManager.h"
#include "ibtran.h" 
    
#define VERSION "1.0"    
#define SS_NAME "SecurityServer"    

/*subject table*/       
static struct proc_dir_entry *ss_dir, *sbj_file, *obj_file, *at_file;  
/*extern value*/
struct sbj_t subject;
struct obj_t object;
struct at_t authority; 

/*subject*/
static int proc_read_sbj(char *page, char **start,    
                off_t off, int count,     
                int *eof, void *data)    
{    
    int len = 0;    
	int table_len;
	int i;
	char ch[20];
    struct sbj_t *sbj_table = (struct sbj_t *)data;
	memset(page, 0, sizeof(page));
	table_len = sbj_table->len;
	for(i=0; i<table_len; i++){
		sprintf(ch, "%d = %d\n",     
		  (sbj_table->sbj[i]).uid, (sbj_table->sbj[i]).type); 
		strcat(page,ch);
		printk(KERN_INFO "%s %s reading %d = '%d' \n",    
		   SS_NAME, VERSION, (sbj_table->sbj[i]).uid, (sbj_table->sbj[i]).type);
	}
	len = strlen(page);
    return len;    
}      
static int proc_write_sbj(struct file *file,    
                 const char *buffer,    
                 unsigned long count,     
                 void *data)    
{    
    int len;    
    struct sbj_t *sbj_table = (struct sbj_t *)data;     
	char c[1];
	
	len = 1;
    if(copy_from_user(c, buffer, len))    
        return -EFAULT;    
    (sbj_table->sbj[0]).type=c[0]-48;
	
	printk(KERN_INFO "%s %s writing %d \n",    
	   SS_NAME, VERSION, (sbj_table->sbj[0]).type);
    return len;    
}    

/*object*/
static int proc_read_obj(char *page, char **start,    
                off_t off, int count,     
                int *eof, void *data)    
{    
    int len = 0;    
	int table_len;
	int i;
	char ch[DATA_LEN + 20];
    struct obj_t *obj_table = (struct obj_t *)data;    
	memset(page, 0, sizeof(page));
	table_len = obj_table->len;    
	for(i=0; i<table_len; i++){
		sprintf(ch, "%s = %d\n",     
		  (obj_table->obj[i]).path, (obj_table->obj[i]).type); 
		strcat(page,ch);
		printk(KERN_INFO "%s %s reading %s = '%d' \n",    
		   SS_NAME, VERSION, (obj_table->obj[i]).path, (obj_table->obj[i]).type);
	}
	len = strlen(page);
    return len;    
}      
static int proc_write_obj(struct file *file,    
                 const char *buffer,    
                 unsigned long count,     
                 void *data)    
{    
    int len;    
    struct obj_t *obj_table = (struct obj_t *)data;  
	char c[1];
	
	len = 1;
    if(copy_from_user(c, buffer, len))    
        return -EFAULT;    
    (obj_table->obj[0]).type=c[0]-48;
	
	printk(KERN_INFO "%s %s writing %d \n",    
	   SS_NAME, VERSION, (obj_table->obj[0]).type);
    return len;    
}   
 
/*authority*/
static int proc_read_at(char *page, char **start,    
                off_t off, int count,     
                int *eof, void *data)    
{    
	int len = 0;    
	int table_len;
	int i;
	char ch[DATA_LEN + 30];  
    struct at_t *at_table = (struct at_t *)data;
	memset(page, 0, sizeof(page));
	table_len = at_table->len;  	
	for(i=0; i<table_len; i++){
		sprintf(ch, "[%d , %d] = '%d'\n",     
		  (at_table->at[i]).subject_type, (at_table->at[i]).object_type, (at_table->at[i]).authority_type); 
		strcat(page,ch);
		printk(KERN_INFO "%s %s reading [%d , %d] = '%d' \n",    
		   SS_NAME, VERSION, 
		   (at_table->at[i]).subject_type, (at_table->at[i]).object_type, (at_table->at[i]).authority_type);
	}
	len = strlen(page);
    return len;    
}      
static int proc_write_at(struct file *file,    
                 const char *buffer,    
                 unsigned long count,     
                 void *data)    
{    
    int len;    
    //struct at_t *at_table = (struct at_t *)data;    
    char input[DATA_LEN+1];

	if(count > DATA_LEN)    
        len = DATA_LEN;    
    else    
        len = count;
    
    if(copy_from_user(input, buffer, len))    
        return -EFAULT;    
    //(at_table->at[0]).object_type=c[0]-48;
	
	input[len] = '\0';
	
	
	//strcpy(input,"deny 1 3 R|R|W|O|M");
	setvalue(input);
	   
	printk(KERN_INFO "%s %s write done\n",    
	   SS_NAME, VERSION);
    return len;  	
}  

/*Enquire Security server*/
int getSbjByType(int type){
	int i;
	//printk("%d\n", subject.len);
	for(i=0; i< subject.len; i++){
		//printk("Type %d\n", subject.sbj[i].type);
		if(subject.sbj[i].type == type){
			return 1;
		}
	}
	return -1;
}
int getObjByType(int type){
	int i;
	//printk("%d\n", object.len);
	for(i=0; i< object.len; i++){
		//printk("Type %d\n", object.obj[i].type);
		if(object.obj[i].type == type){
			return 1;
		}
	}
	return -1;
}
int getSbjType(uid_t uid){
	int i;
	//printk("sbj uid %d,", uid);
	for(i=0; i< subject.len; i++){
		if(subject.sbj[i].uid == uid){
			//printk("Type %d\n", subject.sbj[i].type);
			return subject.sbj[i].type;
		}
	}
	//printk("sbj not found\n");
	return -1;
}
int getObjType(char* filename){
	int i;
	//printk("obj name %s,", filename);
	for(i=0; i< object.len; i++){
		if(strcmp(filename, object.obj[i].path) == 0){
			//printk("Type %d\n", object.obj[i].type);
			return object.obj[i].type;
		}
	}
	//printk("obj not found\n");
	return -1;
}
int enquireAT(int sbj_type,int obj_type, char* bit, int len){
	int i;
	//char bit[14];
	//printk("enquireAT %d, %d, ", sbj_type, obj_type);
	for(i=0; i< authority.len; i++){
		if(authority.at[i].subject_type == sbj_type && authority.at[i].object_type == obj_type){
			printk("enquireAT %d, %d, authority %d = ", 
				authority.at[i].subject_type, 
				authority.at[i].object_type, 
				authority.at[i].authority_type);
			myitob(authority.at[i].authority_type,bit,len);
			printk("%s\n",bit);
			return 1;
		}
	}
	//printk("AT no match\n");
	return -1;
}
int enquireSS(uid_t uid, char* filename, char* at_bit, int len){
	int sbj_type, obj_type;
	sbj_type = getSbjType(uid);
	obj_type = getObjType(filename);
	if(sbj_type == -1 || obj_type == -1)
		return -1;
	else 
		return enquireAT(sbj_type, obj_type, at_bit, len);
}

void setSSAuthen(int sbj_type,int obj_type,int authenNumber){
	int i = 0;
	for(i=0; i<authority.len; i++ ){
		if(authority.at[i].subject_type == sbj_type && authority.at[i].object_type == obj_type){
			printk("setSSAuthen %d, %d, authority %d\n", 
				authority.at[i].subject_type, 
				authority.at[i].object_type, 
				authority.at[i].authority_type);
				authority.at[i].authority_type = authenNumber;
			printk("Changed to %d, %d, authority %d\n", 
				authority.at[i].subject_type, 
				authority.at[i].object_type, 
				authority.at[i].authority_type);
		}
	}
}
void addSSAuthen(int sbj_type,int obj_type,int authenNumber){
	int i;
	authority.len = authority.len + 1;
	i = authority.len - 1;
	printk("add no.%d\n",authority.len);
	authority.at[i].subject_type = sbj_type;
	authority.at[i].object_type = obj_type;
	authority.at[i].authority_type = authenNumber;
	printk("adding %d\n",authority.at[i].authority_type);
}
    
static int this_init(void){
	int rv = 0;    
    
    /* create directory */    
    ss_dir = proc_mkdir(SS_NAME, NULL);    
    if(ss_dir == NULL) {    
        rv = -ENOMEM;    
         return rv;   
    }    
	printk(KERN_INFO "%s %s create dir\n",    
	   SS_NAME, VERSION); 
	   
    /* create AuthorityTable files using same callback   
     * functions    
     */    
	sbj_file = create_proc_entry("SubjectTable", 0777, ss_dir);    
	obj_file = create_proc_entry("ObjectTable", 0777, ss_dir);    
    at_file = create_proc_entry("AuthorityTable", 0777, ss_dir);    
    if(sbj_file == NULL || obj_file == NULL || at_file == NULL) {    
		printk("no memory!\n");
        rv = -ENOMEM;    
        remove_proc_entry("jiffies", ss_dir);  
		return rv;		
    }    
	printk(KERN_INFO "%s %s create entry\n",    
	   SS_NAME, VERSION); 
    
	/*initialize some testing value */
	//subject
	subject.len = 2;
		
	subject.sbj[0].uid = 0;
	subject.sbj[0].type = 1;
	
	subject.sbj[1].uid = 1000;
	subject.sbj[1].type = 2;
	
	sbj_file->data = &subject;
	sbj_file->read_proc = proc_read_sbj;    
    sbj_file->write_proc = proc_write_sbj;  
	
	//object
	object.len = 6;
		
	strcpy(object.obj[0].path, "/home/shang/Desktop/Code/Kernel/test_mac");
	object.obj[0].type = 1;
	
	strcpy(object.obj[1].path, "/home/shang/Desktop/Code/Kernel/test_macc");
	object.obj[1].type = 2;
	
	strcpy(object.obj[2].path, "/home/shang/Desktop/Code/Kernel");
	object.obj[2].type = 3;
	
	strcpy(object.obj[3].path, "/proc/SecurityServer/SubjectTable");
	object.obj[3].type = 0;
	
	strcpy(object.obj[4].path, "/proc/SecurityServer/ObjectTable");
	object.obj[4].type = 0;
	
	strcpy(object.obj[5].path, "/proc/SecurityServer/AuthorityTable");
	object.obj[5].type = 0;

	obj_file->data = &object;
	obj_file->read_proc = proc_read_obj;    
    obj_file->write_proc = proc_write_obj;   
	
	//authority
	authority.len = 7;
		
    authority.at[0].subject_type = 1;    
    authority.at[0].object_type = 1;    
	authority.at[0].authority_type = 6143;//1011111111111
	
	authority.at[1].subject_type = 1;    
    authority.at[1].object_type = 2;    
	authority.at[1].authority_type = 7167;//1101111111111
	
	authority.at[2].subject_type = 2;    
    authority.at[2].object_type = 1;    
	authority.at[2].authority_type = 7679;//1110111111111
	
	authority.at[3].subject_type = 2;    
    authority.at[3].object_type = 2;    
	authority.at[3].authority_type = 8191;//1111111111111
	
	authority.at[4].subject_type = 1;    
    authority.at[4].object_type = 3;    
	authority.at[4].authority_type = 4095;//0111111111111
	
	authority.at[5].subject_type = 1;    
    authority.at[5].object_type = 0;    
	authority.at[5].authority_type = 0;//0000000000000
	
	authority.at[6].subject_type = 2;    
    authority.at[6].object_type = 0;    
	authority.at[6].authority_type = 8191;//1111111111111
	
    at_file->data = &authority;    
    at_file->read_proc = proc_read_at;    
    at_file->write_proc = proc_write_at;    
           
	//printk("%d %d %d\n",subject.len, object.len,authority.len);	   
		
    /* everything OK */    
    printk(KERN_INFO "%s %s initialised\n",    
           SS_NAME, VERSION);  
		   
    return 0; 
}    
  
static void this_fini(void){
	remove_proc_entry("SubjectTable", ss_dir); 
	remove_proc_entry("ObjectTable", ss_dir); 
    remove_proc_entry("AuthorityTable", ss_dir);      
    remove_proc_entry(SS_NAME, NULL);    
    
    printk(KERN_INFO "%s %s removed\n",    
           SS_NAME, VERSION); 
}
  
int SS_init(void)    
{    	
	this_init();
}
  
void SS_fini(void)    
{    
   this_fini();
}    
    

#include <linux/kernel.h>    
#include <linux/string.h>

#include "securityManager.h"
#include "securityServer.h"
#include "ibtran.h"

int getAuthority(char *authen){
	char* const delim = "|";  
	int ret = 0;

	char *token, *cur = authen; 
	token = strsep(&cur, delim);	
	while (token) {  
		if(strcmp(token,"M") == 0){
			ret = ret|4096;	//1000000000000
			//printk("%s %d \n", token, ret);
		}
		else if(strcmp(token,"O") == 0){
			ret = ret|2048;	//0100000000000
			//printk("%s %d \n", token, ret);
		}
		else if(strcmp(token,"R") == 0){
			ret = ret|1024;	//0010000000000
			//printk("%s %d \n", token, ret);
		}
		else if(strcmp(token,"W") == 0){
			ret = ret|512;	//0001000000000
			//printk("%s %d \n", token, ret);
		}
		else{
			return -1;
		}
		token = strsep(&cur, delim);
	} 
	return ret;
}
int setvalue(char *input){
	char at_bit[AT_LEN+1];
	char opr[10];
	int sbj_type, obj_type;
	char authen[10];
	char *token, *cur = input; 
	int authenNumber = 0;
	int authenType = 0;
	int eatret;
	
	printk("%s\n",input);
	
	token = strsep(&cur," ");
	strcpy(opr,token);
	
	token = strsep(&cur," ");
	sbj_type = simple_strtol(token,(char **)NULL,10);
	
	token = strsep(&cur," ");
	obj_type = simple_strtol(token,(char **)NULL,10);
	
	token = strsep(&cur," ");
	strcpy(authen,token);
	
	authenNumber = getAuthority(authen);
	if(authenNumber == -1)
		return -1;
		
	printk("%s %d %d %s %d\n",opr,sbj_type,obj_type,authen,authenNumber);
	
	if(strcmp(opr,"allow") == 0){
		//printk("allow %d %d\n",getSbjByType(sbj_type),getObjByType(obj_type));
		if(getSbjByType(sbj_type) ==1 && getObjByType(obj_type) == 1){
			eatret = enquireAT(sbj_type,obj_type,at_bit,sizeof(at_bit));
			//printk("%s\n",at_bit);
			authenType = mybtoi(at_bit,sizeof(at_bit));
			//printk("%d\n",authenType);
			authenNumber = authenNumber|authenType;
			//printk("%d\n",authenNumber);
			if(eatret == -1){
				addSSAuthen(sbj_type,obj_type,authenNumber);
			}
			else{
			setSSAuthen(sbj_type,obj_type,authenNumber);
			}
		}
		else{
			addSSAuthen(sbj_type,obj_type,authenNumber);
		}
		return 0;
	}
	else if(strcmp(opr,"deny") == 0){
		//printk("deny %d %d\n",sbj_type,obj_type);
		if(getSbjByType(sbj_type) ==1 && getObjByType(obj_type) == 1){
			authenNumber = myneg(authenNumber,sizeof(at_bit));
			//printk("%d\n",authenNumber);
			eatret = enquireAT(sbj_type,obj_type,at_bit,sizeof(at_bit));
			if(eatret == -1){
				printk("not exist\n");
				return -1;
			}
			//printk("%s\n",at_bit);
			authenType = mybtoi(at_bit,sizeof(at_bit));
			//printk("%d\n",authenType);
			authenNumber = authenNumber&authenType;
			//printk("%d\n",authenNumber);
			setSSAuthen(sbj_type,obj_type,authenNumber);
			return 0;
		}
		else{
			printk("not exist\n");
			return -1;
		}
	}
	else{
		printk("input error\n");
		return -1;
	}
}
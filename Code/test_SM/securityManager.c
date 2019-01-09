#include <linux/string.h>
static int getAuthority(char *authen){
	char* const delim = "|";  

	char *token, *cur = authen; 
	token = strsep(&cur, delim);	
	while (token) {  
		if(strcmp(token,"M") == 0){
			printk("%s\n", token);
		}
		else if(strcmp(token,"O") == 0){
			printk("%s\n", token);
		}
		else if(strcmp(token,"R") == 0){
			printk("%s\n", token);
		}
		else if(strcmp(token,"W") == 0){
			printk("%s\n", token);
		}
		else{
			return -1;
		}
		token = strsep(&cur, delim);
	} 
	return 0;
}

static int setvalue(char *input){
	char opr[10];
	int sbj_type, obj_type;
	char authen[10];
	char *token, *cur = input; 
	int authenNumber = 0;
	
	printk("%s\n",input);
	
	token = strsep(&cur," ");
	strcpy(opr,token);
	
	token = strsep(&cur," ");
	sbj_type = simple_strtol(token,(char **)NULL,10);
	
	token = strsep(&cur," ");
	obj_type = simple_strtol(token,(char **)NULL,10);
	
	token = strsep(&cur," ");
	strcpy(authen,token);
	
	printk("%s %d %d %s\n",opr,sbj_type,obj_type,authen);
	
	if(strcmp(opr,"allow") == 0){
		printk("allow %d %d\n",sbj_type,obj_type);
		authenNumber = getAuthority(authen);
	}
	else if(strcmp(opr,"deny") == 0){
		printk("deny %d %d\n",sbj_type,obj_type);
		authenNumber = getAuthority(authen);
	}
	else{
		printk("input error\n");
		return -1;
	}
	
	if(authenNumber == -1){
		return -1;
	}
	else{
			return 0;
	}
}
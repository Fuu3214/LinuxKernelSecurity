#include <linux/string.h>
#include "ibtran.h"
void myitob(int a, char * b, int d)
{
	int i;
	//printk("\nmyitob a %d, d %d ",a, d);
	memset(b,'0',d);
	b[d-1] = '\0';
	//printk("b %s\n",b);
	for(i=1;i<=d-1;i++){
		//printk("i=%d,a=%d\n",i,a);
		b[d-i-1] = a%2+48;
		//printk("b %c\n",b[d-i]);
		a = a>>1;
	}
	//printk("%s\n",b);
}
int mybtoi(char * b, int d)
{
	int a = 0;
	int i;
	for(i=1;i<=d-1;i++){
		a = a<<1;
		a += b[i-1]-48;
	}
	return a;
}
int myneg(int a, int d){
	int i;
	char num[d];
	myitob(a,num,d);
	for(i=0;i<d;i++){
		if(num[i] == '0')
			num[i] = '1';
		else if(num[i] == '1')
			num[i] = '0';
	}
	return mybtoi(num,d);
}
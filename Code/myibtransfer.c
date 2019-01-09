#include<string.h>

void myitob(int a, char * b, int d)
{
	int i;
	for(i=1;i<=d;i++){
		b[d-i]=(a%2)+48;
		a = a>>1;
	}
}
int mybtoi(char * b, int d)
{
	int a = 0;
	int i;
	for(i=1;i<=d;i++){
		a = a<<1;
		a += b[i-1]-48;
	}
	return a;
}

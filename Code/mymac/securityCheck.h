#ifndef _SECURITYCHECK_H_
#define _SECURITYCHECK_H_

#define AT_LEN 13
#define MAX_TMPPATH 512

/**
* mkdir 
*/
extern int checkMkdir(const char __user *, int );
/**
* open 
*/
extern int checkOpen(char __user *, int , int );
/**
* read 
*/
extern int checkRead(unsigned int);
/**
* write
*/
/*static int checkWrite(unsigned int );
*/	
#endif
#ifndef _SECURITYSERVER_H_
#define _SECURITYSERVER_H_

#define TABLE_LEN 100
#define DATA_LEN 50 

typedef struct sbj_d{
	uid_t uid;
	int type;
}sbj_d;    
typedef struct sbj_t{
	sbj_d sbj[TABLE_LEN];
	int len;
}sbj_t;	

/*object table*/
typedef struct obj_d{
	char path[DATA_LEN + 1];
	int type;
}obj_d; 
typedef struct obj_t{
	obj_d obj[TABLE_LEN];
	int len;
}obj_t;

/*authority table*/
typedef struct at_d{
    int subject_type;    
    int object_type; 
	int authority_type;
}at_d; 
typedef struct at_t {    
	at_d at[TABLE_LEN];
	int len;
}at_t; 

/*check authority*/
extern int getSbjByType(int);
extern int getObjByType(int);
extern int getSbjType(uid_t );
extern int getObjType(char* );
extern int enquireAT(int ,int , char* , int );
extern int enquireSS(uid_t , char* , char* , int );
extern void setSSAuthen(int,int,int);
extern void addSSAuthen(int,int,int);

extern int SS_init(void);
extern void SS_fini(void);

#endif
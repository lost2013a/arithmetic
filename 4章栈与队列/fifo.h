#ifndef __FIFO_H
#define __FIFO_H	


#ifndef OK
#define OK 1
#define ERROR 0
#endif
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define MAXSIZE 10 /* 存储空间初始分配量 */

#define MAX_MSG_DATA_LEN 20

struct sys_time{
	unsigned short year;
	unsigned char mon;
	unsigned char day;
	unsigned char hour;
	unsigned char min;
	unsigned char sec;
};

typedef struct __comm_msg{
	struct sys_time t;
	unsigned char data[MAX_MSG_DATA_LEN];
}COMM_MSG;


typedef unsigned char Status; 
#if 1
typedef int  QElemType; 
#else
typedef COMM_MSG  QElemType; 

#endif

typedef struct
{
	unsigned int file_crc;
	int front;    	/* 头指针 */
	int rear;		/* 尾指针，若队列不空，指向队列尾元素的下一个位置 */
	QElemType data[MAXSIZE];
}SqQueue;


Status InitQueue(SqQueue *Q);
int QueueLength(SqQueue Q);
Status GetHead(SqQueue Q,QElemType *e);
Status EnQueue(SqQueue *Q,QElemType e);
Status DeQueue(SqQueue *Q,QElemType *e);

int queue_empty(void);
int queue_in(QElemType e);
int queue_out(QElemType *e);
int queue_not_full(void);
void queue_init(void);

QElemType* queue_get_tail(void);

int queue_add(QElemType e);


#endif

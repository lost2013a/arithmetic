#ifndef __FIFO_H
#define __FIFO_H	

#include "msg_log.h"

#ifndef OK
#define OK 1
#define ERROR 0
#endif
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define MAXSIZE 20 /* 存储空间初始分配量 */

#define MAX_MSG_DATA_LEN LOG_LEN

typedef struct __comm_msg{
	unsigned char msg_len;
	unsigned char *msg;
}COMM_MSG;

typedef COMM_MSG  QElemType; 

typedef struct
{

	unsigned int file_crc;
	int front;    	/* 头指针 */
	int rear;		/* 尾指针，若队列不空，指向队列尾元素的下一个位置 */
	QElemType data[MAXSIZE];
	unsigned char msgbuf[MAXSIZE*MAX_MSG_DATA_LEN];
}SqQueue;




int queue_empty(void);
int queue_in(QElemType e);
void queue_init(void);
void ext_init(unsigned char *buf, unsigned int len);

QElemType* queue_get_tail(void);



int queue_add(unsigned char *data, unsigned char len);
void queue_visit(int (*parse_fun)(unsigned char*, unsigned char));




#endif

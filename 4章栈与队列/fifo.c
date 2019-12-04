#include <stdio.h>
#include <stdlib.h>   
#include <string.h>

#include "fifo.h"   

static SqQueue SQ;

#if 1
#define fifo_dbg printf
#else
#define fifo_dbg(...)
#endif


int InitQueue(SqQueue *Q)
{
	Q->front=0;
	Q->rear=0;	
	return  OK;
}

int QueueLength(SqQueue Q)
{
	return	(Q.rear-Q.front+MAXSIZE)%MAXSIZE;
}

int QueueEmpty(SqQueue Q)
{ 
	if(Q.front==Q.rear) 
		return TRUE;
	else
		return FALSE;
}


int GetHead(SqQueue Q,QElemType *e)
{
	if(Q.front==Q.rear) /* 队列空 */
		return ERROR;
	*e=Q.data[Q.front];
	return OK;
}

int EnQueue(SqQueue *Q,QElemType e)
{
	if ((Q->rear+1)%MAXSIZE == Q->front)	/* 队列满的判断 */
		return	ERROR;
	Q->data[Q->rear]=e;						/* 将元素e赋值给队尾 */
	Q->rear=(Q->rear+1)%MAXSIZE;			/* rear指针向后移一位置， */
	return  OK;
}

int DeQueue(SqQueue *Q,QElemType *e)
{
	if (Q->front == Q->rear)				/* 队列空的判断 */
		return ERROR;
	*e=Q->data[Q->front];					/* 将队头元素赋值给e */
	Q->front=(Q->front+1)%MAXSIZE;			/* front指针向后移一位置， */
	return OK;
}

int AddQueue(SqQueue *Q,QElemType e)
{
	if ((Q->rear+1)%MAXSIZE == Q->front)	/* 队列满的判断 */
		return	ERROR;
	Q->data[Q->rear]=e;						/* 将元素e赋值给队尾 */
	Q->rear=(Q->rear+1)%MAXSIZE;			/* rear指针向后移一位置， */
	return  OK;
}




int queue_empty(void)
{
	return QueueEmpty(SQ);
}


void ext_init(unsigned char *buf, unsigned int len)
{
	int i;
	for(i=0; i< MAXSIZE; i++)
	{
		SQ.data[i].msg= &SQ.msgbuf[i*MAX_MSG_DATA_LEN];
	}

#if 1
	int num= len/LOG_LEN;
	
	printf("old log numb=%d\n", num);
	if(num && buf){
		for(i=num-1; i>=0; i--){
			queue_add(&buf[i*LOG_LEN], LOG_LEN);
		}
	}
	printf("old add ok\n");
#endif
}


void queue_init(void)
{
	InitQueue(&SQ);
}


int queue_add(unsigned char *data, unsigned char len)
{
	QElemType *p_msg;
	if ((SQ.rear+1)%MAXSIZE == SQ.front)
		SQ.front=(SQ.front+1)%MAXSIZE;	
	p_msg= &SQ.data[SQ.rear];	
	memcpy(p_msg->msg, data, len);
	p_msg->msg_len= len;
	SQ.rear=(SQ.rear+1)%MAXSIZE;
	return  OK;
}	


extern int parse_log_str(unsigned char *buf, unsigned char len);

void queue_visit(int (*parse_fun)(unsigned char*, unsigned char))
{ 
	QElemType *p_msg;
	int i,len,idx;
	len= QueueLength(SQ);
	for(i=0; i< len; i++){
		idx= (SQ.rear+MAXSIZE-1-i)%MAXSIZE;
		p_msg= &SQ.data[idx];	
		parse_fun(p_msg->msg, p_msg->msg_len);
	}
	printf("\n\n");
}







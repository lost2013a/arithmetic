#include "stdio.h"    
#include "stdlib.h"   
#include "fifo.h"   
#include "string.h"

static SqQueue SQ;

#if 1
#define fifo_dbg printf
#else
#define fifo_dbg(...)
#endif


Status InitQueue(SqQueue *Q)
{
	Q->front=0;
	Q->rear=0;	
	return  OK;
}

int QueueLength(SqQueue Q)
{
	return	(Q.rear-Q.front+MAXSIZE)%MAXSIZE;
}

Status QueueEmpty(SqQueue Q)
{ 
	if(Q.front==Q.rear) /* 闃熷垪绌虹殑鏍囧織 */
		return TRUE;
	else
		return FALSE;
}


Status GetHead(SqQueue Q,QElemType *e)
{
	if(Q.front==Q.rear) /* 队列空 */
		return ERROR;
	*e=Q.data[Q.front];
	return OK;
}

Status EnQueue(SqQueue *Q,QElemType e)
{
	if ((Q->rear+1)%MAXSIZE == Q->front)	/* 队列满的判断 */
		return	ERROR;
	Q->data[Q->rear]=e;						/* 将元素e赋值给队尾 */
	Q->rear=(Q->rear+1)%MAXSIZE;			/* rear指针向后移一位置， */
	return  OK;
}

Status DeQueue(SqQueue *Q,QElemType *e)
{
	if (Q->front == Q->rear)				/* 队列空的判断 */
		return ERROR;
	*e=Q->data[Q->front];					/* 将队头元素赋值给e */
	Q->front=(Q->front+1)%MAXSIZE;			/* front指针向后移一位置， */
	return OK;
}

Status AddQueue(SqQueue *Q,QElemType e)
{
	if ((Q->rear+1)%MAXSIZE == Q->front)	/* 队列满的判断 */
		return	ERROR;
	Q->data[Q->rear]=e;						/* 将元素e赋值给队尾 */
	Q->rear=(Q->rear+1)%MAXSIZE;			/* rear指针向后移一位置， */
	return  OK;
}


int queue_in(QElemType e)
{
	return EnQueue(&SQ, e);
}

int queue_out(QElemType *e)
{
	return DeQueue(&SQ, e);
}

int queue_empty(void)
{
	return QueueEmpty(SQ);
}

int queue_not_full(void)
{
	if ((SQ.rear+1)%MAXSIZE == SQ.front)	
		return	ERROR;
	else
		return	OK;
}




QElemType* queue_get_tail(void)
{
	if ((SQ.rear+1)%MAXSIZE == SQ.front)	
		return	NULL;
	return &SQ.data[SQ.rear];
}



void queue_init(void)
{
	InitQueue(&SQ);
}



int queue_add(QElemType e)
{
	if ((SQ.rear+1)%MAXSIZE == SQ.front)
#if 1
	SQ.front=(SQ.front+1)%MAXSIZE;	
#else
	return	ERROR;
#endif
	SQ.data[SQ.rear]=e;						
	SQ.rear=(SQ.rear+1)%MAXSIZE;
	return  OK;
}	

#if 0
int QueueTraverse(struct squeue p_sq)
{ 
	int i;
	i=p_sq.head;
	while((i+p_sq.head)!=p_sq.tail)
	{
		printf("%d \n", p_sq.data[i]);
		i=(i+1)%MAXSIZE;
	}
	printf("\n");
	return OK;
}
#endif

#if 0
void queue_visit(void)
{ 
	int i,len,idx;
	len= QueueLength(SQ);
	for(i=0; i< len; i++){
		idx= (i+ SQ.front)%MAXSIZE;
		printf("%d ", SQ.data[idx]);

	}
	printf("\n");
}
#else
void queue_visit(void)
{ 
	int i,len,idx;
	len= QueueLength(SQ);
	for(i=0; i< len; i++){
		idx= (SQ.rear+MAXSIZE-1-i)%MAXSIZE;
		printf("%d ", SQ.data[idx]);

	}
	printf("\n");
}

#endif


int main(void )
{
	int max=10;
	int i=0;
	queue_init();
	for(; i< 10; i++){
		if(ERROR == queue_add(i))
			printf("fifo is full at %d\n", i);
	}

	queue_visit();

while(1){

	sleep(2);
	for(i=0 ; i< 5; i++){
		if(ERROR == queue_add(max++))
			printf("fifo is full at %d\n", i);
	}

	queue_visit();

}

	
	return 0;
}




#include "stdio.h"    
#include "stdlib.h"    
#include "math.h"  
#include "time.h"

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define MAXSIZE 10 

typedef int QElemType; 
struct squeue
{
	QElemType data[MAXSIZE];
	int head;    	
	int tail;		
};

int InitQueue(struct squeue *p_sq)
{
	p_sq->head=0;
	p_sq->tail=0;
	return  OK;
}

int ClearQueue(struct squeue *p_sq)
{
	p_sq->head=p_sq->tail=0;
	return OK;
}

int QueueEmpty(struct squeue p_sq)
{ 
	if(p_sq.head==p_sq.tail)
		return TRUE;
	else
		return FALSE;
}

int QueueLength(struct squeue p_sq)
{
	return  (p_sq.tail-p_sq.head+MAXSIZE)%MAXSIZE;
}

int EnQueue(struct squeue *p_sq,QElemType e)
{
	if ((p_sq->tail+1)%MAXSIZE == p_sq->head)	
		return ERROR;
	p_sq->data[p_sq->tail]=e;			
	p_sq->tail=(p_sq->tail+1)%MAXSIZE;					
	return  OK;
}

int DeQueue(struct squeue *p_sq,QElemType *e)
{
	if (p_sq->head == p_sq->tail)			
		return ERROR;
	*e=p_sq->data[p_sq->head];				
	p_sq->head=(p_sq->head+1)%MAXSIZE;									
	return  OK;
}

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

int main()
{
	int i=0,l;
	QElemType d;
	struct squeue Q;
	InitQueue(&Q);
	

	do
	{
		d=i+100;
		if(d==-1)
			break;
		i++;
		EnQueue(&Q,d);
	}while(i<MAXSIZE);

	QueueTraverse(Q);

	printf("连续%d次由队头删除元素,队尾插入元素:\n",MAXSIZE);
	for(l=1;l<=MAXSIZE;l++)
	{
		DeQueue(&Q,&d);
		d=l+1000;
		EnQueue(&Q,d);
	}
	QueueTraverse(Q);
	return 0;
}


#include "stdio.h"    
#include "stdlib.h"    
#include "math.h"  
#include "time.h"

#include "u32fifo.h"

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define dbg printf
#define INIT_FLAG 0x5a5a

#define ElemType_LEN 4 

int fifo32_init(struct squeue *p_sq, unsigned short depth, void (*pfun_lock)(void), void (*pfun_unlock)(void))
{
   
    p_sq->init_flag = 0;
    p_sq->depth = depth;
	p_sq->head=0;
	p_sq->tail=0;
    p_sq->data = malloc(ElemType_LEN*depth);
    if(NULL != p_sq->data ) {
        p_sq->pfun_lock = pfun_lock;
        p_sq->pfun_unlock = pfun_unlock;
        p_sq->init_flag = INIT_FLAG;
        return  OK;
    }
	return ERROR;
}

int fifo32_relase(struct squeue *p_sq)
{
    p_sq->pfun_unlock();
    p_sq->init_flag=0;
	p_sq->head=0;
	p_sq->tail=0;
    free(p_sq->data);
    p_sq->pfun_lock();
	return  OK;
}

int fifo32_clear(struct squeue *p_sq)
{
    p_sq->pfun_unlock();
	p_sq->head=p_sq->tail=0;
    p_sq->pfun_lock();
	return OK;
}

int fifo32_isempty(struct squeue p_sq)
{ 
    int ret = FALSE;
    p_sq.pfun_unlock();
	if(p_sq.head==p_sq.tail)
		ret = TRUE;
    p_sq.pfun_lock();
    return ret;
}

int fifo32_len(struct squeue p_sq)
{
    int len;
    p_sq.pfun_unlock();
    len = (p_sq.tail-p_sq.head+p_sq.depth)%p_sq.depth;
    p_sq.pfun_lock();
	return len;
}

int fifo32_in(struct squeue *p_sq,QElemType e)
{
    if(p_sq->init_flag != INIT_FLAG) {
        printf("fifo err\r\n");
        return 0;
    }
    p_sq->pfun_unlock();
	if ((p_sq->tail+1)%p_sq->depth != p_sq->head) {
        p_sq->data[p_sq->tail]=e;
	    p_sq->tail=(p_sq->tail+1)%p_sq->depth;	
	}else {
        //printf("err\r\n");
    }
    p_sq->pfun_lock();
    printf("in msg %d\r\n", e);
	return  1;
}

int fifo32_out(struct squeue *p_sq,QElemType *e)
{
    int ret = 0;
    if(p_sq->init_flag != INIT_FLAG) {
        printf("fifo err\r\n");
        return 0;
    }
    p_sq->pfun_unlock();
	if (p_sq->head != p_sq->tail) {
        *e=p_sq->data[p_sq->head];				
	    p_sq->head=(p_sq->head+1)%p_sq->depth;	
        ret = 1;
	}else {
        ///printf("empty\r\n");
    }
	p_sq->pfun_lock();	
	return  ret;
}


int fifo32_visit(struct squeue p_sq)
{ 
	int i;
    p_sq.pfun_unlock();
	i=p_sq.head;
    printf("q len %d\n", p_sq.depth);
	while((i+p_sq.head)!=p_sq.tail)
	{
		printf("%d \n", p_sq.data[i]);
		i=(i+1)%p_sq.depth;
	}
    p_sq.pfun_lock();	
	printf("\n");
	return OK;
}

#include <pthread.h> 
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static void fifo_lock(void) {pthread_mutex_lock(&lock); }
static void fifo_unlock(void) {pthread_mutex_unlock(&lock); }

struct squeue Q;

static void pthread_read(void *arg)
{
    QElemType d;
    while(1) {
        if(0 != fifo32_out(&Q,&d)) {
            printf("get %d\n", d);
        }
        usleep(10*1000);
    }

}
static void creat_p(void)
{
    pthread_t tid;
    pthread_create(&tid, NULL, pthread_read, NULL);
}

int main()
{
    
	int i=0,l;
	QElemType d = 0;
	fifo32_init(&Q, 20, fifo_lock, fifo_unlock);
    creat_p();
    while(1) {
        fifo32_in(&Q, d++);
        
        fifo32_in(&Q, d++);
        fifo32_in(&Q, d++);
        sleep(1);
    }

}


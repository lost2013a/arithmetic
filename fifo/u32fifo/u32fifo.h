#ifndef __FIFO_H
#define __FIFO_H	


typedef int QElemType; 

struct squeue
{
    unsigned short init_flag;
    unsigned short depth;
	int head;    	
	int tail;
    void (*pfun_lock)(void);
    void (*pfun_unlock)(void);	
    QElemType *data;
};

int fifo32_init(struct squeue *p_sq, unsigned short depth, void (*pfun_lock)(void), void (*pfun_unlock)(void));

int fifo32_in(struct squeue *p_sq, QElemType e);

int fifo32_out(struct squeue *p_sq, QElemType *e);

int fifo32_relase(struct squeue *p_sq);

int fifo32_clear(struct squeue *p_sq);

int fifo32_isempty(struct squeue p_sq);

int fifo32_len(struct squeue p_sq);




#endif

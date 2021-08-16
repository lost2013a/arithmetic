#include "kfifo.h"
#include <string.h>
#include <stdio.h>

#ifdef __GNUC__
	#define smp_wmb __sync_synchronize
#else
    #define smp_wmb() 
#endif
#define min(a,b) ((a)<(b)?(a):(b))
#define is_power_of_2(x)	((x) != 0 && (((x) & ((x) - 1)) == 0))

/*
要点：
1. 从内核lib/kfifo移植，只适合单生产者，单消费者模型
2. buff空间只能是2的幂数
3. 不使用lenth，消费者只修改out，生产者只修改in。
4. in和out增加的时候，不对buf->size取余，而是在整个unsigned int空间增加。方便判断buff空和满。
5. 因为第4点，in和out转化成buf的上的idx时，需要对buf->size取余，因为buf->size是二的指数倍，可转换成
&(buf->size - 1)，调高效率。

*/

void kfifo_init(struct kfifo *fifo, void *buffer, unsigned int size)
{
    printf("kfifo init size: %d\n", size);
    if(is_power_of_2(size) != 0) {
        printf("kfifo size err\n");
        return;
    }
	fifo->buffer = buffer;
	fifo->size = size;
	kfifo_reset(fifo);
}
 
static inline void __kfifo_in_data(struct kfifo *fifo,
		const void *from, unsigned int len)
{
	unsigned int l;
    unsigned int off = __kfifo_off(fifo, fifo->in);
    unsigned int csize = fifo->csize;
	l = min(len, fifo->size - off);
    if(csize != 1) {
        l *= csize;
        len *= csize;
        off *= csize;
    }
	memcpy(fifo->buffer + off, from, l);
	/* then put the rest (if any) at the beginning of the buffer */
	memcpy(fifo->buffer, (unsigned char*)from + l, len - l);
}
 
static inline void __kfifo_out_data(struct kfifo *fifo,
		void *to, unsigned int len)
{
	unsigned int l;
	unsigned int off = __kfifo_off(fifo, fifo->out);
    unsigned int csize = fifo->csize;
	l = min(len, fifo->size - off);
    if(csize != 1) {
        l *= csize;
        len *= csize;
        off *= csize;
    }
	memcpy(to, fifo->buffer + off, l);
	memcpy((unsigned char*)to + l, fifo->buffer, len - l);
}

unsigned int kfifo_in(struct kfifo *fifo, const void *from, unsigned int len)
{
    unsigned int l = kfifo_avail(fifo);
    if (len > l)
		len = l;
	__kfifo_in_data(fifo, from, len);
    smp_wmb(); /*多核CPU防止乱序，确保in指针的修改在数据拷贝之后*/
	__kfifo_add_in(fifo, len);
	return len;
}
                
unsigned int kfifo_out(struct kfifo *fifo, void *to, unsigned int len)
{
	unsigned int l = kfifo_len(fifo);
    if (len > l)
		len = l;
	__kfifo_out_data(fifo, to, len);
    smp_wmb();
	__kfifo_add_out(fifo, len);
	return len;
}






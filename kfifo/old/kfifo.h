#ifndef _KFIFO_H
#define _KFIFO_H


struct kfifo {
	unsigned int in;	/* data is added at offset (in % size) */
	unsigned int out;	/* data is extracted from off. (out % size) */
    unsigned int size;	/* the size of the allocated buffer */
    void *buffer;	/* the buffer holding the data */
};

static inline void kfifo_reset(struct kfifo *fifo)
{
	fifo->in = fifo->out = 0;
}

static inline unsigned int kfifo_size(struct kfifo *fifo)
{
	return fifo->size;
}

static inline unsigned int kfifo_len(struct kfifo *fifo)
{
	register unsigned int out;
	out = fifo->out;
	return fifo->in - out;
}

static inline int kfifo_is_empty(struct kfifo *fifo)
{
	return fifo->in == fifo->out;
}

static inline int kfifo_is_full(struct kfifo *fifo)
{
	return kfifo_len(fifo) == kfifo_size(fifo);
}

static inline unsigned int kfifo_avail(struct kfifo *fifo)
{
	return kfifo_size(fifo) - kfifo_len(fifo);
}

static inline void __kfifo_add_out(struct kfifo *fifo, unsigned int off)
{
	fifo->out += off;
}

static inline void __kfifo_add_in(struct kfifo *fifo, unsigned int off)
{
	fifo->in += off;
}

static inline unsigned int __kfifo_off(struct kfifo *fifo, unsigned int off)
{
	return off & (fifo->size - 1);
}

/*初始化1： 推荐， 必须是2的幂树*/
#define DECLARE_KFIFO(fifo, size) \
    static unsigned char _##fifo_buf[((size < 2) || (size & (size - 1))) ? -1 : size]; \
    struct kfifo fifo = {\
        0, \
        0, \
        size, \
        _##fifo_buf, \
    } 
/*初始化2*/    
extern void kfifo_init(struct kfifo *fifo, void *buffer, unsigned int size);
extern unsigned int kfifo_in(struct kfifo *fifo, const void *from, unsigned int len);
extern unsigned int kfifo_out(struct kfifo *fifo, void *to, unsigned int len);

 
 
#endif	/* _KFIFO_H */


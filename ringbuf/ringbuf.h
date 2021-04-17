#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_


struct ring_buffer_t
{
	unsigned char  *buffer_ptr;
	unsigned short buffer_size;
	unsigned short rd_index;
	unsigned short wr_index;
};
 

extern void ring_buffer_init(struct ring_buffer_t *rb,unsigned char *pool,unsigned short size);
extern unsigned short ring_buffer_write(struct ring_buffer_t *rb,unsigned char *pdest,unsigned short rd_len);
extern unsigned short ring_buffer_read(struct ring_buffer_t *rb,unsigned char *pdest,unsigned short rd_len);
 

#endif 

#include "ringbuf.h"
#include "string.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>



//static void produce_loop(void *parm)
//{
//    

//}

//static void creat_produce(void)
//{
//	pthread_t threads;
//	int rc;
//	long t;
//	rc = pthread_create(&threads, NULL, monitor_fifo, (void *)t);
//	if (rc){
//		printf("ERROR; return code from pthread_create() is %d\n", rc);
//		exit(-1);
//	}
//}

int main(void)
{
extern void ring_buffer_init(struct ring_buffer_t *rb,unsigned char *pool,unsigned short size);
extern unsigned short ring_buffer_write(struct ring_buffer_t *rb,unsigned char *pdest,unsigned short rd_len);
extern unsigned short ring_buffer_read(struct ring_buffer_t *rb,unsigned char *pdest,unsigned short rd_len);
    static unsigned char buf[1024*10];
    struct ring_buffer_t rb;
    ring_buffer_init(&rb, buf, 1024*10);

    char tmpbuf[256];
    char rtmpbuf[2408];
    int t = 0;
    while(1) {
        snprintf(tmpbuf, 255, "t = %04d ", t++);
        ring_buffer_write(&rb, (unsigned char*)tmpbuf, strlen(tmpbuf) );
        if(t%10 == 0)
       {
            unsigned short rlen = ring_buffer_read(&rb, (unsigned char*)rtmpbuf, 2408);
            if(rlen != 0) {
                rtmpbuf[rlen] = 0;
                printf("read %d bytes : %s\n", rlen, rtmpbuf);
            }
        }
        usleep(100*1000);
    }

    return 0;
}



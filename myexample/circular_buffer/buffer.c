#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define NDEBUG 
#include <assert.h> /*注意assert.h 不具有幂等性*/

#define OS_MUTEX_CREAT()  do{}while(0)
#define OS_MUTEX_UNLOCK() do{}while(0)
#define OS_MUTEX_LOCK()   do{}while(0)

typedef struct {
    int32_t size;
    int32_t widx;
    int32_t ridx;
    uint8_t *buf;  
}cbuf;

static inline int32_t _unlock_get_len(cbuf* p) {
    int32_t ret = p->widx - p->ridx;
    if(ret < 0) {
        ret += p->size;
    }
    return ret;
}

cbuf* cbuf_init(int32_t size) {
    cbuf *p = NULL;
    if(size > 0) {
        p = (cbuf*)malloc(sizeof(cbuf));
        if(p) {
            memset(p, 0, sizeof(cbuf));
            p->size = size;
            p->buf = malloc(size);
            if(p->buf == NULL) {
                printf("mem err\n");
                p = NULL;
            }
            OS_MUTEX_CREAT();
        }  
    }
    return p;
}

int32_t cbuf_read(cbuf* p, uint8_t* buf, int32_t size) {
    int32_t rlen, buflen;
    assert(p);
    assert(buf);
    OS_MUTEX_UNLOCK();
    buflen = _unlock_get_len(p);
    if(buflen == 0) {
        OS_MUTEX_LOCK();
        return 0;
    }
    rlen = buflen > size ? size : buflen;
    if(rlen <= p->size - p->ridx) {
        memcpy(buf, p->buf + p->ridx, rlen);
    }else {
        int t0 = p->size - p->ridx;
        int t1 = rlen - t0;
        memcpy(buf, p->buf + p->ridx, t0);
        memcpy(buf + t0, p->buf, t1);
    }
    p->ridx = (p->ridx + rlen)%p->size;
    OS_MUTEX_LOCK();
    printf("read ok %d\n", rlen);
    return rlen;
}

int32_t cbuf_write(cbuf* p, uint8_t* buf, int32_t size) {
    int32_t buflen;
    assert(p);
    assert(buf);
    OS_MUTEX_UNLOCK();
    buflen = p->size - _unlock_get_len(p);
    buflen-=1;
    if(buflen < size) {
        OS_MUTEX_LOCK();
        return 0;
    }
    if(size <= p->size - p->widx) {
        memcpy(p->buf + p->widx, buf, size);
    }else {
        int t0 = p->size - p->widx;
        int t1 = size -t0;
        memcpy(p->buf + p->widx, buf, t0);
        memcpy(p->buf + t0, buf, t1);
    }   
    
    p->widx = (p->widx + size)%p->size;    
    OS_MUTEX_LOCK();
    printf("write ok %d\n", size);
    return size;
}


int main(int argc, char** argv) {
    uint8_t *a = (uint8_t*)"12345678901234567890123456789012345678901234567890";
    const int32_t buflen = 100;
    uint8_t tbuf[100];
    int32_t rlen = 10;
    int32_t wlen = 30;
    if(argc ==2 ) {
        rlen = atoi(argv[1]);
        printf("rlen is %d\n", rlen);
    }
    cbuf* p = cbuf_init(buflen);
    cbuf_write(p, a, wlen);
    cbuf_read(p, tbuf, rlen);

    cbuf_write(p, a, wlen);
    cbuf_read(p, tbuf, rlen);
    cbuf_write(p, a, wlen);
    cbuf_read(p, tbuf, rlen);
    
    cbuf_write(p, a, wlen);
    
    int32_t remian_len = _unlock_get_len(p);
    cbuf_read(p, tbuf, remian_len);
    printf("read %d: %s\n", remian_len, (char*)tbuf);
    printf("test down\n");
    return 0;
}








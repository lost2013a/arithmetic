#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <pthread.h>
#include "kfifo.h"

typedef unsigned int element;
//typedef int element;
//typedef unsigned short element;
//typedef short element;
//typedef unsigned char element;
//typedef char element;



DECLARE_KFIFO(pkfifo, element, 1024*1024);

void *test_in(void* parm) {
    //static int a[1000] = {0};
    int i;
    int wlen;
    for (i = 0; i < 1000*1000*100;){
        wlen = kfifo_in(&pkfifo, &i, 1);
        //wlen = kfifo_put(&pkfifo, element,  i);
        if(wlen) {
            i+=wlen;
        }
    }
    return 0;
}

static int out_idx = 0;
static void print_buf(element *buf, unsigned int len) {
    static int last_nb = -1;
    while(len--) {
        int nb = *buf++;
        if(nb - last_nb != 1) {
            printf("%d - %d\n", nb, last_nb);
        }
        last_nb = nb;
    }

}

void *test_out(void* parm) {
    element buf[4000];
    int rlen;
    while(1) {
        rlen = kfifo_out(&pkfifo, buf, 4000);
        if(rlen > 0) {
            out_idx+=rlen;
            print_buf(buf, rlen);
        }
    }
    return 0;
}

#include <sys/times.h>

/*系统的时间片大小，一般是10MS 或者 1MS*/
#define TICKS_PER_SEC 10 

static long getNowTime(void)
{
    long t = times(0);
	return t;
}


int main(int argc,char *argv[])
{
    pthread_t ids_r, ids_w;
    long t0, t1;
    t0 = getNowTime();
    pthread_create(&ids_w, NULL, test_in, NULL);
    pthread_create(&ids_r, NULL, test_out, NULL);
	pthread_join(ids_w, NULL);
    t1 = getNowTime();
    printf("t is %ld\n", t1 - t0);
    usleep(1000*100);
    printf("out_idx is %d\n", out_idx);
	return 0;
}


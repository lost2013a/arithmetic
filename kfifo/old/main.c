#include<stdio.h>
#include<stdlib.h>
#include "kfifo.h"
#include <pthread.h>


DECLARE_KFIFO(pkfifo, unsigned short, 1024*1024);

void *test_in(void* parm) {
    static int i = 0;
    int wlen;
    for (i = 0; i < 1000*1000*10;){
        wlen = kfifo_in(&pkfifo, &i, 1);
        if(wlen == 1) {
            i++;
        }
    }
    return 0;
}

static int out_idx = 0;


static void print_buf(unsigned short *buf, unsigned int len) {
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
    unsigned short buf[4000];
    int rlen;
    while(1) {
        rlen = kfifo_out(&pkfifo, buf, 100);
        if(rlen > 0) {
            out_idx+=rlen;
            print_buf(buf, rlen);
        }
    }
    return 0;
}

int main(int argc,char *argv[])
{
    pthread_t ids_r, ids_w;
    pthread_create(&ids_w, NULL, test_in, NULL);
    pthread_create(&ids_r, NULL, test_out, NULL);
	pthread_join(ids_w, NULL);
    usleep(1000*100);
    printf("out_idx is %d\n", out_idx);
	return 0;
}


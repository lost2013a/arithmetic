#include<stdio.h>
#include<stdlib.h>
#include "kfifo.h"
#include <pthread.h>





DECLARE_KFIFO(pkfifo, 1024*1024);


void *test_in(void* parm) {
    static int i = 0;
    int wlen;
    for (i = 0; i < 1000*1;){
        wlen = kfifo_in(&pkfifo, &i, 4);
        if(wlen == 4) {
            i++;
        }
    }
    return 0;
}

static int out_idx = 0;

void *test_out(void* parm) {
    int buf[4000];
    int rlen;
    while(1) {
        rlen = kfifo_out(&pkfifo, buf, 4);
        if(rlen > 0) {
            out_idx+=rlen;
            printf("%d\n", *buf);
        }
        
    }
    return 0;
}



int main(int argc,char *argv[])
{
    pthread_t ids_r, ids_w;
    pthread_create(&ids_w, NULL, test_in, NULL);
    pthread_create(&ids_r, NULL, test_out, NULL);
    printf("out_idx is %d\n", out_idx);
	pthread_join(ids_w, NULL);
    usleep(1000*100);
    printf("out_idx is %d\n", out_idx);

 
	return 0;
}


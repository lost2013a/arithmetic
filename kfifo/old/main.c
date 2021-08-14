#include<stdio.h>
#include<stdlib.h>
#include "kfifo.h"
#include <pthread.h>

#define BUFF_SIZE 256
 
char queue_buff[BUFF_SIZE]={0};

struct kfifo *pkfifo;
void *test_in(void* parm) {
    int i = 0;
    int wlen;
    for (i = 0; i < 1000*1;){
        wlen = kfifo_in(pkfifo, (unsigned char*)&i, sizeof(i));
        if(wlen == sizeof(i)) {
            i++;
        }
    }
    return 0;
}

static int out_idx = 0;
int test_out(void* parm) {
    unsigned char buf[4];
    int rlen;
    while(1) {
        rlen = kfifo_out(pkfifo, buf, 4);
        if(rlen > 0) {
            out_idx+=rlen;
            printf("read %d\n", out_idx);
        }
    }
    return 0;
}


int main(int argc,char *argv[])
{
    pthread_t ids_r, ids_w;
	pkfifo = malloc(sizeof(struct kfifo));
	if(pkfifo == NULL)
	{
		printf("malloc queue_buffer error!\n");
		return 0;
	}
	//param1 分配缓冲区的fifo
	//param2 要使用的预分配缓冲区
	//param3 内部缓冲区的大小必须是2的幂
	kfifo_init(pkfifo, queue_buff, BUFF_SIZE);

	char str[]={"hello_world!"};
	//将一些数据放入FIFO
	//param1 要使用的fifo
	//param2 要添加的数据
	//param3 要添加的数据的长度

    
    pthread_create(&ids_r, NULL, test_in, NULL);
    pthread_create(&ids_w, NULL, test_out, NULL);
	pthread_join(&ids_w, NULL);
    sleep(12);
    printf("out_idx is %d\n", out_idx);
 
	return 0;
}


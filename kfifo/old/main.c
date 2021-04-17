#include<stdio.h>
#include<stdlib.h>
#include "kfifo.h"
 
#define BUFF_SIZE 256
 
char queue_buff[BUFF_SIZE]={0};
 
int main(int argc,char *argv[])
{
	struct kfifo *pkfifo;
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
	kfifo_in(pkfifo,str,sizeof(str));  //一个线程专门负责添加数据，返回值为添加数据的字节数
 
	printf("in = %d out = %d\n",pkfifo->in,pkfifo->out);
 
	char rcv_data[256]={0};
	kfifo_out(pkfifo,rcv_data,sizeof(rcv_data)); //一个线程专门负责取出数据，返回值为取出数据的字节数
 
	printf("in = %d out = %d\n",pkfifo->in,pkfifo->out);
	
    printf("%s\n",rcv_data);
 
	return 0;
}


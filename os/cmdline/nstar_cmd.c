#include<sys/types.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include <errno.h>  
#include <signal.h>
#include "nstar_cmd.h"
#include <string.h>
#include <pthread.h>


#define cmd_dbg printf

static void monitor_fifo(void*param)
{ 
	struct fifostru  fifoStru;	
	int pipe_fd = -1;
	int readLen;
	for(;;){
		if(access(FIFO_NAME, F_OK) == 0)  {
			pipe_fd = open(FIFO_NAME, O_RDONLY); 
			cmd_dbg("start recv cmd\n");
			if(pipe_fd != -1)  
			{
				readLen = read(pipe_fd, (char*)(&fifoStru), FIFO_STRU_SIZE);
				cmd_dbg("read fifoStru.len:%d \n ",fifoStru.len);
				if( (FIFO_STRU_SIZE == readLen) && fifoStru.fifoType == FIFO_TYPE){
					//nstar_adt_cmd((unsigned char*)(fifoStru.fifoText),fifoStru.len);
				}
				close(pipe_fd);
				pipe_fd = -1;
			}
			else{
				cmd_dbg(" open %s errno:%d\n",FIFO_NAME,errno);
			}
		}
		usleep(50*1000);
	}
}



static void *test_pthread(void *parm)
{
	while(1)
	{
		printf("hello\n");
		sleep(1);
	}
}

static void creat_test_pthread(void)
{
	pthread_t threads;
	int rc;
	long t;
	rc = pthread_create(&threads, NULL, monitor_fifo, (void *)t);
	if (rc){
		printf("ERROR; return code from pthread_create() is %d\n", rc);
		exit(-1);
	}
}


static void _cmd_send(char *p_data,unsigned int len)
{
	unsigned int send;
	int pipe_fd = -1;
	//printf("cmd[%d]: %x %x %s",len ,p_data[0], p_data[1], p_data);
	int i=0;
	for(; i< 30;i++){
		printf("%02x ", p_data[i]);

	}
	printf("\r\n");
#if 1
	if(len > 0 && access(FIFO_NAME, F_OK) == 0) {
		pipe_fd = open(FIFO_NAME, O_WRONLY);  
		if(pipe_fd != -1)  {
			if((send =write(pipe_fd, p_data, len)) !=  len){
				printf(" nstar cmd send errno :%d\n",errno);
			}
			close(pipe_fd);
			pipe_fd = -1;
		}
	}
#endif	
}

int main(int argc , char**argv)
{
	char*	p_text;
	int i = 1;
	unsigned int len;
	struct fifostru fifoStru = {0,};
	fifoStru.len = 0;
	fifoStru.fifoType = FIFO_TYPE;
	p_text = fifoStru.fifoText;
	creat_test_pthread();
	if( argc > 1){
		while(i < argc && fifoStru.len < FIFO_STRU_LEN){
			len = snprintf(p_text+fifoStru.len,FIFO_STRU_LEN - fifoStru.len,"%s ",argv[i]);
			len = strlen(argv[i]);
			if( fifoStru.len + len +1 >= FIFO_STRU_LEN){
				break;
			}
			memcpy(p_text+fifoStru.len,argv[i],len);
			fifoStru.len += len;
			p_text[fifoStru.len] = ' ';
			fifoStru.len++;
			i++;
		}
		if(fifoStru.len < FIFO_STRU_LEN){	
			fifoStru.len--;
			fifoStru.fifoText[fifoStru.len] = 0;
			printf("read to send\n");
			_cmd_send((char*)(&fifoStru),FIFO_STRU_SIZE);
		}
	}
	while(1){
	sleep(1);
	}
	printf("run over\n");
	return 0;
}



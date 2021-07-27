#include <stdio.h> 
#include <stdlib.h>  
#include <fcntl.h>  
#include <string.h> 
#include <unistd.h>  
#include <sys/stat.h> 


#define FIFO_NAME "/tmp/cmdFifo" 

#define FIFO_STRU_LEN (128)
#define FIFO_STRU_SIZE (sizeof(struct fifostru))

struct fifostru    
{       
    unsigned short type;
    unsigned short len; 
	char fifoText[FIFO_STRU_LEN];    
}; 

#define test_dbg printf

static void monitor_fifo(void*param)
{ 
    struct fifostru *fifoStru = NULL;
    int fd = -1;
	int len;
    fifoStru = malloc(FIFO_STRU_SIZE);
    if(fifoStru == NULL) {
        return;
    }
	memset(fifoStru, 0, FIFO_STRU_SIZE); 
    
    while(1) 
    {  
        /*if none writer put in, block at here*/
        fd = open(FIFO_NAME, O_RDONLY);
        if(fd < 0) {
            printf("nstart cmd fd open err\n");
            break;
        }  
        len = read(fd, (char*)fifoStru, FIFO_STRU_SIZE);
        if (len < 0 ) {
            printf("nstart cmd fd read err\n");
            close(fd);
            break;
        }else if( 0 == fifoStru->type && len == fifoStru->len ) {
			//nstar_adt_cmd((unsigned char*)(fifoStru.fifoText),fifoStru.len);
			printf("rec %d bytes cmd %s \n", len, fifoStru->fifoText);
	    }else {
            printf("nstar cmd fmt wrong\n");
        }
        close(fd);
        usleep(10*1000);
    }
 exit:  
    free(fifoStru);
}



int nstar_cmd_init(void) {
    int ret = 0;
    if(access(FIFO_NAME, F_OK) != 0) {
        mkfifo(FIFO_NAME, 0777);
    }
    if(access(FIFO_NAME, W_OK|R_OK) == 0) {
        monitor_fifo(0);
    }else {
        printf("nstar cmd init err\n");
    }
}


int main() 
{  
    nstar_cmd_init();
}  



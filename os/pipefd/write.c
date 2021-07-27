#include <fcntl.h>  
#include <stdio.h> 
#include <string.h> 
#include <unistd.h>  

#define FIFO_NAME "/tmp/cmdFifo" 
#define FIFO_STRU_LEN (128)
#define FIFO_STRU_SIZE (sizeof(struct fifostru))

struct fifostru    
{       
    unsigned short type;
    unsigned short len; 
	char fifoText[FIFO_STRU_LEN];    
}; 

int send_cmd(int argc, char* argv[]) {
    char *strfmt = "%s";
    int idx = 0, i = 1, len;
    struct fifostru fifostru;
    fifostru.type = 0;
    while(i < argc && idx < FIFO_STRU_LEN) {
        len = snprintf(fifostru.fifoText + idx, FIFO_STRU_LEN - idx, strfmt, argv[i++]);
        if(len > 0) {
            strfmt = " %s";
            idx+=len;
        }
    }
    if(idx > 0) {
        int fd = open(FIFO_NAME, O_WRONLY);
        fifostru.len = strlen(fifostru.fifoText) + 4;
        if(fd >= 0) {
            write(fd, (unsigned char*)&fifostru, fifostru.len);
            close(fd);
        }
    }
}

int main(int argc, char* argv[])
{  
    int fd;  
    int nwrite;  
    if(argc < 1) {
       return -1;
    }
    send_cmd(argc, argv);
    close(fd);
    return 0;  
}  


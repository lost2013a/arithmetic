#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


#define DBG_FILE "write_log.txt"
#define max_dbg_len (1024*20)

static volatile int file_fd=-1;

static int write_file(unsigned char *buf, unsigned int len)
{
	if(write(file_fd, buf ,len) < 0){
		printf("erro, log write failed %x\n", file_fd);	
		return -1;
	}
	return 0;
}


int read_file(unsigned char *buf, unsigned int len)
{
	int read_len= read(file_fd, buf ,len);
	if(read_len < 0){
		printf("erro, log read file failed %x\n", file_fd);	
	}
	return read_len;
}



static void write_flush(void)
{
	fsync(file_fd);
}


int log_file_open(void)
{
	int ret=0;
	file_fd = open(DBG_FILE, O_RDWR| O_CREAT, 0x777); 
	if(file_fd < 0){
		printf("log file open failed\n");
		ret= -1;
	}
	return ret;
}

void log_file_close(void)
{
	close(file_fd);	
}



void log_write_(unsigned char *p_dta, unsigned int dtalen)
{  	
	write_file(p_dta, dtalen);
	write_flush();
}




void log_direct_lseek(void)
{
	if(file_fd > 0){
		lseek(file_fd, 0, SEEK_SET);
	}	
}

void log_direct_write(unsigned char *p_dta, unsigned int dtalen)
{  	
	if(file_fd > 0){
		log_write_(p_dta, dtalen);
	}	
}



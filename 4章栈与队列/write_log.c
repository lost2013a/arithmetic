#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "write_log.h"


static int file_fd=-1;


#define max_dbg_len (1024*20)
#define FILE_FLAG 0X665

void log_clear_breakfile(const char *file)
{
	int fd=-1;
	fd= open(file, O_TRUNC, FILE_FLAG); 
	close(fd);
}


int log_read_file(const char *file, unsigned char *p_dta, unsigned int maxlen)
{  	
	int fd=-1, file_len;
	fd= open(file, O_RDWR| O_CREAT, FILE_FLAG);  
	if(fd < 0){
		printf("err: %s file open failed\n", file);
		return -1;
	}
	file_len= read(fd, p_dta ,maxlen);
	if(file_len < 0){
		printf("err: %s file write failed\n", file);
	}
	close(fd);
	return file_len;
}


void file_direct_write(const char *name, unsigned char *p_dta, unsigned int dtalen)
{  	
	int fd=-1;
	fd= open(name, O_RDWR| O_CREAT, FILE_FLAG);  
	if(fd < 0){
		printf("err: %s file open failed\n", name);
		return;
	}
	
	//lseek(file_fd, 0, SEEK_SET);
	
	if(write(fd, p_dta ,dtalen) < 0){
		printf("err: %s file write failed\n", name);
		return;
	}
	fsync(fd);
	close(fd);
}




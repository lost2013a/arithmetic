#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "write_log.h"




#define max_dbg_len (1024*20)


int read_log_file(unsigned char *buf, unsigned int len)
{
	return log_read_file(DBG_FILE, buf, len);
}

int read_log_bak(unsigned char *buf, unsigned int len)
{
	return log_read_file(DEV_LOG_BAK, buf, len);
}


void clear_file(const char *file)
{
	int fd=-1;
	fd= open(file, O_TRUNC, 0x664); 
	close(fd);
}

void clear_log_file(void)
{
	close(file_fd);
	clear_file(DBG_FILE);
}

void clear_log_bak(void)
{
	clear_file(DEV_LOG_BAK);
}



static int file_fd=-1;

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

int log_read_file(const char *file, unsigned char *p_dta, unsigned int maxlen)
{  	
	int fd=-1, file_len;
	fd= open(file, O_RDWR| O_CREAT, 0x664);  
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



static void write_flush(void)
{
	fsync(file_fd);
}


int log_file_open(void)
{
	int ret=0;
	file_fd = open(DBG_FILE, O_RDWR| O_CREAT, 0x664); 
	if(file_fd < 0){
		printf("log file open failed\n");
		ret= -1;
	}
	return ret;
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

void file_direct_write(const char *name, unsigned char *p_dta, unsigned int dtalen)
{  	
	int fd=-1;
	fd= open(name, O_RDWR| O_CREAT, 0x664);  
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




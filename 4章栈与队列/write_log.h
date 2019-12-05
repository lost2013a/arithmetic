#ifndef   _WRITE_LOG_H_
#define   _WRITE_LOG_H_

#include "fifo.h" 

#define DBG_FILE "nstar.log"
#define DEV_LOG_BAK "nstar.log.bak"

#define MAX_LOGFILE_lEN		sizeof(struct log_file)
#define LOGFILE_HEAD_LEN	(MAX_LOGFILE_lEN- MAXSIZE*LOG_LEN)

struct log_file{

#define LOG_FILE_MAGIC 0x1122aabb
#define LOG_FILE_VERSION 0x01

	unsigned int magic;
	unsigned char version;
	unsigned char board;
	unsigned short file_len;
	unsigned int crc_val;
	unsigned int res;
	unsigned char data[MAXSIZE*LOG_LEN];
}__attribute__((packed));




void log_clear_breakfile(const char *file);
int log_read_file(const char *file, unsigned char *p_dta, unsigned int maxlen);
void file_direct_write(const char *name, unsigned char *p_dta, unsigned int dtalen);



#endif



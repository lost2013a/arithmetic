#ifndef   _WRITE_LOG_H_
#define   _WRITE_LOG_H_

#include "fifo.h" 
#define DBG_FILE "write_log.txt"
#define DEV_LOG_BAK "log.bak"


#define MAX_LOGFILE_lEN sizeof(struct log_file)
#define LOGFILE_HEAD_LEN  (MAX_LOGFILE_lEN- MAXSIZE*LOG_LEN)
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





int log_file_open(void);
void log_file_close(void);

void log_printf(char* fmt,...);
void log_write_(unsigned char *p_dta, unsigned int dtalen);
int read_file(unsigned char *buf, unsigned int len);



void log_direct_lseek(void);
void log_direct_write(unsigned char *p_dta, unsigned int dtalen);


int log_read_file(const char *file, unsigned char *p_dta, unsigned int maxlen);


int read_log_file(unsigned char *buf, unsigned int len);
int read_log_bak(unsigned char *buf, unsigned int len);

void clear_log_file(void);
void clear_log_bak(void);



#endif



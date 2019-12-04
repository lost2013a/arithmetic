#ifndef   _WRITE_LOG_H_
#define   _WRITE_LOG_H_


#define MAX_LOGFILE_lEN sizeof(struct log_file)
struct log_file{
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




#endif



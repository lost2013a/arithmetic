#include "stdio.h"    
#include "stdlib.h"   
#include "fifo.h"   
#include "string.h"

#include <time.h>
#include <sys/time.h>

#include "msg_log.h"
#include "write_log.h"
#include <unistd.h>


#include <pthread.h>
#include <malloc.h>


static unsigned char init_ok=0;
static pthread_mutex_t log_mutex;
#define LOG_UNLOCK() 	pthread_mutex_unlock(&log_mutex)	
#define LOG_LOCK		pthread_mutex_lock(&log_mutex)


static const unsigned char crc_table[] =
{
    0x00,0x31,0x62,0x53,0xc4,0xf5,0xa6,0x97,0xb9,0x88,0xdb,0xea,0x7d,0x4c,0x1f,0x2e,
    0x43,0x72,0x21,0x10,0x87,0xb6,0xe5,0xd4,0xfa,0xcb,0x98,0xa9,0x3e,0x0f,0x5c,0x6d,
    0x86,0xb7,0xe4,0xd5,0x42,0x73,0x20,0x11,0x3f,0x0e,0x5d,0x6c,0xfb,0xca,0x99,0xa8,
    0xc5,0xf4,0xa7,0x96,0x01,0x30,0x63,0x52,0x7c,0x4d,0x1e,0x2f,0xb8,0x89,0xda,0xeb,
    0x3d,0x0c,0x5f,0x6e,0xf9,0xc8,0x9b,0xaa,0x84,0xb5,0xe6,0xd7,0x40,0x71,0x22,0x13,
    0x7e,0x4f,0x1c,0x2d,0xba,0x8b,0xd8,0xe9,0xc7,0xf6,0xa5,0x94,0x03,0x32,0x61,0x50,
    0xbb,0x8a,0xd9,0xe8,0x7f,0x4e,0x1d,0x2c,0x02,0x33,0x60,0x51,0xc6,0xf7,0xa4,0x95,
    0xf8,0xc9,0x9a,0xab,0x3c,0x0d,0x5e,0x6f,0x41,0x70,0x23,0x12,0x85,0xb4,0xe7,0xd6,
    0x7a,0x4b,0x18,0x29,0xbe,0x8f,0xdc,0xed,0xc3,0xf2,0xa1,0x90,0x07,0x36,0x65,0x54,
    0x39,0x08,0x5b,0x6a,0xfd,0xcc,0x9f,0xae,0x80,0xb1,0xe2,0xd3,0x44,0x75,0x26,0x17,
    0xfc,0xcd,0x9e,0xaf,0x38,0x09,0x5a,0x6b,0x45,0x74,0x27,0x16,0x81,0xb0,0xe3,0xd2,
    0xbf,0x8e,0xdd,0xec,0x7b,0x4a,0x19,0x28,0x06,0x37,0x64,0x55,0xc2,0xf3,0xa0,0x91,
    0x47,0x76,0x25,0x14,0x83,0xb2,0xe1,0xd0,0xfe,0xcf,0x9c,0xad,0x3a,0x0b,0x58,0x69,
    0x04,0x35,0x66,0x57,0xc0,0xf1,0xa2,0x93,0xbd,0x8c,0xdf,0xee,0x79,0x48,0x1b,0x2a,
    0xc1,0xf0,0xa3,0x92,0x05,0x34,0x67,0x56,0x78,0x49,0x1a,0x2b,0xbc,0x8d,0xde,0xef,
    0x82,0xb3,0xe0,0xd1,0x46,0x77,0x24,0x15,0x3b,0x0a,0x59,0x68,0xff,0xce,0x9d,0xac
};

unsigned char cal_crc_table(unsigned char *ptr, unsigned char len) 
{
    unsigned char  crc = 0x00;
    while (len--)
    {
        crc = crc_table[crc ^ *ptr++];
    }
    return (crc);
}

#define EXTERN_CRC32_CAL(data,len) cal_crc_table(data,len)



#if 0
struct log_file{
	unsigned char data[MAXSIZE * MAXSIZE];
};
#endif

void read_time(struct sys_time *p_systime)
{
	struct tm  *sysTime;
	struct timeval  tv;
	gettimeofday(&tv, NULL);
	sysTime   = localtime(&tv.tv_sec);
	p_systime->year = (sysTime->tm_year +1900)%100;
	p_systime->mon  = sysTime->tm_mon+1;
	p_systime->day  = sysTime->tm_mday;
	p_systime->hour = sysTime->tm_hour;
	p_systime->min  = sysTime->tm_min;
	p_systime->sec  = sysTime->tm_sec;
}

int add_new_log(char *name, unsigned char emergency, unsigned char type, 
	unsigned char ops, unsigned char *logic_id)
{
	unsigned char buf[LOG_LEN]={0};
	unsigned int name_len;
	struct msg_log *plog;
	if(1 != init_ok)
		return -1;
	plog= (struct msg_log *)buf;
	read_time(&plog->t);
	plog->emergency= emergency;
	plog->type= type;
	plog->ops= ops;
	memcpy(plog->logic_id, logic_id, 6);
	name_len= strlen(name);
	if(name_len  > MAX_lOGNAME_LEN-1)
		name_len= MAX_lOGNAME_LEN-1;
	memcpy(plog->name, name, name_len);
	plog->name[name_len]= 0;
	printf("name :%s\n", plog->name);
	return queue_add(buf, LOG_LEN);
}

int parse_log_str(unsigned char *buf, unsigned char len)
{
	struct msg_log *plog;
	struct sys_time *ptime;
	plog= (struct msg_log *)buf;
	ptime= &plog->t;
	printf("log: %02d-%02d-%02d, %02d:%02d:%02d ", ptime->year,  ptime->mon, ptime->day,
		 ptime->hour, ptime->min, ptime->sec);
	printf("%s, %d, %d , %d, %02x%02x%02x%02x%02x%02x\n", plog->name, plog->emergency, plog->type, plog->ops,
		plog->logic_id[0], plog->logic_id[1], plog->logic_id[2], plog->logic_id[3], plog->logic_id[4], plog->logic_id[5]);
	return 0;
}




#define MSG_LOG_PRINT() queue_visit(parse_log_str)


static unsigned char *resolver_buf=NULL;
static unsigned int resolver_len= 0;
	

static int func_visit_logbuf(unsigned char *buf, unsigned char len)
{
	if(resolver_buf){
		memcpy(resolver_buf+ resolver_len, buf, len);
		resolver_len+= len;
	}
	return 0;
}


static unsigned int resolver_log(unsigned char *buf)
{
	resolver_len=0;
	if(NULL != buf){
		resolver_buf= buf;
		queue_visit(func_visit_logbuf);
	}
	return resolver_len;
}


void sync_log_file(void)
{
	
	unsigned char *buf= malloc(MAX_LOGFILE_lEN);
	if(buf == NULL){
		printf("malloc log file faild\n");
		return;
	}
	struct log_file *plog= (struct log_file*)buf;
	plog->file_len= 0;
	plog->magic= LOG_FILE_MAGIC;
	plog->version= LOG_FILE_VERSION;
	plog->file_len=  resolver_log(plog->data);
	plog->crc_val= EXTERN_CRC32_CAL(plog->data, plog->file_len);
	log_direct_write((unsigned char*)plog, plog->file_len + LOGFILE_HEAD_LEN);
	free(buf);
}


static unsigned char _recover_log_file(const char* filename, struct log_file *plog)
{
	unsigned char ret=0;
	int read_len=0;
	unsigned int crc_cal;
	read_len= log_read_file(filename, (unsigned char*)plog, MAX_LOGFILE_lEN + 1);
	if(read_len < LOGFILE_HEAD_LEN){
		printf("file too short\n");
		return 0;
	}
	crc_cal= EXTERN_CRC32_CAL(plog->data, plog->file_len);
	printf("crc: %x / %x\n", crc_cal, plog->crc_val);

	if(plog->magic ==  LOG_FILE_MAGIC && plog->version == LOG_FILE_VERSION 
			&& read_len <= MAX_LOGFILE_lEN && plog->crc_val == crc_cal)
	{
		printf("old log file len=%d\n", plog->file_len);
		ext_init(plog->data, plog->file_len);
		ret= 1;
	}
	else{
		clear_log_file();
		printf("clear \n");
	
	}
	return ret;
}






void log_init(void)
{
	unsigned char *tmpbuf;
	if (pthread_mutex_init(&log_mutex, NULL) != 0){
		printf("err: log creat mutex faild\n"); 	
	}
	tmpbuf= malloc(MAX_LOGFILE_lEN + 1);//add 1
	if(tmpbuf == NULL){
		printf("err: log init malloc  faild\n");
		return;
	}
	if(1 != _recover_log_file(DBG_FILE, (struct log_file*)tmpbuf)){
		_recover_log_file(DEV_LOG_BAK, (struct log_file*)tmpbuf);
	}
		

	free(tmpbuf);
	init_ok=1;
	
}

int main(void )
{
unsigned char id[6]={1,2,3,4,5,6};
	static unsigned int ctl=0;
	queue_init();
	
	log_file_open();
	log_init();
	
	MSG_LOG_PRINT();
	
	while(1){
		usleep(1000*1000);
		
		add_new_log("test", 0, 1, 2, id);
		id[5]++;
		
		MSG_LOG_PRINT();
		if((ctl++)%1 == 0){
			log_direct_lseek();
			sync_log_file();
		}
	}
	
	return 0;
}


	



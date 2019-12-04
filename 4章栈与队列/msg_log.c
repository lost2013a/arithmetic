#include "stdio.h"    
#include "stdlib.h"   
#include "fifo.h"   
#include "string.h"

#include <time.h>
#include <sys/time.h>

#include "msg_log.h"
#include "write_log.h"
#include <unistd.h>

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



#if 0
//int sync_log_str(unsigned char *buf, unsigned char len)
//{
//	log_direct_write((unsigned char*)buf, len);
//	return 0;
//}
#else
int sync_log_str(unsigned char *buf, unsigned char len)
{
	log_direct_write((unsigned char*)buf, len);
	return 0;
}
#endif

#define MSG_LOG_PRINT() queue_visit(parse_log_str)

#define MSG_LOG_SYNC() queue_visit(sync_log_str)



static unsigned char *log= NULL;
static unsigned int log_len;

//int sync_log_str(unsigned char *buf, unsigned char len)
//{
//	if(log != NULL){
//		memcpy(&log[log_len], buf, len);
//		log_len+= len;
//	}
//	return 0;
//}

void sync_log_file(void)
{
	log= malloc(MAX_LOGFILE_lEN);
	if(log == NULL){
		printf("malloc log file faild\n");
		return;
	}
	log_len=0;
	MSG_LOG_SYNC();
	log_direct_write(log, log_len);
	
	free(log);
	log= NULL;
}



unsigned char rbuf[1024*10];
int rlen=0;
void log_init(void)
{
	rlen= read_file(rbuf, 1024*10);
	
}

int main(void )
{
unsigned char id[6]={1,2,3,4,5,6};

	static unsigned int ctl=0;

	log_file_open();
	log_init();
	
	queue_init();
	ext_init(rbuf, rlen);
	MSG_LOG_PRINT();
	
	while(1){
		usleep(500*1000);
		
		add_new_log("test", 0, 1, 2, id);
		id[5]++;
		
		MSG_LOG_PRINT();
		if((ctl++)%1 == 0){
			//log_direct_lseek();
			//sync_log_file();
			//MSG_LOG_SYNC();
		}
	}
	
	return 0;
}


	



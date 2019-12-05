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

#define LOG_INTERVAL 3		/*写入间隔单位s*/
#define LOG_FORCE_CNT 3		/*大于多少条时，立刻同步到FLASH*/


static pthread_mutex_t log_mutex;
#define LOG_UNLOCK() 	pthread_mutex_unlock(&log_mutex)	
#define LOG_LOCK()		pthread_mutex_lock(&log_mutex)

#define EXTERN_CRC32_CAL(data,len) cal_crc_table(data,len)

static unsigned char init_ok=0;
static volatile unsigned int force_write_cnt=0;
static unsigned char *resolver_buf=NULL;
static unsigned int resolver_len= 0;


static int parse_log_print(unsigned char *buf, unsigned char len);

	
static int func_visit_logbuf(unsigned char *buf, unsigned char len)
{
	if(resolver_buf){
		memcpy(resolver_buf+ resolver_len, buf, len);
		resolver_len+= len;
	}
	return 0;
}

unsigned int transform_log_direct(unsigned char *buf)
{
	unsigned int ret;
	if(1 != init_ok)
		return 0;
	LOG_UNLOCK();
	resolver_len=0;
	if(NULL != buf){
		resolver_buf= buf;
		queue_visit(func_visit_logbuf);
	}
	ret= resolver_len;
	LOG_LOCK();
	return ret;
}

static void read_time(struct sys_time *p_systime)
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




static int parse_log_print(unsigned char *buf, unsigned char len)
{
	struct msg_log *plog;
	struct sys_time *ptime;
	plog= (struct msg_log *)buf;
	ptime= &plog->t;
	printf("%02d/%02d/%02d %02d:%02d:%02d ", ptime->year,  ptime->mon, ptime->day,
		 ptime->hour, ptime->min, ptime->sec);
	printf("%s, %d,%d,%d, %02x%02x%02x%02x%02x%02x\n", plog->name, plog->emergency, plog->type, plog->ops,
		plog->logic_id[0], plog->logic_id[1], plog->logic_id[2], plog->logic_id[3], plog->logic_id[4], plog->logic_id[5]);
	return 0;
}


static unsigned char add_new_log(const char *name, unsigned char emergency, unsigned char type, 
	unsigned char ops, unsigned char *logic_id)
{
	unsigned char buf[LOG_LEN]={0};
	unsigned char ret;
	struct msg_log *plog;
	if(1 != init_ok)
		return -1;
	plog= (struct msg_log *)buf;
	read_time(&plog->t);
	plog->emergency= emergency;
	plog->type= type;
	plog->ops= ops;
	memcpy(plog->logic_id, logic_id, 6);
	strncpy(plog->name, name, MAX_lOGNAME_LEN);
	LOG_UNLOCK();
	ret=  queue_add(buf, LOG_LEN);
	LOG_LOCK();
	return ret;
}

void nstar_add_log(const char *name, unsigned char emergency, unsigned char type, 
	unsigned char ops, unsigned char *logic_id)
{
	force_write_cnt++;
	add_new_log(name, emergency, type, ops, logic_id);
}

void nstar_log_print(void)
{
	if(1 != init_ok)
		return;
	LOG_UNLOCK();
	queue_visit(parse_log_print);
	LOG_LOCK();
}




static void nstar_sync_log(void)
{
	static unsigned char ctl=0;
	if(force_write_cnt >= 3 || ((ctl++)%5 == 0 && force_write_cnt)){
		unsigned char *buf= malloc(MAX_LOGFILE_lEN);
		if(buf == NULL){
			printf("malloc log file faild\n");
			return;
		}
		struct log_file *plog= (struct log_file*)buf;
		plog->file_len= 0;
		plog->magic= LOG_FILE_MAGIC;
		plog->version= LOG_FILE_VERSION;
		plog->file_len=  transform_log_direct(plog->data);
		plog->crc_val= EXTERN_CRC32_CAL(plog->data, plog->file_len);
		file_direct_write(DBG_FILE, (unsigned char*)plog, plog->file_len + LOGFILE_HEAD_LEN);
		file_direct_write(DEV_LOG_BAK, (unsigned char*)plog, plog->file_len + LOGFILE_HEAD_LEN);
		free(buf);
		force_write_cnt=0;
	}
}

static unsigned char _recover_log_file(const char* filename, struct log_file *plog)
{
	unsigned char ret=0;
	int read_len=0;
	unsigned int crc_cal;
	read_len= log_read_file(filename, (unsigned char*)plog, MAX_LOGFILE_lEN + 1);
	if(read_len < LOGFILE_HEAD_LEN){
		return 0;
	}
	crc_cal= EXTERN_CRC32_CAL(plog->data, plog->file_len);
	
	if(plog->magic ==  LOG_FILE_MAGIC && plog->version == LOG_FILE_VERSION 
			&& read_len <= MAX_LOGFILE_lEN && plog->crc_val == crc_cal)
	{
		ext_init(plog->data, plog->file_len);
		ret= 1;
	}
	else{
		log_clear_breakfile(filename);
		printf("log: clear break file %s\n", filename);
	}
	return ret;
}

void nstar_log_init(void)
{
	unsigned char *tmpbuf;
	if (pthread_mutex_init(&log_mutex, NULL) != 0){
		printf("err: log creat mutex faild\n"); 	
	}
	tmpbuf= malloc(MAX_LOGFILE_lEN + 1);//多加1，判断文件是否超长
	if(tmpbuf == NULL){
		printf("err: log init malloc  faild\n");
		return;
	}
	
	queue_init();
	
	if(1 != _recover_log_file(DBG_FILE, (struct log_file*)tmpbuf)){
		_recover_log_file(DEV_LOG_BAK, (struct log_file*)tmpbuf);
	}
	
	free(tmpbuf);
	init_ok=1;
}


void test_add_log(void *parm)
{
	unsigned char id[6]={1,2,3,4,5,6};
	while(1){
		sleep(3);
		nstar_add_log("test", 1, 2, 3, id);
		printf("add new log\n");
	}

}


void creat_new_thread(void)
{
	pthread_t threads;
	int rc;
	long t=0;
	rc = pthread_create(&threads, NULL, test_add_log, (void *)t);
	if (rc){
		printf("ERROR; return code from pthread_create() is %d\n", rc);
		exit(-1);
	}
}

void nstar_log_sver(void *parm)
{
	

	nstar_log_init();
	nstar_log_print();
	creat_new_thread();
	while(1){
		sleep(1);
		nstar_sync_log();
		nstar_log_print();
	}
}



int main(void )
{
	unsigned int parm;
	nstar_log_sver(&parm);
	return 0;
}


	



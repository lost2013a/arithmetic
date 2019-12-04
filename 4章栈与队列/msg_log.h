#ifndef __NSTAR_DEV_LOG_H
#define __NSTAR_DEV_LOG_H	

#include "fifo.h"

#define LOG_LEN sizeof(struct msg_log)


struct sys_time{
	unsigned char year;
	unsigned char mon;
	unsigned char day;
	unsigned char hour;
	unsigned char min;
	unsigned char sec;
};

enum{
LOG_TYPE=0,

};

enum{
LOG_OPS_START=0x0,
LOG_OPS_END,
};

#define MAX_lOGNAME_LEN 10
struct msg_log{
	char name[MAX_lOGNAME_LEN];/*GB2312*/
	struct sys_time t;
	unsigned char emergency:1;
	unsigned char type:3;
	unsigned char ops:4;
	unsigned char logic_id[6];
}__attribute__((packed));


#endif

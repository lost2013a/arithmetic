#ifndef __NSTAR_CMD__
#define __NSTAR_CMD__


#define FIFO_NAME	("/tmp/nstar_cmd_fifo")

#define FIFO_STRU_LEN (256)
#define FIFO_STRU_SIZE (sizeof(struct fifostru))
#define FIFO_TYPE	(0xa04)

struct fifostru    
{       
	unsigned short fifoType;
	unsigned short len;   
	char fifoText[FIFO_STRU_LEN];    
}; 




#endif //__NSTAR_CMD__



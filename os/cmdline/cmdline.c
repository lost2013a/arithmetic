#include <ctype.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
 
#define FIFO_NAME	("/tmp/nstar_cmd_fifo")
#define cmd_dbg printf
void main(int argc, char *argv[])
{
	
	if(access(FIFO_NAME, F_OK) != 0)  
	{
		if(mkfifo(FIFO_NAME, 0777) == 0)  
		{
			cmd_dbg("nstar_cmd_init ThreadCreate OK  \n");
		}
		else
		{  
			cmd_dbg("Could not create fifo %s \n", FIFO_NAME);  
		}  
		
	}
}


#include <fcntl.h>
#include <stdio.h> 
#include <errno.h>

int main(void)
{
    int fd;
   char buf[5] __attribute__((at(0x10000)));
    printf(" app buf:%x, tid %d\n",(int)buf, getpid());
    fd = open("/dev/helldev", O_RDWR);
    if(fd < 0){
        printf("can't open! %d \n",errno);
	}
	else{
		write(fd, buf, 5);
		printf(" app buf:%d,%d,%d,%d,%d\n",buf[0],buf[1],buf[2],buf[3],buf[4]);
    }

    return 0;
}
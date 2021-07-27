#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char* argv[])
{
    int fd = -1;
    int res, cur, new_cur, offset = 0;
    char sz_filename[] = "hello.txt";
    char buf[128] = {0};
    
    fd = open(sz_filename, O_RDONLY);		/* 以只读方式打开文件 */
    if(fd < 0) 
    {
        printf("open file failed, errno=%d.\n", errno);
		fd = open(sz_filename, O_WRONLY | O_CREAT); 
        return -1;
    }

	lseek(fd, 0, SEEK_SET);	
	int file_size= lseek(fd, 0, SEEK_END);
	printf("file size= %d\n", file_size);

	cur = lseek(fd, 0, SEEK_CUR);		
	printf("cur= %d\n", cur);
	
    res = read(fd, buf, sizeof(buf));		
    buf[res]='\0';
	
    printf( "read[%d]: %s\n" , cur, buf);
	
	lseek(fd, 0, SEEK_SET);
    cur = lseek(fd, 0, SEEK_END);		
    offset = -7;				
    new_cur = lseek(fd, offset, SEEK_END);	
    if(new_cur == -1)
        printf("lseek failed, errno=%d.\n", errno);
    else
        printf("SEEK_END: current=%d, offset=%d, new_cur=%d.\n",
                cur, offset, new_cur);
    res = read(fd, buf, sizeof(buf));
    buf[res]='\0';
    printf("read %d bytes from file \"%s\","" offset=%d, data=\"%s\"\n",res, sz_filename, new_cur, buf);
    close(fd);					
    return 0;

}
/*
2点10分
4点20分
8点15分
10点40分


*/




#include<stdlib.h>
#include<stdio.h>

int main(int argc, char* argv[])
{

	FILE * fp;
    char buffer[80];
	
//    system("./syscall1");
//	system("./syscall2");
	 
    fp=popen("./syscall2","r");
   
    printf("%s","syscall2 down\n");
	pclose(fp);
	 printf("%s","syscall2 close\n");
	fp=popen("./syscall1","r");
  
    printf("%s","syscall1 down\n");
	
    pclose(fp);
	printf("%s","syscall1 close\n");
    return 0;
}


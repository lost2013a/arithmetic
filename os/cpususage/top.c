#include <ctype.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
 
typedef struct CPU_PACKED         //定义一个cpu occupy的结构体
{
char name[20];             //定义一个char类型的数组名name有20个元素
unsigned int user;        //定义一个无符号的int类型的user
unsigned int nice;        //定义一个无符号的int类型的nice
unsigned int system;    //定义一个无符号的int类型的system
unsigned int idle;         //定义一个无符号的int类型的idle
unsigned int iowait;
unsigned int irq;
unsigned int softirq;
}CPU_OCCUPY;



double cal_cpuoccupy (CPU_OCCUPY *o, CPU_OCCUPY *n)
{
    double od, nd;
    double id, sd;
    double cpu_use ;
 
    od = (double) (o->user + o->nice + o->system +o->idle+o->softirq+o->iowait+o->irq);//第一次(用户+优先级+系统+空闲)的时间再赋给od
    nd = (double) (n->user + n->nice + n->system +n->idle+n->softirq+n->iowait+n->irq);//第二次(用户+优先级+系统+空闲)的时间再赋给od
 
    id = (double) (n->idle);    //用户第一次和第二次的时间之差再赋给id
    sd = (double) (o->idle) ;    //系统第一次和第二次的时间之差再赋给sd
    if((nd-od) != 0)
    cpu_use =100.0- ((id-sd))/(nd-od)*100.00; //((用户+系统)乖100)除(第一次和第二次的时间差)再赋给g_cpu_used
    else cpu_use = 0;
    return cpu_use;
}

//intr 563073 0 0 0 0 0 0 0 0 0 0 214781 0 0 0 0 1844

void get_cpuoccupy (CPU_OCCUPY *cpust)
{
    FILE *fd;
    int n;
    char buff[256];
    CPU_OCCUPY *cpu_occupy;
    cpu_occupy=cpust;
 
    fd = fopen ("/proc/stat", "r");
    fgets (buff, sizeof(buff), fd);
 
    sscanf (buff, "%s %u %u %u %u %u %u %u", cpu_occupy->name, &cpu_occupy->user, &cpu_occupy->nice,&cpu_occupy->system, &cpu_occupy->idle ,&cpu_occupy->iowait,&cpu_occupy->irq,&cpu_occupy->softirq);
 
    fclose(fd);
}
 
double getCpuRate()
{
    CPU_OCCUPY cpu_stat1;
    CPU_OCCUPY cpu_stat2;
    double cpu;
    get_cpuoccupy((CPU_OCCUPY *)&cpu_stat1);
    sleep(1);
 
    //第二次获取cpu使用情况
    get_cpuoccupy((CPU_OCCUPY *)&cpu_stat2);
 
    //计算cpu使用率
    cpu = cal_cpuoccupy ((CPU_OCCUPY *)&cpu_stat1, (CPU_OCCUPY *)&cpu_stat2);
 
    return cpu;
}


double raw_getCpuRate()
{
    CPU_OCCUPY cpu_stat1;
    CPU_OCCUPY cpu_stat2;
    double cpu;
    get_cpuoccupy((CPU_OCCUPY *)&cpu_stat1);
    sleep(1);
 
    //第二次获取cpu使用情况
    get_cpuoccupy((CPU_OCCUPY *)&cpu_stat2);
 
    //计算cpu使用率
    cpu = cal_cpuoccupy ((CPU_OCCUPY *)&cpu_stat1, (CPU_OCCUPY *)&cpu_stat2);
 
    return cpu;
}



typedef struct MEM_PACKED         //定义一个mem occupy的结构体
{
        char name[20];      //定义一个char类型的数组名name有20个元素
        unsigned long total;
        char name2[20];
}MEM_OCCUPY;
 
 
typedef struct MEM_PACK         //定义一个mem occupy的结构体
{
        double total,used_rate;
}MEM_PACK;


MEM_PACK *get_memoccupy ()    // get RAM message
{
    FILE *fd;
    int n;
    double mem_total,mem_used_rate;;
    char buff[256];
    MEM_OCCUPY *m=(MEM_OCCUPY *)malloc(sizeof(MEM_OCCUPY));;
    MEM_PACK *p=(MEM_PACK *)malloc(sizeof(MEM_PACK));
    fd = fopen ("/proc/meminfo", "r");
 
    fgets (buff, sizeof(buff), fd);
    sscanf (buff, "%s %lu %s\n", m->name, &m->total, m->name2);
    mem_total=m->total;
    fgets (buff, sizeof(buff), fd);
    sscanf (buff, "%s %lu %s\n", m->name, &m->total, m->name2);
    mem_used_rate=(1-m->total/mem_total)*100;
    mem_total=mem_total/(1024*1024);
    p->total=mem_total;
    p->used_rate=mem_used_rate;
    fclose(fd);     //关闭文件fd
    return p ;
}



int main(void)
{
	MEM_PACK *mem= get_memoccupy();
	printf("cpu:%lf\n",getCpuRate());
	printf("mem:%lf / %lf\n", mem->used_rate ,mem->total);


	system("top -n 1 |grep Cpu | cut -d \",\" -f 1 | cut -d \":\" -f 2 >cpu.txt");
	system("top -n 1 |grep Cpu | cut -d \",\" -f 2 >>cpu.txt");
	system("top -n 1 |grep Mem | cut -d \",\" -f 1 | cut -d \":\" -f 2 >>cpu.txt");
	system("top -n 1 |grep Mem | cut -d \",\" -f 2 >>cpu.txt");
	return 0;
}


 


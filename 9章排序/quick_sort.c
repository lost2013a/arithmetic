#include <stdio.h>    
#include <string.h>
#include <ctype.h>      
#include <stdlib.h>   
//#include <io.h>  
#include <math.h>  
#include <time.h>

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

#define MAX_LENGTH_INSERT_SORT 7 /* 用于快速排序时判断是否选用插入排序阙值 */

typedef int Status; 


#define MAXSIZE 10000  /* 用于要排序数组个数最大值，可根据需要修改 */
typedef struct
{
	int r[MAXSIZE+1];	/* 用于存储要排序数组，r[0]用作哨兵或临时变量 */
	int length;			/* 用于记录顺序表的长度 */
}SqList;


/* 交换L中数组r的下标为i和j的值 */
void swap(SqList *L,int i,int j) 
{ 
	int temp=L->r[i]; 
	L->r[i]=L->r[j]; 
	L->r[j]=temp; 
}

void print(SqList L)
{
	int i;
	for(i=1;i<L.length;i++)
		printf("%d,",L.r[i]);
	printf("%d",L.r[i]);
	printf("\n");
}


int Partition(SqList *L,int low,int high)
{ 
	int pivotkey;
	pivotkey=L->r[low]; /* 用子表的第一个记录作枢轴记录 */
	while(low<high) /*  从表的两端交替地向中间扫描 */
	{ 
		 while(low<high&&L->r[high]>=pivotkey)
			high--;
		 swap(L,low,high);/* 将比枢轴记录小的记录交换到低端 */
		 while(low<high&&L->r[low]<=pivotkey)
			low++;
		 swap(L,low,high);/* 将比枢轴记录大的记录交换到高端 */
	}
	return low; /* 返回枢轴所在位置 */
}


void QSort(SqList *L,int low,int high)
{ 
	int pivot;
	if(low<high)
	{
    	pivot=Partition(L,low,high); /*  将L->r[low..high]一分为二，算出枢轴值pivot */
    	QSort(L,low,pivot-1);		/*  对低子表递归排序 */
    	QSort(L,pivot+1,high);		/*  对高子表递归排序 */
	}
}

/* 对顺序表L作快速排序 */
void QuickSort(SqList *L)
{ 
	QSort(L,1,L->length);
}


#define N 9
int main()
{
   int i;
   
   /* int d[N]={9,1,5,8,3,7,4,6,2}; */
   int d[N]={50,10,90,30,70,40,80,60,20};
   /* int d[N]={9,8,7,6,5,4,3,2,1}; */

   SqList l0,l1,l2,l3,l4,l5,l6,l7,l8,l9,l10;
   
   for(i=0;i<N;i++)
     l0.r[i+1]=d[i];
   l0.length=N;
   l1=l2=l3=l4=l5=l6=l7=l8=l9=l10=l0;
   printf("排序前:\n");
   print(l0);

   printf("快速排序:\n");
   QuickSort(&l9);
   print(l9);


	return 0;
}

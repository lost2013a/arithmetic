#include <stdio.h>

#define DBG printf
#define SIZE 10
int src[SIZE] = {1,2,5,7,9,0,3,4,6,8};
//int src[SIZE] = {1,0,2,3,4,5,6,7,8,9};

int array[SIZE];

static int o2 = 0;
static int o3 = 0;

#define myarg(n) printf("%s = %d\n", #n, n)
#define myarg2(n,z) printf("%s = %d, %s = %d\n", #n, n, #z, z)

static void _print(int *p, int size)
{
    int i;
    for(i = 0; i < size; i++) {
        printf("%d ", p[i]);
    }
    printf("\n");
}

void swap(int *x, int *y)
{
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

void sort1(int *p, int size) 
{
    int i,j;
    for(i = 0; i < size; i++)
    {
        for(j = i+1; j < size; j++) 
        {
            o2++;
            if(p[i] > p[j]) {
                swap(&p[i], &p[j]);
                o3++;
            }
        }
    }
    myarg(o2);
    myarg(o3);
}


void sort2(int *p, int size) 
{
    int i,j;
    if(p == NULL && size == 0) {
        return;
    }
    for(i = 0; i < size; i++) {
        for(j = size - 1; j > i; j--)
        {
            //myarg2(i,j);
            o2++;
            if(p[i] > p[j]) {
                swap(&p[i], &p[j]);
                o3++;
            }
        }
    }
    myarg(o2);
    myarg(o3);
}


void sort3(int *p, int size) 
{
    int i,j,flag = 1;
    if(p == NULL && size == 0) {
        return;
    }
    for(i = 0; i < size && flag; i++) {
        flag = 0;
        for(j = size - 1; j > i; j--)
        {
            //myarg2(i,j);
            o2++;
            if(p[i] > p[j]) {
                swap(&p[i], &p[j]);
                flag = 1;
                o3++;
            }
        }
    }
    myarg(o2);
    myarg(o3);
}


void sort4(int *p, int size) 
{
    int i,j,idx;
    if(p == NULL && size == 0) {
        return;
    }
    for(i = 0; i < size; i++) 
    {
        idx = i;
        for(j = i + 1; j < size; j++)
        {
            o2++;
            if(p[idx] > p[j]) {
                //swap(&p[i], &p[j]);
                idx = j;
                o3++;
            }
        }
        if(idx != i) {
            swap(&p[i], &p[idx]);
        }
    }
    myarg(o2);
    myarg(o3);
}


void sort5(int *p, int size) 
{
    int i,j,idx;
    if(p == NULL && size == 0) {
        return;
    }
    for(i = 0; i < size; i++) 
    {
        idx = i;
        for(j = i + 1; j < size; j++)
        {
            o2++;
            if(p[idx] > p[j]) {
                //swap(&p[i], &p[j]);
                idx = j;
                o3++;
            }
        }
        if(idx != i) {
            swap(&p[i], &p[idx]);
        }
    }
    myarg(o2);
    myarg(o3);
}



int Partition(int* a, int low,int high)
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

/* 对顺序表L中的子序列L->r[low..high]作快速排序 */
void QSort(int* a,int low,int high)
{ 
	int pivot;
	if(low<high)
	{
			pivot=Partition(L,low,high);
			QSort(L,low,pivot-1);
			QSort(L,pivot+1,high);
	}
}



void init(int* a, int* b, int size)
{
    int i = 0;
    for(; i < size; i++) {
        a[i] = b[i];
    }
}

void inint2(void)
{
    init(array, src, SIZE);
    o2 = 0;
    o3 = 0;
}


int main(int argv, char** argc)
{   
    inint2();
    sort3(array, SIZE);

    
    inint2();
    _print(array, SIZE);
    sort4(array, SIZE);
    _print(array, SIZE);
    
    return 0;
}













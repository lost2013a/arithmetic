#include <stdio.h>

#define DBG printf


#define myarg(n) printf("%s = %d\n", #n, n)
#define myarg2(n,z) printf("%s = %d, %s = %d\n", #n, n, #z, z)


typedef int NODE;
#define SIZE 10

int array1[SIZE] = {1,2,5,7,9,0,3,4,6,8};
int array2[SIZE] = {1,2,5,7,9,0,3,4,6,8};


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
            if(p[i] > p[j]) {
                swap(&p[i], &p[j]);
            }
        }
    }
    _print(p, size);
}


//快速排序
void quick_sort(int s[], int l, int r)
{
    if (l < r)
    {
        //Swap(s[l], s[(l + r) / 2]); //将中间的这个数和第一个数交换 参见注1
        int i = l, j = r, x = s[l], tmp;
        while (i < j)
        {
            while(i < j && s[j] >= x) // 从右向左找第一个小于x的数
                j--;  
            
            while(i < j && s[i] < x) // 从左向右找第一个大于等于x的数
                i++;  
            if(i < j) {
                tmp = s[i];
                s[i] = s[j];
                s[j] = tmp;
            }
        }
        s[i] = x;
        quick_sort(s, l, i - 1); // 递归调用 
        quick_sort(s, i + 1, r);
    }
}

void qsort(int a[], int l, int r)
{
    int i = l, j = r, key = a[l], tmp;
    tmp = a[l];
    while(i < j) 
    {
        while(i<j && a[j] >= key ) //从右边开始找比key小的值。
            j--;
        while(i<j && a[i] < key) //从左开始找比key大的值。
            i++;

       if(i < j) {
        //swap
            tmp = a[i];
            a[j] = a[i];
            a[i] = tmp;
       }
    }
    //归为
    a[l] = a[i];
    a[i] = tmp;
    //递归
    qsort(a, l, i-1); //左半部分
    qsort(a, i+1, r); //右半部分
}



void sort2(int* a, int size)
{ 
	quick_sort(a,0,size-1);
    _print(a, size);
}




int main(int argv, char** argc)
{   
    sort1(array1, SIZE);
    sort2(array1, SIZE);
    return 0;
}















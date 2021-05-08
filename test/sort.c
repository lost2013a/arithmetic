#include <stdio.h>

#define DBG printf
#define SIZE 10
int array[SIZE]={1,3,5,7,9,0,2,4,6,8};

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
    for(i = 0; i < size; i++) {
        for(j = i+1; j < size; j++) {
            DBG("i,j = %d/%d\n", i, j);
            if(p[i] > p[j]) {
                swap(&p[i], &p[j]);
            }
        }
    }
}



int main(int argv, char** argc)
{
    _print(array, SIZE);
    sort1(array, SIZE);
    _print(array, SIZE);
    return 0;
}






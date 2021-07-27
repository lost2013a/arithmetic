#include <stdio.h>


int main()
{
    int cnt = 0;
    while(++cnt < 5)
    {
        sleep(1);
        printf("2 loop:%d\n",cnt);
    }
}
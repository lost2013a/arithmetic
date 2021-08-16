
#include <stdio.h>
#include <pthread.h>
#include "../kfifo.h"
#include <unistd.h>
#include <string.h>


DEFINE_KFIFO(g_fifobuf, unsigned char, 1024/*size必须是2的倍数*/); 
#define gbuf_in(p_data, len) kfifo_in(&g_fifobuf, p_data, len)
#define gbuf_out(p_data, len) kfifo_out(&g_fifobuf, p_data, len)
/*


*/


// DEFINE_KFIFO(g_fifobuf, int, 16);
#if 0
void* write_thread(void* arg) {
    for (int i = 0; i < 20; i++) {
        if (kfifo_is_full(&g_fifobuf)) {
            int x = 0;
            kfifo_out(&g_fifobuf, &x, 1);
            printf("discard %d\n", x);
        }
        if (kfifo_in(&g_fifobuf, &i, 1) == 1) {
            printf("write %d\n", i);
        } else {
            printf("write fail\n");
        }
    }
    printf("write over\n");
    return NULL;
}
#else
const char *str = "abcdefghigk";
void* write_thread(void* arg) {
    unsigned int wlen = (unsigned int)(strlen(str));
    while(1) {
//        if (kfifo_is_full(&g_fifobuf)) {
//            int x = 0;
//            kfifo_out(&g_fifobuf, &x, 1);
//            printf("discard %d\n", x);
//        }
        if (gbuf_in((unsigned char*)str, wlen) ==wlen) {
            //printf("write %d\n", i);
        } else {
            printf("write fail \n");
            usleep(10*1000);
        }
        usleep(10*1000);
    }
    //printf("write over\n");
    return NULL;
}
#endif

#if 0


void* read_thread(void* arg) {
    printf("read...\n");
    int x = 0;
    for (int i = 0; i < 8; i++) {
        if (kfifo_out(&g_fifobuf, &x, 1) == 1)
            printf("read %d\n", x);
        usleep(1*1000);
    }
    return NULL;
}
#else

void* read_thread(void* arg) {
    printf("read...\n");
    unsigned char x[100];
    while(1) {
        unsigned int len = gbuf_out(x, 100);
        if (len == strlen(str)) {
            //printf("read ok %s\n", (char*)x);
        }else if(len != 0) {
            printf("read err %d\n", len);
            
        }
        usleep(1*1000);
    }
    return NULL;
}

#endif


void test1(void) {
    pthread_t r, w;
    pthread_create(&w, NULL, write_thread, NULL);
    printf("..............\n");
    pthread_create(&r, NULL, read_thread, NULL);
    pthread_join(r, NULL);
    pthread_join(w, NULL);
}



int main() {
    INIT_KFIFO(g_fifobuf);
    
    int n = kfifo_avail(&g_fifobuf);
    printf("hello kfifo, avail: %d\n", n);
    test1();



    printf("over\n");
    return 0;
}

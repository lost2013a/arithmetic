#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
 
#define custom_count 1
#define produce_count 1
 
int nNum, nLoop;
int g_count = 0;
 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_MUTEX_INITIALIZER;
 
void *consume(void *arg)
{
	while (1)
	{
		pthread_mutex_lock(&mutex);
		while (g_count == 0)
		{
			printf("等待生产\n");
			pthread_cond_wait(&cond, &mutex); //阻塞在cond
		}
		g_count--;
		printf("消费\n");
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	pthread_exit(NULL);
}
 
void *produce(void *arg)
{
	while (1)
	{
		pthread_mutex_lock(&mutex);
		if (g_count >= 10)
		{
			printf("产品太多了，需要休眠5秒\n");
			pthread_mutex_unlock(&mutex);
			sleep(5);
			continue;
		}
		//不需要解锁再上锁，大于10，会解锁，会continue，不会执行下面语句
		g_count++;
		printf("生成%d\n", g_count);
		//printf("produce is %lu, g_count is %d\n", pthread_self(), g_count);
		pthread_cond_signal(&cond);	//通知wait  cond的线程，数据变化
		pthread_mutex_unlock(&mutex);
		usleep(100*1000);
	}
	pthread_exit(NULL);
}
 
int main()
{
	int i = 0;
	pthread_t tidCustom[custom_count];
	pthread_t tidProduce[produce_count];
 
	//创建消费者线程
	for (i = 0; i < custom_count; i++)
	{
		pthread_create(&tidCustom[i], NULL, consume, NULL);
	}
	sleep(3);
	//创建生产者线程
	for (i = 0; i < produce_count; i++)
	{
		pthread_create(&tidProduce[i], NULL, produce, NULL);
	}
	
	//等待消费者线程
	for (i = 0; i < custom_count; i++)
	{
		pthread_join(tidCustom[i], NULL);
	}
 
	//等待生产者线程
	for (i = 0; i < produce_count; i++)
	{
		pthread_join(tidProduce[i], NULL);
	}
	printf("parent exit\n");
	exit(0);
}

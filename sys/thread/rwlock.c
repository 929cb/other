#include <stdio.h>
#include <pthread.h>	
#include <unistd.h>
#include <string.h>

int num;
pthread_rwlock_t rwlock;

void* thread_r_hand(void* arg)
{
	while(1)
	{
		pthread_rwlock_rdlock(&rwlock);
		printf("%dr: %d\n", (int)arg, num);
		pthread_rwlock_unlock(&rwlock);
		sleep(2);
	}
	return NULL;
}

void* thread_w_hand(void* arg)
{
	while(1)
	{	
		pthread_rwlock_wrlock(&rwlock);
		num++;
		sleep(1);
		printf("%dw: %d\n", (int)arg, num);
		pthread_rwlock_unlock(&rwlock);
		sleep(1);
	}
	return NULL;
}

int main()
{
	pthread_t tid;
	int ret;

	pthread_rwlock_init(&rwlock, NULL);

	for(int i = 0; i < 3; i++)
	{	
		if(pthread_create(&tid, NULL, thread_w_hand, (void*)i) != 0)
		{
			perror("fail to pthread_create");
			return 1;
		}
	}
	
	for(int i = 0; i < 5; i++)
	{	
		if(pthread_create(&tid, NULL, thread_r_hand, (void*)i) != 0)
		{
			perror("fail to pthread_create");
			return 1;
		}
	}

	for(int i = 0; i < 8; i++)
	{
		if((ret = pthread_join(tid, NULL)) != 0)
		{
			printf("pthread_join: %s\n", strerror(ret));
			return 1;
		}
	}

	pthread_rwlock_destroy(&rwlock);


	return 0;
}

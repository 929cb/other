#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex;

void* thread_hand(void* arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		printf("hello ");
		sleep(rand() % 2);
		printf("world\n");
		pthread_mutex_unlock(&mutex);
		sleep(rand() % 2);
	}
}

int main()
{
	pthread_t tid;

	srand(time(NULL));
		
	pthread_mutex_init(&mutex, NULL);

	if(pthread_create(&tid, NULL, thread_hand, NULL) != 0)
	{
		perror("fail to pthread_create");
		return 0;
	}

	while(1)
	{
		pthread_mutex_lock(&mutex);
		printf("HELLO ");
		sleep(rand() % 2);
		printf("WORLD\n");
		pthread_mutex_unlock(&mutex);
		sleep(rand() % 2);
	}

	pthread_join(tid, NULL);

	pthread_mutex_destroy(&mutex);

	return 0;
}

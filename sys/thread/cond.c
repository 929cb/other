#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

struct msg
{
	int num;
	struct msg* next;
};

struct msg* head = NULL;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* product(void* arg)
{
	struct msg* msg;

	for(;;)
	{
		msg = (struct msg*)malloc(sizeof(struct msg));
		msg->num = rand() % 100 + 1;
		printf("product: %d\n", msg->num);

		pthread_mutex_lock(&mutex);
		msg->next = head;
		head = msg;
		pthread_mutex_unlock(&mutex);

		pthread_cond_signal(&cond);	//	通知条件达到

		sleep(rand() % 5);
	}
	return NULL;
}

void* consume(void* arg)
{
	struct msg* msg;

	for(;;)
	{
		pthread_mutex_lock(&mutex);

		//	多个消费者需要用while判断，因为可能里面数据会被其他消费者拿掉，造成没有数据可读

		//	阻塞等待条件的同时会解锁， 等到条件会重新加锁
		if(head == NULL) pthread_cond_wait(&cond, &mutex);

		msg = head;
		head = head->next;
		pthread_mutex_unlock(&mutex);

		printf("consume: %d\n", msg->num);
		free(msg);
		sleep(rand() % 5);
	}
	return NULL;
}

int main()
{
	pthread_t tid1;
	pthread_t tid2;

	srand(time(NULL));

	if(pthread_create(&tid1, NULL, product, NULL) != 0)
	{
		perror("fail to pthread_create p");
		return 1;
	}

	if(pthread_create(&tid2, NULL, consume, NULL) != 0)
	{
		perror("fail to pthread_create r");
		return 1;
	}

	if(pthread_join(tid1, NULL) != 0 || pthread_join(tid2, NULL) != 0)
	{
		perror("fail to pthread_join");
		return 1;
	}

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);

	return 0;
}

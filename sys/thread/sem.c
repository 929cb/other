#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

struct msg
{
	int num;
	struct msg* next;
};

sem_t sem;

struct msg* head = NULL;

void* product(void* arg)
{
	struct msg* msg;

	for(;;)
	{
		msg = (struct msg*)malloc(sizeof(struct msg));
		msg->num = rand() % 100 + 1;

		sem_wait(&sem);

		msg->next = head;
		head = msg;

		sem_post(&sem);

		printf("product: %d\n", msg->num);

		sleep(rand() % 5);
	}
}

void* consume1(void* arg)
{
	struct msg* msg;

	for(;;)
	{
		while(1)
		{
			sem_wait(&sem);

			if(head != NULL)
			{
				break;
			}

			sem_post(&sem);
			sleep(1);
		}

		msg = head;
		head = head->next;

		sem_post(&sem);

		printf("consume: %ld\t%d\n", pthread_self(), msg->num);
		free(msg);

		sleep(rand() % 5);
	}

	return NULL;
}

void* consume2(void* arg)
{
	struct msg* msg;

	for(;;)
	{
		while(1)
		{
			sem_wait(&sem);

			if(head != NULL) break;

			sem_post(&sem);
			sleep(1);
		}

		msg = head;
		head = head->next;

		sem_post(&sem);

		printf("consume: %ld\t%d\n", pthread_self(), msg->num);
		free(msg);

		sleep(rand() % 5);
	}

	return NULL;
}

int main()
{
	pthread_t tid1, tid2, tid3;

	sem_init(&sem, 0, 1);

	if(pthread_create(&tid1, NULL, consume1, NULL) != 0 || 
			pthread_create(&tid2, NULL, consume1, NULL) != 0)
	{
		perror("fail to pthread_create");
		return 1;
	}

	if(pthread_create(&tid3, NULL, product, NULL) != 0)
	{
		perror("fail to pthread_create");
		return 1;
	}

	pthread_t tid[3] = {tid1, tid2, tid3};

	for(int i = 0; i < 3; i++)
	{
		if(pthread_join(tid[i], NULL) != 0)
		{
			perror("fail to pthread_join");
			return 1;
		}
	}

	sem_destroy(&sem);

	return 0;
}

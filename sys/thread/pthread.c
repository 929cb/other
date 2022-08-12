#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread_hand(void* arg)
{
	if((int)arg == 2) pthread_exit(0);

	printf("---%d---\n",(int)arg);
}

int main()
{
	pthread_t tid;

	printf("pid = %d, 线程号 = %lu\n", getpid(), pthread_self());
	int i;
	for(i = 1; i <= 5; i++)
	{
		if(pthread_create(&tid, NULL, thread_hand, (void*)i) != 0)
		{
			perror("fail to pthread_create");
			return 1;
		}
	}

	sleep(1);
	//	pthread_exit(NULL);
	return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct thrd
{
	int val;
	char str[256];
};

void* thread_hand(void* arg)
{
	struct thrd* trd;
	trd = (struct thrd*)malloc(sizeof(struct thrd));
	trd->val = 10;
	strcpy(trd->str, "hello thrd");

	//return (void*)trd;
	pthread_exit((void*)trd);
}

int main()
{
	struct thrd* td;
	pthread_t tid;

	if(pthread_create(&tid, NULL, thread_hand, NULL) != 0)
	{
		perror("fail to pthread_creadte");
		return 1;
	}
	printf("tid = %ld\n", tid);

	int ret = pthread_join(tid, (void**)&td);

	if(ret != 0)
	{
		if(ret == EDEADLK) printf("线程发生死锁\n");
		else if(ret == EINVAL) printf("要么目标线程本身不允许其它线程获取它的返回值，要么事先就已经有线程调用 pthread_join() 函数获取到了目标线程的返回值");
		else if(ret == ESRCH) printf("找不到指定的线程\n");

		perror("fail to pthread_join");
	//	    fprintf(stderr, "pthread_join: %s", strerror(ret));
		return 1;
	}

	printf("%d\t%s\n", td->val, td->str);

	free(td);

	return 0;
}

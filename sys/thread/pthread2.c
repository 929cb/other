#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* thread_hand(void* arg)
{
	return NULL;
}

//	pthread_cancel()取消线程，注意需要取消点（可以理解为能进内核就找到了取消点）
//	pthread_testcancel()设置取消点

int main()
{
	pthread_t tid;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	//	设置线程分离属性，也可创建后设置分离pthread_detach() (分离后线程结束会自动回收)
	if(pthread_create(&tid, &attr, thread_hand, NULL) != 0)
	{
		perror("fail to pthread_create");
		return 1;
	}

	pthread_attr_destroy(&attr);

	return 0;
}

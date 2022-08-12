#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main()
{
	sigset_t sigset, pendset;

	sigemptyset(&sigset);	//	清空信号集
	sigaddset(&sigset, SIGINT);	//	设置信号集位图位2为1

	if(sigprocmask(SIG_BLOCK, &sigset, NULL) == -1)	//	设置阻塞信号集位2阻塞
	{
		perror("fail to sigprocmask");
		return 1;
	}

	while(1)
	{
		if(sigpending(&pendset) == -1)	//	获取未决信号集位图状态
		{
			perror("fail to sigpending");
			return 1;
		}

		for(int i = 1; i < 32; i++)
		{
			if(sigismember(&pendset, i)) putchar('1');	//	判断位图对应位是否为1
			else putchar('0');
		}
		printf("\n");

		sleep(1);
	}


	while(1);

	return 0;
}

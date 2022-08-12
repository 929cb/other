#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void catch_child(int signo)
{
	pid_t wpid;
	
	//	由于屏蔽字，在函数调用期间不管收到多少个SIGCHLD，在函数结束后只当一个处理，就会造出僵尸进程，因此需要用while循环判断是否已经将当前已死亡的所有进程都回收调
	while((wpid = wait(NULL)) != -1)	
	{
		printf("***catch_child = %d\n", wpid);
	}
}

int main()
{
	pid_t pid;
	int i;

	for(i = 1; i <= 10; i++)
	{
		if((pid = fork()) == 0) break;
	}

	if(i == 11)
	{
		struct sigaction act;
		
		//	先阻塞，防止还没来得及注册回调函数，子进程先死光了
		sigset_t sigset;
		sigemptyset(&sigset);
		sigaddset(&sigset, SIGINT);	//	SIGINT为了测试屏蔽字是否作并集处理
		sigaddset(&sigset, SIGCHLD);
		sigprocmask(SIG_BLOCK, &sigset, NULL);
		act.sa_handler = catch_child;
		sigemptyset(&act.sa_mask);	//	屏蔽字在进入回调函数后，会和当前的阻塞信号集作并集处理，在结束回调函数后会恢复当前阻塞信号集状态
		act.sa_flags = 0;	//	设置默认属性，本信号自动屏蔽
		
		sigaction(SIGCHLD, &act, NULL);
		//	注册完解除阻塞
		sigprocmask(SIG_UNBLOCK, &sigset, NULL);

		printf("parent = %d\n", getpid());
		sleep(5);
		while(1);
	}
	else
		printf("child = %d\n", getpid());

	return 0;
}

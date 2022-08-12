#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	pid_t pid, wpid;
	int status;

	pid = fork();
	if(pid == -1)
	{
		perror("fail to fork");
		return 1;
	}
	else if(pid == 0)
	{
		printf("child\n");
		sleep(2);
		printf("child die\n");
		return 73;
	}
	else if(pid > 0)
	{
		wpid = wait(&status);
		if(wpid == -1)
		{
			perror("fail to wait");
			return 1;
		}
		if(WIFEXITED(status))	//	为真说明子进程正常退出
		{
			printf("status=%d\n", WEXITSTATUS(status));
		}
		else if(WIFSIGNALED(status))	//	判断是否因为信号
		{
			printf("status=%d\n", WTERMSIG(status));
		}
	}

	return 0;
}

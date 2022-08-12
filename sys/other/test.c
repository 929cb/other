#include <stdio.h>
#include <unistd.h>

int main()
{
	int i = 10, k = 20;

	printf("你好\n");

	pid_t pid = fork();
	
	int j = 30;

	if(pid == -1)
	{
		perror("fail to fork");
		return 0;
	}
	else if(pid == 0)
	{
		//execlp("ls", "ls", NULL);
		printf("父进程id=%d 子进程代码区\t%d\t%d\n",getppid(), ++i, j);
		sleep(1);
	}
	else if(pid > 0)
	{
		printf("子进程id=%d 父进程代码区\t%d\t%d\n", pid, ++k, ++j);
		sleep(1);
	}

	printf("pid = %d --> i=%d\tk=%d\tj=%d\n",getpid(), i, k, j);

	return 0;
}

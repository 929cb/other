#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main()
{
	int pipe_fd[2];

	pipe(pipe_fd);

	pid_t pid;

	if((pid = fork()) == -1)
	{
		perror("field to create process\n");
	}
	else if(pid == 0)
	{
		printf("子进程代码区\n");

		while(1)
		{
			char buf[32] = "";
			ssize_t size;

			size = read(pipe_fd[0], buf, sizeof(buf) - 1);
			buf[strlen(buf) - 1] = '\0';

			printf("buf=[%s]	%ld\n", buf, size - 1);

		}
	}
	else if(pid > 0)
	{
		printf("父进程代码区\n");
		
		while(1)
		{
			char buf[31] = "";
			fgets(buf, sizeof(buf), stdin);
			write(pipe_fd[1], buf, strlen(buf));
	
		}
	}

	return 0;
}

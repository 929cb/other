#include <stdio.h>
#include <unistd.h>

int main()
{
	int pipe_fd[2];
	pid_t pid;

	if(pipe(pipe_fd) == -1)
	{
		perror("fail to pipe");
		return 1;
	}

	if((pid = fork()) == -1)
	{
		perror("fail to fork");
		return 1;
	}
	else if(pid == 0)
	{
		close(pipe_fd[1]);
		char buf[128] = "";
		if(read(pipe_fd[0], buf, 128) != 0)
		{
			printf("%s\n", buf);
		}
		close(pipe_fd[0]);
	}
	else if(pid > 0)
	{
		close(pipe_fd[0]);
		char buf[128] = "";
		fgets(buf, 128, stdin);
		write(pipe_fd[1], buf, 128);
		close(pipe_fd[1]);
	}

	return 0;
}

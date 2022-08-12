#include <stdio.h>
#include <unistd.h>

int main()
{
	pid_t pid;
	int pipe_fd[2];

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
		close(pipe_fd[0]);
		dup2(pipe_fd[1], 1);
		close(pipe_fd[1]);
		execl("/bin/ls", "ls", NULL);
		perror("fail to execl");
		return 1;
	}
	else if(pid > 0)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], 0);
		close(pipe_fd[0]);
		execlp("wc", "wc", "-l", NULL);
		perror("fail to execlp");
		return 1;
	}
	
	return 0;
}

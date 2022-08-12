#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 2

int main()
{
	pid_t pid;
	int pipe_fd[2];
	int i;

	if(pipe(pipe_fd) == -1)
	{
		perror("fail to pipe");
		return 1;
	}

	for(i = 1; i <= N; i++)
	{
		if((pid = fork()) == -1)
		{
			perror("fail to fork");
			return i;
		}
		else if(pid == 0) break;
	}

	if(i > N)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		int k = N;
		while(k--) wait(NULL);
	}
	else if(i == 1)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], 1);
		close(pipe_fd[1]);
		execlp("ls", "ls", NULL);
		perror("fail to execlp ls");
		return 1;
	}
	else if(i == 2)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], 0);
		close(pipe_fd[0]);
		execlp("wc", "wc", "-l", NULL);
		perror("fail to execlp wc");
		return 1;
	}


	return 0;
}

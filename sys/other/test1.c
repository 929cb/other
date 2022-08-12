#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	pid_t pid;
	int i;

	for(i = 1; i <= 5; i++)
	{
		if(fork() == 0) break;
	}

	wait(NULL);
	printf("%d\n", i);

	return 0;
}

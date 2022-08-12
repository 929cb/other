#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int fd = open("1.txt", O_WRONLY | O_CREAT, 0664);
	if(fd == -1)
	{
		perror("fail to open");
		return 1;
	}

	dup2(fd, 1);

	execlp("ps", "ps", "aux", NULL);

	return 0;
}

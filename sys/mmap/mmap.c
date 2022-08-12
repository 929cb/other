#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int main()
{
	pid_t pid;
	int fd;
	char* p;

	char* ch = "hello world!";
		
				//	/dev/zero	/dev/null
	if((fd = open("testmman", O_RDWR | O_CREAT, 0664)) == -1)
	{
		perror("fail to open");
		return 1;
	}

	unlink("testmman");

	if(ftruncate(fd, 128) == -1)
	{
		perror("fail to ftruncate");
		return 1;
	}

	if((p = (char*)mmap(NULL, 128, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
		perror("fail to mmap");
		return 1;
	}
	
	sleep(5);

	close(fd);


	if((pid = fork()) == -1)
	{
		perror("fail to fork");
		return 1;
	}
	else if(pid == 0)
	{
		strcpy(p, ch);

		sleep(10);

		printf("child\t%s\n", p);
	}
	else if(pid > 0)
	{
		printf("parent\t%s\n", p);
		wait(NULL);

		if(munmap(p, 128) == -1)
		{
			perror("fail to munmap");
			return 1;
		}
	}
	
	return 0;
}

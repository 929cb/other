#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	if(mkfifo("fifo1", 0664) == -1)
	{
		if(errno != EEXIST)
		{
			perror("fail to mkfifo1");
			exit(1);
		}
	}

	if(mkfifo("fifo2", 0664) == -1)
	{
		if(errno != EEXIST)
		{
			perror("fail to mkfifo2");
			exit(1);
		}
	}

	printf("我是1号选手\n");

	char buf[1024] = "";
	pid_t pid;
	if((pid = fork()) == -1)
	{
		perror("fail to fork");
		exit(1);
	}
	else if(pid == 0)
	{
		//	子进程读
		int fd_r;

		if((fd_r = open("fifo2", O_RDONLY)) == -1)
		{
			perror("fail to open_r");
			exit(1);
		}

		ssize_t size;

		while(1)
		{
			memset(buf, 0, sizeof(buf));
			if((size = read(fd_r, buf, sizeof(buf))) == -1)
			{
				perror("fail to read");
				exit(1);
			}

			if(size > 0) printf("来自2号选手的消息: %s\n", buf);
		}
	}
	else if(pid > 0)
	{
		//	父进程写
		int fd_w;

		if((fd_w = open("fifo1", O_WRONLY)) == -1)
		{
			perror("fail to open_w");
			exit(1);
		}
		
		while(1)
		{
			memset(buf, 0, sizeof(buf));
			fgets(buf, sizeof(buf), stdin);
			buf[strlen(buf) - 1] = '\0';

			if(write(fd_w, buf, strlen(buf)) == -1)
			{
				perror("fail to write");
				exit(1);
			}

			usleep(50000);
		}

	}
		
}

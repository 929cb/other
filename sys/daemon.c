#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

/* 创建守护进程 */
int main()
{
	pid_t pid;
	int fd;

	if((pid = fork()) == -1)
	{
		perror("fail to fork");
		return 1;
	}
	else if(pid > 0) return 0;

	//	子进程创建新会话（只有子进程能创建，创建后自己成为组长和会长）
	if((pid = setsid()) == -1)
	{
		perror("fail to setsid");
		return 1;
	}

	if(chdir("/home/cb/sys") == -1)	//	改变工作目录
	{
		perror("fail to chdir");
		return 1;
	}

	umask(0022);	//	改变文件权限掩码

	close(STDIN_FILENO);
	if((fd = open("/dev/null", O_RDWR)) == -1)
	{
		perror("fail to open");
		return 1;
	}
	dup2(fd, STDOUT_FILENO);	//	关闭/重定向文件描述符
	dup2(fd, STDERR_FILENO);

	while(1);

	return 0;
}

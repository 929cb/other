#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
	if(argc < 3)
	{
		fprintf(stderr, "%s 源文件 目标文件\n", *argv);
		return 1;
	}

	int fd_r = open(*(argv + 1), O_RDONLY);
	if(fd_r == -1)
	{
		perror("源文件错误");
		return 1;
	}

	int fd_w = open(*(argv + 2), O_CREAT | O_WRONLY, 0664);
	if(fd_w == -1)
	{
		perror("拷贝文件错误");
		return 1;
	}

	char buf[1024] = "";
	ssize_t bytes = 0;
	do
	{
		bytes = read(fd_r, buf, sizeof(buf));
		write(fd_w, buf, bytes);
	}while(bytes != 0);

	close(fd_w);
	close(fd_r);

	return 0;
}

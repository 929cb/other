#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

int main()
{
	char* p;

	//	MAP_ANONYMOUS创建匿名映射区，不需要打开文件不需要扩充文件容量
	if((p = mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
	{
		perror("fail to mmap");
		return 1;
	}

	printf("%s\n", p);
	
	munmap(p, 4);

	return 0;
}

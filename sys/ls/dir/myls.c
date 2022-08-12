#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

int swap(const char* dest, const char* src)
{
	return (*dest - *src);
}

void my_sort(char* arr[], int len)
{
	int i, j;
	char* t;

	for(i = 1; i < len; i++)
	{
		for(j = 0; j < len - i; j++)
		{
			if(swap(arr[j], arr[j + 1]) > 0)
			{
				t = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = t;	
			}
		}
	}
}

int my_strcmp(const char* dest, const char* src)
{
	while(*dest == *src && *dest)
		dest++, src++;

	if(*dest == '\0' && *src == '\0') return 0;

	return *dest - *src;
}

void recursion(const char* path)
{
	DIR* dp = opendir(path);
	if(!dp)
	{
		perror("--fail to opendir");
		exit(1);
	}
printf("%s\n", path);
	struct dirent* sdp = NULL;
	struct stat s = {0};
	
	while((sdp = readdir(dp)) != NULL)
	{
		if(my_strcmp(sdp->d_name, ".") == 0 || my_strcmp(sdp->d_name, "..") == 0) continue;

		char *newpath;
		// newpath = getcwd(NULL, 0);
		//printf("%s\n", sdp->d_name);
		sprintf(newpath, "%s%s", path, sdp->d_name);
		//printf("%s\n", newpath);
		if(stat(newpath, &s) == -1)
		{
			perror("fail to lstat");
			exit(1);
		}
		if(S_ISDIR(s.st_mode) == 1)
		{
			//char buf[256] = "";
			printf("\n");
			sprintf(newpath, "%s/", newpath);
			recursion(newpath);
		}
		else
		{
			printf("%s\t%ld\n", sdp->d_name, s.st_size);
		}
	}

	closedir(dp);
}

int main(int argc, char const *argv[])
{
	DIR* dp = NULL;
	struct dirent* sdp = NULL;

	int i = 0;

	if(argc == 1)
	{
		i++;
		dp = opendir("./");
	}
	else if(argc == 2)
	{
		if(my_strcmp("-r", argv[1]) == 0)
		{
		//printf("%s\n", argv[1]);
			recursion("./");
			return 0;
		}
		dp = opendir(argv[++i]);
	}
	else if(argc == 3)
	{
		if(my_strcmp("-r", argv[1]) == 0)
		{
		//printf("%s\n", argv[1]);
			char* path;
			path = getcwd(NULL, 0);
			sprintf(path, "%s/%s", path, argv[2]);
			recursion(path);
			return 0;
		}

		i = 0;
		a:dp = opendir(argv[++i]);
	}

	if(!dp)
	{
		perror("*fail to opendir");
		return 1;
	}

	if(argc >= 3) printf("%s:\n", argv[i]);

	int factor = 1;
	int index = 0;
	char** arr = (char**)malloc(10 * 125500);
	if(!arr)
	{
		perror("fail to malloc");
		return 1;
	}

	while((sdp = readdir(dp)))
	{
		if(index >= 10 * factor)
		{
			void* p = realloc(arr, (++factor) * 1255);
			if(!p)
			{
				perror("fail to realloc");
				return 1;
			}
			arr = (char**)p;
		}
		if(sdp->d_name[0] == '.') continue;
		arr[index++] = sdp->d_name;
	}

	my_sort(arr, index);

	for(int i = 0; i < index; i++)
	{
		//if(arr[i][0] == '.') continue;
		printf("%s\t", arr[i]);
		if((i + 1) % 6 == 0) printf("\n");
	}
	printf("\n");

	free(arr);

	closedir(dp);

	if(argc == 3 && i < 2) goto a;

	return 0;
}

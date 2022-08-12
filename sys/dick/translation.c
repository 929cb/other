#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>

int my_strcmp(const char* dest, const char* src)
{
    assert(dest && src);

    while(*dest && *dest++ == *src++);
	
	if(*dest == '\0' && *src) return *dest - *src;

    return (*(--dest) - *(--src));
}

int  my_strncmp(const char* dest, const char* src, int n)
{
    assert(dest && src);

    while(*dest == *src)
    {
        if(--n == 0) return 0;

        dest++;
        src++;
    }

    return (*dest - *src);
}

int my_strlen(const char* str)
{
    assert(str);

    int count = 0;

    while(*str++) count++;

    return count;
}

int bf(const char* dest, const char* src)   
{
    assert(dest && src);

    char* d = (char*)dest;
    char* s = (char*)src;

    while(*dest && *src)
    {
        if(*dest == *src)
        {
            dest++;
            src++;
        }
        else
        {
            dest = ++d;
            src = s;
        }
    }

    if(*src == '\0')
        return 0;
    else return -1;
}

int main(int argc, char const *argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "%s [*]单词\n", *argv);
        return 1;
    }

    char obscure = 0;
    //int len = my_strlen(argv[1]);
    if(my_strncmp("*", argv[1], 1) == 0)
    {
        obscure = 1;
        (argv[1])++;
    }
    else if(my_strncmp("-l", argv[1], 2) == 0)
    {
        obscure = 2;
    }
    

    FILE* fp = fopen("/home/cb/sys/dick/dick", "r");
    if(!fp)
    {
        perror("fail to fopen");
        return 1;
    }

    char en[45] = "";
    char cn[128] = "";
    ssize_t bytes = 0;
    char ret = -1;
    int num = 1;
	char flag = 0;

    while((bytes = fscanf(fp, "%s %s\n", en, cn)) != EOF)
    {
        if(obscure == 2)
        {	
			if(!flag)
			{
				printf("		%s  %s\n", en, cn);
				flag = 1;
				continue;
			}
            printf("%d) %s --> %s\n", num++, en, cn);
        }
        else if(obscure == 1)
        {
            ret = bf(en, argv[1]);

            if(ret == 0)
            {
                printf("%d) %s --> %s\n", num++, en, cn);
            }
        }
        else if(obscure == 0)
        {
            if(my_strlen(en) != my_strlen(argv[1])) continue;

            ret = my_strcmp(en, argv[1]);  

            if(ret == 0)
            {
                printf("%s\n", cn);
                break;
            }
        }
    }

    fclose(fp);


    return 0;
}

// #include <stdio.h>
// #include <sys/socket.h>
// // #include <sys/types.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>

// int main(int n, char const *c[])    //  tcp客户端
// {
//     if(n < 3)
//     {
//         fprintf(stderr, "%s ip port\n", c[0]);
//         return 1;
//     }

//     int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
//     if(sockfd == -1)
//     {
//         perror("create socket");
//         return 1;
//     }

//     printf("%d\n", sockfd);


//     struct sockaddr_in serveraddr;
//     serveraddr.sin_family = AF_INET;
//     serveraddr.sin_addr.s_addr = inet_addr(c[1]);
//     serveraddr.sin_port = htons(atoi(c[2]));

//     if(connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1)
//     {
//         perror("fail to connect");
//         return 1;
//     }

//     char buf[128];

//     while(1)
//     {
//         memset(buf, 0, sizeof(buf));

//         fgets(buf, sizeof(buf), stdin);
//         buf[strlen(buf) - 1] = '\0';

//         if(send(sockfd, buf, strlen(buf), 0) == -1) //  0表示阻塞
//         {
//             perror("fail to send");
//             return 1;
//         }
//     }

//     close(sockfd);

//     return 0;
// }


// #include <stdio.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>

// int main(int n, char const *c[])
// {
//     if(n < 3)
//     {
//         fprintf(stderr, "依次输入 %s ip port\n", c[0]);
//         return 1;
//     }

//     int sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if(sockfd == -1)
//     {
//         perror("create socket");
//         return 1;
//     }

//     struct sockaddr_in serveraddr;
//     serveraddr.sin_family = AF_INET;
//     serveraddr.sin_addr.s_addr = inet_addr(c[1]);
//     serveraddr.sin_port = ntohs(atoi(c[2]));

//     if(bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1)
//     {
//         perror("fail to bind");
//         return 1;
//     }

//     if(listen(sockfd, 10) == -1)
//     {
//         perror("fial to listen");
//         return 1;
//     }

//     int acceptfd1;
//     struct sockaddr_in clientaddr1 = {0};
//     int len = sizeof(clientaddr1);
//     if((acceptfd1 = accept(sockfd, (struct sockaddr*)&clientaddr1, &len)) == -1)
//     {
//         perror("fail to accept");
//         return 1;
//     }

//     printf("%s [%d]\n", inet_ntoa(clientaddr1.sin_addr), ntohs(clientaddr1.sin_port));

//     char buf[128] = "";
//     while(1)
//     {
//         memset(buf, 0, sizeof(buf));

//         if(recv(acceptfd1, buf, sizeof(buf), 0) == -1)
//         {
//             perror("fail to recv");
//             return 1;
//         }

//         printf("%s\n", buf);
//     }

//     close(acceptfd1);

//     close(sockfd);

//     return 0;
// }


// #include <stdio.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <stdlib.h>
// #include <signal.h>
// #include <sys/wait.h>
// #include <unistd.h>
// #include <string.h>

// void sighand(int sig)
// {
//     //  wait()会暂时停止目前进程的执行, 直到有信号来到或子进程结束. 如果在调用wait()时子进程已经结束, 则wait()会立即返回子进程结束状态值.
//     //  子进程的结束状态值会由参数status 返回, 而子进程的进程识别码也会一快返回. 如果不在意结束状态值, 则参数 status 可以设成NULL. 
//     wait(NULL);
//     printf("The client quited\n\n");
// }

// /* 并发-多进程实现一个tcp服务端支持多个tcp客户端 */
// int main(int n, char const *c[])
// {
//     if(n < 3)
//     {
//         fprintf(stderr, "%s ip port\n", *c);
//         return 1;
//     }

//     int sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if(sockfd == -1)
//     {
//         perror("create socket");
//         return 1;
//     }

//     //  将套接字设置为允许重复使用本机地址或设置为端口复用(这可不设置)
//     int on = 1;
//     if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
//     {
//         perror("fail to setsockopt");
//         return 1;
//     }

//     struct sockaddr_in serveraddr;
//     serveraddr.sin_family = AF_INET;
//     serveraddr.sin_addr.s_addr = inet_addr(c[1]);
//     serveraddr.sin_port = ntohs(atoi(c[2]));

//     if(bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1)
//     {
//         perror("fail to bind");
//         return 1;
//     }

//     if(listen(sockfd, 10) == -1)
//     {
//         perror("fail to listen");
//         return 1;
//     }

//     //  子进程终止时会向父进程发送SIGCHLD信号，告知父进程回收自己
//     //  但该信号的默认处理动作为忽略，因此父进程仍然不会去回收子进程，需要捕捉处理实现子进程的回收；
//     signal(SIGCHLD, sighand);

//     int acceptfd;
//     struct sockaddr_in clientaddr;
//     int len = sizeof(clientaddr);

//     pid_t pid;

//     while(1)
//     {
//         if((acceptfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len)) == -1)
//         {
//             perror("fail to accept");
//             return 1;
//         }

//         pid = fork();

//         if(pid == -1)
//         {
//             perror("fail to fork");
//             return 1;
//         }        
//         else if(pid > 0) ;
//         else if(pid == 0)
//         {
//             char buf[128] = "";
//             int bytes = 0;

//             while(1)
//             {
//                 if((bytes = recv(acceptfd, buf, sizeof(buf), 0)) == -1)
//                 {
//                     perror("fail to recv");
//                     return 1;
//                 }
//                 else if(bytes == 0) //  如果recv函数在等待协议接收数据时网络中断了，那么它返回0
//                 {
//                     printf("%s [%d]客户端即将关闭...\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
                    
//                     close(acceptfd);
//                     return 0;
//                 }
//                 printf("%s [%d]\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

//                 printf("%s\n", buf);
//                 memset(buf, 0, sizeof(buf));
//             }
//         }
//     }

//     close(sockfd);

//     return 0;
// }



// #include <stdio.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <stdlib.h>
// #include <pthread.h>
// #include <unistd.h>
// #include <string.h>

// typedef struct PARAM
// {
//     int acceptfd;
//     struct sockaddr_in clientaddr;
// }Param;

// void* thread_fun(void* arg)
// {
//     char buf[128] = "";
//     int bytes = 0;

//     while(1)
//     {
//         if((bytes = recv(((Param*)arg)->acceptfd, buf, sizeof(buf), 0)) == -1)
//         {
//             perror("fail to recv");
//             pthread_exit("fail to recv");
//         }    
//         else if(bytes == 0)
//         {
//             printf("%s [%d] client quited\n\n", inet_ntoa(((Param*)arg)->clientaddr.sin_addr), ntohs(((Param*)arg)->clientaddr.sin_port));
//             pthread_exit("network not connect");
//         }

//         if(strcmp(buf, "quit") == 0)
//         {
//             printf("%s [%d] client quited\n\n", inet_ntoa(((Param*)arg)->clientaddr.sin_addr), ntohs(((Param*)arg)->clientaddr.sin_port));
//             pthread_exit(NULL);
//         }

//         printf("%s [%d] :\n", inet_ntoa(((Param*)arg)->clientaddr.sin_addr), ntohs(((Param*)arg)->clientaddr.sin_port));

//         printf("%s\n", buf);
//         //fflush(stdout);

//         memset(buf, 0, sizeof(buf));
//     }
// }


// /* 并发-多线程实现一个tcp服务端服务多个tcp客户端 */
// int main(int n, char const *c[])
// {
//     if(n < 3)
//     {
//         fprintf(stderr, "%s ip port\n", *c);
//         return 1;
//     }

//     int sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if(sockfd == -1)
//     {
//         perror("fail to socket");
//         return 1;
//     }

//     struct sockaddr_in serveraddr;
//     serveraddr.sin_family = AF_INET;
//     serveraddr.sin_addr.s_addr = inet_addr(c[1]);
//     serveraddr.sin_port = ntohs(atoi(c[2]));

//     if(bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1)
//     {
//         perror("fail to bind");
//         return 1;
//     }

//     if(listen(sockfd, 10) == -1)
//     {
//         perror("fail to listen");
//         return 1;
//     }


//     Param param;
//     int len = sizeof(struct sockaddr_in);

//     while(1)
//     {
//         if((param.acceptfd = accept(sockfd, (struct sockaddr*)&(param.clientaddr), &len)) == -1)
//         {
//             perror("fail to accept");
//             return 1;
//         }
        
//         pthread_t tid;

//         pthread_create(&tid, NULL, thread_fun, &param);

//         pthread_detach(tid);    //  设置子线程为分离态，即子线程执行完就立即释放占用的系统资源
//     }

//     close(sockfd);

//     return 0;
// }



#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <errno.h>

#define ERROR(arg)      do                                                                  \
                        {                                                                   \
                            perror(arg);                                                    \
                            printf("%s - %s - %d\n", __FILE__, __FUNCTION__, __LINE__);     \
                            exit(1);                                                        \
                        }while(0)

void* thread_fun(void* arg)
{
    int acceptfd = *(int*)arg;
    char buf[128] = "";                         /* 告诉浏览器文件类型text/html */ 
    char head[] = "HTTP/1.1 200 OK\r\n"         \
                "Content-Type: image/jpeg\r\n"  \
                "\r\n";
    char err[] = "HTTP/1.1 404 Not Found\r\n"   \
                "Content-Type: text/html\r\n"   \
                "\r\n"                          \
                "<HTML><BODY> File not found <BODY><HTML>";
    
    //  接收浏览器通过http协议发送的数据包
    if(recv(acceptfd, buf, sizeof(buf), 0) == -1)
    {
        ERROR("fail to recv");
    }
    printf("*******************************************\n");
    printf("%s\n", buf);
    printf("*******************************************\n");

    //  通过获取的数据包中得到浏览器需要的网页文件名
    //  GET /文件名 http/1.1
    char filename[128] = "";
    sscanf(buf, "GET /%s", filename);   //sscanf遇空格也会结束

    // if(strncmp(filename, "HTTP/1.1", strlen("http/1.1")) == 0)
    // {
    //     strcpy(filename, "index.html");
    // }

    printf("filename = %s\n", filename);

    char path[128] = "./web/";
    strcat(path, filename);

    //  通过解析出来的网页文件名，查找服务器本地有没有该文件
    int fd;
    if((fd = open(path, O_RDONLY)) == -1)
    {
        //  如果文件不存在，则发送对应指令
        if(errno == ENOENT)
        {
            if(send(acceptfd, err, strlen(err), 0) == -1)
            {
                ERROR("fail to send");
            }

            close(acceptfd);
            pthread_exit("file not found");
        }
        else
        {
            ERROR("fail to open");
        }
    }

    //  文件存在， 先发送确认给客户端
    if(send(acceptfd, head, strlen(head), 0) == -1)
    {
        ERROR("fail to send");
    }

    //  读取网页文件内容发送给浏览器
    ssize_t bytes = 0;
    char text[1024] = "";
    while((bytes = read(fd, text, sizeof(text))) > 0)
    {
        if(write(acceptfd, text, bytes) == -1)
        //if(send(acceptfd, text, bytes, 0) == -1)
        {
            ERROR("fail to send");
        }
    }

    close(fd);

    pthread_exit("send success");
}

/* 仿http服务器 */
int main(int n, char const *c[])
{
    if(n < 3)
    {
        fprintf(stderr, "%s, ip port\n", *c);
        return 1;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        ERROR("fail to socket");
    }

    int on = 1;     //  将套接字设置为允许重复使用本机地址或设置为端口复用(这可不设置)
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
    {
        ERROR("fail to setsockopt");
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(c[1]);
    serveraddr.sin_port = ntohs(atoi(c[2]));

    if(bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1)
    {
        ERROR("fail to bind");
    }

    if(listen(sockfd, 10) == -1)
    {
        ERROR("fail to listen");
    }


    int acceptfd;
    struct sockaddr_in clientaddr;
    int len = sizeof(clientaddr);

    while(1)
    {
        if((acceptfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len)) == -1)
        {
            ERROR("fail to accept");
        }

        printf("%s [%d]\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        pthread_t tid;
        if(pthread_create(&tid, NULL, thread_fun, &acceptfd) != 0)
        {
            ERROR("fail to pthread_create");
        }
        pthread_detach(tid);
    }


    return 0;
}
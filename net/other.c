// #include <stdio.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <stdlib.h>
// #include <sys/select.h>
// #include <unistd.h>
// #include <errno.h>

// int main(int argc, char const *argv[])
// {
//     if(argc < 3)
//     {
//         fprintf(stderr, "%s ip port\n", *argv);
//         return 1;
//     }

//     int listenfd = socket(AF_INET, SOCK_STREAM, 0);
//     if(listenfd == -1)
//     {
//         perror("fail to socket");
//         return 1;
//     }

//     int opt = 1;
//     if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
//     {
//         perror("fail to setsockopt");
//         return 1;
//     }

//     struct sockaddr_in serveraddr;
//     serveraddr.sin_family = AF_INET;
//     serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
//     serveraddr.sin_port = htons(atoi(argv[2]));
//     bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

//     if(listen(listenfd, 10) == -1)
//     {
//         perror("fail to listen");
//         return 1;
//     }


//     fd_set allset = {0}, rset;    //  需要监听的全部集合， 读传入传出实际触发集合
//     int maxfd = listenfd;   //  当前最大数文件描述符

//     FD_SET(listenfd, &allset);

//     int fd_num;
//     char buf[128] = "";
//     int data_num;
//     struct sockaddr_in clientaddr;
//     int clientaddr_len;
//     int connet_fd;
//     while(1)
//     {
//         rset = allset;
//         //  返回监听集合中满足条件的个数
//         fd_num = select(maxfd + 1, &rset, NULL, NULL, NULL);
//         //  poll()
//         if(fd_num == -1)
//         {
//             perror("fail to select");
//             return 1;
//         }

//         if(FD_ISSET(listenfd, &rset))   //  有新的客户端连接请求
//         {
//             clientaddr_len = sizeof(clientaddr);
//             if((connet_fd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddr_len)) == -1)
//             {
//                 perror("fail to accept");
//                 return 1;
//             }

//             //  为了防止需要监听的文件描述符存在比如（3、1023）两个极端而造成大量不必要的工作
//             //  可以将【文件描述符加入数组】进行遍历
//             FD_SET(connet_fd, &allset);
//             if(maxfd < connet_fd)
//                 maxfd = connet_fd;

//             if(fd_num == 1) continue;
//         }

//         for(int i = listenfd + 1; i <= maxfd; i++)
//         {
//             if(FD_ISSET(i, &rset))
//             {
//                 if((data_num = read(i, buf, sizeof(buf))) == -1)
//                 {
//                     if(errno == EINTR)
//                         perror("被异常中断，需要重启");
//                     else if(errno == EAGAIN)
//                         perror("非阻塞读，但没有数据可读");
//                     else if(errno == ECONNRESET)
//                         perror("连接被重置");
//                     perror("fail to read");
//                     return 1;
//                 }
//                 else if(data_num > 0)
//                 {
//                     for(int j = 0; j < data_num; j++)
//                         buf[j] ^= 32;
//                     buf[data_num - 1] = '\0';
//                     write(i, buf, data_num);
//                 }
//                 else if(data_num == 0)  //  对方断开连接
//                 {
//                     close(i);
//                     FD_CLR(i, &allset);

//                     if(i == maxfd) maxfd--;
//                 }
//             }
//         }
//     }    

//     return 0;
// }


// #include <stdio.h>
// #include <stdlib.h>
// #include <arpa/inet.h>
// #include <unistd.h>
// #include <sys/epoll.h>
// #include <errno.h>

// int main(int argc, char const *argv[])
// {
//     if(argc < 3)
//     {
//         fprintf(stderr, "%s ip port\n", *argv);
//         return 1;
//     }

//     int listenfd = socket(AF_INET, SOCK_STREAM, 0);
//     if(listenfd == -1)
//     {
//         perror("fail to socket");
//         return 1;
//     }

//     int opt = 1;
//     if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
//     {
//         perror("fail to setsockopt");
//         return 1;
//     }

//     struct sockaddr_in serveraddr;
//     serveraddr.sin_family = AF_INET;
//     serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
//     serveraddr.sin_port = htons(atoi(argv[2]));
//     bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

//     if(listen(listenfd, 10) == -1)
//     {
//         perror("fail to listen");
//         return 1;
//     }


//     struct epoll_event ep_event;
//     ep_event.events = EPOLLIN;  //  设置需要监听的事件（读）
//     ep_event.data.fd = listenfd;

//     int epfd = epoll_create(2048);  //  创建监听红黑树，返回根结点
//     if(epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ep_event) == -1)
//     {
//         perror("fail to epoll_ctl");
//         return 1;
//     }

//     int waited_num;
//     struct epoll_event ep_events[2048];
//     int connect_fd;
//     struct sockaddr_in client_addr;
//     int addr_len;
//     char buf[128] = "";
//     int data_len;

//     while(1)
//     {
//         if((waited_num = epoll_wait(epfd, ep_events, sizeof(ep_events) / sizeof(ep_events[0]), -1)) == -1)
//         {
//             perror("fail to epoll_wait");
//             return 1;
//         }

//         for(int i = 0; i < waited_num; i++)
//         {
//             if(ep_events[i].data.fd == listenfd)
//             {
//                 addr_len = sizeof(client_addr);
//                 if((connect_fd = accept(listenfd, (struct sockaddr*)&client_addr, &addr_len)) == -1)
//                 {
//                     perror("fail to accept");
//                     return 1;
//                 }

//                 /*
//                 【epoll事件模型】
//                     ET模式：边沿触发：(高效，只支持非阻塞模式)
//                         缓冲区有剩余未读的数据时 epoll_wait不会返回，必须等到新监听事件
//                     LT模式：水平触发（默认）
//                         缓冲区剩余未读的数据时 epoll_wait也会返回而不阻塞等待
//                 */
//                 ep_event.events = EPOLLIN;  //  | EPOLLET
//                 ep_event.data.fd = connect_fd;
//                 if(epoll_ctl(epfd, EPOLL_CTL_ADD, connect_fd, &ep_event) == -1)
//                 {
//                     perror("fail to epoll_ctl");
//                     return 1;
//                 }

//                 printf("%s [%d]\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
//             }
//             else
//             {
//                 if((data_len = read(ep_events[i].data.fd, buf, sizeof(buf))) == -1)
//                 {
//                     if(errno == EINTR)
//                         perror("被异常中断，需要重启");
//                     else if(errno == EAGAIN)
//                         perror("非阻塞读，但没有数据可读");
//                     else if(errno == ECONNRESET)
//                         perror("连接被重置");
//                     else
//                     {
//                         perror("fail to read");
//                         return 1;
//                     }
//                 }
//                 else if(data_len == 0)
//                 {
//                     if(epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL) == -1)
//                     {
//                         perror("fail to epoll_ctl");
//                         return 1;
//                     }
//                     close(ep_events[i].data.fd);
//                 }
//                 else
//                 {
//                     for(int j = 0; j < data_len; j++)
//                         buf[j] ^= 32;
//                     buf[data_len - 1] = '\0';

//                     write(ep_events[i].data.fd, buf, data_len);
//                 }
//             }
//         }
//     }


//     return 0;
// }



/* epoll反应堆模型： epoll ET模式 + 非阻塞 + void* ptr */
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

#define MAX_EVENTS 1024

struct myevent_s
{
    int fd;         //  要监听的文件描述符
    int events;     //  对应的监听事件
    void* arg;      //  泛型参数
    void (*call_back)(int fd, int events, void* arg);
    int status;     //  是否在监听
    char buf[BUFSIZ];
    int len;
    long last_active;   //  记录每次加入红黑树的时间
};

int g_epfd;
struct myevent_s g_events[MAX_EVENTS + 1];  //  +1用于listen_fd


void setEvent(struct myevent_s* ev, int fd, void (*call_back)(int, int, void*), void* arg)
{
    ev->fd = fd;
    ev->events = 0;
    ev->call_back = call_back;
    ev->arg = arg;
    ev->status = 0;
    //ev->len = 0;
    //memset(ev->buf, 0, sizeof(ev->buf));
    ev->last_active = time(NULL);
}

void addEvent(int g_epfd, int event, struct myevent_s* ev)
{
    struct epoll_event epv = {0};
    epv.data.ptr = ev;
    epv.events = ev->events = event | EPOLLET;

    if(ev->status == 0)
    {
        if(epoll_ctl(g_epfd, EPOLL_CTL_ADD, ev->fd, &epv) == -1)
        {
            fprintf(stderr, "%s:-- %s\n", __FILE__, strerror(errno));
            exit(1);
        }
        ev->status = 1;
    }
}

void delEvent(int g_epfd, struct myevent_s* ev)
{
    if(ev->status == 0)
        return;

    ev->status = 0;

    if(epoll_ctl(g_epfd, EPOLL_CTL_DEL, ev->fd, NULL) == -1)
    {
        fprintf(stderr, "---%s: %s\n", __FILE__, strerror(errno));
        exit(1);
    }
}

void recvData(int, int, void*);

void sendData(int fd, int event, void* arg)
{
    struct myevent_s* ev = (struct myevent_s*)arg;

    if(send(fd, ev->buf, ev->len, 0) == -1)
    {
        delEvent(g_epfd, ev);
        close(fd);
        perror("fail to send");
        exit(1);
    }

    //  需要先将ev的读事件从监听树上摘下，再将ev的写挂上；否则会出错（再次添加时报错File exists）
    delEvent(g_epfd, ev);
    
    setEvent(ev, ev->fd, recvData, arg);
    addEvent(g_epfd, EPOLLIN, ev);
}

void recvData(int fd, int event, void* arg)
{
    int len;
    struct myevent_s* ev = (struct myevent_s*)arg; 
    
    if((len = recv(ev->fd, ev->buf, sizeof(ev->buf), 0)) == -1)
    {
        delEvent(g_epfd, ev);
        close(ev->fd);
        perror("fail to recv");
        exit(1);
    }
    else if(len == 0)
    {
        delEvent(g_epfd, ev);
        close(ev->fd);
    }
    else if(len > 0)
    {
        delEvent(g_epfd, ev);
        
        for(int i = 0; i < len; i++)
        {
            putchar(ev->buf[i]);
            ev->buf[i] ^= 32;
        }
        ev->buf[len - 1] = '\0';
        ev->len = len;

        setEvent(ev, ev->fd, sendData, ev);
        addEvent(g_epfd, EPOLLOUT, ev);
    }
}

void acceptConn(int listen_fd, int event, void* arg)
{
    struct sockaddr_in client_addr = {0};
    socklen_t client_addr_len = sizeof(client_addr);
    int conn_fd, i;

    if((conn_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len)) == -1)
    {
        if(errno == EAGAIN || errno ==  EINTR)
        {

        }
        fprintf(stderr, "***%s: %s\n", __FILE__, strerror(errno));
        exit(1);
    }

    do
    {
        for(i = 0; i < MAX_EVENTS; i++)
            if(g_events[i].status == 0) //  找一个空闲的空间
                break;

        if(i == MAX_EVENTS)
        {
            fprintf(stderr, "___%s: %s\n", __FILE__, strerror(errno));
            exit(1);
        }        

        fcntl(conn_fd, F_SETFL, O_NONBLOCK);

        setEvent(g_events + i, conn_fd, recvData, g_events + i);
        addEvent(g_epfd, EPOLLIN, g_events + i);

        printf("new connect %d: %s [%d]\n", g_events[i].fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    } while (0);
}

void initListenSocket(int g_epfd, const char* ip, const char* port)
{
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1)
    {
        perror("fail to socket");
        exit(1);
    }

    //  设置socket为非阻塞
    int flag = fcntl(listen_fd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(listen_fd, F_SETFL, flag);

    int opt = 1;
    if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        perror("fail to setsockopt");
        exit(1);
    }

    struct sockaddr_in listen_addr = {0};
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_addr.s_addr = inet_addr(ip);
    listen_addr.sin_port = htons(atoi(port));
    bind(listen_fd, (struct sockaddr*)&listen_addr, sizeof(listen_addr));

    if(listen(listen_fd, 10) == -1)
    {
        perror("fail to listen");
        exit(1);
    }

    //  将监听套接字加入监听红黑树最后一个空间
    setEvent(g_events + MAX_EVENTS, listen_fd, acceptConn, g_events + MAX_EVENTS);
    addEvent(g_epfd, EPOLLIN, g_events + MAX_EVENTS);
}

int main(int argc, char const *argv[])
{
    if(argc < 3)
    {
        fprintf(stderr, "%s ip port\n", *argv);
        return 1;
    }

    //  创建监听红黑树
    if((g_epfd = epoll_create(MAX_EVENTS + 1)) == -1)
    {
        perror("fail to epoll_create");
        return 1;
    }

    //  创建监听套接字
    initListenSocket(g_epfd, argv[1], argv[2]);

    //  保存已经满足事件的文件描述符
    struct epoll_event events[MAX_EVENTS + 1];

    int i, checkpos = 0;
    struct myevent_s* my_ev;
    while(1)
    {
        /* 超时验证，测试除监听套接字外的所有套接字 */
        long now = time(NULL);
        //  每次检测100个连接       
        for(i = 0; i < 100; i++, checkpos++)
        {
            if(checkpos == MAX_EVENTS)
                checkpos = 0;

            if(g_events[checkpos].status == 0)
                continue;

            //  客户端不活跃的时间
            long duration = now - g_events[checkpos].last_active;
            if(duration >= 60)
            {
                delEvent(g_epfd, &g_events[checkpos]);
                close(g_events[checkpos].fd);
                
                printf("timeout: %d\n", g_events[checkpos].fd);
            }
        }

        /* 监听红黑树，将满足事件的文件描述符加入events中 */
        int nfd = epoll_wait(g_epfd, events, MAX_EVENTS + 1, 1000);
        if(nfd == -1)
        {
            perror("fail to epoll_wait");
            return 1;
        }

        for(i = 0; i < nfd; i++)
        {
            my_ev = (struct myevent_s*)events[i].data.ptr;

            //  读/写事件就绪，调用回调函数
            my_ev->call_back(my_ev->fd, events[i].events, my_ev->arg);
        }
    }

    return 0;
}
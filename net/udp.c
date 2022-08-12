// #include <stdio.h>

// union
// {
//     int a;
//     char b;
// }u;

// int main()
// {
//     u.a = 1;

//     if(u.b == 1) printf("小端\n");

//     return 0;
// }

// #include <arpa/inet.h>
// #include <stdio.h>

// int main()
// {
//     uint32_t a = 0x12345678;

//     printf("%#x\n", htonl(a));
//     printf("%#x\n", ntohl(a));

//     return 0;
// }

// #include <arpa/inet.h>
// #include <stdio.h>
// #include <stdlib.h>

// void f(int n)
// {
//     if (n)
//         f(n / 2);
//     else
//         return;
//     printf("%d", n % 2);
// }

// int main()
// {
//     char ip_c[] = "127.0.0.1";
//     unsigned int ip_i = 0;

//     inet_pton(AF_INET, ip_c, &ip_i);
//     printf("%d\n", ip_i);

//     f(ip_i);

//     char *c;
//     c = (char *)&ip_i;
//     printf("\n%d %d %d %d\n", *c, *(c + 1), *(c + 2), *(c + 3));



//     char ip_int[] = {127, 0, 0, 1};
//     char ip_str[16] = "";

//     inet_ntop(AF_INET, ip_int, ip_str, 32);
//     printf("\n\n%s\n", ip_str);


//     /*      【都只能用在ipv4地址的转换】
//         inet_addr() 将点分十进制ip地址转化为整形数据
//         inet_ntoa() 将整形数据转化为点分十进制ip地址
//     */

//     return 0;
// }


// #include <sys/socket.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <unistd.h>

// int main(int n, char const *c[])    //仿客户端
// {
//     if(n < 3)
//     {
//         fprintf(stderr, "%s ip port\n", c[0]);
//         return 1;
//     }


//     int sock;

//     sock = socket(AF_INET, SOCK_DGRAM, 0);

//     if(sock == -1)
//     {
//         perror("socket");
//         return 1;
//     }

//     printf("%d\n", sock);

//     //bind();
    
//     struct sockaddr_in serveraddr;

//     serveraddr.sin_family = AF_INET;
//     serveraddr.sin_addr.s_addr = inet_addr(c[1]);  //inet_addr("192.168.2.77")
//     serveraddr.sin_port = htons(atoi(c[2])); //htons(8080)

//     char buf[128];
//     while(1)
//     {
//         fgets(buf, 128, stdin);
//         buf[strlen(buf) - 1] = '\0';

//         sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

//     }

//     close(sock);


//     return 0;
// }


// #include <stdio.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>

// int main(int n, char const *c[])    //  仿服务器
// {
//     if(n < 3)
//     {
//         fprintf(stderr, "Usage: %s ip port\n", c[0]);
//         return 1;
//     }

//     int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

//     if(sockfd == -1)
//     {
//         perror("create socket field");
//         return 1;
//     }

//     struct sockaddr_in serveraddr;

//     serveraddr.sin_family = AF_INET;
//     serveraddr.sin_addr.s_addr = inet_addr(c[1]);
//     serveraddr.sin_port = htons(atoi(c[2]));

//     if(bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)))
//     {
//         perror("fail to bind");
//         return 1;
//     }

//     char buf[128] = "";
//     struct sockaddr_in clientaddr = {0};

//     unsigned int len = sizeof(clientaddr);

//     while(1)
//     {
//         memset(buf, 0, 128);
// printf("2\n");
//         if(recvfrom(sockfd, buf, 128, 0, (struct sockaddr*)&clientaddr, &len) == -1)
//         {
//             perror("recvfrom error");
//             return 1;
//         }printf("1\n");
//         printf("%s [%d]\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

//         printf("%s\n", buf);

//         printf("----------\n");
//         sendto(sockfd, "yes\n", strlen("yes\n"), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
//     }

//     close(sockfd);

//     return 0;
// }


// #include <stdio.h>
// #include <netinet/in.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <stdlib.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>

// void download(int sockfd, struct sockaddr_in serveraddr)
// {
//     char buf[128] = "";
//     char recvbuf[516] = "";
//     char filename[128] = "";
//     int bytes = 0;

//     printf("需要下载的文件名：");
//     scanf("%s", filename);

//     //  拼接tftp数据报
//     sprintf(buf, "0%c%s0%s0", 1, filename, "octet");

//     if(sendto(sockfd, buf, 128, 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1)
//     {
//         perror("sendto fail");
//         exit(1);
//     }


//     int fp;
//     if(fp = open(filename, O_CREAT | O_WRONLY, 0664) == -1)
//     {
//         perror("fail to open");
//         exit(1);
//     }

//     //struct sockaddr_in clientaddr = {0};
//     int len = sizeof(serveraddr);

//     int num = 0;

//     while(1)
//     {printf("1\n");
//         bytes = recvfrom(sockfd, recvbuf, 516, 0, (struct sockaddr*)&serveraddr, &len);
//         printf("2\n");
//         if(recvbuf[1] == 5)
//         {
//             printf("%s\n", recvbuf + 4);
//             exit(1);
//         }
//         else if(recvbuf[1] == 3 && (num + 1) == ntohs(*(short int*)(recvbuf + 2)) && bytes == 516)
//         {
//             num = ntohs(*(short int*)(recvbuf + 2));

//             if(write(fp, recvbuf + 4, bytes - 4) == -1)
//             {
//                 perror("fail to write");
//                 exit(1);
//             }

//             recvbuf[1] = 4;
//             if(sendto(sockfd, recvbuf, 4, 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1)
//             {
//                 perror("fail to sendto");
//                 exit(1);
//             }
//         }
//         else if(recvbuf[1] = 3 && (num + 1) == ntohs(*(short int*)(recvbuf + 2)) && bytes < 516)
//         {
//             num = ntohs(*(short int*)(recvbuf + 2));

//             if(write(fp, recvbuf + 4, bytes - 4) == -1)
//             {
//                 perror("fail to write");
//                 exit(1);
//             }

//             recvbuf[1] = 4;
//             if(sendto(sockfd, recvbuf, 4, 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1)
//             {
//                 perror("fail to sendto");
//                 exit(1);
//             }

//             close(fp);

//             printf("文件下载完成\n");

//             return;
//         }
//     }

// }

// int main(int n, char const *c[])    //  tftp客户端
// {
//     if(n < 2)
//     {
//         fprintf(stderr, "%s ip\n", c[0]);
//         return 1;
//     }

//     int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
//     if(sockfd == -1)
//     {
//         perror("create socket field");
//         return 1;
//     }

//     struct sockaddr_in serveraddr;
//     serveraddr.sin_family = AF_INET;
//     serveraddr.sin_addr.s_addr = inet_addr(c[1]);
//     serveraddr.sin_port = htons(69);  

//     download(sockfd, serveraddr); 

//     close(sockfd); 

//     return 0;
// }


//  setsockopt()  （对应端口才能接受）
//  设置允许发送广播权限（发送方允许之后就是向广播地址发送数据）(ip地址必须设置为同一子网内或4个255)
//  设置允许接受多播(组播) （接受方允许之后可以接受发送方向该组播地址发送的数据）（ip地址必须为组播地址224.0.0.1~239.255.255.254）
//  加入多播组结构体struct ip_mreq
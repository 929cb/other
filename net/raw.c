// #include <stdio.h>
// #include <sys/socket.h>
// #include <netinet/ether.h> 
// #include <arpa/inet.h>
// #include <unistd.h> 

// int main(int n, char const *c[])
// {
//     int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
//     if(sockfd == -1)
//     {
//         perror("fail to socket");
//         return 1;
//     }

//     printf("%d\n", sockfd);

//     close(sockfd);

//     return 0;
// }


// #include <stdio.h>
// #include <sys/socket.h>
// #include <netinet/ether.h>
// #include <arpa/inet.h>
// #include <unistd.h>

// typedef struct IP_HEAD
// {
//     unsigned char version : 4;
//     unsigned char head_len : 4;
//     unsigned char serve_type;
//     unsigned short int total_len;
//     unsigned short int logotype;
//     unsigned short int sign : 3;
//     unsigned short int slice_offset : 13;
//     unsigned char ttl;
//     unsigned char protocal;
//     unsigned short int head_checksum;
//     unsigned int src_ip;
//     unsigned int dest_ip;
// }Ip_Head;

// int main()  /*  仿抓取网络中的数据包并解析 */
// {
//     int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL)); 
//     if(sockfd == -1)
//     {
//         perror("fail to socket");
//         return 1;        
//     }

//     unsigned char msg[1518] = "";

//     while(1)
//     {
//         //  recvfrom recv read都可以使用
//         if(recv(sockfd, msg, sizeof(msg), 0) == -1)
//         {
//             perror("fail to recv");
//             return 1;
//         }

//         unsigned char dest_mac[18] = "";
//         unsigned char src_mac[18] = "";
//         unsigned short int type = 0;
//         unsigned char ip_head[20] = "";
//         sprintf(dest_mac, "%x:%x:%x:%x:%x:%x", msg[0], msg[1], msg[2], msg[3], msg[4], msg[5]);
//         sprintf(src_mac, "%x:%x:%x:%x:%x:%x", msg[6], msg[7], msg[8], msg[9], msg[10], msg[11]);
//         type = ntohs(*(unsigned short int *)(msg + 12));

//         printf("\n\n-------------------------------------------------------------\n");
//         printf("源mac: %s --> 目的mac: %s\n", src_mac, dest_mac);
//         printf("类型：%#x\n", type);
//         //  printf("数据：%s\n", msg + 14 + 20 + 20);

//         unsigned char* ip = NULL;
        
//         if(type == 0x0800)
//         {
//             printf("\n*** ip ***\n");

//             ip = msg + 14;
//             Ip_Head ip_head;
//             //printf("%ld\n", sizeof(Ip_Head));

//             ip_head.version = *ip >> 4;
//             ip_head.head_len = *ip;
//             ip_head.serve_type = ip[1];
//             ip_head.total_len = ntohs(*(unsigned short int*)(ip + 2));
//             ip_head.logotype = ntohs(*(unsigned short int*)(ip + 4));
//             ip_head.sign = *(ip + 6) >> 5;
//             ip_head.slice_offset = ntohs(*(unsigned short int*)(ip + 6));
//             ip_head.ttl = ip[8];
//             ip_head.protocal = ip[9];
//             ip_head.head_checksum = ntohs(*(unsigned short int*)(ip + 10));
//             ip_head.src_ip = ntohl(*(unsigned int*)(ip + 12));
//             ip_head.dest_ip = ntohl(*(unsigned int*)(ip + 16));

//             printf("协议版本：ipv%d\n", ip_head.version);
//             printf("首部长度：%d字节\n", ip_head.head_len * 4);
//             printf("服务类型：%s\n", ip_head.serve_type == 0 ? "一般服务" : "其他服务");
//             printf("总长度：%d字节\n", ip_head.total_len);
//             printf("标识：%d\n", ip_head.logotype);
//             printf("标志：%d\n", ip_head.sign);
//             printf("片偏移：%d\n", ip_head.slice_offset);
//             printf("生存时间：%d\n", ip_head.ttl);
//             printf("协议：%s\n", ip_head.protocal == 1 ? "ICMP" : (ip_head.protocal == 2 ?
//              "IGMP" : (ip_head.protocal == 6 ? "TCP" : "UDP")));
//             printf("首部校验和：%d\n", ip_head.head_checksum);
//             printf("源IP地址：%d.%d.%d.%d\n", ip_head.src_ip >> 24, ip_head.src_ip && 0xff0000, ip_head.src_ip && 0xff00, ip_head.src_ip && 0xff);
//             printf("目的IP地址：%d.%d.%d.%d\n", ip_head.dest_ip >> 24, ip_head.dest_ip && 0xff0000, ip_head.dest_ip && 0xff00, ip_head.dest_ip && 0xff);
        

//             //  可继续解析tcp/udp...
//         }
//         else if(type == 0x0806)
//         {
//             printf("\n*** arp ***\n");
//         }
//         else if(type == 0x8035)
//         {
//             printf("\n*** rarp ***\n");
//         }
//     } 

//     close(sockfd);

//     return 1;
// }



#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netpacket/packet.h>
#include <netinet/ether.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int sockfd = socket(AF_PACKET, SOCK_RAW, ETH_P_ALL);
    if(sockfd == -1)
    {
        perror("fail to socket");
        return 1;
    }

    //  组装arp请求数据包
    unsigned char msg[1600] = {
        //  以太网首部  【一个一个字节组装可以不考虑字节序】
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //  目的mac地址（广播地址）
        0x74, 0x40, 0xbb, 0x82, 0x20, 0x37, //  源mac地址(74:40:bb:82:20:37)
        0x08, 0x06, //  帧类型（arp报文：0x0806）

        //  组装arp报文
        0x00, 0x01, //  硬件类型（以太网：1）
        0x08, 0x00, //  协议类型（ip：0x0800）
        6,  //  硬件地址长度
        4,  //  协议地址长度
        0x00, 0x01, //  op（arp请求：1）
        0x74, 0x40, 0xbb, 0x82, 0x20, 0x37, //  源mac地址
        192, 168, 2, 77,    //  源ip地址
        0, 0, 0, 0, 0, 0,   //  目的mac地址
        192, 168, 2, 77,    //  目的ip地址
    };

    //  获取网络接口信息
    //  将arp请求报文通过本机网络接口发送出去
    //  使用ioctl函数获取本机网络接口
    struct ifreq ethreq;
    //ethreq.ifr_name = "wlp3s0";
    strncpy(ethreq.ifr_name, "wlp3s0", IFNAMSIZ);
    if(ioctl(sockfd, SIOCGIFINDEX, &ethreq) == -1)
    {
        perror("fail to ioctl");
        return 1;
    }

    //  设置本机网络接口
    struct sockaddr_ll sll = {0};
    sll.sll_ifindex = ethreq.ifr_ifindex;

    //  发送数据
    if(sendto(sockfd, msg, 14 + 28, 0, (struct sockaddr*)&sll, sizeof(sll)) == -1)
    {
        perror("fail to sendto");
        return 1;
    }

    unsigned char rec_msg[1600] = "";
    unsigned char mac[18] = "";
    while(1)
    {
        if(recvfrom(sockfd, rec_msg, sizeof(rec_msg), 0, NULL, NULL) == -1)
        {
            perror("fail to recvfrom");
            return 1;
        }

        //  如果arp数据包，并且是arp应答
        if(ntohs(*(unsigned short int*)(rec_msg + 12)) == 0x0806 && 
            ntohs(*(unsigned short int*)(rec_msg + 20)) == 2)
        {
            sprintf(mac, "%x:%x:%x:%x:%x:%x", rec_msg[6], rec_msg[7], rec_msg[8], rec_msg[9], rec_msg[10], rec_msg[11]);
            printf("192.168.2.%d --> %s\n", msg[41], mac);            
        }
    }

    close(sockfd);

    return 0;
}
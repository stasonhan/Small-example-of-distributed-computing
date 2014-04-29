/*
*sock.cpp
*/
#include <sys/socket.h>
#include "sock.h"

//#ifndef IN

void setnonblocking(int sock)
{
    int opts;
    opts = fcntl(sock,F_GETFL);
    if(opts < 0)
    {
        perror("fcntl(sock,GETFL)");
        exit(1);
    }
    opts = opts | O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts) < 0)
    {
        perror("fcntl(sock,GETFL)");
        exit(1);

    }
}

void setblocking(int sock)
{
    int opts;
    opts = fcntl(sock,F_GETFL);
    if(opts < 0)
    {
        perror("fcntl(sock,GETFL)");
        exit(1);
    }
    opts = opts & ~O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts) < 0)
    {
        perror("fcntl(sock,GETFL)");
        exit(1);

    }

}
int set_local_ip(struct sockaddr_in* addr,int sock_fd)
{
    struct ifconf conf;
    struct ifreq *ifr;
    char buff[128];
    conf.ifc_len = 128;
    conf.ifc_buf = buff; 
    struct sockaddr_in *sin;
    int i;
    if(addr == NULL)
    {
        return -1;
    }
    if(ioctl(sock_fd, SIOCGIFCONF, &conf) < 0)
    {
        perror("ioctl");
        return  -1;
    }
    int num = conf.ifc_len / sizeof(struct ifreq); 
    ifr = conf.ifc_req;
    for(i = 0; i < num; i++)
    {
        sin = (struct sockaddr_in *)(&ifr->ifr_addr);
        ioctl(sock_fd, SIOCGIFFLAGS, ifr);
        if(((ifr->ifr_flags & IFF_LOOPBACK) == 0) && (ifr->ifr_flags & IFF_UP))
        {
           if(strncmp(ifr->ifr_name,"eth0",4) == 0)
           {
               addr->sin_addr = sin->sin_addr;
           }

        }
    }
    return 0;
}
int create_udp_socket()
{
    int udpfd;
    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    udpfd = socket(AF_INET,SOCK_DGRAM,0);
    set_local_ip(&addr,udpfd);
    addr.sin_family = AF_INET;
    int SERV_PORT = 50005;
    addr.sin_port = htons(SERV_PORT);

    if(bind(udpfd,(struct sockaddr*)&addr,sizeof(addr)) < 0)
    {
         perror("bind wrong");
         exit(1);
    }

    return udpfd;
    
}
int create_server()
{
    int listenfd;
    socklen_t clilen;
    struct sockaddr_in serveraddr;
    int reuse_port = 1;
    int len = sizeof(reuse_port);
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(listenfd < 0)
    {
        perror("socket create wrong!");
        exit(1);
    }

    //setnonblocking(listenfd);
    set_local_ip(&serveraddr,listenfd);
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    
  //  const char* local_addr = "127.0.0.1";
  //  inet_aton(local_addr,&(serveraddr.sin_addr));
    set_local_ip(&serveraddr,listenfd);
    int SERV_PORT = 50005;
    serveraddr.sin_port = htons(SERV_PORT);
    
    if(bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)) < 0)
    {
        perror("bind wrong");
        exit(1);
    }

    if(/*setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&reuse_port,len) < 0*/0)
    {
        perror("setsockopt");
        exit(1);
    }
    return listenfd;

}

int Writen(int sockfd,IN const char* buf,int len)
{
     int n = 0;
     int length = len;
     int res = -1;
     
     if(length <= 0)
     {
         perror("write length < 0");
         exit(1);
     }
     
     while(length > 0)
     {
         res = write(sockfd,buf + n,length);
         if(res < length){
         
             if(res == -1 && (errno == EAGAIN || errno == EINTR))
             {
                 perror("write error!");
                 return n;
             }
         }
         else{
             n += res;
             length = length - n;
         }
         
     }
     return n;
}

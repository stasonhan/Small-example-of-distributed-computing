
#ifndef SOCK_H
#define SOCK_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/sockios.h> 
#include <net/if.h>
#include <sys/types.h>

#ifndef IN
#define IN
#endif
void setnonblocking(int sock);
int create_udp_socket();
int create_server();

int Writen(int sockfd,IN const char* buf,int len);

void setblocking(int sock);

int set_local_ip(struct sockaddr_in* addr,int sock_fd);
#endif

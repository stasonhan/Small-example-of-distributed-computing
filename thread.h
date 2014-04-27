/*
* thread.h
*/

#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include "sock.h"
#include <sys/socket.h>

void create_listen_thread();
void add_epollin_ctl(int epfd,int sockfd);
void add_epollout_ctl(int epfd,int sockfd);
void del_epoll_ctl(int epfd,int sockfd);
void *listen_thread(void* args);

#endif

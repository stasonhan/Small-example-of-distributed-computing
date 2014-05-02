
#include "thread.h"
#include "sock.h"
#include "db_interface.h"
#define LISTENQ 20

int g_poll;
struct epoll_event ev,events[20];

void create_listen_thread()
{
    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,listen_thread,NULL);

}
void create_worker_thread()
{
    ;
}
//
void add_epollin_ctl(int epfd,int sockfd)
{
    struct epoll_event ev;
    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&ev);
}

//Add sockfd to epfd by the way of writing
void add_epollout_ctl(int epfd,int sockfd)
{
    struct epoll_event ev;
    ev.data.fd = sockfd;
    ev.events = EPOLLOUT | EPOLLET;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&ev);
}

//delete the sockfd from epfd.
void del_epoll_ctl(int epfd,int sockfd)
{
    struct epoll_event ev;
    ev.data.fd = sockfd;
    epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,&ev);

}

void *listen_thread(void* args)
{
    int connfd = -1,sockfd;
    int revfd,wtfd;
    struct sockaddr_in clientaddr;
    int clilen = sizeof(clientaddr);
    int nfds = 0,maxi = 0,i;
    
    sockfd = create_server();
    setnonblocking(sockfd);
    
    g_poll = epoll_create(20);
    add_epollin_ctl(g_poll,sockfd);
    
    listen(sockfd,LISTENQ);
    
    while(1)
    {
        nfds = epoll_wait(g_poll,events,20,500);
        for(i = 0;i < nfds;i++)
        {
            if(events[i].data.fd == sockfd)
            {
                connfd = accept(sockfd,(struct sockaddr*)&clientaddr,&clilen);
                if(connfd < 0)
                {
                    perror("connfd < 0");
                    exit(1);
                }
                setnonblocking(connfd);
                const char* str = inet_ntoa(clientaddr.sin_addr);
                printf("connec_from: %s\n",str);
                add_epollin_ctl(g_poll,connfd);
            }
            else if(events[i].events & EPOLLIN)
            {
                printf("reading!\n");
                if((revfd = events[i].data.fd) < 0)
                {
                    continue;
                }
                char buf[1024];
                int n= read(revfd, buf, 1024);
                if(n <= 0)
                {
                    if(errno == ECONNRESET)//if the other socket is terminate 
                    {
                        close(sockfd);
                        continue;
                    }
                    else{
                        if(errno == EAGAIN)
                            printf("reading error!\n");//Read again
                        continue;
                    }
                }
                else
                {
                    //printf("reading:%s\n",buf);
                    //operate_db(buf);
                    parse_request(buf);
                    del_epoll_ctl(g_poll,revfd);
                    add_epollout_ctl(g_poll,revfd);
                }
            }
            else if(events[i].events & EPOLLOUT)
            {
                wtfd = events[i].data.fd;
                char *wtstr = "receive success!\n";
                printf("Reply:%s ",wtstr);
                write(wtfd,wtstr,strlen(wtstr) + 1);
                del_epoll_ctl(g_poll,wtfd);
                add_epollin_ctl(g_poll,wtfd);
            }
        }

    }
    
}

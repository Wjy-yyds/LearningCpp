#include "head.h"
int epollAdd(int epfd,int fd){
    struct epoll_event event;
    event.data.fd=fd;
    event.events=EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
    return 0;
}

int epollDel(int epfd,int fd){
    epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
    return 0;
}

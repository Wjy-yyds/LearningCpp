#include <my_header.h>

typedef struct client_fd_time{
    int fd;
    time_t lastactivity;
}client_fd_time;

/* Usage:  */
int main(int argc, char *argv[]){
    
    //建立sockfd对象
    int sockfd = socket(AF_INET,SOCK_STREAM,0);

    //定义bind要绑定的服务端端口和地址
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(atoi(MYPORT));
    addr.sin_addr.s_addr=inet_addr(MYIPSTR);

    //消除wait
    int flag=1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag));

    int ret = bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    listen(sockfd,50);
    
    //一切皆是文件
    int epfd = epoll_create(1);
    
    //创建结构体，赋值fd和监听事件
    struct epoll_event event;
    
     event.data.fd = sockfd;
     event.events = EPOLLIN; 
     epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&event);
    
    //添加和删除监听同函数，option不同
    //每次添加都要重写结构体
    //删除时不需要写结构体
    //epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&event);

    

    char buf[4096];
    // int netfd[1024];
     client_fd_time netfd_t[1024];
     for(int i=0;i<1024;i++){
        netfd_t[i].fd=-1;
        netfd_t[i].lastactivity=0;
     }
    
    int curindex=0;
    
    
    struct epoll_event readyset[1024];


    //逻辑处理
    // 最开始只监听sock
    // 在主循环中处理accept!!!
    time_t last_check_time = time(NULL); 
    while(1){

        //wait 
        int readynum = epoll_wait(epfd,readyset,1,1000);

        for(int i = 0; i < readynum; ++i){
           

            if(readyset[i].data.fd==sockfd){
                netfd_t[curindex].fd=accept(sockfd,NULL,NULL);
                netfd_t[curindex].lastactivity=time(NULL);
                
                struct epoll_event client_ev = {
                    .events = EPOLLIN,
                    .data.ptr = &netfd_t[curindex]
                };
                epoll_ctl(epfd,EPOLL_CTL_ADD,netfd_t[curindex].fd,&client_ev);
                curindex++;
            }else{
                client_fd_time *client = (client_fd_time*)readyset[i].data.ptr;

                memset(buf,0,sizeof(buf));
                int sret = recv(client->fd,buf,sizeof(buf),0);
                if(sret == 0){
                    epoll_ctl(epfd,EPOLL_CTL_DEL,client->fd,NULL);
                    close(client->fd);
                    client->fd = -1;
                    printf("close fd\n");
                    continue;
                }
                
                client->lastactivity = time(NULL);


                for(int j=0; j < curindex;j++){
                    if(netfd_t[j].fd!=-1 && netfd_t[j].fd != client->fd){
                    send(netfd_t[j].fd,buf,strlen(buf),0);
                    }
                }
            }

            

        }

        time_t now = time(NULL);
        if(now - last_check_time >= 1){
            last_check_time = now;
            for(int i=0; i<curindex; i++){
                if(netfd_t[i].fd!=-1 && now-netfd_t[i].lastactivity>10){
                    printf("超时踢出:%d\n", netfd_t[i].fd);
                    epoll_ctl(epfd,EPOLL_CTL_DEL,netfd_t[i].fd,NULL);
                    close(netfd_t[i].fd);
                    netfd_t[i].fd=-1;
                }
            }
        }
    }

    close(sockfd);
   

    return 0;
}


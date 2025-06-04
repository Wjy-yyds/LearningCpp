#include <my_header.h>

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
    // int netfd1 = accept(sockfd,NULL,NULL);
    // int netfd2 = accept(sockfd,NULL,NULL);
    int netfd1=-1,netfd2=-1;
    //一切皆是文件
    int epfd = epoll_create(1);
    
    //创建结构体，赋值fd和监听事件
    struct epoll_event event;
    event.data.fd = sockfd;
    event.events = EPOLLIN; 
    epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&event);
    // event.data.fd=STDIN_FILENO;
    // event.events = EPOLLIN;
    
    //添加和删除监听同函数，option不同
    //每次添加都要重写结构体
    //删除时不需要写结构体
    //epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&event);

    

    char buf[4096];

    
    struct epoll_event readyset[1];


    //逻辑处理
    // 最开始只监听sock
    // 在主循环中处理accept!!!
    
    while(1){

        //wait 
        int readynum = epoll_wait(epfd,readyset,1,-1);

        for(int i = 0; i < readynum; ++i){
            // if(readyset[0].data.fd == STDIN_FILENO){
            //     memset(buf,0,sizeof(buf));
            //     read(STDIN_FILENO,buf,sizeof(buf));
            //     send(netfd,buf,strlen(buf),0);
            // }

            if(readyset[0].data.fd==sockfd){
                if(netfd1==-1){
                    netfd1 = accept(sockfd,NULL,NULL);
                }else{
                    netfd2 = accept(sockfd,NULL,NULL);
                    epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,NULL);
                    event.data.fd = netfd1;
                    event.events = EPOLLIN;
                    epoll_ctl(epfd,EPOLL_CTL_ADD,netfd1,&event);
                    event.data.fd = netfd2;
                    event.events = EPOLLIN;
                    epoll_ctl(epfd,EPOLL_CTL_ADD,netfd2,&event);
                }
            }
            if(readyset[0].data.fd == netfd1){
                memset(buf,0,sizeof(buf));
                recv(netfd1,buf,sizeof(buf),0);
                send(netfd2,buf,strlen(buf),0);
                // printf("Client send message is %s\n",buf);
            }
            if(readyset[0].data.fd == netfd2){
                memset(buf,0,sizeof(buf));
                recv(netfd2,buf,sizeof(buf),0);
                send(netfd1,buf,strlen(buf),0);
                // printf("Client send message is %s\n",buf);
            }
        }
    }

    close(sockfd);
    close(netfd1);
    close(netfd2);

    return 0;
}


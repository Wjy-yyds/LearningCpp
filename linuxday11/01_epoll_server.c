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
    int netfd = accept(sockfd,NULL,NULL);

    //一切皆是文件
    int epfd = epoll_create(1);
    
    //创建结构体，赋值fd和监听事件
    struct epoll_event event;
    event.data.fd=STDIN_FILENO;
    event.events = EPOLLIN;
    
    //添加和删除监听同函数，option不同
    //每次添加都要重写结构体
    //删除时不需要写结构体
    epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&event);

    event.data.fd = netfd;
    event.events = EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,netfd,&event);

    char buf[4096];

    //定义epoll_event数组,前一个event数组用来注册监听
    //后一个epoll数组用来做wait的返回值，是个传入传出参数
    struct epoll_event readyset[1];

    while(1){

        //wait 
        int readynum = epoll_wait(epfd,readyset,1,-1);

        for(int i = 0; i < readynum; ++i){
            if(readyset[0].data.fd == STDIN_FILENO){
                memset(buf,0,sizeof(buf));
                read(STDIN_FILENO,buf,sizeof(buf));
                send(netfd,buf,strlen(buf),0);
            }

            if(readyset[0].data.fd == netfd){
                memset(buf,0,sizeof(buf));
                recv(netfd,buf,sizeof(buf),0);
                printf("Client send message is %s\n",buf);
            }
        }
    }

    close(sockfd);
    close(netfd);

    return 0;
}


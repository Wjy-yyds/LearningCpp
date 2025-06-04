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

    bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    listen(sockfd,50);
    // int netfd = accept(sockfd,NULL,NULL);

    //一切皆是文件
    int epfd = epoll_create(1);
    
    //创建结构体，赋值fd和监听事件
    //添加和删除监听同函数，option不同
    //每次添加都要重写结构体
    //删除时不需要写结构体
    struct epoll_event event;
   

    event.data.fd = sockfd;
    event.events = EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&event);


    int netfd=-1;
    //event.data.fd = netfd;
    //event.events = EPOLLIN;
    //epoll_ctl(epfd,EPOLL_CTL_ADD,netfd,&event);

    char buf[4096];

    //定义epoll_event数组,前一个event数组用来注册监听
    //后一个epoll数组用来做wait的返回值，是个传入传出参数
    struct epoll_event readyset[1];

    while(1){

        //wait 
        int readynum = epoll_wait(epfd,readyset,1,-1);
        
        for(int i = 0; i < readynum; ++i){
                
            if(readyset[0].data.fd == sockfd){
                netfd=accept(sockfd,NULL,NULL);
                printf("User is login now!\n");
                event.data.fd=STDIN_FILENO;
                event.events = EPOLLIN;
                epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&event);
                event.data.fd = netfd;
                event.events = EPOLLIN;
                epoll_ctl(epfd,EPOLL_CTL_ADD,netfd,&event);
                epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,NULL);
                printf(" Sockfd is remove from epfd\n");
            }

            if(readyset[0].data.fd == STDIN_FILENO){
                memset(buf,0,sizeof(buf));
                read(STDIN_FILENO,buf,sizeof(buf));
                send(netfd,buf,strlen(buf),0);
            }

            if(netfd!=-1&&readyset[0].data.fd == netfd){
                memset(buf,0,sizeof(buf));
                int sret=recv(netfd,buf,sizeof(buf),0);
                if(sret == 0){
                    printf("User is out,rebegin socket.\n");
//                     （1）​​TCP连接断开的底层机制​​
// 当客户端主动关闭连接（发送 FIN 包）时：
// 服务器调用 recv 会返回 0（表示对端已关闭）。
// ​​但内核仍会标记该 socket 为可读​​（因为收到了 FIN 包，属于可读事件）。
// 如果不从 epoll 移除监听，epoll_wait 会​​持续返回该 socket 的就绪事件​​。
                    epoll_ctl(epfd,EPOLL_CTL_DEL,netfd,NULL);
                    epoll_ctl(epfd,EPOLL_CTL_DEL,STDIN_FILENO,NULL);
                    event.data.fd=sockfd;
                    event.events=EPOLLIN;
                    epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&event);
                    close(netfd);
                    netfd=-1;
                    printf("Wait for new user connect.\n");
                    continue;

                }
                printf("Client send message is %s\n",buf);
            }
        }
    }

    close(sockfd);
    close(netfd);

    return 0;
}


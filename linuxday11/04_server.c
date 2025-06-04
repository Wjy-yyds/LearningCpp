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
    int netfd[1024];
    for(int i=0;i<1024;i++){
        netfd[i]=-1;
    }
    int curindex=0;
    
    
    struct epoll_event readyset[1];


    //逻辑处理
    // 最开始只监听sock
    // 在主循环中处理accept!!!
    
    while(1){

        //wait 
        int readynum = epoll_wait(epfd,readyset,1,-1);

        for(int i = 0; i < readynum; ++i){
           

            if(readyset[0].data.fd==sockfd){
                netfd[curindex]=accept(sockfd,NULL,NULL);
                event.data.fd=netfd[curindex];
                event.events=EPOLLIN;
                epoll_ctl(epfd,EPOLL_CTL_ADD,netfd[curindex],&event);
                curindex++;
            }
            for(int i=0;i<curindex;i++){
                memset(buf,0,sizeof(buf));
                if(netfd[i]!=-1 && readyset[0].data.fd==netfd[i]){
                    int sret=recv(netfd[i],buf,sizeof(buf),0);
                    if(sret == 0){
                        epoll_ctl(epfd,EPOLL_CTL_DEL,netfd[i],NULL);
                        close(netfd[i]);
                        netfd[i]=-1;
                        printf("close netfd[%d]\n",i);
                        continue;
                    }
                    for(int j=0; j < curindex;j++ ){
                        if(j!=i && netfd[j]!=-1){
                            send(netfd[j],buf,strlen(buf),0);
                        }
                    }
                }
            }

        }
    }

    close(sockfd);
   

    return 0;
}


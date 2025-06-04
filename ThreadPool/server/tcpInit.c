#include "threadPool.h"

int tcpInit(){
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int flag = 1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag));
    //赋值用具体，传参用抽象
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(MYPORT));
    addr.sin_addr.s_addr = inet_addr(MYIPSTR);

    int ret = bind(sockfd,(struct sockaddr *)&addr ,sizeof(addr));
    ERROR_CHECK(ret,-1,"bind error");
    listen(sockfd,50);
    return sockfd;
}

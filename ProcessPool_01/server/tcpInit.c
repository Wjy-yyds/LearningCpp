#include "head.h"

int tcpInit(char *ip,char *port){
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int flag=1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag));

    struct sockaddr_in sockaddr;
    sockaddr.sin_family=AF_INET;
    sockaddr.sin_port=htons(atoi(port));
    sockaddr.sin_addr.s_addr=inet_addr(ip);
    
    int ret = bind(sockfd,(struct  sockaddr *)&sockaddr,sizeof(sockaddr));
    ERROR_CHECK(ret,-1,"bind");
    listen(sockfd,50);
    return sockfd;

}

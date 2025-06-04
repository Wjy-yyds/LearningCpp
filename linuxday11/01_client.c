#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){

    int sockfd =socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port = htons(atoi(MYPORT));
    addr.sin_addr.s_addr=inet_addr(MYIPSTR);

    int ret=connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));//sizeof 大小填的是具体结构体的大小
    ERROR_CHECK(ret,-1,"connect");

    char buf[4096];
    fd_set rdset;

    while(1){
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        FD_SET(sockfd,&rdset);
        select(1024,&rdset,NULL,NULL,NULL);
        if(FD_ISSET(STDIN_FILENO,&rdset)){
            memset(buf,0,sizeof(buf));
           ssize_t ret= read(STDIN_FILENO,buf,sizeof(buf));
           if(ret==0){
                printf("client send EOF,communication is over\n");
                break;
           }
            send(sockfd,buf,strlen(buf),0);
        }
        if(FD_ISSET(sockfd,&rdset)){
            //printf("aaaa\n");
            memset(buf,0,sizeof(buf));
            ssize_t sret = recv(sockfd,buf,sizeof(buf),0);
            if(sret==0){
                printf("this mean other is close\n");
                break;
            }
            printf(" the server send message is %s\n",buf);
        }
    }
    
    close(sockfd);
    return 0;
}


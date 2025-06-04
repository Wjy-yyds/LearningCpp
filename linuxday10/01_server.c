#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(atoi(MYPORT));
    addr.sin_addr.s_addr=inet_addr(MYIPSTR);

    int bret=bind(sockfd,(struct sockaddr *)&addr,sizeof(addr));
    ERROR_CHECK(bret,-1,"bind error");
    listen(sockfd,50);

    char buf[4096]={0};
    fd_set fdset;
    int nret=accept(sockfd,NULL,NULL);

    while(1){
        FD_ZERO(&fdset);
        FD_SET(nret,&fdset);
        FD_SET(STDIN_FILENO,&fdset);
        select(1024,&fdset,NULL,NULL,NULL);

        if(FD_ISSET(STDIN_FILENO,&fdset)){
            memset(buf,0,sizeof(buf));
            int rret=read(STDIN_FILENO,buf,sizeof(buf));
            if(rret==0){
                printf("server wirte EOF,communciation over.\n");
                break;
            }
            send(nret,buf,strlen(buf),0);
        }

        if(FD_ISSET(nret,&fdset)){
            memset(buf,0,sizeof(buf));
            int sret=recv(nret,buf,sizeof(buf),0);
            if(sret==0){
                printf("client is over\n");
                break;
            }
            printf("The client send is %s\n",buf);
        }
    }

    close(sockfd);
    close(nret);


    return 0;
}


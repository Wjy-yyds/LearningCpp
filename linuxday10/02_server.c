#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    int flag=1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag));
    
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(atoi(MYPORT));
    addr.sin_addr.s_addr=inet_addr(MYIPSTR);

    int bret=bind(sockfd,(struct sockaddr *)&addr,sizeof(addr));
    ERROR_CHECK(bret,-1,"bind error");
    listen(sockfd,50);

    char buf[4096]={0};
    fd_set monitorset;
    fd_set readyset;


    FD_ZERO(&monitorset);
    FD_SET(sockfd,&monitorset);
    //int nret=accept(sockfd,NULL,NULL);
    int nret=-1;
    while(1) {
        memcpy(&readyset, &monitorset, sizeof(readyset));  // 修正参数顺序
        select(1024,&readyset,NULL,NULL,NULL);
        
        if(FD_ISSET(sockfd,&readyset)){
            nret=accept(sockfd,NULL,NULL);
            printf("A new socket has remove fome queue.\n");
            FD_CLR(sockfd,&monitorset);
            FD_SET(nret,&monitorset);
            FD_SET(STDIN_FILENO,&monitorset);
        }

        if(nret!=-1&&FD_ISSET(nret,&readyset)){
            memset(buf,0,sizeof(buf));
            int sret=recv(nret,buf,sizeof(buf),0);
            if(sret==0){
                printf("client is over\n");
                close(nret);
                FD_CLR(nret,&monitorset);
                FD_CLR(STDIN_FILENO,&monitorset);
                FD_SET(sockfd,&monitorset);
                printf("waiting for new client...\n");
                nret=-1;
                continue;
            }
            printf("recv from client: %s\n",buf);
        }
        if(nret!=-1&&FD_ISSET(STDIN_FILENO,&readyset)){
            memset(buf,0,sizeof(buf));
            int sret=read(STDIN_FILENO,buf,sizeof(buf));
            if(sret==0){
                printf("stdin is over\n");
                break;
            }
            send(nret,buf,strlen(buf),0);
        }
        

    }

    close(sockfd);
    close(nret);


    return 0;
}


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
    int nret1=-1,nret2=-1;
    while(1) {
        memcpy(&readyset, &monitorset, sizeof(readyset));  // 修正参数顺序
        select(1024,&readyset,NULL,NULL,NULL);
        
        if(FD_ISSET(sockfd,&readyset)){
            // nret1=accept(sockfd,NULL,NULL);
            // printf("A new socket has remove fome queue.\n");
            // FD_CLR(sockfd,&monitorset);
            // FD_SET(nret,&monitorset);
            // FD_SET(STDIN_FILENO,&monitorset);
            if(nret1==-1){
                nret1=accept(sockfd,NULL,NULL);
                printf("A new socket1 has remove fome queue,fd is %d\n",nret1);
            }else{
                nret2=accept(sockfd,NULL,NULL);
                printf("A new socket2 has remove fome queue,fd is %d\n",nret2);
                FD_CLR(sockfd,&monitorset);
                FD_SET(nret1,&monitorset);
                FD_SET(nret2,&monitorset);
            }
        }

        if(nret1!=-1&&FD_ISSET(nret1,&readyset)){
            memset(buf,0,sizeof(buf));
            int sret=recv(nret1,buf,sizeof(buf),0);
            // if(sret==0){
            //     printf("client2 is over\n");
            //     close(nret1);
            //     FD_CLR(nret1,&monitorset);
            //     FD_SET(sockfd,&monitorset);
            //     printf("waiting for new client...\n");
            //     nret1=-1;
            //     continue;
            // }
            send(nret2,buf,strlen(buf),0);
        }
        if(nret2!=-1&&FD_ISSET(nret2,&readyset)){
            memset(buf,0,sizeof(buf));
            int sret=recv(nret2,buf,sizeof(buf),0);
            // if(sret==0){
            //     printf("client1 is over\n");
            //     close(nret2);
            //     FD_CLR(nret2,&monitorset);
            //     FD_SET(sockfd,&monitorset);
            //     printf("waiting for new client...\n");
            //     nret2=-1;
            //     continue;
            // }
            send(nret1,buf,strlen(buf),0);
          
        }
        

    }

    close(sockfd);
    close(nret1);
    close(nret2);

    return 0;
}


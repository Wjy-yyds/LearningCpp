#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sockfd, -1, "socket");

    int flag=1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag));
    
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(atoi(MYPORT));
    addr.sin_addr.s_addr=inet_addr(MYIPSTR);

    int bret=bind(sockfd,(struct sockaddr *)&addr,sizeof(addr));
    ERROR_CHECK(bret,-1,"bind error");

    int lret = listen(sockfd, 50);
    ERROR_CHECK(lret, -1, "listen");
     char buf[4096]={0};
    fd_set monitorset;
    fd_set readyset;


    FD_ZERO(&monitorset);
    FD_SET(sockfd,&monitorset);
    //int nret=accept(sockfd,NULL,NULL);
    // int nret1=-1,nret2=-1;
    int nret[1024];
    time_t lasttime[1024];
    for(int i = 0; i < 1024; ++i){
    nret[i] = -1; 
    lasttime[i]=0;
    }
    int curindex=0;
    time_t curtime=0;
    
    
    
    while(1) {
        memcpy(&readyset, &monitorset, sizeof(readyset));  // 修正参数顺序
        select(1024,&readyset,NULL,NULL,NULL);
        curtime=time(NULL);
        if(FD_ISSET(sockfd,&readyset)){
          nret[curindex]=accept(sockfd,NULL,NULL);
          printf("New user is coming now,name  is user%d.\n",curindex);
          lasttime[curindex]=curtime;
          FD_SET(nret[curindex],&monitorset);
          printf("monitor add success.\n");
          curindex++;
        }

        for(int i=0;i<curindex;i++){
            if(nret!=-1)
        if(curtime-lasttime[i]>10){
                printf("用户%d超时\n",i);
                FD_CLR(nret[i],&monitorset);
                close(nret[i]);
                nret[i]=-1;
                continue;
            }
        }
         
        //思考处理接受位置在循环的哪个地方
        //处理发送的位置在哪个地方
        for(int i=0;i<curindex;i++){
            printf("for 循环 i=%d\n",i);
            if(nret[i]!=-1&&FD_ISSET(nret[i],&readyset)){
                
                printf("readyset 检测到 nret[%d] 有数据\n",i);
                memset(buf,0,sizeof(buf));
                int sret=recv(nret[i],buf,sizeof(buf),0);
                printf("服务端收到 nert [%d] 数据\n",i);
                if(sret == 0){
                    printf("the user%d is outline now.\n",i);
                    FD_CLR(nret[i],&monitorset);
                    close(nret[i]);
                    nret[i]=-1;
                    printf("成功关闭nert[%d]\n",i);
                    continue;
                }
                lasttime[i]=curtime;
                printf("开始向其他客户端发送数据\n");
                for(int j=0;j<curindex;j++){
                if( j!=i && nret[j]!=-1){
                    send(nret[j],buf,strlen(buf),0);
                    printf("此时buf内的数据是%s\n",buf);
                    printf("向发送nert[%d]数据完成\n",j);
                }
                
            }
            }
            
        }   
        
        printf("一次while循环结束\n");
        printf("_____________________________\n");
            
    }

    close(sockfd);
    return 0;
        
       

}

    



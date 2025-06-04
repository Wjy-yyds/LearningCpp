#include "head.h"

/* Usage:  */
int main(int argc, char *argv[]){
    
    //ARGS_CHECK(argc,4);

    int workernum =3;
    workerdata_t * workerArr=(workerdata_t *)calloc(3,sizeof(workerdata_t));
    makeWorker(workernum,workerArr);
    
    //int sockfd = socket(AF_INET,SOCK_STREAM,0);

    char *ip=MYIPSTR;
    char *port=MYPORT;
    int sockfd =tcpInit(ip,port);
    int epfd = epoll_create(1);
    epollAdd(epfd,sockfd);
    for(int i=0;i < workernum;i++){
        epollAdd(epfd,workerArr[i].sockfd);
    }
    struct epoll_event readyset[1024];
    while(1){

        int readysetNum = epoll_wait(epfd,readyset,1024,-1);

        for(int i = 0;i < readysetNum;i++){
            if(readyset[i].data.fd == sockfd){
                int netfd = accept(sockfd,NULL,NULL);
                printf("mian got netfd = %d\n",netfd);
                for(int j=0;j<workernum;j++){
                    if(workerArr[j].status == FREE){
                        sendfd(workerArr[j].sockfd,netfd);
                        workerArr[j].status = BUSY;
                        break;
                    }
                }
                close(netfd);
            }
            else{
                for(int j =0; j < workernum;j++){
                    if(readyset[i].data.fd == workerArr[j].sockfd){
                        workerArr[j].status = FREE;
                        pid_t pid;
                        read(workerArr[j].sockfd,&pid,sizeof(pid));
                        break;
                    }
                }
            }
        }
    }

    return 0;
}


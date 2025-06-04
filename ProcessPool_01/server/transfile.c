#include "head.h"

typedef struct{
    int length;
    char data[1000];
}train_t;


int recvn(int sockfd,void *buf,ssize_t length){
    char *p=(char *)buf;
    
}


//v1.0 解决粘包问题
int transfile(int sockfd){
    train_t train;
    train.length = 5;
    char path[] = "file1";
    memcpy(train.data,path,train.length);
    send(sockfd,&train,sizeof(train.length)+train.length,0);


    int fd = open("file1",O_RDWR);
    //bzero(&train,sizeof(train));
    char buf[1024] = {0};
    int sret = read(fd,&train.data,sizeof(train.data));
    train.length = sret;

    send(sockfd,&train,sizeof(train.length)+train.length,0);
}


//int transfile(int sockfd)

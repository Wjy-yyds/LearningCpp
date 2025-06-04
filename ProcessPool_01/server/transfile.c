#include "head.h"

typedef struct{
    int length;
    char data[1000];
}train_t;


int recvn(int sockfd,void *buf,ssize_t length){
    char *p=(char *)buf;
    
}


//v1.0 解决粘包问题
// int transfile(int sockfd){
//     train_t train;
//     train.length = 5;
//     char path[] = "file1";
//     memcpy(train.data,path,train.length);
//     send(sockfd,&train,sizeof(train.length)+train.length,0);


//     int fd = open("file1",O_RDWR);
//     //bzero(&train,sizeof(train));
//     char buf[1024] = {0};
//     int sret = read(fd,&train.data,sizeof(train.data));
//     train.length = sret;

//     send(sockfd,&train,sizeof(train.length)+train.length,0);
// }
int transfile(int sockfd){
    train_t train;
    char filename[] = "file1";
    train.length = 5;//火车头
    memcpy(train.data,filename,train.length);
    send(sockfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);//不能写sizeof(train)

    int fd = open(filename,O_RDWR);
    struct stat statbuf;
    fstat(fd,&statbuf);
    printf("filesize = %ld\n", statbuf.st_size);
    off_t filesize = statbuf.st_size;
    train.length = sizeof(filesize);
    memcpy(train.data,&filesize,sizeof(off_t));
    send(sockfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);//不能写sizeof(train)

   // // 先执行ftruncate
   // ftruncate(fd,filesize);
   // // 建立映射区
   // char * p = (char *)mmap(NULL,filesize,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
   // send(sockfd,p,filesize,MSG_NOSIGNAL);
   // // 释放映射区
   // munmap(p,filesize);
    sendfile(sockfd,fd,NULL,filesize);
    return 0;
}


//int transfile(int sockfd)

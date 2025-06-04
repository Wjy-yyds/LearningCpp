#include "threadPool.h"

typedef struct train_s{
    int length;
    char data[1000];
}train_t;

// int transfile(int sockfd){
//     train_t train;
//     //发送文件名
//     char filename[] = "file1";
//      train.length = 5;
//     memcpy(train.data,filename,train.length);
//     send(sockfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);
    
//     //发送文件大小
//     int fd = open(filename,O_RDWR);
//     struct stat statbuf;
//     fstat(fd,&statbuf);
//     printf("send filesize = %ld\n",statbuf.st_size);
    
    
//     off_t filesize = statbuf.st_size;
//     train.length = sizeof(filesize);
//     memcpy(train.data,&filesize,sizeof(off_t));
//     int ret1=send(sockfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);
//     ERROR_CHECK(ret1,-1,"send file size error");
//     //发送文件正文
//     int ret2 = ftruncate(fd,filesize);
//     ERROR_CHECK(ret2,-1,"ftruncate error");

//     char *p = (char *)mmap(NULL,filesize,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
//     off_t offset = 0;//已经发送的数据大小

//     while(1){

//         if(offset >= filesize){
//             break;
//         }
//         if(filesize - offset > 1000){
//             train.length = 1000;
//         }else{
//             train.length = filesize - offset;
//         }
        
//         send(sockfd,&train.length,sizeof(train.length),MSG_NOSIGNAL);
//         send(sockfd,p+offset,train.length,MSG_NOSIGNAL);
//         offset+=train.length;
//     }
//     train.length = 0;
//     send(sockfd,&train.length,sizeof(train.length),MSG_NOSIGNAL);
//     // 释放映射区
//     munmap(p,filesize);
//     return 0;
    
// }


int transfile(int sockfd){
    train_t train;
    char filename[] = "file1";
    train.length = 5;
    memcpy(&train.data, filename , train.length);
    send(sockfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);

    int fd = open(filename,O_RDWR);
    struct stat statbuf;
    fstat(fd,&statbuf);
    printf("文件大小：%ld\n",statbuf.st_size);
    off_t filesize = statbuf.st_size;
    train.length = sizeof(filesize);
    memcpy(train.data,&filesize,sizeof(off_t));
    send(sockfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);

   sendfile(sockfd,fd,NULL,filesize);

   return 0;

}

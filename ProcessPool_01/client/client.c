#include <my_header.h>
typedef struct {
    int length;
    char data[1000];
}train_t;

int recvn(int sockfd, void *buf, ssize_t length){
    char *p = (char *)buf;//char* 是因为偏移以1个字节为单位
    ssize_t cursize = 0;
    while(cursize < length){
        ssize_t sret = recv(sockfd,p+cursize,length-cursize,0);
        cursize += sret;
    }
    return 0;
}

int recvfile(int sockfd) {
    train_t train;
    recvn(sockfd, &train.length, sizeof(train.length));
    recvn(sockfd, train.data, train.length);
    char filename[4096] = {0};
    memcpy(filename, train.data, train.length);
    int fd = open(filename, O_RDWR|O_CREAT|O_TRUNC, 0666);

    off_t filesize;
    recvn(sockfd, &train.length, sizeof(train.length));
    recvn(sockfd, train.data, train.length);
    memcpy(&filesize, train.data, train.length);
    printf("filesize = %ld\n", filesize);

    ftruncate(fd, filesize);
    char *p = (char *)mmap(NULL, filesize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    recvn(sockfd, p, filesize);
    munmap(p, filesize);
    close(fd);
    return 0;
}
// int recvfile(int sockfd){
//     char filename[4096]={0};
//     train_t train;
//     recv(sockfd,&train.length,sizeof(train.length),0);
//     recv(sockfd,train.data,train.length,0);
//     memcpy(filename,train.data,train.length);

//     int fd = open(filename,O_RDWR|O_CREAT|O_TRUNC,0666);
//     recv(sockfd,&train.length,sizeof(train.length),0);
//     recv(sockfd,train.data,train.length,0);
//     write(fd,train.data,train.length);
//     close(fd);
//     return 0;
// }

int main(int argc, char *argv[]){
   
    //ARGS_CHECK(argc,3);
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(MYPORT));
    addr.sin_addr.s_addr  = inet_addr(MYIPSTR);
    int ret = connect(sockfd,(struct sockaddr *)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"connect");
    recvfile(sockfd);
    return 0;
}

#include <my_header.h>
typedef struct train_s {
    int length;
    char data[1000];
} train_t;//客户端和服务端协议设计要一致

int recvn(int sockfd, void *buf, ssize_t length){
    char *p = (char *)buf;//char* 是因为偏移以1个字节为单位
    ssize_t cursize = 0;
    while(cursize < length){
        ssize_t sret = recv(sockfd,p+cursize,length-cursize,0);
        ERROR_CHECK(sret,-1,"recv erro");
        cursize += sret;
    }
    return 0;
}

int recvfile(int sockfd) {
    train_t train;
    // 接收文件名
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

int main(int argc, char *argv[]){                                  
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(MYPORT));
    addr.sin_addr.s_addr = inet_addr(MYIPSTR);
    int ret = connect(sockfd,(struct sockaddr *)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"connect");
    recvfile(sockfd);
    close(sockfd);
    return 0;
}


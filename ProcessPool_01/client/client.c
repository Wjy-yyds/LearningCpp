#include <my_header.h>
typedef struct {
    int length;
    char data[1000];
}train_t;


int recvfile(int sockfd){
    char filename[4096]={0};
    train_t train;
    recv(sockfd,&train.length,sizeof(train.length),0);
    recv(sockfd,train.data,train.length,0);
    memcpy(filename,train.data,train.length);

    int fd = open(filename,O_RDWR|O_CREAT|O_TRUNC,0666);
    recv(sockfd,&train.length,sizeof(train.length),0);
    recv(sockfd,train.data,train.length,0);
    write(fd,train.data,train.length);
    close(fd);
    return 0;
}

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

#include <my_header.h>

enum{
    FREE,
    BUSY 
};
typedef struct workerdata_s{
    pid_t pid;
    int status;
    int sockfd;
}workerdata_t;

int makeWorker(int workerNum,workerdata_t  *workerArr );

int tcpInit(char *ip,char *port);

int epollAdd(int epfd,int fd);

int epollDel(int epfd,int fd);

int sendfd(int sockfd, int fdtosend, int exitflag);
int recvfd(int sockfd, int *pfdtorecv ,int *pexitflag);
int transfile(int sockfd);

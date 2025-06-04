#include <my_header.h>


int sendfd(int sockfd,int fdtosend,int exitFlag){
    struct msghdr hdr;
    bzero(&hdr,sizeof(hdr));

    struct iovec arr[1];
    arr[0].iov_base = &exitFlag;
    arr[0].iov_len = sizeof(exitFlag);

    hdr.msg_iov = arr;
    hdr.msg_iovlen = 1;
    struct cmsghdr *cmsg = (struct cmsghdr *)malloc(CMSG_LEN(sizeof(int)));
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    *(int *)CMSG_DATA(cmsg) = fdtosend;

    hdr.msg_control = cmsg;
    hdr.msg_controllen = cmsg->cmsg_len;

    sendmsg(sockfd, &hdr, 0);
    return 0;
}


int recvfd(int sockfd, int *pfdtorecv, int *pexitFlag) {
    struct msghdr hdr;
    bzero(&hdr, sizeof(hdr));
    struct iovec arr[1];
    arr[0].iov_base = pexitFlag;
    arr[0].iov_len = sizeof(int);
    
    hdr.msg_iov = arr;
    hdr.msg_iovlen = 1;
    
    struct cmsghdr *cmsg = (struct cmsghdr *)malloc(CMSG_LEN(sizeof(int)));
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    
    hdr.msg_control = cmsg;
    hdr.msg_controllen = cmsg->cmsg_len;
    
    recvmsg(sockfd, &hdr, 0);
    printf("data = %d\n", *(int *)CMSG_DATA(cmsg));
    *pfdtorecv = *(int *)CMSG_DATA(cmsg);
    
    return 0;
}


/* Usage:  */
int main(int argc, char *argv[]){                                  
    int fds[2];
    socketpair(AF_UNIX, SOCK_STREAM, 0,fds);

    pid_t pid = fork();

    if(pid == 0){
        close(fds[1]);
        int exitFlag;
        int fd_received;
        recvfd(fds[0],&fd_received,&exitFlag);
        char buf[] = "Data from child\n";
        write(fd_received,buf,strlen(buf));
        close(fd_received);
        exit(0);

    }else{

        close(fds[0]);
        
        int fd = open("file1",O_RDWR|O_CREAT|O_TRUNC,0666);
        char buf[] = "This is father write\n";
        write(fd,buf,sizeof(buf));
        int exitFlag = 0;
        sendfd(fds[1],fd,exitFlag);
        wait(NULL);
        close(fd);

    }
    return 0;
}


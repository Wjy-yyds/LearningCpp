#include "head.h"
int sendfd(int sockfd, int fdtosend, int exitflag){
    struct msghdr hdr; //准备好一个
    bzero(&hdr,sizeof(hdr));
    //准备正文
    struct iovec vec[1];//离散的区域只有一个碎片
    vec[0].iov_base = &exitflag; //碎片首地址
    vec[0].iov_len = sizeof(int);//碎片长度
    hdr.msg_iov = vec;//将离散区域和hdr扯上关系
    hdr.msg_iovlen = 1;
    //准备控制字段
    struct cmsghdr *cmsg = (struct cmsghdr *)malloc( CMSG_LEN(sizeof(int)) );
    cmsg->cmsg_len = CMSG_LEN(sizeof(int)); //已知data长4，求整个结构体长度
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS; // 这个选项说明要发送的数据结构是文件对象
    *(int *)CMSG_DATA(cmsg) = fdtosend; // 找到data首地址，强转成int *，再解引用，再赋值
    hdr.msg_control = cmsg;
    hdr.msg_controllen = CMSG_LEN(sizeof(int));
    sendmsg(sockfd,&hdr,0);
    return 0;
}
int recvfd(int sockfd, int *pfdtorecv, int *pexitflag){
    struct msghdr hdr; //准备好一个
    bzero(&hdr,sizeof(hdr));
    //准备正文
    struct iovec vec[1];//离散的区域只有一个碎片
    vec[0].iov_base = pexitflag; //碎片首地址
    vec[0].iov_len = sizeof(int);//碎片长度
    hdr.msg_iov = vec;//将离散区域和hdr扯上关系
    hdr.msg_iovlen = 1;
    //准备控制字段
    struct cmsghdr *cmsg = (struct cmsghdr *)malloc( CMSG_LEN(sizeof(int)) );
    cmsg->cmsg_len = CMSG_LEN(sizeof(int)); //已知data长4，求整个结构体长度
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS; // 这个选项说明要发送的数据结构是文件对象
    hdr.msg_control = cmsg;
    hdr.msg_controllen = CMSG_LEN(sizeof(int));
    recvmsg(sockfd,&hdr,0);
    *pfdtorecv = *(int *)CMSG_DATA(cmsg);
    return 0;
}
//int main(int argc, char *argv[])
//{
//    int fds[2];
//    socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
//    if(fork()){
//        //父
//        close(fds[0]);
//        int fd = open("file1",O_RDWR);
//        printf("parent fd = %d\n", fd);
//        write(fd,"hello",5);
//        sendfd(fds[1],fd);
//        wait(NULL);
//    }
//    else{
//        //子
//        close(fds[1]);
//        int fd;
//        recvfd(fds[0],&fd);
//        printf("child fd = %d\n", fd);
//        write(fd,"world",5);
//    }
//    return 0;
//}


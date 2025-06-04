#include "head.h"

//根据指定的主函数创建相应的子进程
//PID通过在父进程中获取fork返回值
//注意不要使用fork后倍增进程
int makeWorker(int workerNum,workerdata_t *workerArr){
     
    for(int i=0;i<workerNum;i++){
    int fds[2];
    socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
        pid_t pid = fork();
        if(pid == 0){
            while(1){

            int netfd;
            recvfd(fds[1],&netfd);
            printf("I am worker %d, netfd = %d\n", i, netfd);
            transfile(netfd);
            close(netfd);
                // 工人干完活了
            pid_t pid = getpid();
            write(fds[1],&pid,sizeof(pid));
            }
        }
        //只在父进程中操作子进程结构体
        close(fds[1]);
        workerArr[i].pid=pid;
        workerArr[i].status=FREE;
        workerArr[i].sockfd = fds[0];
        printf("the worker%d pid = %d\n",i,pid);
    }
    return 0;
}

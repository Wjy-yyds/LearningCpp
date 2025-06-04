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
            int exitflag;
            recvfd(fds[1],&netfd,&exitflag);

            printf("I am worker %d, netfd = %d, exitflag = %d\n", i, netfd, exitflag);
            if(exitflag == 1){
                //如果exitflag为1，说明是退出信号
                //close(fds[1]);
                printf("worker %d exit\n", i);
                exit(0);
            }
            transfile(netfd);
            close(netfd);
                // 工人干完活了
            pid_t pid = getpid();
            write(fds[1],&pid,sizeof(pid));//将自己的pid写入到管道中,说明结束信号
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

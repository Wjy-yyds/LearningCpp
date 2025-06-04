#include "threadPool.h"
int exitPipe[2];
void handler(int signum){
    printf("In handler :signum = %d\n",signum);
    write(exitPipe[1],"1",1);//向子进程写
}
/* Usage:  */
int main(int argc, char *argv[]){
    //子返回0，父进程返回子进程的ID
    //4.处理线程，子进程退出问题
    pipe(exitPipe);
    if(fork()){
        close(exitPipe[0]);
        signal(SIGUSR1,handler);
        wait(NULL);
        printf("父进程要结束\n");
        exit(0);
    }

    //子进程作为线程池处理工人
    close(exitPipe[1]);
        
    //1.先创建并初始化线程池
    threadPool_t threadPool;
    int workerNum = 2;
    threadPoolInit(&threadPool,workerNum);
    
    //2.通过makeworker创建工人线程
    makeWorker(&threadPool);
    
    //3.建立socket
    int sockfd = tcpInit();
    int epfd = epoll_create(1);
    epollAdd(epfd,sockfd);
    struct epoll_event readyset[1024];

    while(1){
        int readynum = epoll_wait(epfd,readyset,1024,-1);
        for(int i=0; i < readynum;i++){

            //处理当新的客户端连接时的逻辑
            if(readyset[i].data.fd == sockfd){
                int netfd = accept(sockfd,NULL,NULL);
                ERROR_CHECK(netfd,-1,"accept erro");
                printf("Main thread get a netfd = %d\n",netfd);

                //系统结构体是所有线程都可以访问的数据，加锁
                pthread_mutex_lock(&threadPool.mutex);
                enQueue(&threadPool.taskQueue,netfd);
                //主线程视为生产者，生产后通知工人消费（处理与客户端的通信）
                
                pthread_cond_broadcast(&threadPool.cond);
                pthread_mutex_unlock(&threadPool.mutex);
            }
            //监听到父进程发送给子进程
            //子进程读端就绪
            else if (readyset[i].data.fd == exitPipe[0]){
                printf("线程池准备退出\n");
                for(int j=0;j<workerNum;j++){
                    pthread_cancel(threadPool.tidArr.arr[j]);
                }
                for(int j=0; j < workerNum; j++){
                    pthread_join(threadPool.tidArr.arr[j],NULL);
                }
                printf("进程池退出完毕\n");
                exit(0);
            }
        }
    }
    return 0;
}


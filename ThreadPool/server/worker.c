#include "threadPool.h"

void unlock(void *arg){
    threadPool_t *pthreadPool = (threadPool_t *)arg;
    printf("unlock\n");
    pthread_mutex_unlock(&pthreadPool->mutex);
}
void *threadFunc(void *arg){
    threadPool_t *pthreadPool = (threadPool_t *)arg;
    int netfd;
    while(1){
        //4.子线程处理消费逻辑
        //while判断是否有新的到来，避免虚假唤醒
        //处理完任务后出队列，这个过程中都把锁拿在手里
        pthread_mutex_lock(&pthreadPool->mutex);
        //pthread_cleanup_push(unlock,pthreadPool);
        while(pthreadPool->exitFlag == 0&&pthreadPool->taskQueue.queueSize == 0){
            pthread_cond_wait(&pthreadPool->cond,&pthreadPool->mutex);
        }
        if(pthreadPool->exitFlag ==1){
            printf("工人线程准备退出了\n");
            pthread_mutex_unlock(&pthreadPool->mutex);
            pthread_exit(NULL);
        }
        //从任务队列头中获取一个通信文件符
        //netfd在用资源清理机后变成了一个局部变量
        netfd = pthreadPool->taskQueue.pFront->netfd;
        //消费一个
        deQueue(&pthreadPool->taskQueue);
        printf("This is worker get netfd = %d\n",netfd);
        //pthread_cleanup_pop(1);
        transfile(netfd);
        pthread_mutex_unlock(&pthreadPool->mutex);
    }
}

//选择包含信息最多的一个结构体!注意传递了（用了）两次线程
int makeWorker(threadPool_t *pthreadPool){
    //tidArr.arr是线程数组可以通过下标访问，线程业务内容为threadFunc
    //传递给线程的参数也为整个结构体
    for(int i =0;i < pthreadPool->tidArr.workerNum;i++){
        pthread_create(&pthreadPool->tidArr.arr[i],NULL,threadFunc,pthreadPool);
    }
    return 0;
}
